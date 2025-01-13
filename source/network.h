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
  SP_OPCODE_ECHO = 0,
  SP_OPCODE_REQUEST_TOKEN = 1,
  SP_OPCODE_MATCH_REQUEST = 10,
  SP_OPCODE_BEGIN_MATCH = 20,
  SP_OPCODE_MATCH_EVENT = 100
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
  sp_net_magic_t magic;
  sp_token_t token;
  sp_net_opcode_t op;
  union {
    sp_net_echo_request_t echo;
    sp_net_match_request_t match;
    sp_net_token_request_t token_req;
  };
} sp_net_request_t;


//////////////////
// MATCH EVENTS //
//////////////////
typedef struct {
  sp_username_t username;
  sp_card_id_t hand [SP_HAND_SIZE];
  sp_pokemon_type_t energy [2];
  sp_pokemon_type_t opponent_energy [2];
  sp_turn_order_t turn;
} sp_net_begin_match_data_t;


typedef struct {
  sp_match_event_t kind;

  union {
    sp_net_begin_match_data_t begin;
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