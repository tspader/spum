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
#define SP_POKEMON_MAX_MOVES 2
#define SP_POKEMON_MOVE_MAX_ENERGY 5
#define SP_DECK_SIZE 20
#define SP_DECK_MAX_INDEX (SP_DECK_SIZE - 1)
#define SP_DISCARD_SIZE 20
#define SP_FIRST_HAND_SIZE 5
#define SP_HAND_SIZE 10
#define SP_DECK_MAX_ENERGY 3
#define SP_BENCH_SIZE 3
#define SP_NUM_PLAYERS 2

typedef enum {
  SP_POKEMON_TYPE_NONE,
  SP_POKEMON_TYPE_COLORLESS,
  SP_POKEMON_TYPE_GRASS,
  SP_POKEMON_TYPE_FIRE,
  SP_POKEMON_TYPE_COUNT,
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
} sp_move_id_t;

typedef struct {
  sp_move_id_t id;
  u32 damage;
  sp_pokemon_type_t energy [SP_POKEMON_MOVE_MAX_ENERGY];
  dn_string_t name;
} sp_move_t;

typedef enum {
  SP_ABILITY_NONE,
  SP_ABILITY_FRAGRANT_TRAP,
} sp_pokemon_ability_t;

typedef struct {
  dn_string_t name;
  sp_pokemon_ex_t ex;
  sp_pokemon_type_t type;
  sp_pokemon_stage_t stage;
  sp_move_id_t moves [2];
  sp_pokemon_ability_t ability;
  u32 hp;
} sp_pokemon_t;

typedef enum {
  SP_CARD_SET_NONE,
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
  SP_CARD_COUNT
} sp_card_id_t;

typedef enum {
  SP_CARD_KIND_NONE,
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
  sp_card_id_t cards [SP_DECK_SIZE];
  sp_pokemon_type_t energy [SP_DECK_MAX_ENERGY];
} sp_deck_t;

typedef struct {
  sp_card_id_t card;
  u32 count;
} sp_deck_count_item_t;

typedef struct {
  sp_deck_count_item_t cards [SP_DECK_SIZE];
} sp_deck_count_t;


///////////
// MATCH //
///////////
typedef enum {
  SP_MATCH_STATE_INIT,
  SP_MATCH_STATE_SETUP,
  SP_MATCH_STATE_TURN,
  SP_MATCH_STATE_DONE,
} sp_match_state_t;

typedef enum {
  SP_TURN_ORDER_FIRST,
  SP_TURN_ORDER_SECOND,
} sp_turn_order_t;

typedef enum {
  SP_CARD_PILE_NONE,
  SP_CARD_PILE_DECK,
  SP_CARD_PILE_HAND,
  SP_CARD_PILE_BENCH,
  SP_CARD_PILE_ACTIVE,
  SP_CARD_PILE_DISCARD,
} sp_card_pile_t;

typedef enum {
  SP_PLAYER_ID_NONE,
  SP_PLAYER_ID_0,
  SP_PLAYER_ID_1,
} sp_match_player_id_t;

typedef enum {
 SP_PLAYER_STATE_NONE,
 SP_PLAYER_STATE_SETUP,
 SP_PLAYER_STATE_READY,
 SP_PLAYER_STATE_WAIT,
} sp_player_state_t;

#define SP_MAX_ATTACHED_ENERGY 16
typedef struct {
  sp_card_id_t card;
  u32 hp;
  sp_pokemon_type_t energy [SP_MAX_ATTACHED_ENERGY];
} sp_field_card_t;

typedef struct {
  sp_card_pile_t pile;
  u32 slot;
  sp_match_player_id_t player;
} sp_card_location_t;
#define SP_CARD_LOCATION_ACTIVE()    (sp_card_location_t) { .pile = SP_CARD_PILE_ACTIVE }
#define SP_CARD_LOCATION_BENCH(SLOT) (sp_card_location_t) { .pile = SP_CARD_PILE_BENCH, .slot = SLOT }

typedef struct {
	u32 energy [SP_POKEMON_TYPE_COUNT];
} sp_energy_count_t;

typedef struct {
  sp_match_player_id_t id;
  sp_player_state_t state;
  sp_deck_t deck_list;
  sp_card_id_t deck [SP_DECK_SIZE];
  sp_card_id_t hand [SP_HAND_SIZE];
  sp_card_id_t discard [SP_DECK_SIZE];
  sp_field_card_t active;
  sp_field_card_t bench [3];
  sp_pokemon_type_t energy [2];
  sp_turn_order_t turn_order;
  u32 time_remaining;
} sp_player_t;

typedef struct {
  sp_match_state_t state;
  sp_rng_flip_t order_flip;
	sp_match_player_id_t winner;
	u32 turn;
  sp_player_t players [2];
} sp_match_t;

typedef struct {
  sp_card_id_t* cards;
  u32 max_cards;
  s32 index;
} sp_card_iter_t;


