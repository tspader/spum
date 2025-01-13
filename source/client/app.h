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

typedef struct {
    sp_deck_t deck;
    dn_string_t password;
    bool search_requested;
    bool cancel_requested;
    bool ready;
} sp_client_match_search_t;

typedef struct {
    sp_deck_t deck;
    dn_string_t username;
    sp_card_id_t hand [SP_HAND_SIZE];
    sp_card_id_t discard [SP_DECK_SIZE];
    sp_pokemon_type_t energy [2];
    sp_pokemon_type_t opponent_energy [2];
    sp_turn_order_t turn;
} sp_client_match_state_t;

typedef struct {
    sg_pass_action pass_action;
    
    sp_ws_t websocket;
    sp_token_t token;

    sp_client_state_t state;
    sp_client_match_search_t match_search;
    sp_client_match_state_t match;
    dn_string_t username;

    sp_deck_t deck;

    nk_context* nk;
} sp_client_t;
sp_client_t sp_client;

void        sp_client_init();
void        sp_client_update();
void        sp_client_render();
void        sp_client_shutdown();
void        sp_client_generate_username();
bool        sp_client_on_websocket_open(int event_type, const EmscriptenWebSocketOpenEvent* event, void* user_data);
bool        sp_client_on_websocket_error(int event_type, const EmscriptenWebSocketErrorEvent* event, void* user_data);
bool        sp_client_on_websocket_close(int event_type, const EmscriptenWebSocketCloseEvent* event, void* user_data);
bool        sp_client_on_websocket_message(int event_type, const EmscriptenWebSocketMessageEvent* event, void* user_data);
void        sp_client_process_response(sp_net_response_t* response);
void        sp_client_process_match_event(sp_net_match_event_t* event);
void        sp_client_submit_request(sp_net_request_t* request);
dn_string_t sp_client_state_to_string(sp_client_state_t state);
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

    // Data
    sp_client.match_search.password = dn_string_alloc(SP_MAX_PASSWORD_LEN, &dn_allocators.standard.allocator);

    // @string Generate a fixed size string buffer which is editable
    sp_client.username = dn_string_alloc(SP_MAX_USERNAME_LEN, &dn_allocators.standard.allocator);
    sp_client_generate_username();

    sp_client.deck = sp_deck_gen_random();
}

