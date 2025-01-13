#include "stdio.h"

#include "libwebsockets.h"

#define DN_MATH_BACKEND_HANDMADE
#define DN_RUN_INTERNAL_TESTS
#define DN_IMPL
#include "dn.h"

#define SP_IMPL
#include "sp.h"

#include "lws.h"

#define SP_LWS_IMPL
#include "lws.h"

////////////
// HEADER //
////////////
typedef struct lws sp_ws_instance_t;
typedef enum lws_callback_reasons sp_ws_event_t;
typedef struct lws_context lws_context;

#define SP_MAX_RESPONSE_QUEUE 16


typedef struct {
  u8 padding [LWS_PRE];
  sp_net_response_t payload;
} sp_net_padded_response_t;

typedef struct {
  sp_token_t token;
  sp_ws_instance_t* instance;
  dn_string_t username;
  dn_ring_buffer(sp_net_padded_response_t) response_queue;
  dn_pool_handle_t match;
} sp_session_data_t;

typedef struct {
  sp_token_t token;
  sp_deck_t deck;
} sp_match_search_t;

typedef struct {
  sp_ws_instance_t* players [2];
  sp_match_data_t match;
} sp_match_t;

typedef struct {
  lws_context* context;
  gs_hash_table(sp_token_t, sp_session_data_t*) sessions;
  gs_hash_table(dn_hash_t, sp_match_search_t) match_searches;
  dn_pool(sp_match_t) matches;
} sp_server_t;
sp_server_t sp_server;

void sp_server_init();
void sp_server_queue_response(sp_session_data_t* session, sp_net_padded_response_t* response);
void sp_server_process_request(sp_session_data_t* session, sp_net_request_t* request);
int  sp_protocol_handler(sp_ws_instance_t* instance, sp_ws_event_t event, void* userdata, void* data, size_t len);
int  sp_http_protocol_handler(struct lws* websocket, enum lws_callback_reasons event, void* userdata, void* data, size_t len);

sp_token_t sp_gen_token();

static struct lws_protocols protocols[] = {
	{
    .name = "sp-http-protocol",
    .callback = sp_http_protocol_handler,
    .per_session_data_size = sizeof(sp_session_data_t),
    .rx_buffer_size = 0,
    .id = 0,
    .tx_packet_size = 0,
    .user = NULL
	},
	{
    .name = "sp-protocol",
    .callback = sp_protocol_handler,
    .per_session_data_size = sizeof(sp_session_data_t),
    .rx_buffer_size = sizeof(sp_net_request_t),
    .id = 0,
    .tx_packet_size = sizeof(sp_net_response_t),
    .user = NULL
  },
  LWS_PROTOCOL_LIST_TERM
};


////////////////////
// IMPLEMENTATION //
////////////////////

////////////
// SERVER //
////////////
void sp_server_init() {
  sp_server.sessions = NULL;
  sp_server.match_searches = NULL;
  sp_server.matches = dn_zero_struct(dn_pool_t);

  lws_set_log_level(LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE, NULL);
  lwsl_user("spums");

  struct lws_context_creation_info lws_info = dn_zero_initialize();
  lws_info.port = 8000;
  lws_info.protocols = protocols;
  lws_info.pt_serv_buf_size = 32 * 1024;
	lws_info.options = LWS_SERVER_OPTION_VALIDATE_UTF8 | LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

  sp_server.context = lws_create_context(&lws_info);
  dn_pool_init(&sp_server.matches, 1024, sizeof(sp_match_t));

}

void sp_server_queue_response(sp_session_data_t* session, sp_net_padded_response_t* response) {
  dn_os_memory_copy(sp_magic, response->payload.magic, sizeof(sp_net_magic_t));
  dn_ring_buffer_push(&session->response_queue, response);
  lws_callback_on_writable_all_protocol(lws_get_context(session->instance), lws_get_protocol(session->instance));
}

