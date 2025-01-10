#ifndef SP_CLIENT_APP_H
#define SP_CLIENT_APP_H

typedef EMSCRIPTEN_WEBSOCKET_T sp_ws_t;

typedef enum {
    SP_CLIENT_STATE_INIT,
    SP_CLIENT_STATE_WS_INIT,
    SP_CLIENT_STATE_TOKEN_WAIT,
    SP_CLIENT_STATE_IDLE,
    SP_CLIENT_STATE_MATCH_WAIT,
} sp_client_state_t;

typedef struct {
    dn_string_t password;
    bool search_requested;
    bool cancel_requested;
} sp_client_match_search_t;

typedef struct {
    sg_pass_action pass_action;
    
    sp_ws_t websocket;
    sp_token_t token;

    sp_client_state_t state;
    sp_client_match_search_t match_search;

    sp_deck_t deck;

    nk_context* nk;
} sp_client_t;
sp_client_t sp_client;

void        sp_client_init();
void        sp_client_update();
void        sp_client_render();
void        sp_client_shutdown();
bool        sp_client_on_websocket_open(int event_type, const EmscriptenWebSocketOpenEvent* event, void* user_data);
bool        sp_client_on_websocket_error(int event_type, const EmscriptenWebSocketErrorEvent* event, void* user_data);
bool        sp_client_on_websocket_close(int event_type, const EmscriptenWebSocketCloseEvent* event, void* user_data);
bool        sp_client_on_websocket_message(int event_type, const EmscriptenWebSocketMessageEvent* event, void* user_data);
void        sp_client_submit_request(sp_net_request_t* request);
dn_string_t sp_client_state_to_string(sp_client_state_t state);
#endif



#ifdef SP_CLIENT_APP_IMPL
void sp_client_init() {
    dn_init();

    dn_log("spumc");

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
    sp_client.match_search.password = (dn_string_t){
        .data = (u8*)dn_allocator_alloc(&dn_allocators.standard.allocator, SP_MAX_PASSWORD_LEN),
        .len = 0
    };

    sp_client.deck = (sp_deck_t){
        .cards = dn_zero_initialize(),
        .energy = { SP_POKEMON_TYPE_GRASS, SP_POKEMON_TYPE_NONE, SP_POKEMON_TYPE_NONE }
    };
    dn_for(i, 20) {
        sp_client.deck.cards[i] = sp_rng_pick_from_set((u32 []) { SP_CARD_BULBASAUR, SP_CARD_WEEDLE, SP_CARD_ODDISH }, 3);
    }
}

void sp_client_update() {
    dn_update();

    sp_client.nk = snk_new_frame();
    nk_context* nk = sp_client.nk;

    nk_begin(nk, "spum", nk_rect(10, 10, 400, 400), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE);

    if (nk_tree_push(nk, NK_TREE_TAB, "Client Information", NK_MINIMIZED)) {
        nk_layout_row_dynamic(nk, 0, 2);

        dn_string_builder_t builder = dn_tstring_builder();
        dn_string_builder_append_fmt(&builder, dn_string_literal("%llu"), sp_client.token);
        nk_dn_string(nk, dn_string_literal("ID: "), NK_TEXT_LEFT);
        nk_dn_string(nk, dn_string_builder_write(&builder), NK_TEXT_LEFT);

        nk_dn_string(nk, dn_string_literal("State: "), NK_TEXT_LEFT);
        nk_dn_string(nk, sp_client_state_to_string(sp_client.state), NK_TEXT_LEFT);

        nk_tree_pop(nk);
    }    

    if (nk_tree_push(nk, NK_TREE_TAB, "Decks", NK_MINIMIZED)) {
        nk_layout_row_dynamic(nk, 0, 4);

        gs_hash_table(sp_card_id_t, bool) 
        dn_for(i, 20) {
            sp_card_t card = sp_client.deck.cards[i];
            nk_labelf()
            card.
        }
        nk_labelf()
    }

2 Ralts A1 130
2 Kirlia A1 131
2 Gardevoir A1 132
2 Mewtwo ex A1 129
1 Jynx A1 127

2 Professor's Research P-A 7
1 Giovanni A1 223
1 Sabrina A1 225
1 Leaf A1a 68
2 Poké Ball P-A 5
2 Mythical Slab A1a 65
1 X Speed P-A 2
1 Red Card P-A 6
    if (nk_tree_push(nk, NK_TREE_TAB, "Find a Match", NK_MINIMIZED)) {
        nk_layout_row(nk, NK_STATIC, 30, 3, NK_RATIO(150, 100, 100));

        nk_edit_dn_string(nk, NK_EDIT_SIMPLE, &sp_client.match_search.password, SP_MAX_PASSWORD_LEN, nk_filter_default);

        if (nk_button_label(nk, "Search")) {
            sp_client.match_search.search_requested = true;
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

    switch (sp_client.state) {
        case SP_CLIENT_STATE_WS_INIT: {
            u16 is_ws_ready = 0;
            emscripten_websocket_get_ready_state(sp_client.websocket, &is_ws_ready);

            if (is_ws_ready) {
                DN_LOG("SP_CLIENT_STATE_WS_INIT");
                sp_client.state = SP_CLIENT_STATE_TOKEN_WAIT;

                sp_net_request_t request = dn_zero_initialize();
                request.op = SP_OPCODE_REQUEST_TOKEN;
                sp_client_submit_request(&request);
            }
            break;
        }
        case SP_CLIENT_STATE_TOKEN_WAIT: {
            break;
        }
        case SP_CLIENT_STATE_IDLE: {
            if (sp_client.match_search.search_requested) {
                sp_client.match_search.search_requested = false;

                sp_net_request_t request;
                request.op = SP_OPCODE_MATCH_REQUEST;
                request.match.password = dn_hash_string(sp_client.match_search.password);
                request.match.deck = sp_client.deck;
                sp_client_submit_request(&request);

                DN_LOG("sizeof(size_t) == %d, %zu", sizeof(size_t), request.match.password);

                sp_client.state = SP_CLIENT_STATE_MATCH_WAIT;
            }
            break;
        }
        case SP_CLIENT_STATE_MATCH_WAIT: {
            
            break;
        }

        default: {
            DN_UNREACHABLE();
            break;
        }
    }

    nk_end(nk);

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

            DN_ASSERT(sp_client.state == SP_CLIENT_STATE_TOKEN_WAIT);
            sp_client.state = SP_CLIENT_STATE_IDLE;
            break;
        }
        case SP_OPCODE_MATCH_REQUEST: {
            dn_log("SP_OPCODE_ECHO");
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
