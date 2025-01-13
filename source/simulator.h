#ifndef SP_SIMULATOR_H
#define SP_SIMULATOR_H
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


///////////////
// GAME DATA //
///////////////
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
    SP_CARD_IVYSAUR,
    SP_CARD_VENUSAUR,
    SP_CARD_VENUSAUR_EX,
    SP_CARD_CATERPIE,
    SP_CARD_METAPOD,
    SP_CARD_BUTTERFREE,
    SP_CARD_WEEDLE,
    SP_CARD_KAKUNA,
    SP_CARD_BEEDRILL,
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

#define SP_DECK_SIZE 20
#define SP_DECK_MAX_INDEX (SP_DECK_SIZE - 1)
#define SP_FIRST_HAND_SIZE 5
#define SP_HAND_SIZE 10
#define SP_DECK_MAX_ENERGY 3

typedef struct {
    sp_card_id_t cards [SP_DECK_SIZE];
    sp_pokemon_type_t energy [SP_DECK_MAX_ENERGY];
} sp_deck_t;


typedef struct {
    gs_hash_table(sp_card_id_t, u32) cards;
} sp_deck_count_t;


///////////
// MATCH //
///////////
typedef enum {
    SP_MATCH_STATE_INIT,
    SP_MATCH_STATE_SETUP,
    SP_MATCH_STATE_TURN_IDLE,
} sp_match_state_t;

typedef enum {
    SP_MATCH_EVENT_BEGIN,
} sp_match_event_t;

typedef enum {
    SP_TURN_ORDER_FIRST,
    SP_TURN_ORDER_SECOND,
} sp_turn_order_t;

typedef struct {
    sp_pokemon_t info;
    u32 hp;
} sp_active_pokemon_t;

typedef struct {
    sp_deck_t deck_list;
    sp_card_id_t deck [SP_DECK_SIZE];
    sp_card_id_t hand [SP_HAND_SIZE];
    sp_card_id_t discard [SP_DECK_SIZE];
    sp_active_pokemon_t active;
    sp_active_pokemon_t bench [3];
    sp_pokemon_type_t energy [2];
    u32 time_remaining;
} sp_player_t;

typedef struct {
    sp_match_state_t state;
    sp_rng_flip_t order_flip;
    sp_player_t players [2];
} sp_match_data_t;

typedef struct {
    sp_card_id_t* cards;
    u32 max_cards;
    i32 index;
} sp_card_iter_t;



void            sp_rng_init();
u32             sp_rng_int();
u32             sp_rng_ranged_u32(u32 inclusive_min, u32 inclusive_max);
sp_rng_flip_t   sp_rng_flip();
u32             sp_rng_pick_from_set(u32* options, u32 num_options);
void            sp_match_init(sp_match_data_t* match, sp_deck_t decks [2]);
void            sp_match_update(sp_match_data_t* match);
bool            sp_match_is_player_first(sp_match_data_t* match, sp_player_t* player);
sp_turn_order_t sp_match_turn_order(sp_match_data_t* match, sp_player_t* player);
void            sp_player_init(sp_player_t* player, sp_deck_t* deck);
void            sp_player_shuffle(sp_player_t* player);
void            sp_player_shuffle_hand_into_deck(sp_player_t* player);
u32             sp_player_deck_size(sp_player_t* player);
u32             sp_player_hand_size(sp_player_t* player);
u32             sp_player_discard_size(sp_player_t* player);
void            sp_player_draw(sp_player_t* player);
void            sp_player_draw_n(sp_player_t* player, u32 num_cards);
bool            sp_player_is_starting_hand_valid(sp_player_t* player);
void            sp_player_generate_energy(sp_player_t* player);
sp_deck_count_t sp_deck_count(sp_deck_t* deck);
void            sp_deck_count_remove_cards(sp_deck_count_t* count, sp_card_id_t* cards, u32 num_cards);
void            sp_deck_print(sp_deck_t* deck);
void            sp_deck_print_ordered(sp_deck_t* deck);
sp_deck_t       sp_deck_gen_random();
dn_string_t     sp_card_set_to_string(sp_card_set_t set);
dn_string_t     sp_card_set_to_short_string(sp_card_set_t set);
sp_card_iter_t  sp_card_iter_forward(sp_card_id_t* cards, u32 max_cards);
sp_card_iter_t  sp_card_iter_backward(sp_card_id_t* cards, u32 max_cards);
bool            sp_card_iter_valid(sp_card_iter_t* it);
void            sp_card_iter_next(sp_card_iter_t* it);
void            sp_card_iter_prev(sp_card_iter_t* it);
sp_card_id_t    sp_card_iter_get(sp_card_iter_t* it);
#define         sp_for_card(it, cards) for (sp_card_iter_t it = sp_card_iter_forward(cards, sizeof(cards)); sp_card_iter_valid(&it); sp_card_iter_next(&it))
#define         sp_for_card_backward(it, cards) for (sp_card_iter_t it = sp_card_iter_backward(cards, sizeof(cards)); sp_card_iter_valid(&it); sp_card_iter_prev(&it))
#define         sp_for_card_n(it, cards, n) for (sp_card_iter_t it = sp_card_iter_forward(cards, n); sp_card_iter_valid(&it); sp_card_iter_next(&it))

