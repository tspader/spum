#ifndef SP_NETWORK_H
#define SP_NETWORK_H

// store user and password combination in the map
// figure out how to periodically purge entries from this map (how to make lws_service return immediately when there is nothing, is this even the right model)
// make the client ping the server periodically to see if the request is done?
// cache the WSI to send from server to client?
// make a buffer of these messages to send, when you get a match put it in there and clear out the data structure
// add basic buttons 

typedef u64 sp_token_t;
typedef u64 sp_match_id_t;

typedef u8 sp_net_magic_t [8];
sp_net_magic_t sp_magic = { 0x69, 0x69, 0x69, 'j', 'e', 'r', 'r', 'y'};

#define SP_MAX_USERNAME_LEN 64
typedef dn_str_buffer_t(SP_MAX_USERNAME_LEN) sp_username_t;

/////////////////////////
// CLIENT REQUESTS OPS //
/////////////////////////
typedef enum {
  SP_OPCODE_ECHO,
  SP_OPCODE_REQUEST_TOKEN,
  SP_OPCODE_MATCH_REQUEST,
  SP_OPCODE_MATCH_EVENT,
  SP_OPCODE_CLIENT_MATCH_ACTION
} sp_net_opcode_t;

#define SP_MAX_PASSWORD_LEN 16
typedef u8 sp_net_match_password_t [SP_MAX_PASSWORD_LEN];

typedef struct {
  dn_hash_t password;
  sp_deck_t deck;
} sp_net_match_request_t;

typedef struct {
  u32 value;
} sp_net_echo_request_t;

typedef struct {
  sp_username_t username;
} sp_net_token_request_t;

typedef struct {
  sp_match_action_t action;
} sp_net_match_action_request_t;

typedef struct {
  sp_net_magic_t magic;
  sp_token_t token;
  sp_net_opcode_t op;
  union {
    sp_net_echo_request_t echo;
    sp_net_match_request_t match;
    sp_net_token_request_t token_req;
    sp_net_match_action_request_t match_action;
  };
} sp_net_request_t;


//////////////////
// MATCH EVENTS //
//////////////////
typedef enum {
    SP_NET_MATCH_EVENT_KIND_NONE,
    SP_NET_MATCH_EVENT_KIND_FOUND_OPPONENT,
    SP_NET_MATCH_EVENT_KIND_BEGIN,
    SP_NET_MATCH_EVENT_KIND_SYNC,
    SP_NET_MATCH_EVENT_KIND_ACTION_RESULT,
} sp_net_match_event_kind_t;

typedef struct {
  sp_token_t token;
  sp_match_action_t action;
  sp_match_action_result_t result;
} sp_net_match_action_event_t;

typedef struct {
  sp_username_t username;
  sp_match_player_id_t your_match_id;
} sp_net_match_found_opponent_event_t;

typedef struct {
  sp_net_match_event_kind_t kind;

  union {
    sp_net_match_found_opponent_event_t found;
    sp_match_data_t state;
    sp_net_match_action_event_t action;
  };
} sp_net_match_event_t;


//////////////////////
// SERVER RESPONSES //
//////////////////////
typedef struct {
  u32 success;
} sp_net_echo_response_t;

typedef struct {
  sp_token_t token;
} sp_net_request_token_response_t;


typedef struct {
  sp_net_magic_t magic;
  sp_net_opcode_t op;

  union {
    sp_net_echo_response_t echo;
    sp_net_request_token_response_t request_token;
    sp_net_match_event_t match_event;
  };
} sp_net_response_t;

#endif