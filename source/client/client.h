/*
  Render the opponent's stuff on the sidebar (active, bench, discard, deck, hand, unknown)
  Format log messages with the player's name, and add a color for each player
  Change the default clear color
  Figure out how to render the lines of text without making every line double height
  Clicking active/bench should select
  Reset your setup
  Finalize setup
  Tell player 1 it's their turn
  Implement basic attacks, energy attach, retreat
  Show next energy
*/

/*
  Put stuff in the bench slots
  Render the opponent's stuff in the playmat
  Format opponent log messages differently

*/
#ifndef SP_CLIENT_APP_H
#define SP_CLIENT_APP_H

typedef EMSCRIPTEN_WEBSOCKET_T sp_ws_t;

typedef enum {
  SP_CLIENT_STATE_INIT,
  SP_CLIENT_STATE_WS_INIT,
  SP_CLIENT_STATE_TOKEN_WAIT,
  SP_CLIENT_STATE_IDLE,
  SP_CLIENT_STATE_MATCH_WAIT,
  SP_CLIENT_STATE_MATCH,
} sp_client_state_t;

typedef enum {
  SP_CLIENT_SEARCH_NONE,
  SP_CLIENT_SEARCH_REQUESTED,
  SP_CLIENT_SEARCH_SEARCHING,
  SP_CLIENT_SEARCH_AWAITING_SYNC,
  SP_CLIENT_SEARCH_READY,
  SP_CLIENT_SEARCH_CANCEL,
} sp_client_search_state_t;

typedef struct {
  f32 playmat_height;
  f32 playmat_panel_height;
  dn_vector2_t card_size;
} sp_client_nuklear_bullshit_t;

typedef struct {
  sp_deck_t deck;
  dn_string_t password;
  sp_client_search_state_t search;
  sp_match_data_t data;
  sp_match_player_id_t id;
} sp_client_match_t;

typedef struct {
  sp_card_location_t selected_card;
  sp_match_action_t selected_action;
} sp_client_ui_state_t;

typedef struct {
  dn_string_t message;
  dn_color_t color;
} sp_client_log_item_t;

typedef struct {
  dn_dynamic_array(sp_client_log_item_t) items;
} sp_client_log_t;

typedef struct {
  sg_pass_action pass_action;
  
  sp_ws_t websocket;
  sp_token_t token;

  sp_client_state_t state;
  sp_client_match_t match;
  sp_client_ui_state_t ui;
  sp_client_log_t log;
  sp_client_nuklear_bullshit_t nkbs;

  struct {
   dn_string_t player;
   dn_string_t opponent;
  } usernames;

  sp_deck_t deck;

  nk_context* nk;
} sp_client_t;
sp_client_t sp_client;

void         sp_client_init();
void         sp_client_update();
void         sp_client_render();
void         sp_client_shutdown();
void         sp_client_update_state();
void         sp_client_process_match_event(sp_net_match_event_t* event);
void         sp_client_process_match_action_event(sp_net_match_action_event_t* event);
void         sp_client_draw_match();
void         sp_client_draw_field_card(sp_field_card_t* card);
dn_string_t  sp_client_generate_username();
sp_player_t* sp_client_get_player();
sp_player_t* sp_client_get_opponent();
bool         sp_client_is_selected(sp_card_location_t location);
void         sp_client_select(sp_card_location_t location);
dn_string_t  sp_client_field_card_label(sp_field_card_t* field_card);
dn_string_t  sp_client_energy_to_short_id(sp_pokemon_type_t energy);
dn_string_t  sp_client_build_action_description(sp_match_action_t* action);
dn_string_t  sp_client_build_action_result_description(dn_string_t username, sp_match_action_t* action, sp_match_action_result_t* result);
bool         sp_client_on_websocket_open(int event_type, const EmscriptenWebSocketOpenEvent* event, void* user_data);
bool         sp_client_on_websocket_error(int event_type, const EmscriptenWebSocketErrorEvent* event, void* user_data);
bool         sp_client_on_websocket_close(int event_type, const EmscriptenWebSocketCloseEvent* event, void* user_data);
bool         sp_client_on_websocket_message(int event_type, const EmscriptenWebSocketMessageEvent* event, void* user_data);
void         sp_client_process_response(sp_net_response_t* response);
void         sp_client_process_match_event(sp_net_match_event_t* event);
void         sp_client_submit_request(sp_net_request_t* request);
dn_string_t  sp_client_state_to_string(sp_client_state_t state);
dn_string_t  sp_client_search_state_to_string(sp_client_search_state_t state);
void         sp_client_log_init(sp_client_log_t* log);
void         sp_client_log_push(sp_client_log_t* log, dn_string_t str);
void         sp_client_log_push_colored(sp_client_log_t* log, dn_string_t str, dn_color_t color);
#endif



