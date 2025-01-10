#ifndef SP_SIMULATOR_H
#define SP_SIMULATOR_H
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
    SP_MOVE_WEEDLE_STING,
    SP_MOVE_ODDISH_RAM,
} sp_pokemon_move_t;

typedef enum {
    SP_ABILITY_NONE,
    SP_ABILITY_FRAGRANT_TRAP,
} sp_pokemon_ability_t;

typedef struct {
    dn_string_t name;
    sp_pokemon_ex_t ex;
    sp_pokemon_type_t type;
    sp_pokemon_stage_t stage;
    sp_pokemon_move_t moves [2];
    sp_pokemon_ability_t ability;
    u32 hp;
} sp_pokemon_t;

typedef enum {
    SP_CARD_SET_GENETIC_APEX,
    SP_CARD_SET_MYTHICAL_ISLAND,
} sp_card_set_t;

typedef enum {
    SP_CARD_NONE,
    SP_CARD_BULBASAUR,
    SP_CARD_WEEDLE,
    SP_CARD_ODDISH,
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
    sp_card_set_t set;
    u32 set_id;
    sp_pokemon_t pokemon;
} sp_card_t;

typedef struct {
    sp_card_id_t cards [20];
    sp_pokemon_type_t energy [3];
} sp_deck_t;

typedef struct {
    gs_hash_table(sp_card_id_t, u32) cards;
} sp_deck_count_t;

sp_deck_count sp_deck_count(sp_deck_t* deck);


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


void sp_rng_init();
u32 sp_rng_int();
sp_rng_flip_t sp_rng_flip();
u32 sp_rng_pick_from_set(u32* options, u32 num_options);


//////////
// DATA //
//////////
sp_card_t cards [] = {
    dn_zero_initialize(),
    {
        .id = SP_CARD_BULBASAUR,
        .kind = SP_CARD_KIND_POKEMON,
        .set = SP_CARD_SET_GENETIC_APEX,
        .set_id = 1,
        .pokemon = {
            .name = dn_string_literal("Bulbasaur"),
            .ex = SP_POKEMON_EX_NONE,
            .type = SP_POKEMON_TYPE_GRASS,
            .stage = SP_POKEMON_STAGE_BASIC,
            .moves = {
                SP_MOVE_BULBASAUR_VINE_WHIP,
            },
            .ability = SP_ABILITY_NONE,
            .hp = 70
        }
    },
    {
        .id = SP_CARD_WEEDLE,
        .kind = SP_CARD_KIND_POKEMON,
        .set = SP_CARD_SET_GENETIC_APEX,
        .set_id = 8,        
        .pokemon = {
            .name = dn_string_literal("Weedle"),
            .ex = SP_POKEMON_EX_NONE,
            .type = SP_POKEMON_TYPE_GRASS,
            .stage = SP_POKEMON_STAGE_BASIC,
            .moves = {
                SP_MOVE_WEEDLE_STING,
            },
            .ability = SP_ABILITY_NONE,
            .hp = 50
        }
    },
    {
        .id = SP_CARD_ODDISH,
        .kind = SP_CARD_KIND_POKEMON,
        .set = SP_CARD_SET_GENETIC_APEX,
        .set_id = 11,
        .pokemon = {
            .name = dn_string_literal("Oddish"),
            .ex = SP_POKEMON_EX_NONE,
            .type = SP_POKEMON_TYPE_GRASS,
            .stage = SP_POKEMON_STAGE_BASIC,
            .moves = {
                SP_MOVE_ODDISH_RAM,
            },
            .ability = SP_ABILITY_NONE,
            .hp = 60
        }
    }
};

#endif

////////////////////
// IMPLEMENTATION //
////////////////////
#ifdef SP_SIMULATOR_IMPL
u32 sp_deck_count(sp_deck_t* deck, sp_card_id_t card_id) {
    u32 count = 0;
    for (i, 20) {
        sp_card_t* card = deck->cards + i;
        if (card->id == card_id) {
            count++;
        }
    }

    return count;
}

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

void sp_rng_init() {
    sp_rng.seed = 69;
    srand(sp_rng.seed);
}

u32 sp_rng_int() {
    return rand();
}

sp_rng_flip_t sp_rng_flip() {
    return sp_rng_int() % 2 ? SP_RNG_FLIP_HEADS : SP_RNG_FLIP_TAILS;
}

u32 sp_rng_pick_from_set(u32* options, u32 num_options) {
    u32 index = sp_rng_int() % num_options;
    return options[index];
}

#endif