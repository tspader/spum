#ifndef SP_H
#define SP_H

#include "simulator.h"
#include "network.h"

dn_string_t sp_username_adjectives [] = {
    dn_string_literal("busting"),
    dn_string_literal("pulmonary"),
    dn_string_literal("sneaky"),
    dn_string_literal("impressionable"),
    dn_string_literal("longing"),
    dn_string_literal("terminal"),
    dn_string_literal("hamfisted"),
    dn_string_literal("camouflaged"),
    dn_string_literal("nacreous"),
    dn_string_literal("dripping"),
    dn_string_literal("moist"),
    dn_string_literal("tight"),
    dn_string_literal("hot"),
    dn_string_literal("turgid"),
    dn_string_literal("rambunctious"),
    dn_string_literal("effervescent"),
    dn_string_literal("serendipitous"),
    dn_string_literal("ethereal"),
    dn_string_literal("resplendent"),
    dn_string_literal("enigmatic"),
    dn_string_literal("nocturnal"),
    dn_string_literal("tempestuous"),
    dn_string_literal("subterranean"),
    dn_string_literal("luminescent"),
    dn_string_literal("macabre"),
    dn_string_literal("arcane"),
    dn_string_literal("frenzied"),
    dn_string_literal("glistening"),
    dn_string_literal("meaty"),
    dn_string_literal("clenching"),
    dn_string_literal("gasping"),
    dn_string_literal("tumescent"),
    dn_string_literal("smoldering"),
    dn_string_literal("weeping"),
    dn_string_literal("fractured"),
    dn_string_literal("squelching"),
    dn_string_literal("sizzling"),
    dn_string_literal("slippery"),
    dn_string_literal("glutinous"),
    dn_string_literal("cavernous"),
    dn_string_literal("quivering"),
    dn_string_literal("labored"),
    dn_string_literal("hollowed"),
    dn_string_literal("writhing"),
    dn_string_literal("swollen"),
    dn_string_literal("prickling"),
    dn_string_literal("sputtering"),
    dn_string_literal("pulsing"),
    dn_string_literal("ragged"),
    dn_string_literal("oozing"),
    dn_string_literal("crumbling"),
    dn_string_literal("muffled"),
    dn_string_literal("feral"),
    dn_string_literal("scalding"),
    dn_string_literal("wailing"),
    dn_string_literal("fractured"),
    dn_string_literal("gnarled"),
    dn_string_literal("bristling"),
    dn_string_literal("warped"),
    dn_string_literal("sinuous"),
    dn_string_literal("rumbling"),
    dn_string_literal("creaking"),
    dn_string_literal("slithering"),
    dn_string_literal("quaking"),
    dn_string_literal("shuddering"),
    dn_string_literal("whispering"),
    dn_string_literal("rasping"),
    dn_string_literal("tender"),
    dn_string_literal("drenched"),
    dn_string_literal("slick"),
    dn_string_literal("pungent"),
    dn_string_literal("coiling"),
    dn_string_literal("unraveled"),
    dn_string_literal("heaving"),
    dn_string_literal("flickering"),
    dn_string_literal("bleeding"),
    dn_string_literal("aching"),
};

dn_string_t sp_username_nouns [] = {
    dn_string_literal("viscera"),
    dn_string_literal("chasm"),
    dn_string_literal("pustule"),
    dn_string_literal("tendon"),
    dn_string_literal("ember"),
    dn_string_literal("marrow"),
    dn_string_literal("quagmire"),
    dn_string_literal("husk"),
    dn_string_literal("ripple"),
    dn_string_literal("vortex"),
    dn_string_literal("mire"),
    dn_string_literal("plume"),
    dn_string_literal("tether"),
    dn_string_literal("bastion"),
    dn_string_literal("bramble"),
    dn_string_literal("thorn"),
    dn_string_literal("abyss"),
    dn_string_literal("shard"),
    dn_string_literal("tremor"),
    dn_string_literal("cinder"),
    dn_string_literal("smog"),
    dn_string_literal("phantasm"),
    dn_string_literal("ether"),
    dn_string_literal("maggot"),
    dn_string_literal("maelstrom"),
    dn_string_literal("crust"),
    dn_string_literal("fissure"),
    dn_string_literal("tide"),
    dn_string_literal("grove"),
    dn_string_literal("shroud"),
    dn_string_literal("spire"),
    dn_string_literal("blister"),
    dn_string_literal("wrack"),
    dn_string_literal("visage"),
    dn_string_literal("sepulcher"),
    dn_string_literal("vault"),
    dn_string_literal("rift"),
    dn_string_literal("ember"),
    dn_string_literal("shale"),
    dn_string_literal("brume"),
    dn_string_literal("canopy"),
    dn_string_literal("monolith"),
    dn_string_literal("cauldron"),
    dn_string_literal("residue"),
    dn_string_literal("slick"),
    dn_string_literal("miasma"),
    dn_string_literal("pinnacle"),
    dn_string_literal("crevice"),
    dn_string_literal("vessel"),
    dn_string_literal("gorge")
};