#ifdef SP_CLIENT_APP_IMPL
void sp_client_init() {
  dn_init();
  dn_log("spumc");

  sp_rng_init();

  // Set up Sokol and Nuklear
  sg_setup(&(sg_desc){
    .environment = sglue_environment(),
    .logger.func = slog_func,
  });

  snk_setup(&(snk_desc_t){
    .enable_set_mouse_cursor = true,
    .dpi_scale = sapp_dpi_scale(),
    .logger.func = slog_func,
  });

  sp_client.pass_action = (sg_pass_action) {
    .colors[0] = {
      .load_action = SG_LOADACTION_CLEAR,
      .clear_value = { 1.0f, 0.0f, 0.0f, 1.0f }
    }
  };

  // Connect to the server
  sp_client.websocket = emscripten_websocket_new(&(EmscriptenWebSocketCreateAttributes){
    .url = "ws://localhost:8000",
    .protocols = "sp-protocol",
    .createOnMainThread = EM_TRUE
  });
  if (sp_client.websocket <= 0) {
    printf("%s: websocket creation failed; error = %d\n", __func__, sp_client.websocket);
    return;
  }
  emscripten_websocket_set_onopen_callback(sp_client.websocket, NULL, sp_client_on_websocket_open);
  emscripten_websocket_set_onerror_callback(sp_client.websocket, NULL, sp_client_on_websocket_error);
  emscripten_websocket_set_onclose_callback(sp_client.websocket, NULL, sp_client_on_websocket_close);
  emscripten_websocket_set_onmessage_callback(sp_client.websocket, NULL, sp_client_on_websocket_message);

  sp_client.state = SP_CLIENT_STATE_WS_INIT;

  nk_context* nk = sp_client.nk;

  // Data  
  sp_client.match.password = dn_string_alloc(SP_MAX_PASSWORD_LEN, &dn_allocators.standard.allocator);

  sp_client.usernames.player = dn_string_alloc(SP_MAX_USERNAME_LEN, &dn_allocators.standard.allocator);
  dn_string_copy_to_str(sp_client_generate_username(), &sp_client.usernames.player, SP_MAX_USERNAME_LEN);

  sp_client.usernames.opponent = dn_string_alloc(SP_MAX_USERNAME_LEN, &dn_allocators.standard.allocator);
  dn_string_copy_to_str(sp_client_generate_username(), &sp_client.usernames.opponent, SP_MAX_USERNAME_LEN);

  sp_client.deck = sp_deck_gen_random();

  sp_client_log_init(&sp_client.log);

  sp_client.match.data = sp_sample_match;
}

void sp_client_update() {
  dn_update();
  sp_client_update_state();

  sp_client.nk = snk_new_frame();
  nk_context* nk = sp_client.nk;

  if (nk_begin(nk, "spum", nk_rect(10, 10, 400, 800), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE)) {
    if (nk_tree_push(nk, NK_TREE_TAB, "Client Information", NK_MAXIMIZED)) {
      nk_layout_row_dynamic(nk, 0, 2);

      nk_dn_string(nk, dn_string_literal("Username: "), NK_TEXT_LEFT);
      nk_edit_dn_string(nk, NK_EDIT_SIMPLE, &sp_client.usernames.player, SP_MAX_USERNAME_LEN, nk_filter_default);

      dn_string_builder_t builder = dn_tstring_builder();
      dn_string_builder_append_fmt(&builder, dn_string_literal("%llu"), sp_client.token);
      nk_dn_string(nk, dn_string_literal("ID: "), NK_TEXT_LEFT);
      nk_dn_string(nk, dn_string_builder_write(&builder), NK_TEXT_LEFT);

      nk_dn_string(nk, dn_string_literal("State: "), NK_TEXT_LEFT);
      nk_dn_string(nk, sp_client_state_to_string(sp_client.state), NK_TEXT_LEFT);

      nk_tree_pop(nk);
    }  

    if (nk_tree_push(nk, NK_TREE_TAB, "Decks", NK_MINIMIZED)) {
      nk_layout_row_static(nk, 18, 100, 4);
      nk_dn_string(nk, dn_string_literal("#"), NK_TEXT_LEFT);
      nk_dn_string(nk, dn_string_literal("Card"), NK_TEXT_LEFT);
      nk_dn_string(nk, dn_string_literal("Set"), NK_TEXT_LEFT);
      nk_dn_string(nk, dn_string_literal("Set Number"), NK_TEXT_LEFT);

      sp_deck_count_t count = sp_deck_count(&sp_client.deck);

      gs_hash_table_for(count.cards, it) {
        sp_card_id_t card_id = gs_hash_table_iter_getk(count.cards, it);
        u32 num_instances = gs_hash_table_iter_get(count.cards, it);
        sp_card_t card = sp_cards[card_id];

        nk_labelf(nk, NK_TEXT_LEFT, "(%d)", num_instances);
        nk_dn_string(nk, card.pokemon.name, NK_TEXT_LEFT);
        nk_dn_string(nk, sp_card_set_to_short_string(card.set), NK_TEXT_LEFT);
        nk_labelf(nk, NK_TEXT_LEFT, "%d", card.set_id);
      }

      nk_tree_pop(nk);
    }

    if (nk_tree_push(nk, NK_TREE_TAB, "Find a Match", NK_MINIMIZED)) {
      nk_layout_row(nk, NK_STATIC, 30, 3, NK_RATIO(150, 100, 100));

      nk_edit_dn_string(nk, NK_EDIT_SIMPLE, &sp_client.match.password, SP_MAX_PASSWORD_LEN, nk_filter_default);

      if (nk_button_label(nk, "Search")) {
        sp_client.match.search = SP_CLIENT_SEARCH_REQUESTED;
        sp_client.match.deck = sp_client.deck;
      }

      if (nk_button_label(nk, "Cancel")) {
        sp_client.match.search = SP_CLIENT_SEARCH_CANCEL;
      }

      if (sp_client.state == SP_CLIENT_STATE_MATCH_WAIT) {
        nk_layout_row_dynamic(nk, 0, 2);
        nk_dn_string(nk, dn_string_literal("State:"), NK_TEXT_LEFT);
        nk_dn_string(nk, sp_client_search_state_to_string(sp_client.match.search), NK_TEXT_LEFT);

        nk_dn_string(nk, dn_string_literal("Password:"), NK_TEXT_LEFT);
        nk_dn_string(nk, sp_client.match.password, NK_TEXT_LEFT);
      }

      nk_tree_pop(nk);
    }

    nk_end(nk);
  }

  sp_client_draw_match();

  draw_demo_ui(nk);

  sp_client_render();
}