// MATCH ACTIONS
typedef enum {
  SP_MATCH_ACTION_NONE,
  SP_MATCH_ACTION_CONCEDE,
  SP_MATCH_ACTION_SETUP_PLAY_BASIC,
  SP_MATCH_ACTION_SETUP_RESET,
  SP_MATCH_ACTION_SETUP_READY,
  SP_MATCH_ACTION_PLAY_BASIC,
  SP_MATCH_ACTION_ATTACK,
  SP_MATCH_ACTION_ATTACH_BASIC_ENERGY,
} sp_match_action_kind_t;

typedef enum {
  SP_MATCH_ACTION_STATUS_NONE,
  SP_MATCH_ACTION_STATUS_OK,
  SP_MATCH_ACTION_STATUS_ERROR
} sp_match_action_status_t;

typedef enum {
  SP_MATCH_ACTION_RESULT_NONE,

  SP_MATCH_ACTION_RESULT_GAME_OVER,
  SP_MATCH_ACTION_RESULT_SETUP_RESET,
  SP_MATCH_ACTION_RESULT_PLAY_BASIC,
  SP_MATCH_ACTION_RESULT_ATTACK,

  SP_MATCH_ACTION_RESULT_INCORRECT_MATCH_STATE,
  SP_MATCH_ACTION_RESULT_INCORRECT_PLAYER_STATE,
  SP_MATCH_ACTION_RESULT_SLOT_OCCUPIED,
} sp_match_action_result_kind_t;

typedef struct {
  sp_card_location_t location;
  sp_card_id_t card;
} sp_match_action_play_basic_data_t;

typedef struct {
  sp_card_location_t attacker;
  sp_card_location_t target;
	sp_move_id_t move;
} sp_match_action_attack_data_t;

typedef struct {
  sp_match_action_kind_t kind;

  union {
    sp_match_action_play_basic_data_t play_basic;
		sp_match_action_attack_data_t attack;
  };
} sp_match_action_t;

// MATCH ACTION RESULTS
typedef struct {
  u32 damage;
  bool weakness;
  bool knockout;
} sp_match_action_attack_result_t;

typedef struct {
  sp_match_action_status_t status;
  sp_match_action_result_kind_t kind;
  sp_match_player_id_t player;

  union {
    sp_match_action_attack_result_t attack;
  };
} sp_match_action_result_t;



void                                sp_rng_init();
u32                                 sp_rng_int();
u32                                 sp_rng_ranged_u32(u32 inclusive_min, u32 inclusive_max);
sp_rng_flip_t                       sp_rng_flip();
u32                                 sp_rng_pick_from_set(u32* options, u32 num_options);
                
void                                sp_match_init(sp_match_t* match, sp_deck_t decks [2]);
void                                sp_match_update(sp_match_t* match);
bool                                sp_match_is_player_first(sp_match_t* match, sp_player_t* player);
sp_turn_order_t                     sp_match_turn_order(sp_match_t* match, sp_player_t* player);
dn_dynamic_array(sp_match_action_t) sp_match_calc_all_actions(sp_match_t* match, sp_player_t* player, sp_card_location_t location);
sp_match_action_result_t            sp_match_process_action(sp_match_t* match, sp_player_t* player, sp_match_action_t* action);
sp_player_t*                        sp_match_find_player(sp_match_t* match, sp_match_player_id_t id);
sp_player_t*                        sp_match_find_opponent(sp_match_t* match, sp_match_player_id_t id);
sp_field_card_t*                    sp_match_find_field_card(sp_match_t* match, sp_card_location_t location);
sp_card_id_t                        sp_match_find_card(sp_match_t* match, sp_card_location_t location);
sp_player_t*                        sp_match_going_first(sp_match_t* match);
sp_player_t*                        sp_match_going_second(sp_match_t* match);
bool 																sp_match_validate_attack(sp_match_t* match, sp_player_t* player, sp_card_location_t location, sp_move_id_t move);
bool 																sp_match_validate_attack_exists(sp_match_t* match, sp_player_t* player, sp_card_location_t location, sp_move_id_t move);
bool 																sp_match_validate_attack_energy(sp_match_t* match, sp_player_t* player, sp_card_location_t location, sp_move_id_t move);
sp_energy_count_t 							 		sp_match_count_attached_energy(sp_match_t* match, sp_player_t* player, sp_card_location_t location);
sp_energy_count_t 							 		sp_match_count_required_energy(sp_match_t* match, sp_player_t* player, sp_move_id_t move);

void                                sp_player_init(sp_player_t* player, sp_deck_t* deck);
void                                sp_player_shuffle(sp_player_t* player);
void                                sp_player_shuffle_hand_into_deck(sp_player_t* player);
u32                                 sp_player_deck_size(sp_player_t* player);
u32                                 sp_player_hand_size(sp_player_t* player);
u32                                 sp_player_discard_size(sp_player_t* player);
void                                sp_player_draw(sp_player_t* player);
void                                sp_player_draw_n(sp_player_t* player, u32 num_cards);
void                                sp_player_draw_opening_hand(sp_player_t* player);
bool                                sp_player_is_starting_hand_valid(sp_player_t* player);
void                                sp_player_generate_energy(sp_player_t* player);
sp_card_id_t                        sp_player_find_card(sp_player_t* player, sp_card_location_t location);
sp_field_card_t*                    sp_player_find_field_card(sp_player_t* player, sp_card_location_t location);
bool                                sp_player_is_location_occupied(sp_player_t* player, sp_card_location_t location);
void                                sp_player_clear_location(sp_player_t* player, sp_card_location_t location);
void                                sp_player_remove_from_pile(sp_player_t* player, sp_card_id_t* pile, u32 pile_size, sp_card_id_t card);
void                                sp_player_remove_from_hand(sp_player_t* player, sp_card_id_t card);
void                                sp_player_add_to_pile(sp_player_t* player, sp_card_id_t* pile, u32 pile_size, sp_card_id_t card);
void                                sp_player_add_to_hand(sp_player_t* player, sp_card_id_t card);
void                                sp_player_play_card_to_field(sp_player_t* player, sp_card_id_t card, sp_card_location_t slot);
bool 																sp_player_is_actionable(sp_player_t* player);
              
