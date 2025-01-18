#include "stdio.h"

#include "libwebsockets.h"

#define DN_MATH_BACKEND_HANDMADE
#define DN_RUN_INTERNAL_TESTS
#define DN_IMPL
#include "dn.h"

#define SP_IMPL
#include "sp.h"

#include "lws.h"
#include "server.h"

#define SP_LWS_IMPL
#include "lws.h"
#define SP_SERVER_IMPL
#include "server.h"



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
