#ifndef SP_SERVER_H
#define SP_SERVER_H
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
 sp_match_player_id_t match_id;
} sp_session_t;

typedef struct {
 sp_token_t token;
 sp_deck_t deck;
} sp_match_search_t;

typedef struct {
 sp_session_t* sessions [2];
 sp_match_t state;
} sp_server_match_t;

typedef struct {
 lws_context* context;
 gs_hash_table(sp_token_t, sp_session_t*) sessions;
 gs_hash_table(dn_hash_t, sp_match_search_t) match_searches;
 dn_pool(sp_server_match_t) matches;
} sp_server_t;
sp_server_t sp_server;

void sp_server_init();
void sp_server_queue_response(sp_session_t* session, sp_net_padded_response_t* response);
void sp_server_process_request(sp_session_t* session, sp_net_request_t* request);
void sp_server_sync_match(sp_server_match_t* match);
sp_session_t* sp_server_find_player_session(sp_server_match_t* match, sp_token_t token);
int sp_protocol_handler(sp_ws_instance_t* instance, sp_ws_event_t event, void* userdata, void* data, size_t len);
int sp_http_protocol_handler(struct lws* websocket, enum lws_callback_reasons event, void* userdata, void* data, size_t len);

sp_token_t sp_gen_token();

static struct lws_protocols protocols[] = {
   {
   .name = "sp-http-protocol",
   .callback = sp_http_protocol_handler,
   .per_session_data_size = sizeof(sp_session_t),
   .rx_buffer_size = 0,
   .id = 0,
   .tx_packet_size = 0,
   .user = NULL
   },
   {
   .name = "sp-protocol",
   .callback = sp_protocol_handler,
   .per_session_data_size = sizeof(sp_session_t),
   .rx_buffer_size = sizeof(sp_net_request_t),
   .id = 0,
   .tx_packet_size = sizeof(sp_net_response_t),
   .user = NULL
 },
 LWS_PROTOCOL_LIST_TERM
};
#endif

#ifdef SP_SERVER_IMPL
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
  dn_pool_init(&sp_server.matches, 1024, sizeof(sp_server_match_t));
}

void sp_server_queue_response(sp_session_t* session, sp_net_padded_response_t* response) {
  dn_os_memory_copy(sp_magic, response->payload.magic, sizeof(sp_net_magic_t));
  dn_ring_buffer_push(&session->response_queue, response);
  lws_callback_on_writable_all_protocol(lws_get_context(session->instance), lws_get_protocol(session->instance));
}