//////////
// DATA //
//////////
#define SP_UNIMPLEMENTED_POKEMON(card) dn_zero_initialize()
sp_card_t sp_cards [] = {
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
    SP_UNIMPLEMENTED_POKEMON(SP_CARD_IVYSAUR),
    SP_UNIMPLEMENTED_POKEMON(SP_CARD_VENUSAUR),
    SP_UNIMPLEMENTED_POKEMON(SP_CARD_VENUSAUR_EX),
    SP_UNIMPLEMENTED_POKEMON(SP_CARD_CATERPIE),
    SP_UNIMPLEMENTED_POKEMON(SP_CARD_METAPOD),
    SP_UNIMPLEMENTED_POKEMON(SP_CARD_BUTTERFREE),
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
    SP_UNIMPLEMENTED_POKEMON(SP_CARD_KAKUNA),
    SP_UNIMPLEMENTED_POKEMON(SP_CARD_BEEDRILL),
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

//////////////////////
// SP CARD ITERATOR //
//////////////////////
sp_card_iter_t sp_card_iter_forward(sp_card_id_t* cards, u32 max_cards) {
    return (sp_card_iter_t) {
        .cards = cards,
        .max_cards = max_cards,
        .index = 0
    };
}

sp_card_iter_t sp_card_iter_backward(sp_card_id_t* cards, u32 max_cards) {
    u32 num_cards = 0;
    dn_for(index, max_cards) { 
        if (cards[index] == SP_CARD_NONE) break;
        num_cards++;
    }

    return (sp_card_iter_t) {
        .cards = cards,
        .max_cards = max_cards,
        .index = num_cards - 1
    };
}

bool sp_card_iter_valid(sp_card_iter_t* it) {
    return (it->index < it->max_cards) && (it->index >= 0) && (sp_card_iter_get(it) != SP_CARD_NONE);
}

void sp_card_iter_next(sp_card_iter_t* it) {
    it->index++;
}

void sp_card_iter_prev(sp_card_iter_t* it) {
    it->index--;
}

sp_card_id_t sp_card_iter_get(sp_card_iter_t* it) {
    return it->cards[it->index];
}


/////////////
// SP DECK //
/////////////
sp_deck_count_t sp_deck_count(sp_deck_t* deck) {
    sp_deck_count_t count = dn_zero_initialize();

    dn_for(i, 20) {
        sp_card_id_t card = deck->cards[i];
        if (!gs_hash_table_exists(count.cards, card)) {
            gs_hash_table_insert(count.cards, card, 0);
        }
        (*gs_hash_table_getp(count.cards, card))++;
    }

    return count;
}

void sp_deck_count_remove_cards(sp_deck_count_t* count, sp_card_id_t* cards, u32 num_cards) {
    sp_for_card_n(it, cards, num_cards) {
        sp_card_id_t id = sp_card_iter_get(&it);
        u32* num_instances = gs_hash_table_getp(count->cards, id);
        *num_instances -= 1;
    }
}

void sp_deck_print_ordered(sp_deck_t* deck) {
    sp_for_card(it, deck->cards) {
        sp_card_t card = sp_cards[sp_card_iter_get(&it)];
        DN_LOG("%.*s", card.pokemon.name.len, card.pokemon.name.data);
    }
}

void sp_deck_print(sp_deck_t* deck) {
    sp_deck_count_t count = sp_deck_count(deck);

    gs_hash_table_for(count.cards, it) {
        sp_card_id_t card_id = gs_hash_table_iter_getk(count.cards, it);
        u32 num_instances = gs_hash_table_iter_get(count.cards, it);
        sp_card_t card = sp_cards[card_id];

        DN_LOG("(%d) %s %s-%d", 
            num_instances, 
            dn_string_to_cstr(card.pokemon.name), 
            dn_string_to_cstr(sp_card_set_to_short_string(card.set)), 
            card.set_id);
    }
}

sp_deck_t sp_deck_gen_random() {
    sp_deck_t deck = (sp_deck_t){
        .cards = dn_zero_initialize(),
        .energy = { SP_POKEMON_TYPE_GRASS, SP_POKEMON_TYPE_NONE, SP_POKEMON_TYPE_NONE }
    };
    dn_for(i, 20) {
        deck.cards[i] = sp_rng_pick_from_set((u32 []) { SP_CARD_BULBASAUR, SP_CARD_WEEDLE, SP_CARD_ODDISH }, 3);
    }

    return deck;
}


//////////////
// SP MATCH //
//////////////
void sp_match_init(sp_match_data_t* match, sp_deck_t decks [2]) {
    dn_os_zero_memory(match, sizeof(sp_match_data_t));
    
    match->state = SP_MATCH_STATE_SETUP;
    match->order_flip = sp_rng_flip();
    
    dn_for(i, 2) {
        sp_deck_t* deck = &decks[i];
        sp_player_t* player = &match->players[i];
        sp_player_init(player, deck);

        while (!sp_player_is_starting_hand_valid(player)) {
            sp_player_shuffle_hand_into_deck(player);
            sp_player_draw_n(player, 5);
        }

        sp_player_generate_energy(player);
        if (sp_match_is_player_first(match, player)) {
            sp_player_generate_energy(player);
        }
    }
}

void sp_match_update(sp_match_data_t* match) {
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

bool sp_match_is_player_first(sp_match_data_t* match, sp_player_t* player) {
    u32 index = player - match->players;
    if ((index == 0) && match->order_flip == SP_RNG_FLIP_HEADS) return true;
    if ((index == 1) && match->order_flip == SP_RNG_FLIP_TAILS) return true;
    return false;
}

sp_turn_order_t sp_match_turn_order(sp_match_data_t* match, sp_player_t* player) {
    if (sp_match_is_player_first(match, player)) return SP_TURN_ORDER_FIRST;
    return SP_TURN_ORDER_SECOND;
}


///////////////
// SP PLAYER //
///////////////
void sp_player_init(sp_player_t* player, sp_deck_t* deck) {
    dn_os_memory_copy(deck, &player->deck_list, sizeof(sp_deck_t));
    dn_os_memory_copy(deck->cards, &player->deck, sizeof(player->deck));
}

void sp_player_shuffle_hand_into_deck(sp_player_t* player) {
    u32 deck_size = sp_player_deck_size(player);

    sp_for_card(it, player->hand) {
        player->deck[it.index] = sp_card_iter_get(&it);
        player->hand[it.index] = SP_CARD_NONE;
    }

    sp_player_shuffle(player);
}

void sp_player_shuffle(sp_player_t* player) {
    sp_for_card_backward(it, player->deck) {
        u32 swap_index = sp_rng_ranged_u32(0, it.index);
        dn_swap(sp_card_id_t, player->deck[swap_index], player->deck[it.index]);
    }
}

u32 sp_player_deck_size(sp_player_t* player) {
    u32 num_cards = 0;
    sp_for_card(it, player->deck) {
        num_cards++;
    }

    return num_cards;
}

u32 sp_player_hand_size(sp_player_t* player) {
    u32 num_cards = 0;
    sp_for_card(it, player->hand) {
        num_cards++;
    }
    
    return num_cards;
}

u32 sp_player_discard_size(sp_player_t* player) {
    u32 num_cards = 0;
    sp_for_card(it, player->discard) {
        num_cards++;
    }
    
    return num_cards;
}

void sp_player_draw(sp_player_t* player) {
    u32 deck_size = sp_player_deck_size(player);
    DN_ASSERT(deck_size);
    u32 hand_size = sp_player_hand_size(player);
    DN_ASSERT(hand_size < SP_HAND_SIZE);
    
    player->hand[hand_size] = player->deck[0];
    dn_for(index, deck_size - 1) {
        player->deck[index] = player->deck[index + 1];
    }
    player->deck[deck_size - 1] = SP_CARD_NONE;
}

void sp_player_draw_n(sp_player_t* player, u32 num_cards) {
    num_cards = dn_min(num_cards, sp_player_deck_size(player));
    dn_for(i, num_cards) {
        sp_player_draw(player);
    }
}

bool sp_player_is_starting_hand_valid(sp_player_t* player) {
    sp_for_card(it, player->hand) {
        sp_card_t* card = &sp_cards[sp_card_iter_get(&it)];
        if (card->pokemon.stage == SP_POKEMON_STAGE_BASIC) {
            return true;
        }
    }

    return false;
}

void sp_player_generate_energy(sp_player_t* player) {
    sp_pokemon_type_t* energy = player->deck_list.energy;
    u32 num_energy = 0;
    dn_for(index, SP_DECK_MAX_ENERGY) {
        if (energy[index] == SP_POKEMON_TYPE_NONE) break;
        num_energy++;
    }

    player->energy[0] = player->energy[1];
    player->energy[1] = sp_rng_pick_from_set(energy, num_energy);
}

void sp_rng_init() {
    sp_rng.seed = time(NULL);
    srand(sp_rng.seed);
}

u32 sp_rng_int() {
    return rand();
}

u32 sp_rng_ranged_u32(u32 inclusive_min, u32 inclusive_max) {
    if (inclusive_min > inclusive_max) {
        dn_swap(u32, inclusive_min, inclusive_max);
    }

    u32 range = inclusive_max - inclusive_min + 1;

    // Avoid modulo bias by discarding out-of-range results
    u32 limit = RAND_MAX - (RAND_MAX % range);
    u32 random_value;
    do {
        random_value = rand();
    } while (random_value >= limit);

    return inclusive_min + (random_value % range);
}

sp_rng_flip_t sp_rng_flip() {
    return sp_rng_int() % 2 ? SP_RNG_FLIP_HEADS : SP_RNG_FLIP_TAILS;
}

u32 sp_rng_pick_from_set(u32* options, u32 num_options) {
    u32 index = sp_rng_int() % num_options;
    return options[index];
}

dn_string_t sp_card_set_to_string(sp_card_set_t set) {
    switch (set) {
        case SP_CARD_SET_GENETIC_APEX: return dn_string_literal("Genetic Apex");
        case SP_CARD_SET_MYTHICAL_ISLAND: return dn_string_literal("Mythical Island");
    }
}

dn_string_t sp_card_set_to_short_string(sp_card_set_t set) {
    switch (set) {
        case SP_CARD_SET_GENETIC_APEX: return dn_string_literal("A1");
        case SP_CARD_SET_MYTHICAL_ISLAND: return dn_string_literal("A1a");
    }
}

#endif