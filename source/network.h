#ifndef SP_NETWORK_H
#define SP_NETWORK_H

// store user and password combination in the map
// figure out how to periodically purge entries from this map (how to make lws_service return immediately when there is nothing, is this even the right model)
// make the client ping the server periodically to see if the request is done?
// cache the WSI to send from server to client?
// make a buffer of these messages to send, when you get a match put it in there and clear out the data structure
// add basic buttons 

typedef u64 sp_token_t;

typedef u8 sp_net_magic_t [8];
sp_net_magic_t sp_magic = { 0x69, 0x69, 0x69, 'j', 'e', 'r', 'r', 'y'};


/////////////////////////
// CLIENT REQUESTS OPS //
/////////////////////////
typedef enum {
  SP_OPCODE_ECHO = 0,
  SP_OPCODE_REQUEST_TOKEN = 1,
  SP_OPCODE_MATCH_REQUEST = 10,
} sp_net_opcode_t;

#define SP_MAX_PASSWORD_LEN 16
typedef u8 sp_net_match_password_t [SP_MAX_PASSWORD_LEN];

typedef struct {
  sp_net_match_password_t password;
} sp_net_match_request_t;

typedef struct {
  u32 value;
} sp_net_echo_request_t;


/////////////
// REQUEST //
/////////////
typedef union {
  sp_net_echo_request_t echo;
  sp_net_match_request_t match;
} sp_net_request_payload_t;

typedef struct {
  sp_net_magic_t magic;
  sp_net_opcode_t op;
  sp_net_request_payload_t payload;
} sp_net_request_t;


//////////////////////
// SERVER RESPONSES //
//////////////////////
typedef struct {
  u32 success;
} sp_net_echo_response_t;

typedef union {
  sp_net_echo_response_t echo;
} sp_net_response_payload_t;

typedef struct {
  sp_net_magic_t magic;
  sp_net_opcode_t op;
  sp_net_response_payload_t payload;
} sp_net_response_t;

#endif