#include "stdio.h"

#include "libwebsockets.h"

#define DN_MATH_BACKEND_HANDMADE
#define DN_IMPL
#include "dn.h"

#include "lws.h"
#include "network.h"

#define SP_LWS_IMPL
#include "lws.h"

// client sends a code
// put the code in a list inside the server
// other client sends a code
// server sends message to both clients that the game started

sp_token_t sp_gen_token() {
  sp_token_t token = dn_zero_initialize();

  for (int i = 0; i < 8; i++) {
      token = (token << 8) | (rand() & 0xFF);
  }

  return token;
}


///////////////////
// HTTP PROTOCOL //
///////////////////
int sp_http_protocol_handler(struct lws* websocket, enum lws_callback_reasons event, void* userdata, void* data, size_t len) {
  sp_log_lws_event(event);

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


/////////////////
// SP PROTOCOL //
/////////////////
typedef struct lws sp_ws_instance_t;
typedef enum lws_callback_reasons sp_ws_event_t;

typedef struct {

} sp_pending_match_t;

#define SP_MAX_RESPONSE_QUEUE 16

typedef struct {
  u32 heartbeat_cooldown;
  sp_token_t id;
  sp_ws_instance_t* instance;
  dn_ring_buffer(sp_net_response_t) response_queue;
} sp_session_data_t;

typedef struct {
  gs_hash_table(sp_net_match_password_t, sp_token_t) match_requests;
  dn_pool(sp_pending_match_t) pending_matches;
} sp_server_t;

void sp_server_queue_response(sp_session_data_t* session, sp_net_response_t* response) {
  dn_os_memory_copy(sp_magic, response->magic, sizeof(sp_net_magic_t));
  dn_ring_buffer_push(&session->response_queue, response);
  lws_callback_on_writable_all_protocol(lws_get_context(session->instance), lws_get_protocol(session->instance));
}

void sp_protocol_process_request(sp_session_data_t* session, sp_net_request_t* request) {
  if (!session) return;
  if (!request) return;

  if (!dn_os_is_memory_equal(request->magic, sp_magic, sizeof(sp_net_magic_t))) {
    lwsl_err("could not verify magic number; session id = %ld, magic = '%02X', '%02X', '%02X', '%02X', '%02X', '%02X', '%02X', '%02X'",
      session->id,
      request->magic[0], request->magic[1], request->magic[2], request->magic[3], request->magic[4], request->magic[5], request->magic[6], request->magic[7]
    );
  }

  switch(request->op) {
    case SP_OPCODE_ECHO: {
      lwsl_user("SP_OPCODE_ECHO");
      sp_net_response_t response;
      break;
    }
    case SP_OPCODE_MATCH_REQUEST: {
      lwsl_user("SP_OPCODE_MATCH_REQUEST");
      break;      
    }
  }
}

int sp_protocol_handler(sp_ws_instance_t* instance, sp_ws_event_t event, void* userdata, void* data, size_t len) {
  sp_log_lws_event(event);

  sp_session_data_t* session = (sp_session_data_t*)userdata;
  sp_net_request_t* request = (sp_net_request_t*)data;

  switch(event) {
    case LWS_CALLBACK_ESTABLISHED: {
      dn_ring_buffer_init(&session->response_queue, SP_MAX_RESPONSE_QUEUE, sizeof(sp_net_response_t));
      session->instance = instance;
      session->id = sp_gen_token();
    }
		case LWS_CALLBACK_RECEIVE: {
      sp_protocol_process_request(session, request);
			break;
    }
		case LWS_CALLBACK_SERVER_WRITEABLE: {
      if (!dn_ring_buffer_is_empty(&session->response_queue)) {
        sp_net_response_t* response = (sp_net_response_t*)dn_ring_buffer_pop(&session->response_queue);
			  lws_write(instance, (u8*)response, sizeof(sp_net_response_t), LWS_WRITE_BINARY);
      }
      
			break;
    }
		default: {
			break;
    }
	}

  return 0;
}

static struct lws_protocols protocols[] =
{
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


int main(int arg_count, char** args) {
  dn_init();

  lws_set_log_level(LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE, NULL);
  lwsl_user("spums");

  struct lws_context_creation_info lws_info = dn_zero_initialize();
  lws_info.port = 8000;
  lws_info.protocols = protocols;
  lws_info.pt_serv_buf_size = 32 * 1024;
	lws_info.options = LWS_SERVER_OPTION_VALIDATE_UTF8 | LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

  lwsl_user("Creating LWS context");
  struct lws_context *context = lws_create_context( &lws_info );

	while(true) {
		lws_service(context, 0);
	}

	lws_context_destroy( context );

	return 0;
}