void sp_client_update() {
    dn_update();

    sp_client.nk = snk_new_frame();
    nk_context* nk = sp_client.nk;

    nk_begin(nk, "spum", nk_rect(10, 10, 400, 800), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE);

    if (nk_tree_push(nk, NK_TREE_TAB, "Client Information", NK_MINIMIZED)) {
        nk_layout_row_dynamic(nk, 0, 2);

        nk_dn_string(nk, dn_string_literal("Username: "), NK_TEXT_LEFT);
        nk_edit_dn_string(nk, NK_EDIT_SIMPLE, &sp_client.username, SP_MAX_USERNAME_LEN, nk_filter_default);

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

        nk_edit_dn_string(nk, NK_EDIT_SIMPLE, &sp_client.match_search.password, SP_MAX_PASSWORD_LEN, nk_filter_default);

        if (nk_button_label(nk, "Search")) {
            sp_client.match_search.search_requested = true;
            sp_client.match_search.deck = sp_client.deck;
        }

        if (nk_button_label(nk, "Cancel")) {
            sp_client.match_search.cancel_requested = true;
        }

        if (sp_client.state == SP_CLIENT_STATE_MATCH_WAIT) {
            dn_string_builder_t builder = dn_tstring_builder();
            dn_string_builder_append(&builder, dn_string_literal("Searching for match with password: "));
            dn_string_builder_append(&builder, sp_client.match_search.password);
            
            nk_layout_row_dynamic(nk, 0, 1);
            nk_label(nk, dn_string_builder_write_cstr(&builder), NK_TEXT_LEFT);
        }

        nk_tree_pop(nk);
    }
    nk_end(nk);

    switch (sp_client.state) {
        case SP_CLIENT_STATE_WS_INIT: {
            u16 is_ws_ready = 0;
            emscripten_websocket_get_ready_state(sp_client.websocket, &is_ws_ready);

            if (is_ws_ready) {
                DN_LOG("SP_CLIENT_STATE_WS_INIT");
                sp_client.state = SP_CLIENT_STATE_TOKEN_WAIT;

                // @string Copy your username into an over-the-wire buffer
                sp_net_request_t request = dn_zero_initialize();
                request.op = SP_OPCODE_REQUEST_TOKEN;

                sp_username_t* username = &request.token_req.username;
                dn_string_copy_to_str_buffer(sp_client.username, username);

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
            if (sp_client.match_search.search_requested) {
                sp_client.match_search.search_requested = false;

                sp_net_request_t request;
                request.op = SP_OPCODE_MATCH_REQUEST;
                request.match = (sp_net_match_request_t){
                    .password = dn_hash_string(sp_client.match_search.password),
                    .deck = sp_client.deck,
                };
                sp_client_submit_request(&request);

                sp_client.state = SP_CLIENT_STATE_MATCH_WAIT;
            }
            break;
        }
        case SP_CLIENT_STATE_MATCH_WAIT: {
            if (sp_client.match_search.ready) {
                sp_client.match.deck = sp_client.match_search.deck;
                sp_client.state = SP_CLIENT_STATE_MATCH;
            }
            break;
        }
        case SP_CLIENT_STATE_MATCH: {
            dn_string_builder_t builder = dn_tstring_builder();
            dn_string_builder_append(&builder, sp_client.username);
            dn_string_builder_append(&builder, dn_string_literal(" vs. "));
            dn_string_builder_append(&builder, sp_client.match.username);
            
            nk_begin(nk, dn_string_builder_write_cstr(&builder), nk_rect(420, 10, 800, 800), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE);
            nk_layout_row_dynamic(nk, 0, 2);

            if (nk_group_begin(nk, dn_string_to_cstr(sp_client.username), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER)) {
                sp_deck_count_t count = sp_deck_count(&sp_client.deck);

                if (nk_tree_push(nk, NK_TREE_TAB, "Hand", NK_MINIMIZED)) {
                    nk_layout_row_static(nk, 12, 100, 1);
                    sp_for_card(it, sp_client.match.hand) {
                        sp_card_id_t id = sp_card_iter_get(&it);
                        sp_card_t* card = &sp_cards[id];
                        nk_dn_string_colored(nk, card->pokemon.name, NK_TEXT_LEFT, dn_colors.zomp);
                    }

                    nk_tree_pop(nk);
                }
                sp_deck_count_remove_cards(&count, sp_client.match.hand, SP_HAND_SIZE);
                

                if (nk_tree_push(nk, NK_TREE_TAB, "Discard", NK_MINIMIZED)) {
                    nk_layout_row_static(nk, 12, 100, 1);
                    sp_for_card(it, sp_client.match.discard) {
                        sp_card_id_t id = sp_card_iter_get(&it);
                        sp_card_t* card = &sp_cards[id];
                        nk_dn_string_colored(nk, card->pokemon.name, NK_TEXT_LEFT, dn_colors.indian_red);
                    }

                    nk_tree_pop(nk);
                }
                sp_deck_count_remove_cards(&count, sp_client.match.discard, SP_DECK_SIZE);

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
            
            if (nk_group_begin(nk, dn_string_to_cstr(sp_client.match.username), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER)) {
                sp_deck_count_t count = sp_deck_count(&sp_client.deck);

                if (nk_tree_push(nk, NK_TREE_TAB, "Hand", NK_MINIMIZED)) {
                    nk_layout_row_static(nk, 12, 100, 1);
                    sp_for_card(it, sp_client.match.hand) {
                        sp_card_id_t id = sp_card_iter_get(&it);
                        sp_card_t* card = &sp_cards[id];
                        nk_dn_string_colored(nk, card->pokemon.name, NK_TEXT_LEFT, dn_colors.zomp);
                    }

                    nk_tree_pop(nk);
                }

                sp_deck_count_remove_cards(&count, sp_client.match.hand, SP_HAND_SIZE);
                nk_group_end(nk);
            }
            nk_end(nk);

            break;
        }
        default: {
            DN_UNREACHABLE();
            break;
        }
    }


    draw_demo_ui(nk);

    sp_client_render();
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

void sp_client_generate_username() {
    dn_string_t adjective = sp_username_adjectives[sp_rng_ranged_u32(0, dn_arr_len(sp_username_adjectives) - 1)];
    dn_string_t noun = sp_username_nouns[sp_rng_ranged_u32(0, dn_arr_len(sp_username_nouns) - 1)];

    dn_string_builder_t builder = {
        .allocator = &dn_allocators.bump.allocator,
        .buffer = {
            .data = sp_client.username.data,
            .count = 0,
            .capacity = SP_MAX_USERNAME_LEN,
        }
    };
    dn_string_builder_append(&builder, adjective);
    dn_string_builder_append(&builder, dn_string_literal("_"));
    dn_string_builder_append(&builder, noun);

    // @string Copy a string into a pre-allocated dn_string_t
    dn_string_copy_to_str(dn_string_builder_write(&builder), &sp_client.username, SP_MAX_USERNAME_LEN);
}


///////////////
// WEBSOCKET //
///////////////
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
        case SP_OPCODE_BEGIN_MATCH: {
            DN_LOG("SP_OPCODE_BEGIN_MATCH");
            sp_client.match_search.ready = true;
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

void sp_client_process_match_event(sp_net_match_event_t* event) {
    switch(event->kind) {
        case SP_MATCH_EVENT_BEGIN: {
            DN_LOG("SP_MATCH_EVENT_BEGIN");
            // @string Copy the opponent's over-the-wire username into a string
            sp_client.match.username = dn_string_copy(dn_str_buffer_view(event->begin.username), &dn_allocators.standard.allocator);
            dn_os_arr_copy(event->begin.hand, sp_client.match.hand);
            dn_os_arr_copy(event->begin.energy, sp_client.match.energy);
            dn_os_arr_copy(event->begin.opponent_energy, sp_client.match.opponent_energy);
            sp_client.match.turn = event->begin.turn;
            break;
        }
        default: {
            DN_UNREACHABLE();
            break;
        }
    }
}

void sp_client_submit_request(sp_net_request_t* request) {
    dn_os_memory_copy(sp_magic, request->magic, sizeof(sp_net_magic_t));
    request->token = sp_client.token;
    emscripten_websocket_send_binary(sp_client.websocket, request, sizeof(sp_net_request_t));
}

dn_string_t sp_client_state_to_string(sp_client_state_t state) {
    switch (state) {
        case SP_CLIENT_STATE_INIT:               return dn_string_literal("SP_CLIENT_STATE_INIT");
        case SP_CLIENT_STATE_WS_INIT:            return dn_string_literal("SP_CLIENT_WS_INIT");
        case SP_CLIENT_STATE_TOKEN_WAIT:         return dn_string_literal("SP_CLIENT_TOKEN_WAIT");
        case SP_CLIENT_STATE_IDLE:               return dn_string_literal("SP_CLIENT_IDLE");
        case SP_CLIENT_STATE_MATCH_WAIT:         return dn_string_literal("SP_CLIENT_MATCH_WAIT");
        case SP_CLIENT_STATE_MATCH:              return dn_string_literal("SP_CLIENT_MATCH");
        default:                                 return dn_string_literal("SP_CLIENT_UNKNOWN");
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

#endif