void sp_server_process_request(sp_session_data_t* session, sp_net_request_t* request) {
  if (!session) return;
  if (!request) return;

  if (!dn_os_is_memory_equal(request->magic, sp_magic, sizeof(sp_net_magic_t))) {
    lwsl_err("could not verify magic number; token = %ld, magic = '%02X', '%02X', '%02X', '%02X', '%02X', '%02X', '%02X', '%02X'",
      session->token,
      request->magic[0], request->magic[1], request->magic[2], request->magic[3], request->magic[4], request->magic[5], request->magic[6], request->magic[7]
    );
  }

  switch(request->op) {
    case SP_OPCODE_ECHO: {
      lwsl_user("SP_OPCODE_ECHO");
      break;
    }
    case SP_OPCODE_REQUEST_TOKEN: {
      lwsl_user("SP_OPCODE_REQUEST_TOKEN");

      // @string Copy the over-the-wire username into the session
      session->username = dn_string_copy(dn_str_buffer_view(request->token_req.username), &dn_allocators.standard.allocator);

      sp_net_padded_response_t response = {
        .payload = {
          .op = SP_OPCODE_REQUEST_TOKEN,
          .request_token = {
            .token = session->token
          }
        }
      };
      sp_server_queue_response(session, &response);

      break;
    }
    case SP_OPCODE_MATCH_REQUEST: {
      lwsl_user("SP_OPCODE_MATCH_REQUEST");

      sp_net_match_request_t* match_request = &request->match;

      if (gs_hash_table_key_exists(sp_server.match_searches, match_request->password)) {
        // Allocate a match
        dn_pool_handle_t handle = dn_pool_reserve(&sp_server.matches);
        sp_match_data_t* match = dn_pool_at(sp_match_data_t, &sp_server.matches, handle);
        dn_os_zero_memory(match, sizeof(sp_match_data_t));

        // Pack a few things we need per-client into a utility struct to iterate over
        typedef struct {
          sp_session_data_t* session;
          sp_match_search_t* search;
          sp_player_t* player;
        } sp_client_t;
        sp_client_t clients [2] = dn_zero_initialize();

        clients[0].search = &(sp_match_search_t) { .token = session->token, .deck = match_request->deck };
        clients[0].session = session;
        clients[0].player = &match->players[0];
        clients[1].search = gs_hash_table_getp(sp_server.match_searches, match_request->password);
        clients[1].session = gs_hash_table_get(sp_server.sessions, clients[1].search->token);
        clients[1].player = &match->players[1];

        // Initialize the match
        sp_match_init(match, (sp_deck_t [2]) {
          clients[0].search->deck,
          clients[1].search->deck
        });

        // Send messages back to the client
        dn_for_arr(clients, i) {
          sp_client_t* client = &clients[i];
          sp_client_t* other_client = &clients[(i + 1) % 2];

          // First, a message that the match started
          sp_server_queue_response(client->session, &(sp_net_padded_response_t){
            .payload = {
              .op = SP_OPCODE_BEGIN_MATCH,
            }
          });

          // Then, send over the first match event, which contains the initial state
          sp_net_padded_response_t match_data = {
            .payload = {
              .op = SP_OPCODE_MATCH_EVENT,
              .match_event = (sp_net_match_event_t){
                .kind = SP_MATCH_EVENT_BEGIN,
                .begin = dn_zero_initialize()
              }
          }};

          sp_net_begin_match_data_t* payload = &match_data.payload.match_event.begin;
          dn_string_copy_to_str_buffer(other_client->session->username, &payload->username);
          dn_os_arr_copy(client->player->hand, payload->hand)
          dn_os_arr_copy(client->player->energy, payload->energy)
          dn_os_arr_copy(other_client->player->energy, payload->opponent_energy)
          payload->turn = sp_match_turn_order(match, client->player);
          sp_server_queue_response(client->session, &match_data);
        }

        // Mark down the match in each client's persistent storage
        dn_for_arr(clients, i) {
          clients[i].session->match = handle;
        }

        // Purge the request from our data structures        
        gs_hash_table_erase(sp_server.match_searches, match_request->password);


      } else {
        sp_deck_print(&match_request->deck);
        gs_hash_table_insert(sp_server.match_searches, match_request->password, ((sp_match_search_t) {
          .token = session->token,
          .deck = match_request->deck,
        }));
      }

      break;      
    }
  }
}


/////////////////
// SP PROTOCOL //
/////////////////
int sp_protocol_handler(sp_ws_instance_t* instance, sp_ws_event_t event, void* userdata, void* data, size_t len) {
  sp_session_data_t* session = (sp_session_data_t*)userdata;
  sp_net_request_t* request = (sp_net_request_t*)data;

  switch(event) {
    case LWS_CALLBACK_ESTABLISHED: {
      dn_ring_buffer_init(&session->response_queue, SP_MAX_RESPONSE_QUEUE, sizeof(sp_net_padded_response_t));
      session->instance = instance;
      session->token = sp_gen_token();
      gs_hash_table_insert(sp_server.sessions, session->token, session);
    }
		case LWS_CALLBACK_RECEIVE: {
      sp_server_process_request(session, request);
			break;
    }
		case LWS_CALLBACK_SERVER_WRITEABLE: {
      while (!dn_ring_buffer_is_empty(&session->response_queue)) {
        sp_net_padded_response_t* response = (sp_net_padded_response_t*)dn_ring_buffer_pop(&session->response_queue);
			  lws_write(instance, (u8*)&response->payload, sizeof(sp_net_response_t), LWS_WRITE_BINARY);
      }

      DN_LOG("rb size: %d", session->response_queue.size);
      
			break;
    }
		default: {
			break;
    }
	}

  return 0;
}




///////////////////
// HTTP PROTOCOL //
///////////////////
int sp_http_protocol_handler(struct lws* websocket, enum lws_callback_reasons event, void* userdata, void* data, size_t len) {
	switch(event) {
		case LWS_CALLBACK_HTTP: {
			lws_serve_http_file(websocket, "example.html", "text/html", NULL, 0);
			break;
    }
		default: {
			break;
    }
	}

	return 0;
}


///////////
// UTILS //
///////////
sp_token_t sp_gen_token() {
  sp_token_t token = dn_zero_initialize();

  for (int i = 0; i < 8; i++) {
      token = (token << 8) | (rand() & 0xFF);
  }

  return token;
}

int main(int arg_count, char** args) {
  dn_init();
  sp_test();
  sp_server_init();

	while(true) {
		lws_service(sp_server.context, 0);
	}

	lws_context_destroy(sp_server.context);

	return 0;
}