dn_string_t sp_client_energy_to_short_id(sp_pokemon_type_t energy) {
  switch (energy) {
    case SP_POKEMON_TYPE_NONE: return dn_string_literal("");
    case SP_POKEMON_TYPE_FIRE: return dn_string_literal("F");
    case SP_POKEMON_TYPE_GRASS: return dn_string_literal("G");
    default: DN_UNREACHABLE();
  }
}

dn_string_t sp_client_field_card_label(sp_field_card_t* field_card) {
  sp_card_t* card = sp_find_card(field_card->card);

  dn_string_builder_t builder = dn_tstring_builder();
  dn_string_builder_append_fmt(&builder, dn_string_literal("%.*s [%d] "), 
    card->pokemon.name.len, card->pokemon.name.data, 
    field_card->hp
  );

  dn_string_builder_append(&builder, dn_string_literal("["));
  dn_for(index, SP_MAX_ATTACHED_ENERGY) {
    sp_pokemon_type_t energy = field_card->energy[index];
    if (energy == SP_POKEMON_TYPE_NONE) break;

    dn_string_builder_append(&builder, sp_client_energy_to_short_id(energy));
  }
  dn_string_builder_append(&builder, dn_string_literal("]"));

  return dn_string_builder_write(&builder);
}

void sp_client_update_state() {
  sp_player_t* player = sp_client_get_player();

  switch (sp_client.state) {
    case SP_CLIENT_STATE_WS_INIT: {
      u16 is_ws_ready = 0;
      emscripten_websocket_get_ready_state(sp_client.websocket, &is_ws_ready);

      if (is_ws_ready) {
        DN_LOG("SP_CLIENT_STATE_WS_INIT");
        sp_client.state = SP_CLIENT_STATE_TOKEN_WAIT;

        sp_net_request_t request = dn_zero_initialize();
        request.op = SP_OPCODE_REQUEST_TOKEN;

        sp_username_t* username = &request.token_req.username;
        dn_string_copy_to_str_buffer(sp_client.usernames.player, username);

        sp_client_submit_request(&request);
      }
      break;
    }
    case SP_CLIENT_STATE_TOKEN_WAIT: {
      if (sp_client.token) {
        sp_client.state = SP_CLIENT_STATE_IDLE;
      }
      break;
    }
    case SP_CLIENT_STATE_IDLE: {
      if (sp_client.match.search == SP_CLIENT_SEARCH_REQUESTED) {
        sp_client.match.search = SP_CLIENT_SEARCH_SEARCHING;
        sp_client.state = SP_CLIENT_STATE_MATCH_WAIT;

        sp_net_request_t request;
        request.op = SP_OPCODE_MATCH_REQUEST;
        request.match = (sp_net_match_request_t){
          .password = dn_hash_string(sp_client.match.password),
          .deck = sp_client.deck,
        };
        sp_client_submit_request(&request);
      }
      break;
    }
    case SP_CLIENT_STATE_MATCH_WAIT: {
      if (sp_client.match.search == SP_CLIENT_SEARCH_READY) {
        sp_client.match.search = SP_CLIENT_SEARCH_NONE;
        sp_client.state = SP_CLIENT_STATE_MATCH;
      }
      break;
    }
    case SP_CLIENT_STATE_MATCH: {
      break;
    }
    default: {
      DN_UNREACHABLE();
      break;
    }
  }
}


