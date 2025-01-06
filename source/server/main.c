#include "stdio.h"

#include "libwebsockets.h"

#define DN_MATH_BACKEND_HANDMADE
#define DN_IMPL
#include "dn.h"

#include "network.h"

dn_string_t lws_event_to_string(enum lws_callback_reasons event) {
  switch (event) {
		case(LWS_CALLBACK_PROTOCOL_INIT): {
				return dn_string_literal("LWS_CALLBACK_PROTOCOL_INIT");
		}
		case(LWS_CALLBACK_PROTOCOL_DESTROY): {
				return dn_string_literal("LWS_CALLBACK_PROTOCOL_DESTROY");
		}
		case(LWS_CALLBACK_WSI_CREATE): {
				return dn_string_literal("LWS_CALLBACK_WSI_CREATE");
		}
		case(LWS_CALLBACK_WSI_DESTROY): {
				return dn_string_literal("LWS_CALLBACK_WSI_DESTROY");
		}
		case(LWS_CALLBACK_WSI_TX_CREDIT_GET): {
				return dn_string_literal("LWS_CALLBACK_WSI_TX_CREDIT_GET");
		}
		case(LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS): {
				return dn_string_literal("LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS");
		}
		case(LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS): {
				return dn_string_literal("LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS");
		}
		case(LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION): {
				return dn_string_literal("LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION");
		}
		case(LWS_CALLBACK_SSL_INFO): {
				return dn_string_literal("LWS_CALLBACK_SSL_INFO");
		}
		case(LWS_CALLBACK_OPENSSL_PERFORM_SERVER_CERT_VERIFICATION): {
				return dn_string_literal("LWS_CALLBACK_OPENSSL_PERFORM_SERVER_CERT_VERIFICATION");
		}
		case(LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED): {
				return dn_string_literal("LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED");
		}
		case(LWS_CALLBACK_HTTP): {
				return dn_string_literal("LWS_CALLBACK_HTTP");
		}
		case(LWS_CALLBACK_HTTP_BODY): {
				return dn_string_literal("LWS_CALLBACK_HTTP_BODY");
		}
		case(LWS_CALLBACK_HTTP_BODY_COMPLETION): {
				return dn_string_literal("LWS_CALLBACK_HTTP_BODY_COMPLETION");
		}
		case(LWS_CALLBACK_HTTP_FILE_COMPLETION): {
				return dn_string_literal("LWS_CALLBACK_HTTP_FILE_COMPLETION");
		}
		case(LWS_CALLBACK_HTTP_WRITEABLE): {
				return dn_string_literal("LWS_CALLBACK_HTTP_WRITEABLE");
		}
		case(LWS_CALLBACK_CLOSED_HTTP): {
				return dn_string_literal("LWS_CALLBACK_CLOSED_HTTP");
		}
		case(LWS_CALLBACK_FILTER_HTTP_CONNECTION): {
				return dn_string_literal("LWS_CALLBACK_FILTER_HTTP_CONNECTION");
		}
		case(LWS_CALLBACK_ADD_HEADERS): {
				return dn_string_literal("LWS_CALLBACK_ADD_HEADERS");
		}
		case(LWS_CALLBACK_VERIFY_BASIC_AUTHORIZATION): {
				return dn_string_literal("LWS_CALLBACK_VERIFY_BASIC_AUTHORIZATION");
		}
		case(LWS_CALLBACK_CHECK_ACCESS_RIGHTS): {
				return dn_string_literal("LWS_CALLBACK_CHECK_ACCESS_RIGHTS");
		}
		case(LWS_CALLBACK_PROCESS_HTML): {
				return dn_string_literal("LWS_CALLBACK_PROCESS_HTML");
		}
		case(LWS_CALLBACK_HTTP_BIND_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_HTTP_BIND_PROTOCOL");
		}
		case(LWS_CALLBACK_HTTP_DROP_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_HTTP_DROP_PROTOCOL");
		}
		case(LWS_CALLBACK_HTTP_CONFIRM_UPGRADE): {
				return dn_string_literal("LWS_CALLBACK_HTTP_CONFIRM_UPGRADE");
		}
		case(LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP): {
				return dn_string_literal("LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP");
		}
		case(LWS_CALLBACK_CLOSED_CLIENT_HTTP): {
				return dn_string_literal("LWS_CALLBACK_CLOSED_CLIENT_HTTP");
		}
		case(LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ): {
				return dn_string_literal("LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ");
		}
		case(LWS_CALLBACK_RECEIVE_CLIENT_HTTP): {
				return dn_string_literal("LWS_CALLBACK_RECEIVE_CLIENT_HTTP");
		}
		case(LWS_CALLBACK_COMPLETED_CLIENT_HTTP): {
				return dn_string_literal("LWS_CALLBACK_COMPLETED_CLIENT_HTTP");
		}
		case(LWS_CALLBACK_CLIENT_HTTP_WRITEABLE): {
				return dn_string_literal("LWS_CALLBACK_CLIENT_HTTP_WRITEABLE");
		}
		case(LWS_CALLBACK_CLIENT_HTTP_REDIRECT): {
				return dn_string_literal("LWS_CALLBACK_CLIENT_HTTP_REDIRECT");
		}
		case(LWS_CALLBACK_CLIENT_HTTP_BIND_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_CLIENT_HTTP_BIND_PROTOCOL");
		}
		case(LWS_CALLBACK_CLIENT_HTTP_DROP_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_CLIENT_HTTP_DROP_PROTOCOL");
		}
		case(LWS_CALLBACK_ESTABLISHED): {
				return dn_string_literal("LWS_CALLBACK_ESTABLISHED");
		}
		case(LWS_CALLBACK_CLOSED): {
				return dn_string_literal("LWS_CALLBACK_CLOSED");
		}
		case(LWS_CALLBACK_SERVER_WRITEABLE): {
				return dn_string_literal("LWS_CALLBACK_SERVER_WRITEABLE");
		}
		case(LWS_CALLBACK_RECEIVE): {
				return dn_string_literal("LWS_CALLBACK_RECEIVE");
		}
		case(LWS_CALLBACK_RECEIVE_PONG): {
				return dn_string_literal("LWS_CALLBACK_RECEIVE_PONG");
		}
		case(LWS_CALLBACK_WS_PEER_INITIATED_CLOSE): {
				return dn_string_literal("LWS_CALLBACK_WS_PEER_INITIATED_CLOSE");
		}
		case(LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION): {
				return dn_string_literal("LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION");
		}
		case(LWS_CALLBACK_CONFIRM_EXTENSION_OKAY): {
				return dn_string_literal("LWS_CALLBACK_CONFIRM_EXTENSION_OKAY");
		}
		case(LWS_CALLBACK_WS_SERVER_BIND_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_WS_SERVER_BIND_PROTOCOL");
		}
		case(LWS_CALLBACK_WS_SERVER_DROP_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_WS_SERVER_DROP_PROTOCOL");
		}
		case(LWS_CALLBACK_CLIENT_CONNECTION_ERROR): {
				return dn_string_literal("LWS_CALLBACK_CLIENT_CONNECTION_ERROR");
		}
		case(LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH): {
				return dn_string_literal("LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH");
		}
		case(LWS_CALLBACK_CLIENT_ESTABLISHED): {
				return dn_string_literal("LWS_CALLBACK_CLIENT_ESTABLISHED");
		}
		case(LWS_CALLBACK_CLIENT_CLOSED): {
				return dn_string_literal("LWS_CALLBACK_CLIENT_CLOSED");
		}
		case(LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER): {
				return dn_string_literal("LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER");
		}
		case(LWS_CALLBACK_CLIENT_RECEIVE): {
				return dn_string_literal("LWS_CALLBACK_CLIENT_RECEIVE");
		}
		case(LWS_CALLBACK_CLIENT_RECEIVE_PONG): {
				return dn_string_literal("LWS_CALLBACK_CLIENT_RECEIVE_PONG");
		}
		case(LWS_CALLBACK_CLIENT_WRITEABLE): {
				return dn_string_literal("LWS_CALLBACK_CLIENT_WRITEABLE");
		}
		case(LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED): {
				return dn_string_literal("LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED");
		}
		case(LWS_CALLBACK_WS_EXT_DEFAULTS): {
				return dn_string_literal("LWS_CALLBACK_WS_EXT_DEFAULTS");
		}
		case(LWS_CALLBACK_FILTER_NETWORK_CONNECTION): {
				return dn_string_literal("LWS_CALLBACK_FILTER_NETWORK_CONNECTION");
		}
		case(LWS_CALLBACK_WS_CLIENT_BIND_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_WS_CLIENT_BIND_PROTOCOL");
		}
		case(LWS_CALLBACK_WS_CLIENT_DROP_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_WS_CLIENT_DROP_PROTOCOL");
		}
		case(LWS_CALLBACK_GET_THREAD_ID): {
				return dn_string_literal("LWS_CALLBACK_GET_THREAD_ID");
		}
		case(LWS_CALLBACK_ADD_POLL_FD): {
				return dn_string_literal("LWS_CALLBACK_ADD_POLL_FD");
		}
		case(LWS_CALLBACK_DEL_POLL_FD): {
				return dn_string_literal("LWS_CALLBACK_DEL_POLL_FD");
		}
		case(LWS_CALLBACK_CHANGE_MODE_POLL_FD): {
				return dn_string_literal("LWS_CALLBACK_CHANGE_MODE_POLL_FD");
		}
		case(LWS_CALLBACK_LOCK_POLL): {
				return dn_string_literal("LWS_CALLBACK_LOCK_POLL");
		}
		case(LWS_CALLBACK_UNLOCK_POLL): {
				return dn_string_literal("LWS_CALLBACK_UNLOCK_POLL");
		}
		case(LWS_CALLBACK_CGI): {
				return dn_string_literal("LWS_CALLBACK_CGI");
		}
		case(LWS_CALLBACK_CGI_TERMINATED): {
				return dn_string_literal("LWS_CALLBACK_CGI_TERMINATED");
		}
		case(LWS_CALLBACK_CGI_STDIN_DATA): {
				return dn_string_literal("LWS_CALLBACK_CGI_STDIN_DATA");
		}
		case(LWS_CALLBACK_CGI_STDIN_COMPLETED): {
				return dn_string_literal("LWS_CALLBACK_CGI_STDIN_COMPLETED");
		}
		case(LWS_CALLBACK_CGI_PROCESS_ATTACH): {
				return dn_string_literal("LWS_CALLBACK_CGI_PROCESS_ATTACH");
		}
		case(LWS_CALLBACK_SESSION_INFO): {
				return dn_string_literal("LWS_CALLBACK_SESSION_INFO");
		}
		case(LWS_CALLBACK_GS_EVENT): {
				return dn_string_literal("LWS_CALLBACK_GS_EVENT");
		}
		case(LWS_CALLBACK_HTTP_PMO): {
				return dn_string_literal("LWS_CALLBACK_HTTP_PMO");
		}
		case(LWS_CALLBACK_RAW_PROXY_CLI_RX): {
				return dn_string_literal("LWS_CALLBACK_RAW_PROXY_CLI_RX");
		}
		case(LWS_CALLBACK_RAW_PROXY_SRV_RX): {
				return dn_string_literal("LWS_CALLBACK_RAW_PROXY_SRV_RX");
		}
		case(LWS_CALLBACK_RAW_PROXY_CLI_CLOSE): {
				return dn_string_literal("LWS_CALLBACK_RAW_PROXY_CLI_CLOSE");
		}
		case(LWS_CALLBACK_RAW_PROXY_SRV_CLOSE): {
				return dn_string_literal("LWS_CALLBACK_RAW_PROXY_SRV_CLOSE");
		}
		case(LWS_CALLBACK_RAW_PROXY_CLI_WRITEABLE): {
				return dn_string_literal("LWS_CALLBACK_RAW_PROXY_CLI_WRITEABLE");
		}
		case(LWS_CALLBACK_RAW_PROXY_SRV_WRITEABLE): {
				return dn_string_literal("LWS_CALLBACK_RAW_PROXY_SRV_WRITEABLE");
		}
		case(LWS_CALLBACK_RAW_PROXY_CLI_ADOPT): {
				return dn_string_literal("LWS_CALLBACK_RAW_PROXY_CLI_ADOPT");
		}
		case(LWS_CALLBACK_RAW_PROXY_SRV_ADOPT): {
				return dn_string_literal("LWS_CALLBACK_RAW_PROXY_SRV_ADOPT");
		}
		case(LWS_CALLBACK_RAW_PROXY_CLI_BIND_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_RAW_PROXY_CLI_BIND_PROTOCOL");
		}
		case(LWS_CALLBACK_RAW_PROXY_SRV_BIND_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_RAW_PROXY_SRV_BIND_PROTOCOL");
		}
		case(LWS_CALLBACK_RAW_PROXY_CLI_DROP_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_RAW_PROXY_CLI_DROP_PROTOCOL");
		}
		case(LWS_CALLBACK_RAW_PROXY_SRV_DROP_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_RAW_PROXY_SRV_DROP_PROTOCOL");
		}
		case(LWS_CALLBACK_RAW_RX): {
				return dn_string_literal("LWS_CALLBACK_RAW_RX");
		}
		case(LWS_CALLBACK_RAW_CLOSE): {
				return dn_string_literal("LWS_CALLBACK_RAW_CLOSE");
		}
		case(LWS_CALLBACK_RAW_WRITEABLE): {
				return dn_string_literal("LWS_CALLBACK_RAW_WRITEABLE");
		}
		case(LWS_CALLBACK_RAW_ADOPT): {
				return dn_string_literal("LWS_CALLBACK_RAW_ADOPT");
		}
		case(LWS_CALLBACK_RAW_CONNECTED): {
				return dn_string_literal("LWS_CALLBACK_RAW_CONNECTED");
		}
		case(LWS_CALLBACK_RAW_SKT_BIND_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_RAW_SKT_BIND_PROTOCOL");
		}
		case(LWS_CALLBACK_RAW_SKT_DROP_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_RAW_SKT_DROP_PROTOCOL");
		}
		case(LWS_CALLBACK_RAW_ADOPT_FILE): {
				return dn_string_literal("LWS_CALLBACK_RAW_ADOPT_FILE");
		}
		case(LWS_CALLBACK_RAW_RX_FILE): {
				return dn_string_literal("LWS_CALLBACK_RAW_RX_FILE");
		}
		case(LWS_CALLBACK_RAW_WRITEABLE_FILE): {
				return dn_string_literal("LWS_CALLBACK_RAW_WRITEABLE_FILE");
		}
		case(LWS_CALLBACK_RAW_CLOSE_FILE): {
				return dn_string_literal("LWS_CALLBACK_RAW_CLOSE_FILE");
		}
		case(LWS_CALLBACK_RAW_FILE_BIND_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_RAW_FILE_BIND_PROTOCOL");
		}
		case(LWS_CALLBACK_RAW_FILE_DROP_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_RAW_FILE_DROP_PROTOCOL");
		}
		case(LWS_CALLBACK_TIMER): {
				return dn_string_literal("LWS_CALLBACK_TIMER");
		}
		case(LWS_CALLBACK_EVENT_WAIT_CANCELLED): {
				return dn_string_literal("LWS_CALLBACK_EVENT_WAIT_CANCELLED");
		}
		case(LWS_CALLBACK_CHILD_CLOSING): {
				return dn_string_literal("LWS_CALLBACK_CHILD_CLOSING");
		}
		case(LWS_CALLBACK_CONNECTING): {
				return dn_string_literal("LWS_CALLBACK_CONNECTING");
		}
		case(LWS_CALLBACK_VHOST_CERT_AGING): {
				return dn_string_literal("LWS_CALLBACK_VHOST_CERT_AGING");
		}
		case(LWS_CALLBACK_VHOST_CERT_UPDATE): {
				return dn_string_literal("LWS_CALLBACK_VHOST_CERT_UPDATE");
		}
		case(LWS_CALLBACK_MQTT_NEW_CLIENT_INSTANTIATED): {
				return dn_string_literal("LWS_CALLBACK_MQTT_NEW_CLIENT_INSTANTIATED");
		}
		case(LWS_CALLBACK_MQTT_IDLE): {
				return dn_string_literal("LWS_CALLBACK_MQTT_IDLE");
		}
		case(LWS_CALLBACK_MQTT_CLIENT_ESTABLISHED): {
				return dn_string_literal("LWS_CALLBACK_MQTT_CLIENT_ESTABLISHED");
		}
		case(LWS_CALLBACK_MQTT_SUBSCRIBED): {
				return dn_string_literal("LWS_CALLBACK_MQTT_SUBSCRIBED");
		}
		case(LWS_CALLBACK_MQTT_CLIENT_WRITEABLE): {
				return dn_string_literal("LWS_CALLBACK_MQTT_CLIENT_WRITEABLE");
		}
		case(LWS_CALLBACK_MQTT_CLIENT_RX): {
				return dn_string_literal("LWS_CALLBACK_MQTT_CLIENT_RX");
		}
		case(LWS_CALLBACK_MQTT_UNSUBSCRIBED): {
				return dn_string_literal("LWS_CALLBACK_MQTT_UNSUBSCRIBED");
		}
		case(LWS_CALLBACK_MQTT_DROP_PROTOCOL): {
				return dn_string_literal("LWS_CALLBACK_MQTT_DROP_PROTOCOL");
		}
		case(LWS_CALLBACK_MQTT_CLIENT_CLOSED): {
				return dn_string_literal("LWS_CALLBACK_MQTT_CLIENT_CLOSED");
		}
		case(LWS_CALLBACK_MQTT_ACK): {
				return dn_string_literal("LWS_CALLBACK_MQTT_ACK");
		}
		case(LWS_CALLBACK_MQTT_RESEND): {
				return dn_string_literal("LWS_CALLBACK_MQTT_RESEND");
		}
		case(LWS_CALLBACK_MQTT_UNSUBSCRIBE_TIMEOUT): {
				return dn_string_literal("LWS_CALLBACK_MQTT_UNSUBSCRIBE_TIMEOUT");
		}
		case(LWS_CALLBACK_MQTT_SHADOW_TIMEOUT): {
				return dn_string_literal("LWS_CALLBACK_MQTT_SHADOW_TIMEOUT");
		}
		case(LWS_CALLBACK_USER): {
				return dn_string_literal("LWS_CALLBACK_USER");
		}
    default: {
				return dn_string_literal("LWS_CALLBACK_UNKNOWN");
    }
  }
};
// client sends a code
// put the code in a list inside the server
// other client sends a code
// server sends message to both clients that the game started