sp_match_data_t sp_sample_match = {
  .state = SP_MATCH_STATE_SETUP,
  .order_flip = SP_RNG_FLIP_HEADS,
  .players = {
    {
      .active = {
        .card = SP_CARD_BULBASAUR,
        .hp = 30
      },
      .bench = { 
        { 
          .card = SP_CARD_BULBASAUR, 
          .hp = 60, 
          .energy = { SP_POKEMON_TYPE_GRASS, SP_POKEMON_TYPE_GRASS } 
        }, 
      },
      .hand = { 
          SP_CARD_BULBASAUR, 
          SP_CARD_ODDISH, 
          SP_CARD_WEEDLE,
          SP_CARD_ODDISH, 
          SP_CARD_BULBASAUR 
      },
      .discard = dn_zero_initialize(),
      .energy = { 
          SP_POKEMON_TYPE_GRASS, 
          SP_POKEMON_TYPE_GRASS 
      },
    },
    {
      .active = {
        .card = SP_CARD_WEEDLE,
        .hp = 50,
        .energy = { SP_POKEMON_TYPE_FIRE, SP_POKEMON_TYPE_GRASS }
      },            
      .bench = { 
        { 
          .card = SP_CARD_WEEDLE, 
          .hp = 40, 
          .energy = { SP_POKEMON_TYPE_FIRE } 
        }, 
      },
      .hand = { 
          SP_CARD_WEEDLE, 
          SP_CARD_ODDISH, 
          SP_CARD_BULBASAUR,
          SP_CARD_ODDISH, 
          SP_CARD_ODDISH 
      },
      .discard = dn_zero_initialize(),
      .energy = { 
          SP_POKEMON_TYPE_FIRE, 
          SP_POKEMON_TYPE_FIRE 
      },
    },
  }
};
#endif


#ifdef SP_IMPL
#define SP_SIMULATOR_IMPL
#include "simulator.h"
#define SP_NETWORK_IMPL
#include "network.h"

void sp_test() {
  dn_test_suite_t suite = {
    .name = dn_string_literal("sp_simulator")
  };
  dn_test_begin_suite(&suite);

  dn_test_begin(dn_string_literal("sp_card_iter"));
    sp_deck_t deck = sp_deck_gen_random();

    u32 num_cards = 0;
    sp_for_card(it, deck.cards) {
      num_cards++;
    }
    dn_test_assert(num_cards == 20);
  dn_test_end();

  dn_test_begin(dn_string_literal("sp_player_shuffle")); {
    sp_player_t player, shuffled_player = dn_zero_struct(sp_player_t);
    sp_player_init(&player, &deck);
    sp_player_init(&shuffled_player, &deck);

    sp_player_shuffle(&shuffled_player);
    dn_test_assert(!dn_os_is_memory_equal(player.deck, shuffled_player.deck, sizeof(player.deck)));
  }
  dn_test_end();

  dn_test_begin(dn_string_literal("sp_player_draw")); {
    sp_player_t player = dn_zero_struct(sp_player_t);
    sp_player_init(&player, &deck);

    sp_card_id_t drawn_card = player.deck[0];
    sp_card_id_t deck_top = player.deck[1];

    sp_player_draw(&player);

    dn_test_assert(player.hand[0] == drawn_card);
    dn_test_assert(player.deck[0] == deck_top);
    dn_test_assert(player.deck[SP_DECK_MAX_INDEX] == SP_CARD_NONE);
    dn_test_assert(sp_player_hand_size(&player) == 1);
    dn_test_assert(sp_player_deck_size(&player) == 19);
  }
  dn_test_end();

  dn_test_begin(dn_string_literal("sp_player_remove_from_pile")); {
    sp_player_t player = dn_zero_struct(sp_player_t);
    sp_player_init(&player, &deck);
    dn_os_memory_copy(
      dn_arr_lval(sp_card_id_t, SP_CARD_BULBASAUR, SP_CARD_IVYSAUR, SP_CARD_VENUSAUR, SP_CARD_VENUSAUR_EX, SP_CARD_WEEDLE), 
      player.hand,
      sizeof(sp_card_id_t) * 5);

    sp_card_id_t hand [5] = dn_zero_initialize();
    dn_for(index, 5) {
      hand[index] = player.hand[index];
    }

    sp_player_remove_from_hand(&player, hand[0]);
    dn_test_assert(sp_player_hand_size(&player) == 4);
    dn_test_assert(player.hand[0] == hand[1]);
    dn_test_assert(player.hand[1] == hand[2]);
    dn_test_assert(player.hand[2] == hand[3]);
    dn_test_assert(player.hand[3] == hand[4]);
    dn_test_assert(player.hand[4] == SP_CARD_NONE);

    sp_player_remove_from_hand(&player, hand[3]);
    dn_test_assert(sp_player_hand_size(&player) == 3);
    dn_test_assert(player.hand[0] == hand[1]);
    dn_test_assert(player.hand[1] == hand[2]);
    dn_test_assert(player.hand[2] == hand[4]);
    dn_test_assert(player.hand[3] == SP_CARD_NONE);
    dn_test_assert(player.hand[4] == SP_CARD_NONE);

  }
  dn_test_end();


  dn_test_begin(dn_string_literal("sp_match_init")); {
    sp_match_data_t match = dn_zero_struct(sp_match_data_t);
    sp_match_init(&match, (sp_deck_t [2]) { sp_deck_gen_random(), sp_deck_gen_random() });

    dn_for(i, 2) {
      dn_test_assert(sp_player_is_starting_hand_valid(&match.players[i]));
      dn_test_assert(sp_player_hand_size(&match.players[i]) == 5);
      dn_test_assert(sp_player_deck_size(&match.players[i]) == 15);
      dn_test_assert(sp_player_discard_size(&match.players[i]) == 0);
    }
    
  }
  dn_test_end();

  dn_test_end_suite();
}
#endif