void sp_server_process_request(sp_session_t* session, sp_net_request_t* request) {
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

    bool found_match = gs_hash_table_key_exists(sp_server.match_searches, match_request->password);
    if (!found_match) {
      gs_hash_table_insert(sp_server.match_searches, match_request->password, ((sp_match_search_t) {
      .token = session->token,
      .deck = match_request->deck,
      }));
      break;
    };

    sp_match_search_t* search = gs_hash_table_getp(sp_server.match_searches, match_request->password);

    // There's another search with the same password, but the client already disconnected
    if (!gs_hash_table_exists(sp_server.sessions, search->token)) {
      gs_hash_table_insert(sp_server.match_searches, match_request->password, ((sp_match_search_t) {
      .token = session->token,
      .deck = match_request->deck,
      }));
      break;
    }

    // Allocate a match
    dn_pool_handle_t handle = dn_pool_reserve(&sp_server.matches);
    sp_server_match_t* match = dn_pool_at(sp_server_match_t, &sp_server.matches, handle);
    dn_os_zero_memory(match, sizeof(sp_server_match_t));

    // Pack a few things we need per-client into a utility struct to iterate over
    typedef struct {
      sp_session_t* session;
      sp_match_search_t* search;
    } sp_client_t;
    sp_client_t clients [2] = dn_zero_initialize();

    clients[0].search = &(sp_match_search_t) { .token = session->token, .deck = match_request->deck };
    clients[0].session = session;
    clients[1].search = gs_hash_table_getp(sp_server.match_searches, match_request->password);
    clients[1].session = gs_hash_table_get(sp_server.sessions, clients[1].search->token);

    // Initialize the match
    sp_match_init(&match->state, (sp_deck_t [2]) {
      clients[0].search->deck,
      clients[1].search->deck
    });

    // Mark down the match in each client's persistent storage and vice versa
    dn_for_arr(clients, i) {
      clients[i].session->match = handle;
      clients[i].session->match_id = match->state.players[i].id;
      match->sessions[i] = clients[i].session;
    }

    sp_net_padded_response_t response = dn_zero_initialize();

    // Send messages back to the client
    dn_for_arr(clients, i) {
      sp_client_t* client = &clients[i];
      sp_client_t* other_client = &clients[(i + 1) % 2];

      // First, a message that the match started
      dn_os_zero_memory(&response, sizeof(sp_net_padded_response_t));
      response = (sp_net_padded_response_t){
      .payload = {
        .op = SP_OPCODE_MATCH_EVENT,
        .match_event = {
         .kind = SP_NET_MATCH_EVENT_KIND_FOUND_OPPONENT,
         .found = {
           .username = dn_zero_initialize(),
           .your_match_id = client->session->match_id,
         }
        }
      }};
      dn_string_copy_to_str_buffer(other_client->session->username, &response.payload.match_event.found.username);
      sp_server_queue_response(client->session, &response);

      // Then, send over the first match event, which contains the initial state
      dn_os_zero_memory(&response, sizeof(sp_net_padded_response_t));
      response = (sp_net_padded_response_t){
      .payload = {
        .op = SP_OPCODE_MATCH_EVENT,
        .match_event = (sp_net_match_event_t){
         .kind = SP_NET_MATCH_EVENT_KIND_BEGIN,
         .state = match->state
        }
      }};
      sp_server_queue_response(client->session, &response);
    }

    // Purge the request from our data structures   
    gs_hash_table_erase(sp_server.match_searches, match_request->password);
    break;  
   }
   case SP_OPCODE_CLIENT_MATCH_ACTION: {
      lwsl_user("SP_OPCODE_CLIENT_MATCH_ACTION");

      sp_server_match_t* match = dn_pool_at(sp_server_match_t, &sp_server.matches, session->match);
      DN_ASSERT(match);

      sp_player_t* player = sp_match_find_player(&match->state, session->match_id);

      sp_match_action_result_t result = sp_match_process_action(&match->state, player, &request->match_action.action);
      sp_net_padded_response_t response = {
        .payload = {
        .op = SP_OPCODE_MATCH_EVENT,
        .match_event = {
           .kind = SP_NET_MATCH_EVENT_KIND_ACTION_RESULT,
           .action = {
           .token = session->token,
           .action = request->match_action.action,
           .result = result
           }
        }
        }
      };
      sp_server_queue_response(match->sessions[0], &response);
      sp_server_queue_response(match->sessions[1], &response);
      sp_server_sync_match(match);
      break;
   }
   default: {
    DN_UNREACHABLE();
    break;
   }
 }
}

void sp_server_sync_match(sp_server_match_t* match) {
 sp_net_padded_response_t message = {
   .payload = {
    .op = SP_OPCODE_MATCH_EVENT,
    .match_event = {
      .kind = SP_NET_MATCH_EVENT_KIND_SYNC,
      .state = match->state
    }
   }
 };
 sp_server_queue_response(match->sessions[0], &message);
 sp_server_queue_response(match->sessions[1], &message);
}

sp_session_t* sp_server_find_player(sp_server_match_t* match, sp_token_t token) {
 dn_for(index, 2) {
   if (match->sessions[index]->token == token) {
    return match->sessions[index];
   }
 }

 DN_UNREACHABLE_MESSAGE("sp_server_find_player");
 return NULL;
}


/////////////////
// SP PROTOCOL //
/////////////////
int sp_protocol_handler(sp_ws_instance_t* instance, sp_ws_event_t event, void* userdata, void* data, size_t len) {
 sp_session_t* session = (sp_session_t*)userdata;
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

        break;
   }
   case LWS_CALLBACK_CLOSED: {
    gs_hash_table_erase(sp_server.sessions, session->token);
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
#endif