typedef u64 sp_uuid_t;

typedef struct {
  u32 heartbeat_cooldown;
  sp_uuid_t id;
} sp_session_t;

sp_uuid_t sp_gen_uuid();

int callback_http(struct lws* websocket, enum lws_callback_reasons event, void* userdata, void* data, size_t len) {
  printf("xxx\n");
  return 0;
  dn_string_t event_name = lws_event_to_string(event);
  printf("%s: event = %s (%d)\n", __func__, dn_string_to_cstr(event_name), event);
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

#define EXAMPLE_RX_BUFFER_BYTES (10)
struct payload {
	unsigned char data[LWS_SEND_BUFFER_PRE_PADDING + EXAMPLE_RX_BUFFER_BYTES + LWS_SEND_BUFFER_POST_PADDING];
	size_t len;
} received_payload;

int callback_example(struct lws *websocket, enum lws_callback_reasons event, void *user, void *in, size_t len) {
printf("xxx\n");
  return 0;  dn_string_t event_name = lws_event_to_string(event);
  printf("%s: event = %s (%d)\n", __func__, dn_string_to_cstr(event_name), event);

  switch(event) {
		case LWS_CALLBACK_RECEIVE: {
			memcpy( &received_payload.data[LWS_SEND_BUFFER_PRE_PADDING], in, len );
			received_payload.len = len;
			lws_callback_on_writable_all_protocol( lws_get_context(websocket), lws_get_protocol(websocket));
			break;
    }
		case LWS_CALLBACK_SERVER_WRITEABLE: {
			lws_write(websocket, &received_payload.data[LWS_SEND_BUFFER_PRE_PADDING], received_payload.len, LWS_WRITE_TEXT );
			break;
    }
		default: {
			break;
    }
	}

	return 0;
}

#if !defined (LWS_PLUGIN_STATIC)
#define LWS_DLL
#define LWS_INTERNAL
#include <libwebsockets.h>
#endif

#include <string.h>

#define RING_DEPTH 4096

/* one of these created for each message */

struct msg {
	void *payload; /* is malloc'd */
	size_t len;
	char binary;
	char first;
	char final;
};

struct per_session_data__minimal_server_echo {
	struct lws_ring *ring;
	uint32_t msglen;
	uint32_t tail;
	uint8_t completed:1;
	uint8_t flow_controlled:1;
	uint8_t write_consume_pending:1;
};

struct vhd_minimal_server_echo {
	struct lws_context *context;
	struct lws_vhost *vhost;

	int *interrupted;
	int *options;
};

static void
__minimal_destroy_message(void *_msg)
{
	struct msg *msg = _msg;

	free(msg->payload);
	msg->payload = NULL;
	msg->len = 0;
}
#include <assert.h>
static int
callback_minimal_server_echo(struct lws *wsi, enum lws_callback_reasons reason,
			  void *user, void *in, size_t len)
{
	struct per_session_data__minimal_server_echo *pss =
			(struct per_session_data__minimal_server_echo *)user;
	struct vhd_minimal_server_echo *vhd = (struct vhd_minimal_server_echo *)
			lws_protocol_vh_priv_get(lws_get_vhost(wsi),
				lws_get_protocol(wsi));
	const struct msg *pmsg;
	struct msg amsg;
	int m, n, flags;

	switch (reason) {

	case LWS_CALLBACK_PROTOCOL_INIT:
		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct vhd_minimal_server_echo));
		if (!vhd)
			return -1;

		vhd->context = lws_get_context(wsi);
		vhd->vhost = lws_get_vhost(wsi);

		/* get the pointers we were passed in pvo */

		vhd->interrupted = (int *)lws_pvo_search(
			(const struct lws_protocol_vhost_options *)in,
			"interrupted")->value;
		vhd->options = (int *)lws_pvo_search(
			(const struct lws_protocol_vhost_options *)in,
			"options")->value;
		break;

	case LWS_CALLBACK_ESTABLISHED:
		/* generate a block of output before travis times us out */
		lwsl_warn("LWS_CALLBACK_ESTABLISHED\n");
		pss->ring = lws_ring_create(sizeof(struct msg), RING_DEPTH,
					    __minimal_destroy_message);
		if (!pss->ring)
			return 1;
		pss->tail = 0;
		break;

	case LWS_CALLBACK_SERVER_WRITEABLE:

		lwsl_user("LWS_CALLBACK_SERVER_WRITEABLE\n");

		if (pss->write_consume_pending) {
			/* perform the deferred fifo consume */
			lws_ring_consume_single_tail(pss->ring, &pss->tail, 1);
			pss->write_consume_pending = 0;
		}

		pmsg = lws_ring_get_element(pss->ring, &pss->tail);
		if (!pmsg) {
			lwsl_user(" (nothing in ring)\n");
			break;
		}

		flags = lws_write_ws_flags(
			    pmsg->binary ? LWS_WRITE_BINARY : LWS_WRITE_TEXT,
			    pmsg->first, pmsg->final);

		/* notice we allowed for LWS_PRE in the payload already */
		m = lws_write(wsi, ((unsigned char *)pmsg->payload) +
			      LWS_PRE, pmsg->len, (enum lws_write_protocol)flags);
		if (m < (int)pmsg->len) {
			lwsl_err("ERROR %d writing to ws socket\n", m);
			return -1;
		}

		lwsl_user(" wrote %d: flags: 0x%x first: %d final %d\n",
				m, flags, pmsg->first, pmsg->final);
		/*
		 * Workaround deferred deflate in pmd extension by only
		 * consuming the fifo entry when we are certain it has been
		 * fully deflated at the next WRITABLE callback.  You only need
		 * this if you're using pmd.
		 */
		pss->write_consume_pending = 1;
		lws_callback_on_writable(wsi);

		if (pss->flow_controlled &&
		    (int)lws_ring_get_count_free_elements(pss->ring) > RING_DEPTH - 5) {
			lws_rx_flow_control(wsi, 1);
			pss->flow_controlled = 0;
		}

		if ((*vhd->options & 1) && pmsg && pmsg->final)
			pss->completed = 1;

		break;

	case LWS_CALLBACK_RECEIVE:

		lwsl_user("LWS_CALLBACK_RECEIVE: %4d (rpp %5d, first %d, "
			  "last %d, bin %d, msglen %d (+ %d = %d))\n",
			  (int)len, (int)lws_remaining_packet_payload(wsi),
			  lws_is_first_fragment(wsi),
			  lws_is_final_fragment(wsi),
			  lws_frame_is_binary(wsi), pss->msglen, (int)len,
			  (int)pss->msglen + (int)len);

		if (len) {
			;
			//puts((const char *)in);
			//lwsl_hexdump_notice(in, len);
		}

		amsg.first = (char)lws_is_first_fragment(wsi);
		amsg.final = (char)lws_is_final_fragment(wsi);
		amsg.binary = (char)lws_frame_is_binary(wsi);
		n = (int)lws_ring_get_count_free_elements(pss->ring);
		if (!n) {
			lwsl_user("dropping!\n");
			break;
		}

		if (amsg.final)
			pss->msglen = 0;
		else
			pss->msglen += (uint32_t)len;

		amsg.len = len;
		/* notice we over-allocate by LWS_PRE */
		amsg.payload = malloc(LWS_PRE + len);
		if (!amsg.payload) {
			lwsl_user("OOM: dropping\n");
			break;
		}

		memcpy((char *)amsg.payload + LWS_PRE, in, len);
		if (!lws_ring_insert(pss->ring, &amsg, 1)) {
			__minimal_destroy_message(&amsg);
			lwsl_user("dropping!\n");
			break;
		}
		lws_callback_on_writable(wsi);

		if (n < 3 && !pss->flow_controlled) {
			pss->flow_controlled = 1;
			lws_rx_flow_control(wsi, 0);
		}
		break;

	case LWS_CALLBACK_CLOSED:
		lwsl_user("LWS_CALLBACK_CLOSED\n");
		lws_ring_destroy(pss->ring);

		if (*vhd->options & 1) {
			if (!*vhd->interrupted)
				*vhd->interrupted = 1 + pss->completed;
			lws_cancel_service(lws_get_context(wsi));
		}
		break;

	default:
		break;
	}

	return 0;
}

#define SP_WS_PROTOCOL_ECHO \
	{ \
		"lws-minimal-server-echo", \
		callback_minimal_server_echo, \
		sizeof(struct per_session_data__minimal_server_echo), \
		1024, \
		0, NULL, 0 \
	}


static struct lws_protocols protocols[] =
{
	SP_WS_PROTOCOL_ECHO,
	LWS_PROTOCOL_LIST_TERM
};

int main(int arg_count, char** args) {
  dn_init();

  printf("hello, world!\n");

  lws_set_log_level(LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE, NULL);
  lwsl_user("spums");

  struct lws_context_creation_info lws_info = dn_zero_initialize();
  lws_info.port = 8000;
  lws_info.protocols = protocols;
  lws_info.pt_serv_buf_size = 32 * 1024;
	lws_info.options = LWS_SERVER_OPTION_VALIDATE_UTF8 | LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;
  printf("hello, world! 2\n");

  struct lws_context *context = lws_create_context( &lws_info );
  // struct lws_context *context = NULL;
  printf("hello, world! 3\n");

	while(true) {
		lws_service( context, /* timeout_ms = */ 1000000 );
	}

	lws_context_destroy( context );

	return 0;
}