void sp_client_draw_match() {
  nk_context* nk = sp_client.nk;
  sp_player_t* player = sp_client_get_player();
  sp_player_t* opponent = sp_client_get_opponent();

  dn_string_builder_t builder = dn_tstring_builder();
  dn_string_builder_append(&builder, sp_client.usernames.player);
  dn_string_builder_append(&builder, dn_string_literal(" vs. "));
  dn_string_builder_append(&builder, sp_client.usernames.opponent);
  
  sp_client.nkbs.playmat_height = 600 + (nk->style.window.padding.y * 5);
  sp_client.nkbs.playmat_panel_height = 150;
  sp_client.nkbs.card_size = (dn_vector2_t) { 
    .x = 50 - (nk->style.window.padding.x * (SP_BENCH_SIZE + 1)), 
    .y = sp_client.nkbs.playmat_panel_height - (nk->style.window.padding.y * 2)
  };


  if (nk_begin(nk, dn_string_builder_write_cstr(&builder), nk_rect(420, 10, 1200, 1200), NK_WINDOW_TITLE | NK_WINDOW_SCALABLE | NK_WINDOW_MOVABLE)) {
    nk_layout_row(nk, NK_DYNAMIC, sp_client.nkbs.playmat_height, 3, dn_arr_lval(f32, .25, .5, .25));

    if (nk_group_begin(nk, "sp_group:deck", NK_WINDOW_NO_SCROLLBAR)) {
      sp_deck_count_t count = sp_deck_count(&player->deck_list);

      if (nk_tree_push(nk, NK_TREE_TAB, "Active", NK_MAXIMIZED)) {
        if (player->active.card != SP_CARD_NONE) {
          nk_layout_row_static(nk, 12, 100, 1);
          sp_card_t* card = &sp_cards[player->active.card];
          nk_dn_string(nk, card->pokemon.name, NK_TEXT_LEFT);
        }

        nk_tree_pop(nk);
      }
      sp_deck_count_remove_active_cards(&count, &player->active, 1);


      if (nk_tree_push(nk, NK_TREE_TAB, "Bench", NK_MAXIMIZED)) {
        nk_layout_row_static(nk, 12, 100, 1);
        dn_for(index, 3) {
          sp_field_card_t* pokemon = &player->bench[index];
          sp_card_id_t id = pokemon->card;
          sp_card_t* card = &sp_cards[id];
          nk_dn_string_colored(nk, card->pokemon.name, NK_TEXT_LEFT, dn_colors.cool_gray);
        }

        nk_tree_pop(nk);
      }
      sp_deck_count_remove_active_cards(&count, player->bench, SP_BENCH_SIZE);


      if (nk_tree_push(nk, NK_TREE_TAB, "Hand", NK_MINIMIZED)) {
        nk_layout_row_static(nk, 12, 100, 1);
        nk_style_push_color(nk, &nk->style.selectable.text_normal, dn_color_to_nk_color(dn_colors.zomp));
        
        sp_for_card(it, player->hand) {
          sp_card_id_t* id = sp_card_iter_get_ptr(&it);
          sp_card_t* card = &sp_cards[*id];
          sp_card_location_t location = { .pile = SP_CARD_PILE_HAND, .slot = it.index };
          nk_bool selected = sp_client_is_selected(location);

          if (nk_selectable_dn_string(nk, card->pokemon.name, NK_TEXT_LEFT, &selected)) {
            sp_client_select(location);
          }
        }

        nk_style_pop_color(nk);
        nk_tree_pop(nk);
      }
      sp_deck_count_remove_cards(&count, player->hand, SP_HAND_SIZE);
      

      if (nk_tree_push(nk, NK_TREE_TAB, "Discard", NK_MINIMIZED)) {
        nk_layout_row_static(nk, 12, 100, 1);
        sp_for_card(it, player->discard) {
          sp_card_id_t id = sp_card_iter_get(&it);
          sp_card_t* card = &sp_cards[id];
          nk_dn_string_colored(nk, card->pokemon.name, NK_TEXT_LEFT, dn_colors.indian_red);
        }

        nk_tree_pop(nk);
      }
      sp_deck_count_remove_cards(&count, player->discard, SP_DECK_SIZE);

      if (nk_tree_push(nk, NK_TREE_TAB, "Deck", NK_MINIMIZED)) {
        nk_layout_row_static(nk, 12, 100, 1);
        gs_hash_table_for(count.cards, it) {
          sp_card_id_t card_id = gs_hash_table_iter_getk(count.cards, it);
          u32 num_instances = gs_hash_table_iter_get(count.cards, it);
          sp_card_t* card = &sp_cards[card_id];

          dn_for(i, num_instances) {
            nk_dn_string_colored(nk, card->pokemon.name, NK_TEXT_LEFT, dn_colors.selective_yellow);
          }
        }

        nk_tree_pop(nk);
      }

      nk_group_end(nk);
    }
    
    if (nk_group_begin(nk, "sp_group:play", NK_WINDOW_NO_SCROLLBAR)) {
      nk_layout_row_dynamic(nk, sp_client.nkbs.playmat_panel_height, 1);

      if (nk_group_begin(nk, "sp_group:play:opponent_bench", NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(nk, sp_client.nkbs.card_size.y, 3);
        dn_for(slot, SP_BENCH_SIZE) {
          nk_button_dn_string(nk, sp_client_field_card_label(&opponent->bench[slot]));
        }
        nk_group_end(nk);
      }

      if (nk_group_begin(nk, "sp_group:play:opponent_active", NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(nk, sp_client.nkbs.card_size.y, 1);
        nk_button_dn_string(nk, sp_client_field_card_label(&opponent->active));
        nk_group_end(nk);
      }

      if (nk_group_begin(nk, "sp_group:play:active", NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(nk, sp_client.nkbs.card_size.y, 1);
        nk_button_dn_string(nk, sp_client_field_card_label(&player->active));
        nk_group_end(nk);
      }

      if (nk_group_begin(nk, "sp_group:play:bench", NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(nk, sp_client.nkbs.card_size.y, 3);
        dn_for(slot, SP_BENCH_SIZE) {
          nk_button_dn_string(nk, sp_client_field_card_label(&player->bench[slot]));
        }
        nk_group_end(nk);
      }
      nk_group_end(nk);
    }

    if (nk_group_begin(nk, "sp_group:opponent_deck", NK_WINDOW_NO_SCROLLBAR)) {
      sp_deck_count_t count = sp_deck_count(&sp_client.deck);

      if (nk_tree_push(nk, NK_TREE_TAB, "Hand", NK_MINIMIZED)) {
        nk_layout_row_static(nk, 12, 100, 1);
        sp_for_card(it, player->hand) {
          sp_card_id_t id = sp_card_iter_get(&it);
          sp_card_t* card = &sp_cards[id];
          nk_dn_string_colored(nk, card->pokemon.name, NK_TEXT_LEFT, dn_colors.zomp);
        }

        nk_tree_pop(nk);
      }

      sp_deck_count_remove_cards(&count, player->hand, SP_HAND_SIZE);
      nk_group_end(nk);
    }

    f32 hand_height = sp_client.nkbs.card_size.y * 2 + nk->style.window.padding.y * 2;
    nk_layout_row(nk, NK_DYNAMIC, hand_height, 1, dn_arr_lval(f32, 1.0));
    if (nk_group_begin(nk, "sp_group:hand", NK_WINDOW_NO_SCROLLBAR)) {
      nk_layout_row_dynamic(nk, sp_client.nkbs.card_size.y, 5);
      dn_for(index, SP_HAND_SIZE) {
        sp_card_id_t* card_id = &player->hand[index];
        sp_card_t* card = &sp_cards[*card_id];
        sp_card_location_t location = { .pile = SP_CARD_PILE_HAND, .slot = index };
        bool selected = sp_client_is_selected(location);

        if (selected) {
          nk_style_push_style_item(nk, &nk->style.button.normal, nk->style.button.active);
        }

        if (nk_button_dn_string(nk, card->pokemon.name)) {
          sp_client_select(location);
        }

        if (selected) {
          nk_style_pop_style_item(nk);
        }
      }

      nk_group_end(nk);
    }

    nk_layout_row_dynamic(nk, 10, 1);

    nk_widget(&(struct nk_rect) { .w = 10, .h = 10, .x = 0, .y = 0 }, nk);

    nk_layout_row(nk, NK_DYNAMIC, 200, 2, dn_arr_lval(f32, .4, .6));
    if (nk_group_begin(nk, "sp_group:actions", NK_WINDOW_BORDER)) {
      f32 row_height = 16 + nk->style.window.padding.x;
      f32 panel_height = 200;
      i32 num_rows = panel_height / row_height;

      // nk_layout_row(nk, NK_DYNAMIC, 200, 2, dn_arr_lval(f32, .333, .667));
      nk_layout_row_dynamic(nk, 16, 1);

      dn_dynamic_array(sp_match_action_t) actions = sp_match_calc_actions(&sp_client.match.data, player, sp_client.ui.selected_card);

      dn_for(index, actions.size) {
        sp_match_action_t* action = (sp_match_action_t*)dn_dynamic_array_at(&actions, index);

        nk_bool selected = dn_os_is_memory_equal(&sp_client.ui.selected_action, action, sizeof(sp_match_action_t));
        if (nk_selectable_dn_string(nk, sp_client_build_action_description(action), NK_TEXT_LEFT, &selected)) {
          sp_client.ui.selected_action = *action;
        }
      }

      i32 num_spacers = num_rows - actions.size - 4;
      nk_spacing(nk, num_spacers);
      
      nk_layout_row_dynamic(nk, 32, 1);

      bool confirm = false;
      if (sp_client.ui.selected_action.kind == SP_MATCH_ACTION_NONE) {
        struct nk_style_button button;
        button = nk->style.button;
        nk->style.button.normal = nk_style_item_color(nk_rgb(40,40,40));
        nk->style.button.hover = nk_style_item_color(nk_rgb(40,40,40));
        nk->style.button.active = nk_style_item_color(nk_rgb(40,40,40));
        nk->style.button.border_color = nk_rgb(60,60,60);
        nk->style.button.text_background = nk_rgb(60,60,60);
        nk->style.button.text_normal = nk_rgb(60,60,60);
        nk->style.button.text_hover = nk_rgb(60,60,60);
        nk->style.button.text_active = nk_rgb(60,60,60);
        confirm = nk_button_dn_string(nk, dn_string_literal("Confirm"));
        nk->style.button = button;
      } else {
        confirm = nk_button_dn_string(nk, dn_string_literal("Confirm"));
      }

      if (confirm) {
        sp_client_submit_request(&(sp_net_request_t) {
          .op = SP_OPCODE_CLIENT_MATCH_ACTION,
          .match_action = {
            .action = sp_client.ui.selected_action
          }
        });
        sp_client_select((sp_card_location_t) { 
          .pile = SP_CARD_PILE_NONE, 
          .slot = 0, 
        });
      }

      nk_group_end(nk);
    }

    if (nk_group_begin(nk, "sp_group:log", NK_WINDOW_BORDER)) {
      nk_layout_row_dynamic(nk, 16, 1);

      dn_for(index, sp_client.log.items.size) {
        sp_client_log_item_t* item = (sp_client_log_item_t*)dn_dynamic_array_at(&sp_client.log.items, index);
        nk_dn_string_wrap_colored(nk, item->message, item->color);
      }
      
      nk_group_end(nk);
    }
    nk_end(nk);
  }
}

void sp_client_render() {
  sg_begin_pass(&(sg_pass){
    .action = {
      .colors[0] = {
        .load_action = SG_LOADACTION_CLEAR, 
        .clear_value = { 0.25f, 0.5f, 0.7f, 1.0f }
      }
    },
    .swapchain = sglue_swapchain()
  });
  snk_render(sapp_width(), sapp_height());
  sg_end_pass();
  sg_commit();
}

void sp_client_event(const sapp_event* event) {
 snk_handle_event(event);
}

void sp_client_shutdown() {
  sg_shutdown();
  snk_shutdown();
}

dn_string_t sp_client_generate_username() {
  dn_string_t adjective = sp_username_adjectives[sp_rng_ranged_u32(0, dn_arr_len(sp_username_adjectives) - 1)];
  dn_string_t noun = sp_username_nouns[sp_rng_ranged_u32(0, dn_arr_len(sp_username_nouns) - 1)];

  dn_string_builder_t builder = {
    .allocator = &dn_allocators.bump.allocator,
    .buffer = dn_zero_initialize()
  };
  dn_string_builder_append(&builder, adjective);
  dn_string_builder_append(&builder, dn_string_literal("_"));
  dn_string_builder_append(&builder, noun);
  return dn_string_builder_write(&builder);
}


///////////////
// WEBSOCKET //
///////////////
void sp_client_process_match_event(sp_net_match_event_t* event) {  
  switch(event->kind) {
    case SP_NET_MATCH_EVENT_KIND_FOUND_OPPONENT: {
      DN_LOG("SP_NET_MATCH_EVENT_KIND_FOUND_OPPONENT");
      sp_client.match.search = SP_CLIENT_SEARCH_AWAITING_SYNC;
      sp_client.match.id = event->found.your_match_id;
      sp_client.usernames.opponent = dn_string_copy(dn_str_buffer_view(event->found.username), &dn_allocators.standard.allocator);
      break;
    }
    case SP_NET_MATCH_EVENT_KIND_BEGIN: {
      DN_LOG("SP_NET_MATCH_EVENT_KIND_BEGIN");

      sp_client.match.data = event->state;
      sp_client.match.search = SP_CLIENT_SEARCH_READY;

      dn_dynamic_array_clear(&sp_client.log.items);

      dn_string_builder_t builder = dn_tstring_builder();
      dn_string_builder_append_fmt(&builder, dn_string_literal("A match between %.*s and %.*s started!"), 
        sp_client.usernames.player.len, sp_client.usernames.player.data,
        sp_client.usernames.opponent.len, sp_client.usernames.opponent.data);
      sp_client_log_push(&sp_client.log, dn_string_builder_write(&builder));

      break;
    }
    case SP_NET_MATCH_EVENT_KIND_SYNC: {
      DN_LOG("SP_NET_MATCH_EVENT_KIND_SYNC");
      sp_client.match.data = event->state;
      break;
    }
    case SP_NET_MATCH_EVENT_KIND_ACTION_RESULT: {
      DN_LOG("SP_NET_MATCH_EVENT_ACTION_RESULT");
      sp_client_process_match_action_event(&event->action);
      break;
    }
    default: {
      DN_UNREACHABLE();
      break;
    }
  }
}

void sp_client_process_match_action_event(sp_net_match_action_event_t* event) {
  sp_match_action_result_t* result = &event->result;

  bool is_you = result->player == sp_client.match.id;
  dn_string_t username = is_you ? sp_client.usernames.player : sp_client.usernames.opponent;
  dn_color_t color = is_you ? dn_colors.zomp : dn_colors.indian_red;

  switch (result->kind) {
    case SP_MATCH_ACTION_RESULT_SETUP_PLAY_BASIC:
      sp_client_log_push_colored(&sp_client.log, sp_client_build_action_result_description(username, &event->action, &event->result), color);
      break;
    case SP_MATCH_ACTION_RESULT_NOT_YOUR_TURN: {
      sp_client_log_push(&sp_client.log, sp_client_build_action_result_description(username, &event->action, &event->result));
      break;
    }
    default: {
      DN_UNREACHABLE();
      break;
    }
  }
}

void sp_client_process_response(sp_net_response_t* response) {
  switch (response->op) {
    case SP_OPCODE_ECHO: {
      dn_log("SP_OPCODE_ECHO");
      break;
    }
    case SP_OPCODE_REQUEST_TOKEN: {
      DN_LOG("SP_OPCODE_REQUEST_TOKEN");
      dn_os_memory_copy(&response->request_token.token, &sp_client.token, sizeof(sp_token_t));
      break;
    }
    case SP_OPCODE_MATCH_EVENT: {
      DN_LOG("SP_OPCODE_MATCH_EVENT");
      sp_client_process_match_event(&response->match_event);
      break;
    }
    default: {
      DN_UNREACHABLE();
      break;
    }
  }
}

sp_player_t* sp_client_get_player() {
  switch (sp_client.match.id) {
    case SP_PLAYER_ID_0: return &sp_client.match.data.players[0];
    case SP_PLAYER_ID_1: return &sp_client.match.data.players[1];
    default:       DN_UNREACHABLE(); return NULL;
  }
}

sp_player_t* sp_client_get_opponent() {
  switch (sp_client.match.id) {
    case SP_PLAYER_ID_0: return &sp_client.match.data.players[1];
    case SP_PLAYER_ID_1: return &sp_client.match.data.players[0];
    default:       DN_UNREACHABLE(); return NULL;
  }
}

bool sp_client_is_selected(sp_card_location_t location) {
  return dn_os_is_memory_equal(&location, &sp_client.ui.selected_card, sizeof(sp_card_location_t));
}

void sp_client_select(sp_card_location_t location) {
  sp_client.ui.selected_card = location;
  sp_client.ui.selected_action = dn_zero_struct(sp_match_action_t);
}


void sp_client_submit_request(sp_net_request_t* request) {
  dn_os_memory_copy(sp_magic, request->magic, sizeof(sp_net_magic_t));
  request->token = sp_client.token;
  emscripten_websocket_send_binary(sp_client.websocket, request, sizeof(sp_net_request_t));
}

dn_string_t sp_client_state_to_string(sp_client_state_t state) {
  switch (state) {
    case SP_CLIENT_STATE_INIT:    return dn_string_literal("SP_CLIENT_STATE_INIT");
    case SP_CLIENT_STATE_WS_INIT:  return dn_string_literal("SP_CLIENT_WS_INIT");
    case SP_CLIENT_STATE_TOKEN_WAIT: return dn_string_literal("SP_CLIENT_TOKEN_WAIT");
    case SP_CLIENT_STATE_IDLE:    return dn_string_literal("SP_CLIENT_IDLE");
    case SP_CLIENT_STATE_MATCH_WAIT: return dn_string_literal("SP_CLIENT_MATCH_WAIT");
    case SP_CLIENT_STATE_MATCH:   return dn_string_literal("SP_CLIENT_MATCH");
    default:             return dn_string_literal("SP_CLIENT_UNKNOWN");
  }
}

dn_string_t sp_client_search_state_to_string(sp_client_search_state_t state) {
  switch (state) {
    case SP_CLIENT_SEARCH_NONE:     return dn_string_literal("SP_CLIENT_SEARCH_NONE");
    case SP_CLIENT_SEARCH_REQUESTED:   return dn_string_literal("SP_CLIENT_SEARCH_REQUESTED");
    case SP_CLIENT_SEARCH_SEARCHING:   return dn_string_literal("SP_CLIENT_SEARCH_SEARCHING");
    case SP_CLIENT_SEARCH_AWAITING_SYNC: return dn_string_literal("SP_CLIENT_SEARCH_AWAITING_SYNC");
    case SP_CLIENT_SEARCH_READY:     return dn_string_literal("SP_CLIENT_SEARCH_READY");
    case SP_CLIENT_SEARCH_CANCEL:    return dn_string_literal("SP_CLIENT_SEARCH_CANCEL");
    default:               return dn_string_literal("SP_CLIENT_SEARCH_UNKNOWN");
  }
}

dn_string_t sp_client_build_action_description(sp_match_action_t* action) {
  dn_string_builder_t builder = dn_tstring_builder();

  switch (action->kind) {
    case SP_MATCH_ACTION_NONE: {
      return dn_string_literal("None");
    }
    case SP_MATCH_ACTION_SETUP: {
      sp_match_action_setup_data_t* data = &action->setup;
      sp_card_t* card = &sp_cards[data->card];

      switch (data->location.pile) {
        case SP_CARD_PILE_BENCH: {
          dn_string_builder_append_fmt(&builder, 
            dn_string_literal("Place %.*s in bench slot %d"), 
            card->pokemon.name.len,
            card->pokemon.name.data,
            data->location.slot
          );
          break;
        }
        case SP_CARD_PILE_ACTIVE: {
          dn_string_builder_append_fmt(&builder, 
            dn_string_literal("Place %.*s in the active slot"), 
            card->pokemon.name.len,
            card->pokemon.name.data
          );
          break;
        }
        default: {
          DN_UNREACHABLE();
        }
      }
      
      
      return dn_string_builder_write(&builder);
    }
    default: {
      DN_UNREACHABLE();
      return dn_string_literal("");
    }
  }
}

dn_string_t sp_client_build_action_result_description(dn_string_t username, sp_match_action_t* action, sp_match_action_result_t* result) {
  dn_string_builder_t builder = dn_tstring_builder();
  dn_string_builder_append(&builder, username);
  dn_string_builder_append(&builder, dn_string_literal(" "));

  switch (result->kind) {
    case SP_MATCH_ACTION_RESULT_SLOT_OCCUPIED: {
      sp_match_action_setup_data_t* data = &action->setup;
      sp_card_t* card = sp_find_card(data->card);
      
      switch (data->location.pile) {
        case SP_CARD_PILE_BENCH: {
          dn_string_builder_append_fmt(&builder, 
            dn_string_literal("Tried to place %.*s in bench slot %d, but it was already occupied"), 
            card->pokemon.name.len,
            card->pokemon.name.data,
            data->location.slot
          );
          break;
        }
        case SP_CARD_PILE_ACTIVE: {
          dn_string_builder_append_fmt(&builder, 
            dn_string_literal("Tried to place %.*s in the active slot, but it was already occupied"), 
            card->pokemon.name.len,
            card->pokemon.name.data
          );
          break;
        }
        default: {
          DN_UNREACHABLE();
        }
      }

      return dn_string_builder_write(&builder);
    }
    case SP_MATCH_ACTION_RESULT_SETUP_PLAY_BASIC: {
      sp_match_action_setup_data_t* data = &action->setup;
      sp_card_t* card = sp_find_card(data->card);

      switch (data->location.pile) {
        case SP_CARD_PILE_BENCH: {
          dn_string_builder_append_fmt(&builder, 
            dn_string_literal("played %.*s to the active slot"), 
            card->pokemon.name.len,
            card->pokemon.name.data
          );
          break;
        }
        case SP_CARD_PILE_ACTIVE: {
          dn_string_builder_append_fmt(&builder, 
            dn_string_literal("played %.*s to the active slot"), 
            card->pokemon.name.len,
            card->pokemon.name.data
          );
        }
        default: {
          DN_UNREACHABLE();
        }
      }
      

      return dn_string_builder_write(&builder);
    }
    case SP_MATCH_ACTION_RESULT_NOT_YOUR_TURN: {

    }
    default: {
      DN_UNREACHABLE();
      return dn_string_literal("");
    }
  }
}

bool sp_client_on_websocket_open(int event_type, const EmscriptenWebSocketOpenEvent* event, void* user_data) {
  DN_LOG();
  return true;
}

bool sp_client_on_websocket_error(int event_type, const EmscriptenWebSocketErrorEvent* event, void* user_data) {
  DN_LOG();
  return true;
}

bool sp_client_on_websocket_close(int event_type, const EmscriptenWebSocketCloseEvent* event, void* user_data) {
  DN_LOG();
  return true;
}

bool sp_client_on_websocket_message(int event_type, const EmscriptenWebSocketMessageEvent* event, void* user_data) {
  DN_ASSERT(event->numBytes == sizeof(sp_net_response_t));
  sp_net_response_t* response = (sp_net_response_t*)event->data;
  sp_client_process_response(response);
  return true;
}

void sp_client_log_init(sp_client_log_t* log) {
  dn_dynamic_array_init(&log->items, sizeof(sp_client_log_item_t), &dn_allocators.standard.allocator);
}

void sp_client_log_push(sp_client_log_t* log, dn_string_t str) {
  dn_string_t copy = dn_string_copy(str, log->items.allocator);
  dn_dynamic_array_push(&log->items, &(sp_client_log_item_t) {
    .message = dn_string_copy(str, log->items.allocator),
    .color = dn_colors.white
  });
}

void sp_client_log_push_colored(sp_client_log_t* log, dn_string_t str, dn_color_t color) {
  dn_dynamic_array_push(&log->items, &(sp_client_log_item_t) {
    .message = dn_string_copy(str, log->items.allocator),
    .color = color
  });
}
#endif