sp_deck_count_t                     sp_deck_count(sp_deck_t* deck);
sp_deck_count_t 										sp_deck_count_from_pile(sp_card_id_t* cards, u32 num_cards);
sp_deck_count_item_t*               sp_deck_count_add(sp_deck_count_t* count, sp_card_id_t card);
sp_deck_count_item_t*               sp_deck_count_find_item(sp_deck_count_t* count, sp_card_id_t card);
u32                                 sp_deck_count_find(sp_deck_count_t* count, sp_card_id_t card);
void                                sp_deck_count_remove_cards(sp_deck_count_t* count, sp_card_id_t* cards, u32 num_cards);
void                                sp_deck_count_remove_active_cards(sp_deck_count_t* count, sp_field_card_t* cards, u32 num_cards);
s32                                 sp_deck_count_sort_kernel_alphabetical(const void* a, const void* b);
void                                sp_deck_print(sp_deck_t* deck);
void                                sp_deck_print_ordered(sp_deck_t* deck);
sp_deck_t                           sp_deck_gen_random();
              
sp_card_t*                          sp_find_card(sp_card_id_t card);
sp_pokemon_t*                       sp_find_pokemon(sp_card_id_t card);
dn_string_t                         sp_find_card_name(sp_card_id_t card);
sp_move_t*												  sp_find_move(sp_move_id_t id);
dn_string_t                         sp_card_set_to_string(sp_card_set_t set);
dn_string_t                         sp_card_set_to_short_string(sp_card_set_t set);
sp_card_iter_t                      sp_card_iter_forward(sp_card_id_t* cards, u32 max_cards);
sp_card_iter_t                      sp_card_iter_backward(sp_card_id_t* cards, u32 max_cards);
bool                                sp_card_iter_valid(sp_card_iter_t* it);
void                                sp_card_iter_next(sp_card_iter_t* it);
void                                sp_card_iter_prev(sp_card_iter_t* it);
sp_card_id_t                        sp_card_iter_get(sp_card_iter_t* it);
sp_card_id_t*                       sp_card_iter_get_ptr(sp_card_iter_t* it);

#define                             sp_for_card(it, cards) for (sp_card_iter_t it = sp_card_iter_forward(cards, dn_arr_len(cards)); sp_card_iter_valid(&it); sp_card_iter_next(&it))
#define                             sp_for_card_backward(it, cards) for (sp_card_iter_t it = sp_card_iter_backward(cards, dn_arr_len(cards)); sp_card_iter_valid(&it); sp_card_iter_prev(&it))
#define                             sp_for_card_n(it, cards, n) for (sp_card_iter_t it = sp_card_iter_forward(cards, n); sp_card_iter_valid(&it); sp_card_iter_next(&it))
          
