#include "luajit-2.1/lua.h"
#include "luajit-2.1/lauxlib.h"
#include "luajit-2.1/lualib.h"
#include "luajit-2.1/luajit.h"

#define DN_MATH_BACKEND_HANDMADE
#define DN_LUA
#define DN_IMPL
#include "dn.h"

#define SP_IMPL
#include "sp.h"

typedef struct {
	sp_player_t player;
	sp_deck_count_t target;
	u32 max_turns;
	u32 num_iterations;
} sp_draw_odd_query_t;

typedef struct {
	u32 num_success;
	u32 num_iterations;
	f32 success;
} sp_draw_odd_query_result_t;

bool sp_cli_is_draw_successful(sp_player_t* player, sp_deck_count_t* target) {
	sp_deck_count_t hand = sp_deck_count_from_pile(player->hand, SP_HAND_SIZE);

	bool success = true;
	dn_for(index, SP_DECK_SIZE) {
		sp_deck_count_item_t target_item = target->cards[index];
		if (target_item.card == SP_CARD_NONE) {
			break;
		}

		u32 count = sp_deck_count_find(&hand, target_item.card);
		if (target_item.count > count) {
			success = false;
			break;
		}
	}

	return success;
}

sp_draw_odd_query_result_t sp_cli_draw_odds(sp_draw_odd_query_t* query) {
	sp_rng_init();

	sp_draw_odd_query_result_t result = dn_zero_initialize();

	dn_for(i, query->num_iterations) {
		result.num_iterations++;

		sp_player_t player = query->player;
		sp_player_draw_opening_hand(&player);

		dn_for(turn, query->max_turns) {
			sp_player_draw(&player);

			if (sp_cli_is_draw_successful(&player, &query->target)) {
				result.num_success++;
				break;
			}
		}
	}

	result.success = (f32)result.num_success / (f32)result.num_iterations;
	return result;
}

int main(int num_args, char** args) {
	dn_init();

	dn_lua_script_file(dn_string_literal("cli.lua"));
	lua_getglobal(dn_lua.state, "spum");
	dn_lua_pcall(dn_string_literal("init"));

	sp_draw_odd_query_t query = {
		.player = {
			.id = SP_PLAYER_ID_0,
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
			.hand = {},
			.discard = dn_zero_initialize(),
			.deck = { 
				SP_CARD_BULBASAUR, SP_CARD_BULBASAUR, SP_CARD_BULBASAUR, SP_CARD_BULBASAUR, 
				SP_CARD_IVYSAUR, SP_CARD_IVYSAUR, SP_CARD_IVYSAUR, SP_CARD_IVYSAUR, 
				SP_CARD_VENUSAUR, SP_CARD_VENUSAUR, SP_CARD_VENUSAUR, SP_CARD_VENUSAUR, 
				SP_CARD_VENUSAUR_EX, SP_CARD_VENUSAUR_EX, SP_CARD_VENUSAUR_EX,
				SP_CARD_ODDISH, SP_CARD_ODDISH, SP_CARD_ODDISH, SP_CARD_ODDISH, 
			},
			.energy = { 
					SP_POKEMON_TYPE_GRASS, 
					SP_POKEMON_TYPE_GRASS 
			},
		},
		.target = {
			.cards = {
				{
					.card = SP_CARD_VENUSAUR_EX,
					.count = 1
				}
			}
		},
		.max_turns = 1,
		.num_iterations = 1000
	};

	sp_draw_odd_query_result_t result = sp_cli_draw_odds(&query);
	dn_log("%d / %d (%.3f)", result.num_success, result.num_iterations, result.success);
}