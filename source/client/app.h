#ifndef SP_CLIENT_APP_H
#define SP_CLIENT_APP_H

#define sp_ws_t EMSCRIPTEN_WEBSOCKET_T

typedef enum {
    SP_CLIENT_STATE_INIT,
    SP_CLIENT_STATE_WS_INIT,
    SP_CLIENT_STATE_TOKEN_WAIT,
    SP_CLIENT_STATE_MATCH_WAIT
} sp_client_state_t;

typedef struct {
    sp_token_t uuid;
    sp_client_state_t state;
    sg_pass_action pass_action;
    sp_ws_t websocket;
    dn_string_t match_password;
} sp_client_t;
sp_client_t sp_client;

void sp_client_init();
void sp_client_update();
void sp_client_shutdown();
bool sp_client_on_websocket_open(int event_type, const EmscriptenWebSocketOpenEvent* event, void* user_data);
bool sp_client_on_websocket_error(int event_type, const EmscriptenWebSocketErrorEvent* event, void* user_data);
bool sp_client_on_websocket_close(int event_type, const EmscriptenWebSocketCloseEvent* event, void* user_data);
bool sp_client_on_websocket_message(int event_type, const EmscriptenWebSocketMessageEvent* event, void* user_data);
void sp_client_submit_request(sp_net_request_t* request);
void sp_client_match_request(sp_net_match_request_t* request);
dn_string_t sp_client_state_to_string(sp_client_state_t state);
#endif



#ifdef SP_CLIENT_APP_IMPL
void sp_client_init() {
    dn_init();

    dn_log("spumc");

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

    sp_client.match_password = (dn_string_t){
        .data = (u8*)dn_allocator_alloc(&dn_allocators.bump.allocator, SP_MAX_PASSWORD_LEN),
        .len = 0
    };

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
}

void sp_client_update() {
    switch (sp_client.state) {
        case SP_CLIENT_STATE_WS_INIT: {
            u16 is_ws_ready = 0;
            emscripten_websocket_get_ready_state(sp_client.websocket, &is_ws_ready);

            if (is_ws_ready) {
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
        default: {
            DN_UNREACHABLE();
            break;
        }
    }
    struct nk_context* nk = snk_new_frame();

    dn_string_t client_state = sp_client_state_to_string(sp_client.state);
    printf("state: %d, size = %d\n", sp_client.state, client_state.len);
    if (nk_begin(nk, "spum", nk_rect(50, 50, 400, 400), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE)) {
        nk_layout_row_dynamic(nk, 0, NK_TEXT_LEFT);
        nk_dn_string(nk, sp_client_state_to_string(sp_client.state), NK_TEXT_LEFT);

        nk_layout_row(nk, NK_STATIC, 30, 100, NK_RATIO(200, 100));
        nk_edit_dn_string(nk, NK_EDIT_SIMPLE, sp_client.match_password, SP_MAX_PASSWORD_LEN, nk_filter_default);
        if (nk_button_label(nk, "Search")) {
            
        }
    }
    nk_end(nk);

    draw_demo_ui(nk);

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

void sp_client_process_response(sp_net_response_t* response) {
    switch (response->op) {
        case SP_OPCODE_ECHO: {
            dn_log("SP_OPCODE_ECHO");
            break;
        }
        case SP_OPCODE_REQUEST_TOKEN: {
            dn_log("SP_OPCODE_ECHO");
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
    emscripten_websocket_send_binary(sp_client.websocket, request, sizeof(sp_net_request_t));
}

void sp_client_match_request(sp_net_match_request_t* request) {
  
}

dn_string_t sp_client_state_to_string(sp_client_state_t state) {
    switch (state) {
        case SP_CLIENT_STATE_INIT:       return dn_string_literal("SP_CLIENT_STATE_INIT");
        case SP_CLIENT_STATE_WS_INIT:    return dn_string_literal("SP_CLIENT_WS_INIT");
        case SP_CLIENT_STATE_TOKEN_WAIT: return dn_string_literal("SP_CLIENT_TOKEN_WAIT");
        case SP_CLIENT_STATE_MATCH_WAIT: return dn_string_literal("SP_CLIENT_MATCH_WAIT");
        default:                         return dn_string_literal("SP_CLIENT_UNKNOWN");
    }
}

bool sp_client_on_websocket_open(int event_type, const EmscriptenWebSocketOpenEvent* event, void* user_data) {
    printf("%s\n", __func__);
    return true;
}

bool sp_client_on_websocket_error(int event_type, const EmscriptenWebSocketErrorEvent* event, void* user_data) {
    printf("%s\n", __func__);
    return true;
}

bool sp_client_on_websocket_close(int event_type, const EmscriptenWebSocketCloseEvent* event, void* user_data) {
    printf("%s\n", __func__);
    return true;
}

bool sp_client_on_websocket_message(int event_type, const EmscriptenWebSocketMessageEvent* event, void* user_data) {
    DN_ASSERT(event->numBytes == sizeof(sp_net_response_t));
    sp_net_response_t* response = (sp_net_response_t*)event->data;
    sp_client_process_response(response);
    return true;
}

#endif