//////////
// DATA //
//////////
#define SP_UNIMPLEMENTED_POKEMON(card) dn_zero_initialize()
sp_card_t sp_cards [] = {
  [SP_CARD_NONE] = SP_UNIMPLEMENTED_POKEMON(),
  [SP_CARD_BULBASAUR] = {
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
  [SP_CARD_WEEDLE] = {
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
  [SP_CARD_ODDISH] = {
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

#define SP_UNIMPLEMENTED_MOVE(move) dn_zero_initialize()
#define SP_DEFINE_MOVE(ID, NAME, DAMAGE, ...) [ID] = { \
  .name = dn_string_literal(NAME), \
  .id = ID, \
  .damage = DAMAGE, \
  .energy = { \
    __VA_ARGS__ \
  } \
}
sp_move_t sp_moves [] = {
  SP_DEFINE_MOVE(SP_MOVE_NONE, "None", 0),
  SP_DEFINE_MOVE(SP_MOVE_BULBASAUR_VINE_WHIP, "Vine Whip", 30, SP_POKEMON_TYPE_GRASS, SP_POKEMON_TYPE_COLORLESS),
  SP_DEFINE_MOVE(SP_MOVE_WEEDLE_STING, "Sting", 20, SP_POKEMON_TYPE_GRASS),
  SP_DEFINE_MOVE(SP_MOVE_ODDISH_RAM, "Ram", 20, SP_POKEMON_TYPE_GRASS),
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

sp_card_id_t* sp_card_iter_get_ptr(sp_card_iter_t* it) {
  return &it->cards[it->index];
}



/////////////
// SP DECK //
/////////////
sp_deck_count_t sp_deck_count(sp_deck_t* deck) {
	return sp_deck_count_from_pile(deck->cards, SP_DECK_SIZE);
}

sp_deck_count_t sp_deck_count_from_pile(sp_card_id_t* cards, u32 num_cards) {
  sp_deck_count_t count = dn_zero_initialize();
  dn_for(index, num_cards) {
    sp_deck_count_add(&count, cards[index]);
  }

  qsort(count.cards, SP_DECK_SIZE, sizeof(sp_deck_count_item_t), sp_deck_count_sort_kernel_alphabetical);

  return count;
}

sp_deck_count_item_t* sp_deck_count_add(sp_deck_count_t* count, sp_card_id_t card) {
  dn_for(index, SP_DECK_SIZE) {
    sp_deck_count_item_t* item = &count->cards[index];
    
    if (item->card == SP_CARD_NONE) {
      item->card = card;
    }

    if (item->card == card) {
      item->count++;
      return item;
    }
  }

  DN_ASSERT(!"sp_deck_count_add: tried to add a card, but the count was full");
  return NULL;
}

sp_deck_count_item_t* sp_deck_count_find_item(sp_deck_count_t* count, sp_card_id_t card) {
  dn_for(index, SP_DECK_SIZE) {
    sp_deck_count_item_t* item = &count->cards[index];
    if (item->card == card) {
      return item;
    }
  }

  return NULL;
}

u32 sp_deck_count_find(sp_deck_count_t* count, sp_card_id_t card) {
  sp_deck_count_item_t* item = sp_deck_count_find_item(count, card);
  return item ? item->count : 0;
}

void sp_deck_count_remove_card(sp_deck_count_t* count, sp_card_id_t card) {
  sp_deck_count_item_t* item = sp_deck_count_find_item(count, card);
  if (!item) return;
  if (!item->count) return;

  item->count--;
}

void sp_deck_count_remove_cards(sp_deck_count_t* count, sp_card_id_t* cards, u32 num_cards) {
  sp_for_card_n(it, cards, num_cards) {
    sp_deck_count_remove_card(count, sp_card_iter_get(&it));
  }
}

void sp_deck_count_remove_active_cards(sp_deck_count_t* count, sp_field_card_t* cards, u32 num_cards) {
  dn_for(index, num_cards) {
    sp_deck_count_remove_card(count, cards[index].card);
  }  
}

s32 sp_deck_count_sort_kernel_alphabetical(const void* a, const void* b) {
  const sp_deck_count_item_t* ia = (const sp_deck_count_item_t*)a;
  const sp_deck_count_item_t* ib = (const sp_deck_count_item_t*)b;
  if (ia->card == SP_CARD_NONE && ib->card == SP_CARD_NONE) return DN_QSORT_EQUAL;
  if (ia->card == SP_CARD_NONE) return DN_QSORT_B_FIRST;
  if (ib->card == SP_CARD_NONE) return DN_QSORT_A_FIRST;
  if (ia->card < ib->card) return DN_QSORT_A_FIRST;
  if (ia->card > ib->card) return DN_QSORT_B_FIRST;
  return DN_QSORT_EQUAL;
}

void sp_deck_print_ordered(sp_deck_t* deck) {
  sp_for_card(it, deck->cards) {
    sp_card_t card = sp_cards[sp_card_iter_get(&it)];
    DN_LOG("%.*s", card.pokemon.name.len, card.pokemon.name.data);
  }
}

void sp_deck_print(sp_deck_t* deck) {
  sp_deck_count_t count = sp_deck_count(deck);

  dn_for(index, SP_DECK_SIZE) {
    sp_deck_count_item_t* item = &count.cards[index];
    sp_card_t card = sp_cards[item->card];

    DN_LOG("(%d) %s %s-%d", 
      item->count, 
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
void sp_match_init(sp_match_t* match, sp_deck_t decks [2]) {
  dn_os_zero_memory(match, sizeof(sp_match_t));
  
  match->state = SP_MATCH_STATE_SETUP;
  match->order_flip = sp_rng_flip();
  
  dn_for(i, 2) {
    sp_deck_t* deck = &decks[i];
    sp_player_t* player = &match->players[i];
    sp_player_init(player, deck);
    player->turn_order = sp_match_turn_order(match, player);
    player->state = SP_PLAYER_STATE_SETUP;

		sp_player_draw_opening_hand(player);

    sp_player_generate_energy(player);
    if (sp_match_is_player_first(match, player)) {
      sp_player_generate_energy(player);
    }
  }

  match->players[0].id = SP_PLAYER_ID_0;
  match->players[1].id = SP_PLAYER_ID_1;
}

bool sp_match_is_player_first(sp_match_t* match, sp_player_t* player) {
  u32 index = player - match->players;
  if ((index == 0) && match->order_flip == SP_RNG_FLIP_HEADS) return true;
  if ((index == 1) && match->order_flip == SP_RNG_FLIP_TAILS) return true;
  return false;
}

sp_turn_order_t sp_match_turn_order(sp_match_t* match, sp_player_t* player) {
  if (sp_match_is_player_first(match, player)) return SP_TURN_ORDER_FIRST;
  return SP_TURN_ORDER_SECOND;
}

void sp_match_calc_hand_actions(sp_match_t* match, sp_player_t* player, sp_card_location_t slot, dn_dynamic_array_t* actions) {
  sp_card_id_t card = sp_player_find_card(player, slot);

  switch (match->state) {
    case SP_MATCH_STATE_SETUP: {
      sp_card_location_t locations [] = {
        { .pile = SP_CARD_PILE_ACTIVE },
        { .pile = SP_CARD_PILE_BENCH, .slot = 0 },
        { .pile = SP_CARD_PILE_BENCH, .slot = 1 },
        { .pile = SP_CARD_PILE_BENCH, .slot = 2 },
      };

      dn_for_arr(locations, index) {
        if (!sp_player_is_location_occupied(player, locations[index])) {
          dn_dynamic_array_push(actions, &(sp_match_action_t){
            .kind = SP_MATCH_ACTION_SETUP_PLAY_BASIC,
            .play_basic = {
              .location = locations[index],
              .card = card
            }
          });
        }
      }

      break;
    }
		case SP_MATCH_STATE_TURN: {
			sp_card_location_t locations [] = {
        { .pile = SP_CARD_PILE_BENCH, .slot = 0 },
        { .pile = SP_CARD_PILE_BENCH, .slot = 1 },
        { .pile = SP_CARD_PILE_BENCH, .slot = 2 },
      };

      dn_for_arr(locations, index) {
        if (!sp_player_is_location_occupied(player, locations[index])) {
          dn_dynamic_array_push(actions, &(sp_match_action_t){
            .kind = SP_MATCH_ACTION_PLAY_BASIC,
            .play_basic = {
              .location = locations[index],
              .card = card
            }
          });
        }
      }
      break;
		}
    default: {
      DN_UNREACHABLE();
      break;
    }
  }
}

void sp_match_calc_active_actions(sp_match_t* match, sp_player_t* player, dn_dynamic_array_t* actions) {
  if (!sp_player_is_location_occupied(player, SP_CARD_LOCATION_ACTIVE())) {
    return;
  }

	sp_pokemon_t* pokemon = sp_find_pokemon(player->active.card);
	dn_for(index, SP_POKEMON_MAX_MOVES) {
		if (sp_match_validate_attack(match, player, SP_CARD_LOCATION_ACTIVE(), pokemon->moves[index])) {
			dn_dynamic_array_push(actions, &(sp_match_action_t) {
				.kind = SP_MATCH_ACTION_ATTACK,
				.attack = {
					.attacker = SP_CARD_LOCATION_ACTIVE(),
					.target = SP_CARD_LOCATION_ACTIVE(),
					.move = pokemon->moves[index]
				}
			});
		}
	}
}

void sp_match_calc_card_actions(sp_match_t* match, sp_player_t* player, sp_card_location_t location, dn_dynamic_array_t* actions) {
  switch(location.pile) {
    case SP_CARD_PILE_HAND: {
      sp_match_calc_hand_actions(match, player, location, actions);
      break;
    }
    case SP_CARD_PILE_ACTIVE: {
      return sp_match_calc_active_actions(match, player, actions);
      break;
    }
    case SP_CARD_PILE_BENCH: {
      // return sp_match_calc_bench_actions(match, location)
      break;
    }
    default: {
      break;
    }
  }
}

void sp_match_calc_other_actions(sp_match_t* match, sp_player_t* player, dn_dynamic_array_t* actions) {
  dn_dynamic_array_push(actions, &(sp_match_action_t) {
    .kind = SP_MATCH_ACTION_CONCEDE
  });

  switch (match->state) {
    case SP_MATCH_STATE_SETUP: {
      dn_dynamic_array_push(actions, &(sp_match_action_t) {
        .kind = SP_MATCH_ACTION_SETUP_RESET
      });

      if (sp_player_is_location_occupied(player, SP_CARD_LOCATION_ACTIVE())) {
        dn_dynamic_array_push(actions, &(sp_match_action_t) {
          .kind = SP_MATCH_ACTION_SETUP_READY
        });
      }
      break;
    }
    default: {
      break;
    }
  }

}

dn_dynamic_array(sp_match_action_t) sp_match_calc_all_actions(sp_match_t* match, sp_player_t* player, sp_card_location_t card) {
  dn_dynamic_array(sp_match_action_t) actions = dn_zero_initialize();
  dn_dynamic_array_init(&actions, sizeof(sp_match_action_t), &dn_allocators.bump.allocator);

  sp_match_calc_other_actions(match, player, &actions);
  sp_match_calc_card_actions(match, player, card, &actions);

  return actions;  
}


sp_match_action_result_t sp_match_process_action(sp_match_t* match, sp_player_t* player, sp_match_action_t* action) {
  sp_match_action_result_t result = {
    .player = player->id
  };

  if (player->state != SP_PLAYER_STATE_SETUP && player->state != SP_PLAYER_STATE_READY) {
    result.status = SP_MATCH_ACTION_STATUS_ERROR;
    result.kind = SP_MATCH_ACTION_RESULT_INCORRECT_PLAYER_STATE;
    return result;
  }

  switch (action->kind) {
    case SP_MATCH_ACTION_CONCEDE: {
			sp_player_t* opponent = sp_match_find_opponent(match, player->id);
			match->state = SP_MATCH_STATE_DONE;
			match->winner = opponent->id;
			result.status = SP_MATCH_ACTION_STATUS_OK;
    	result.kind = SP_MATCH_ACTION_RESULT_GAME_OVER;
    	return result;
		}
    case SP_MATCH_ACTION_SETUP_PLAY_BASIC: {
      DN_ASSERT(match->state == SP_MATCH_STATE_SETUP);
      DN_ASSERT(player->state == SP_PLAYER_STATE_SETUP);

      sp_match_action_play_basic_data_t* data = &action->play_basic;

      if (sp_player_is_location_occupied(player, data->location)) {
        result.status = SP_MATCH_ACTION_STATUS_ERROR;
        result.kind = SP_MATCH_ACTION_RESULT_SLOT_OCCUPIED;
        return result;
      }

      sp_player_remove_from_hand(player, data->card);
      sp_player_play_card_to_field(player, data->card, data->location);

      result.status = SP_MATCH_ACTION_STATUS_OK;
      result.kind = SP_MATCH_ACTION_RESULT_PLAY_BASIC;
      return result;

      break;
    }
    case SP_MATCH_ACTION_SETUP_RESET: {
      DN_ASSERT(match->state == SP_MATCH_STATE_SETUP);
      DN_ASSERT(player->state == SP_PLAYER_STATE_SETUP);

      sp_card_location_t locations [] = {
        { .pile = SP_CARD_PILE_ACTIVE },
        { .pile = SP_CARD_PILE_BENCH, .slot = 0 },
        { .pile = SP_CARD_PILE_BENCH, .slot = 1 },
        { .pile = SP_CARD_PILE_BENCH, .slot = 2 },
      };

      dn_for_arr(locations, index) {
				sp_card_location_t location = locations[index];
        if (sp_player_is_location_occupied(player, location)) {
          sp_player_add_to_hand(player, sp_player_find_card(player, location));
          sp_player_clear_location(player, location);
        }
      }

      result.status = SP_MATCH_ACTION_STATUS_OK;
      result.kind = SP_MATCH_ACTION_RESULT_SETUP_RESET;
      return result;

      break;
    }
    case SP_MATCH_ACTION_SETUP_READY: {
      DN_SOFT_ASSERT(match->state == SP_MATCH_STATE_SETUP);
      DN_SOFT_ASSERT(player->state == SP_PLAYER_STATE_SETUP);

      player->state = SP_PLAYER_STATE_WAIT;

      sp_player_t* opponent = sp_match_find_opponent(match, player->id);
      if (opponent->state == SP_PLAYER_STATE_WAIT) {
        match->state = SP_MATCH_STATE_TURN;
				match->turn = 1;
        sp_match_going_first(match)->state = SP_PLAYER_STATE_READY;
        sp_match_going_second(match)->state = SP_PLAYER_STATE_WAIT;
      }

      result.status = SP_MATCH_ACTION_STATUS_NONE;
      return result;

      break;
    }
    case SP_MATCH_ACTION_PLAY_BASIC: {
      DN_ASSERT(match->state == SP_MATCH_STATE_TURN);
      DN_ASSERT(player->state == SP_PLAYER_STATE_READY);

      sp_match_action_play_basic_data_t* data = &action->play_basic;

      if (sp_player_is_location_occupied(player, data->location)) {
        result.status = SP_MATCH_ACTION_STATUS_ERROR;
        result.kind = SP_MATCH_ACTION_RESULT_SLOT_OCCUPIED;
        return result;
      }

      sp_player_remove_from_hand(player, data->card);
      sp_player_play_card_to_field(player, data->card, data->location);

      result.status = SP_MATCH_ACTION_STATUS_OK;
      result.kind = SP_MATCH_ACTION_RESULT_PLAY_BASIC;
      return result;

      break;
    }
    case SP_MATCH_ACTION_ATTACK: {
      sp_match_action_attack_data_t* data = &action->attack;

      if (!sp_match_validate_attack(match, player, data->attacker, data->move)) {
        result.status = SP_MATCH_ACTION_STATUS_ERROR;
        result.kind = SP_MATCH_ACTION_RESULT_ATTACK;
        return result;
      }

      result.status = SP_MATCH_ACTION_STATUS_OK;
      result.kind = SP_MATCH_ACTION_RESULT_ATTACK;
      result.attack = (sp_match_action_attack_result_t) {
        .damage = sp_find_move(data->move)->damage,
        .weakness = false,
        .knockout = false
      };
      return result;

      break;
    }
    case SP_MATCH_ACTION_ATTACH_BASIC_ENERGY:
    case SP_MATCH_ACTION_NONE: {
      DN_UNREACHABLE();
      break;
    }
  }
}

sp_player_t* sp_match_find_player(sp_match_t* match, sp_match_player_id_t id) {
  dn_for(index, 2) {
    sp_player_t* player = &match->players[index];
    if (player->id == id) {
      return player;
    }
  }

  DN_UNREACHABLE();
  return NULL;
}

sp_player_t* sp_match_find_opponent(sp_match_t* match, sp_match_player_id_t id) {
	dn_for(index, 2) {
    sp_player_t* player = &match->players[index];
    if (player->id != id) {
      return player;
    }
  }

  DN_UNREACHABLE();
  return NULL;
}

sp_card_id_t sp_match_find_card(sp_match_t* match, sp_card_location_t location) {
  DN_ASSERT(location.player != SP_PLAYER_ID_NONE && "sp_match_find_card received SP_PLAYER_ID_NONE");
  sp_player_t* player = sp_match_find_player(match, location.player);
  return sp_player_find_card(player, location);
}

sp_field_card_t* sp_match_find_field_card(sp_match_t* match, sp_card_location_t location) {
  DN_ASSERT(location.player != SP_PLAYER_ID_NONE && "sp_match_find_field_card received SP_PLAYER_ID_NONE");
  sp_player_t* player = sp_match_find_player(match, location.player);

  switch (location.pile) {
    case SP_CARD_PILE_ACTIVE: return &player->active;
    case SP_CARD_PILE_BENCH: return &player->bench[location.slot];
    default: DN_ASSERT(!"sp_match_find_field_card received a location that was not SP_CARD_PILE_ACTIVE or SP_CARD_PILE_BENCH");
  }

  return NULL;
}

sp_player_t* sp_match_going_first(sp_match_t* match) {
  switch (match->order_flip) {
    case SP_RNG_FLIP_HEADS: return sp_match_find_player(match, SP_PLAYER_ID_0);
    case SP_RNG_FLIP_TAILS: return sp_match_find_player(match, SP_PLAYER_ID_1);
    default: DN_UNREACHABLE();
  }
}

sp_player_t* sp_match_going_second(sp_match_t* match) {
  switch (match->order_flip) {
    case SP_RNG_FLIP_HEADS: return sp_match_find_player(match, SP_PLAYER_ID_1);
    case SP_RNG_FLIP_TAILS: return sp_match_find_player(match, SP_PLAYER_ID_0);
    default: DN_UNREACHABLE();
  }
}

bool sp_match_validate_attack(sp_match_t* match, sp_player_t* player, sp_card_location_t location, sp_move_id_t move) {
	if (!sp_match_validate_attack_exists(match, player, location, move)) return false;
	if (!sp_match_validate_attack_energy(match, player, location, move)) return false;
	return true;
}

bool sp_match_validate_attack_exists(sp_match_t* match, sp_player_t* player, sp_card_location_t location, sp_move_id_t move) {
	sp_pokemon_t* pokemon = sp_find_pokemon(sp_player_find_card(player, location));

	dn_for(index, SP_POKEMON_MAX_MOVES) {
		if (move == pokemon->moves[index]) {
			return true;
		}
	}
	return false;
}

bool sp_match_validate_attack_energy(sp_match_t* match, sp_player_t* player, sp_card_location_t location, sp_move_id_t move_id) {
	sp_energy_count_t attached = sp_match_count_attached_energy(match, player, location);
	sp_energy_count_t required = sp_match_count_required_energy(match, player, move_id);

	dn_for(energy, SP_POKEMON_TYPE_COUNT) {
		if (attached.energy[energy] < required.energy[energy]) {
			return false;
		}
	}

	return true;
}

sp_energy_count_t sp_match_count_attached_energy(sp_match_t* match, sp_player_t* player, sp_card_location_t location) {
	sp_energy_count_t count = dn_zero_initialize();
	sp_field_card_t* card = sp_player_find_field_card(player, location);

	dn_for(index, SP_MAX_ATTACHED_ENERGY) {
		sp_pokemon_type_t energy = card->energy[index];
		if (energy == SP_POKEMON_TYPE_NONE) break;

		count.energy[energy]++;
	}

	return count;
}

sp_energy_count_t sp_match_count_required_energy(sp_match_t* match, sp_player_t* player, sp_move_id_t move_id) {
	sp_energy_count_t count = dn_zero_initialize();
	sp_move_t* move = sp_find_move(move_id);

	dn_for(index, SP_POKEMON_MOVE_MAX_ENERGY) {
		sp_pokemon_type_t energy = move->energy[index];
		if (energy == SP_POKEMON_TYPE_NONE) break;

		count.energy[energy]++;
	}

	return count;
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

void sp_player_draw_opening_hand(sp_player_t* player) {
	sp_player_shuffle(player);

	while (!sp_player_is_starting_hand_valid(player)) {
		sp_player_shuffle_hand_into_deck(player);
		sp_player_draw_n(player, 5);
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

sp_card_id_t sp_player_find_card(sp_player_t* player, sp_card_location_t location) {
  switch (location.pile) {
    case SP_CARD_PILE_NONE: {
      return SP_CARD_NONE;
    }
    case SP_CARD_PILE_DECK: {
      DN_ASSERT(location.slot < SP_DECK_SIZE);
      return player->deck[location.slot];
    }
    case SP_CARD_PILE_HAND: {
      DN_ASSERT(location.slot < SP_HAND_SIZE);
      return player->hand[location.slot];
    }
    case SP_CARD_PILE_ACTIVE: {
      return player->active.card;
    }
    case SP_CARD_PILE_BENCH: {
      DN_ASSERT(location.slot < SP_BENCH_SIZE);
      return player->bench[location.slot].card;
    }
    case SP_CARD_PILE_DISCARD: {
      DN_ASSERT(location.slot < SP_DISCARD_SIZE);
      return player->discard[location.slot];
    }
    default: {
      DN_UNREACHABLE();
      return SP_CARD_NONE;
    }
  }
}

sp_field_card_t* sp_player_find_field_card(sp_player_t* player, sp_card_location_t location) {
  switch (location.pile) {
    case SP_CARD_PILE_ACTIVE: {
      return &player->active;
    }
    case SP_CARD_PILE_BENCH: {
      return &player->bench[location.slot];
    }
    default: {
      DN_UNREACHABLE();
      return NULL;
    }
  }
}

bool sp_player_is_location_occupied(sp_player_t* player, sp_card_location_t location) {
  return sp_player_find_card(player, location) != SP_CARD_NONE;
}

void sp_player_clear_location(sp_player_t* player, sp_card_location_t location) {
  switch (location.pile) {
    case SP_CARD_PILE_ACTIVE: {
      player->active = dn_zero_struct(sp_field_card_t);
      break;
    }
    case SP_CARD_PILE_BENCH: {
      player->bench[location.slot] = dn_zero_struct(sp_field_card_t);
      break;
    }
    case SP_CARD_PILE_DECK:
    case SP_CARD_PILE_DISCARD:
    case SP_CARD_PILE_HAND: 
    default: {
      DN_BROKEN();
      break;
    }
  }
}

void sp_player_remove_from_pile(sp_player_t* player, sp_card_id_t* pile, u32 pile_size, sp_card_id_t card) {
  sp_card_iter_t it = sp_card_iter_forward(pile, pile_size);
  for (; sp_card_iter_valid(&it); sp_card_iter_next(&it)) {
    sp_card_id_t pile_card = sp_card_iter_get(&it);
    if (pile_card == card) {
      break;
    }
  }

  for (u32 index = it.index; index < pile_size - 1; index++) {
    pile[index] = pile[index + 1];
  }
  pile[pile_size - 1] = SP_CARD_NONE;
}

void sp_player_remove_from_hand(sp_player_t* player, sp_card_id_t card) {
  sp_player_remove_from_pile(player, player->hand, SP_HAND_SIZE, card);
}

void sp_player_add_to_pile(sp_player_t* player, sp_card_id_t* pile, u32 pile_size, sp_card_id_t card) {
  sp_card_iter_t it = sp_card_iter_forward(pile, pile_size);
  for (; sp_card_iter_valid(&it); sp_card_iter_next(&it)) {
    // Just getting to the last index
  }

  DN_ASSERT(it.index < pile_size && "sp_player_add_to_pile: pile is full");
  pile[it.index] = card;
}

void sp_player_add_to_hand(sp_player_t* player, sp_card_id_t card) {
  sp_player_add_to_pile(player, player->hand, SP_HAND_SIZE, card);
}

void sp_player_play_card_to_field(sp_player_t* player, sp_card_id_t card, sp_card_location_t location) {
  sp_field_card_t* field_card = sp_player_find_field_card(player, location);
  sp_card_t* card_info = sp_find_card(card);
  field_card->card = card;
  field_card->hp = card_info->pokemon.hp;
  dn_os_fill_memory(field_card->energy, SP_MAX_ATTACHED_ENERGY * sizeof(sp_pokemon_type_t), dn_ptr_lval(u32, 0), sizeof(u32));
}

void sp_player_set_active(sp_player_t* player, sp_card_id_t card) {
  sp_card_t* card_info = sp_find_card(card);
  player->active = (sp_field_card_t) {
    .card = card,
    .hp = card_info->pokemon.hp
  };
}

bool sp_player_is_actionable(sp_player_t* player) {
	return player->state == SP_PLAYER_STATE_READY || player->state == SP_PLAYER_STATE_SETUP;
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

sp_card_t* sp_find_card(sp_card_id_t card) {
  return &sp_cards[card];
}

sp_pokemon_t* sp_find_pokemon(sp_card_id_t card) {
	return &sp_find_card(card)->pokemon;
}

sp_move_t* sp_find_move(sp_move_id_t id) {
	return &sp_moves[id];
}

dn_string_t sp_find_card_name(sp_card_id_t card) {
  return sp_find_card(card)->pokemon.name;
}

dn_string_t sp_card_set_to_string(sp_card_set_t set) {
  switch (set) {
    case SP_CARD_SET_NONE: return dn_string_literal("None");
    case SP_CARD_SET_GENETIC_APEX: return dn_string_literal("Genetic Apex");
    case SP_CARD_SET_MYTHICAL_ISLAND: return dn_string_literal("Mythical Island");
  }
}

dn_string_t sp_card_set_to_short_string(sp_card_set_t set) {
  switch (set) {
    case SP_CARD_SET_NONE: return dn_string_literal("N/A");
    case SP_CARD_SET_GENETIC_APEX: return dn_string_literal("A1");
    case SP_CARD_SET_MYTHICAL_ISLAND: return dn_string_literal("A1a");
  }
}

#endif