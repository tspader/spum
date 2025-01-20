#ifndef SP_LWS_H
#define SP_LWS_H
dn_string_t lws_event_to_string(enum lws_callback_reasons event);

#define sp_log_lws_event(event) \
 dn_string_t dn_unique_name() = lws_event_to_string(event); \
 lwsl_user("%s: event = %s (%d)\n", __func__, dn_string_to_cstr(dn_unique_name()), event);

#endif

#ifdef SP_LWS_IMPL
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
#endif