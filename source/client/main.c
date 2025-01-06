// #include "KHR/khrplatform.h"
// #include "glad/glad.h"

#define SOKOL_GFX_IMPL
#include "sokol_gfx.h"
#define SOKOL_APP_IMPL
#include "sokol_app.h"
#define SOKOL_LOG_IMPL
#include "sokol_log.h"
#define SOKOL_GLUE_IMPL
#include "sokol_glue.h"

#define DN_MATH_BACKEND_HANDMADE
#define DN_IMPL
#include "dn.h"

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

typedef struct {
    sg_pass_action pass_action;
} sp_app_t;
sp_app_t sp_app;

void sp_init(void) {
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });
    sp_app.pass_action = (sg_pass_action) {
        .colors[0] = {
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = { 1.0f, 0.0f, 0.0f, 1.0f }
        }
    };
}

void sp_update(void) {
    float g = sp_app.pass_action.colors[0].clear_value.g + 0.01f;
    sp_app.pass_action.colors[0].clear_value.g = (g > 1.0f) ? 0.0f : g;

    sg_begin_pass(&(sg_pass){ 
        .action = sp_app.pass_action, 
        .swapchain = sglue_swapchain() 
    });
    sg_end_pass();
    sg_commit();
}

void sp_shutdown(void) {
    sg_shutdown();
}

sapp_desc sokol_main(int num_args, char** args) {
    dn_allocators_init();
    dn_allocator_t* bump = dn_allocator_find("bump");

    return (sapp_desc){
        .init_cb = sp_init,
        .frame_cb = sp_update,
        .cleanup_cb = sp_shutdown,
        .width = 400,
        .height = 300,
        .window_title = "spum",
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}