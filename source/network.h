#ifndef SP_NETWORK_H
#define SP_NETWORK_H
// #define SP_MESSAGE_BUFFER_LEN 2048
// #define SP_MESSAGE_LEN (LWS_SEND_BUFFER_PRE_PADDING + SP_MESSAGE_BUFFER_LEN + LWS_SEND_BUFFER_POST_PADDING)

typedef enum {
  SP_OPCODE_SEARCH_FOR_MATCH,
} sp_net_opcode_t;

#define SP_MAX_PASSWORD_LEN 16
typedef struct {
  u8 password [SP_MAX_PASSWORD_LEN];
} sp_net_match_request_t;

typedef union {
  sp_net_match_request_t match_request;
} sp_net_payload_t;

typedef struct {
  u8 magic [8];
  sp_net_opcode_t op;
  sp_net_payload_t payload;
} sp_net_message_t;
#endif