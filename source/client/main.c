#include "emscripten/emscripten.h"
#include "emscripten/websocket.h"
#include "emscripten/trace.h"


#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_VARARGS

#define NK_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_demo.h"

#define SOKOL_IMPL
#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#include "sokol_nuklear.h"

#define DN_MATH_BACKEND_HANDMADE
#define DN_NUKLEAR
#define DN_IMPL
#include "dn.h"

#include "sp.h"

#include "client.h"

#define SP_CLIENT_APP_IMPL
#include "client.h"
#define SP_IMPL
#include "sp.h"

sapp_desc sokol_main(int num_args, char** args) {
    return (sapp_desc){
        .init_cb = sp_client_init,
        .frame_cb = sp_client_update,
        .cleanup_cb = sp_client_shutdown,
        .event_cb = sp_client_event,
        .width = 400,
        .height = 300,
        .window_title = "spum",
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}