#ifndef SP_CLIENT_APP_H
#define SP_CLIENT_APP_H

#define sp_ws_t EMSCRIPTEN_WEBSOCKET_T

typedef struct {
    sg_pass_action pass_action;
    sp_ws_t websocket;
} sp_app_t;
sp_app_t sp_app;

void sp_client_init();
void sp_client_update();
void sp_client_shutdown();
bool sp_client_on_websocket_open(int event_type, const EmscriptenWebSocketOpenEvent* event, void* user_data);
bool sp_client_on_websocket_error(int event_type, const EmscriptenWebSocketErrorEvent* event, void* user_data);
bool sp_client_on_websocket_close(int event_type, const EmscriptenWebSocketCloseEvent* event, void* user_data);
bool sp_client_on_websocket_message(int event_type, const EmscriptenWebSocketMessageEvent* event, void* user_data);
void sp_client_match_request(sp_net_match_request_t* request);
#endif



#ifdef SP_CLIENT_APP_IMPL
void sp_client_init() {
    dn_init();

    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });

    snk_setup(&(snk_desc_t){
        .enable_set_mouse_cursor = true,
        .dpi_scale = sapp_dpi_scale(),
        .logger.func = slog_func,
    });

    sp_app.pass_action = (sg_pass_action) {
        .colors[0] = {
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = { 1.0f, 0.0f, 0.0f, 1.0f }
        }
    };

    sp_app.websocket = emscripten_websocket_new(&(EmscriptenWebSocketCreateAttributes){
        .url = "ws://localhost:8000",
        .protocols = "example-protocol",
        .createOnMainThread = EM_TRUE
    });
    if (sp_app.websocket <= 0) {
        printf("%s: websocket creation failed; error = %d\n", __func__, sp_app.websocket);
        return;
    }
    emscripten_websocket_set_onopen_callback(sp_app.websocket, NULL, sp_client_on_websocket_open);
    emscripten_websocket_set_onerror_callback(sp_app.websocket, NULL, sp_client_on_websocket_error);
    emscripten_websocket_set_onclose_callback(sp_app.websocket, NULL, sp_client_on_websocket_close);
    emscripten_websocket_set_onmessage_callback(sp_app.websocket, NULL, sp_client_on_websocket_message);
}

enum {EASY, HARD};
static int op = EASY;
static float value = 0.6f;
static int i =  20;

void sp_client_update() {
    // u16 is_ws_ready = false;
    // emscripten_websocket_get_ready_state(sp_app.websocket, &is_ws_ready);
        // u8 buffer [] = { 69, 10, 20, 30, 40, 50 };
        // emscripten_websocket_send_binary(sp_app.websocket, buffer, dn_arr_len(buffer));


    float g = sp_app.pass_action.colors[0].clear_value.g + 0.01f;
    sp_app.pass_action.colors[0].clear_value.g = (g > 1.0f) ? 0.0f : g;

    struct nk_context* nk = snk_new_frame();

    if (nk_begin(nk, "Show", nk_rect(50, 50, 220, 220), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
      if (nk_button_label(nk, "button")) {
          // event handling
      }
    }
    nk_end(nk);

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

void sp_client_match_request(sp_net_match_request_t* request) {
  
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
    printf("%s\n", __func__);

    if (event->isText) {
        printf("message: %s\n", event->data);
    }

    return true;
}

#endif
