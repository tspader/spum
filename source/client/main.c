#include "emscripten/emscripten.h"
#include "emscripten/websocket.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_VARARGS

// #define NK_INCLUDE_STANDARD_BOOL
// #define NK_INCLUDE_COMMAND_USERDATA
// #define NK_UINT_DRAW_INDEX
#define NK_IMPLEMENTATION
#include "nuklear.h"

#define SOKOL_IMPL
#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#include "sokol_nuklear.h"

#define DN_MATH_BACKEND_HANDMADE
#define DN_IMPL
#include "dn.h"

#include "network.h"
#include "app.h"

#define SP_NETWORK_IMPL
#include "network.h"
#define SP_CLIENT_APP_IMPL
#include "app.h"

typedef enum {
    SP_POKEMON_TYPE_NONE,
    SP_POKEMON_TYPE_GRASS,
} sp_pokemon_type_t;

typedef enum {
    SP_POKEMON_EX_NONE,
    SP_POKEMON_EX,
} sp_pokemon_ex_t;

typedef enum {
    SP_POKEMON_STAGE_NONE,
    SP_POKEMON_STAGE_BASIC,
    SP_POKEMON_STAGE_1,
    SP_POKEMON_STAGE_2,
    SP_POKEMON_STAGE_FOSSIL,
} sp_pokemon_stage_t;

typedef enum {
    SP_MOVE_NONE,
    SP_MOVE_BULBASAUR_VINE_WHIP,
} sp_pokemon_move_t;

typedef enum {
    SP_ABILITY_NONE,
    SP_ABILITY_FRAGRANT_TRAP,
} sp_pokemon_ability_t;

typedef struct {
    sp_pokemon_ex_t ex;
    sp_pokemon_type_t type;
    sp_pokemon_stage_t stage;
    sp_pokemon_move_t moves [2];
    sp_pokemon_ability_t ability;
    u32 hp;
} sp_pokemon_t;

typedef enum {
    SP_CARD_NONE,
    SP_CARD_BULBASAUR,
    SP_CARD_POTION,
    SP_CARD_PROFESSORS_RESEARCH,
} sp_card_id_t;

typedef enum {
    SP_CARD_KIND_POKEMON,
    SP_CARD_KIND_ITEM,
    SP_CARD_KIND_TRAINER,
} sp_card_kind_t;

typedef struct {
    sp_card_id_t id;
    sp_card_kind_t kind;
    sp_pokemon_t pokemon;
} sp_card_t;

typedef struct {
    sp_card_t cards [20];
    sp_pokemon_type_t energy [3];
} sp_deck_t;


///////////
// MATCH //
///////////
typedef enum {
    SP_MATCH_STATE_INIT,
    SP_MATCH_STATE_SETUP,
    SP_MATCH_STATE_TURN_IDLE,
} sp_match_state_t;

typedef struct {
    sp_pokemon_t info;
    u32 hp;
} sp_active_pokemon_t;

typedef struct {
    sp_pokemon_type_t energy [3];
    sp_card_t deck [20];
    sp_card_t hand [10];
    sp_card_t discard [20];
    sp_active_pokemon_t active;
    sp_active_pokemon_t bench [3];
    u32 time_remaining;
} sp_player_t;

typedef struct {
    sp_match_state_t state;
    sp_player_t players [2];
} sp_match_t;

void sp_match_update(sp_match_t* match);


/////////
// RNG //
/////////
typedef enum {
    SP_RNG_FLIP_NONE,
    SP_RNG_FLIP_HEADS,
    SP_RNG_FLIP_TAILS,
} sp_rng_flip_t;

typedef struct {
    u32 seed;
} sp_rng_t;
sp_rng_t sp_rng;


////////////////////
// IMPLEMENTATION //
////////////////////
void sp_match_update(sp_match_t* match) {
    switch (match->state) {
        case SP_MATCH_STATE_INIT: {
            // flip for who goes first
            // generate initial hands
            break;
        }
        case SP_MATCH_STATE_SETUP: {
            // wait for everyone to place their mons
            break;
        }
        case SP_MATCH_STATE_TURN_IDLE: {
            // wait for everyone to place their mons
            break;
        }
    }
}

void sp_rng_init();
sp_rng_flip_t sp_rng_flip();

void sp_rng_init() {
    sp_rng.seed = 69;
    srand(sp_rng.seed);
}

sp_rng_flip_t sp_rng_flip() {
    return rand() % 2 ? SP_RNG_FLIP_HEADS : SP_RNG_FLIP_TAILS;
}

sp_card_t cards [] = {
    { 0 },
    {
        .id = SP_CARD_BULBASAUR,
        .kind = SP_CARD_KIND_POKEMON,
        .pokemon = {
            .ex = SP_POKEMON_EX_NONE,
            .type = SP_POKEMON_TYPE_GRASS,
            .stage = SP_POKEMON_STAGE_BASIC,
            .moves = {
                SP_MOVE_BULBASAUR_VINE_WHIP,
            },
            .ability = SP_ABILITY_NONE,
            .hp = 70
        }
    }
};

sapp_desc sokol_main(int num_args, char** args) {
    return (sapp_desc){
        .init_cb = sp_client_init,
        .frame_cb = sp_client_update,
        .cleanup_cb = sp_client_shutdown,
        .event_cb = sp_client_event,
        .width = 400,
        .height = 300,
        .window_title = "spum",
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}