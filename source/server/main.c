#include "stdio.h"

#include "libwebsockets.h"

#define DN_MATH_BACKEND_HANDMADE
#define DN_IMPL
#include "dn.h"

int callback_http(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len) {
	switch( reason ) {
		case LWS_CALLBACK_HTTP: {
			lws_serve_http_file( wsi, "example.html", "text/html", NULL, 0 );
			break;
    }
		default: {
			break;
    }
	}

	return 0;
}

#define EXAMPLE_RX_BUFFER_BYTES (10)
struct payload {
	unsigned char data[LWS_SEND_BUFFER_PRE_PADDING + EXAMPLE_RX_BUFFER_BYTES + LWS_SEND_BUFFER_POST_PADDING];
	size_t len;
} received_payload;

int callback_example( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
	switch( reason ) {
		case LWS_CALLBACK_RECEIVE: {
			memcpy( &received_payload.data[LWS_SEND_BUFFER_PRE_PADDING], in, len );
			received_payload.len = len;
			lws_callback_on_writable_all_protocol( lws_get_context( wsi ), lws_get_protocol( wsi ) );
			break;
    }
		case LWS_CALLBACK_SERVER_WRITEABLE: {
			lws_write( wsi, &received_payload.data[LWS_SEND_BUFFER_PRE_PADDING], received_payload.len, LWS_WRITE_TEXT );
			break;
    }
		default: {
			break;
    }
	}

	return 0;
}
int main(int arg_count, char** args) {
  printf("hello, world!\n");

  struct lws_context_creation_info lws_context_info = dn_zero_initialize();
  lws_context_info.port = 8000;
  lws_context_info.protocols = (struct lws_protocols []){
    {
      "http-only",   /* name */
      callback_http, /* callback */
      0,             /* No per session data. */
      0,             /* max frame size / rx buffer */
    },
    {
      "example-protocol",
      callback_example,
      0,
      EXAMPLE_RX_BUFFER_BYTES,
    },
    { 
      NULL, 
      NULL, 
      0, 
      0 
    }
  };

  struct lws_context *context = lws_create_context( &lws_context_info );

	while(true) {
		lws_service( context, /* timeout_ms = */ 1000000 );
	}

	lws_context_destroy( context );

	return 0;
}