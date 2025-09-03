#include "stdint.h"
#include "assert.h"
#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>

//  ████████╗██╗   ██╗██████╗ ███████╗███████╗
//  ╚══██╔══╝╚██╗ ██╔╝██╔══██╗██╔════╝██╔════╝
//     ██║    ╚████╔╝ ██████╔╝█████╗  ███████╗
//     ██║     ╚██╔╝  ██╔═══╝ ██╔══╝  ╚════██║
//     ██║      ██║   ██║     ███████╗███████║
//     ╚═╝      ╚═╝   ╚═╝     ╚══════╝╚══════╝                              
#ifndef DN_NO_HIJACK_NUMERIC_TYPES
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;
#endif


typedef u64 dn_hash_t;

#define DN_ASSET_NAME_LEN 64
typedef char dn_asset_name_t [DN_ASSET_NAME_LEN];

//  ███╗   ███╗ █████╗  ██████╗██████╗  ██████╗ ███████╗
//  ████╗ ████║██╔══██╗██╔════╝██╔══██╗██╔═══██╗██╔════╝
//  ██╔████╔██║███████║██║     ██████╔╝██║   ██║███████╗
//  ██║╚██╔╝██║██╔══██║██║     ██╔══██╗██║   ██║╚════██║
//  ██║ ╚═╝ ██║██║  ██║╚██████╗██║  ██║╚██████╔╝███████║
//  ╚═╝     ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝

#define dn_quad_literal(top, bottom, left, right) \
    {                                     \
        { left, top },                    \
        { left, bottom },                 \
        { right, bottom },                \
                                          \
        { left, top },                    \
        { right, bottom },                \
        { right, top }                    \
    }                                     


#define dn_max(a, b) (a) > (b) ? (a) : (b)
#define dn_min(a, b) (a) > (b) ? (b) : (a)

#define dn_arr_len(carray) (sizeof((carray)) / sizeof((carray)[0]))

#define dn_for(it, n) for (u32 it = 0; it < (n); it++)
#define dn_for_arr(arr, it) for (u32 it = 0; it < dn_arr_len(arr); it++)

#define dn_macro_str(x) #x
#define _dn_macro_cat(x, y) x##y
#define dn_macro_cat(x, y) _dn_macro_cat(x, y)
#define dn_unique_name() dn_macro_cat(__dn_unique_name__, __LINE__)

#define dn_typedef_fn(return_type, name, ...) typedef return_type(*name)(__VA_ARGS__)

// #define DN_API __declspec(dllexport)
#define DN_API
#define DN_IMP

#define dn_align(n) __declspec(align(n))
#define DN_ASSERT(condition) assert(condition)
#define DN_SOFT_ASSERT(condition) DN_ASSERT(condition) // For things you want to crash in development, but fix before release (as opposed to assers which are left in release builds and, say, logged)
#define DN_UNTESTED() DN_ASSERT(false)
#define DN_UNREACHABLE() DN_ASSERT(false)
#define DN_UNREACHABLE_MESSAGE(message) DN_ASSERT(false && (message))
#define DN_BROKEN() DN_ASSERT(false)

#define dn_zero_initialize() { 0 }
#define dn_zero_struct(t) (t){ 0 }
#define dn_zero_return(t) { t __dn_zero_return = dn_zero_initialize(); return __dn_zero_return; }
#define dn_type_name(t) (#t)

#define dn_enum_flags(t) DEFINE_ENUM_FLAG_OPERATORS(t)

#define DN_FMT_U64 PRIu64

#define dn_ptr_lval(type, rvalue) (&(type){ rvalue })
#define dn_arr_lval(type, ...) ((type []){ __VA_ARGS__ })

#define dn_swap(t, a, b) { t dn_unique_name() = (a); (a) = (b); (b) = dn_unique_name(); }

#define DN_QSORT_A_FIRST -1
#define DN_QSORT_B_FIRST 1
#define DN_QSORT_EQUAL 0

#include "gs.h"

#define gs_hash_table_for(_HT, it) for (gs_hash_table_iter it = 0; gs_hash_table_iter_valid((_HT), it); gs_hash_table_iter_advance((_HT), it))

//  ███╗   ███╗ █████╗ ████████╗██╗  ██╗
//  ████╗ ████║██╔══██╗╚══██╔══╝██║  ██║
//  ██╔████╔██║███████║   ██║   ███████║
//  ██║╚██╔╝██║██╔══██║   ██║   ██╔══██║
//  ██║ ╚═╝ ██║██║  ██║   ██║   ██║  ██║
//  ╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝

#ifdef DN_MATH_BACKEND_HANDMADE
#include "HandmadeMath.h"
typedef HMM_Vec2 dn_vector2_t;
typedef HMM_Vec3 dn_vector3_t;
typedef HMM_Vec4 dn_vector4_t;
#else 
typedef union {
  struct {
    float x;
    float y;
  };

  struct {
    float r;
    float g;
  };
} dn_vector2_t;

typedef union {
  struct {
    float x;
    float y;
    float z;
  };

  struct {
    float r;
    float g;
    float b;
  };
} dn_vector3_t;

typedef union {
  struct {
    float x;
    float y;
    float z;
    float w;
  };
  struct {
    float r;
    float g;
    float b;
    float a;
  };
} dn_vector4_t;
#endif

typedef struct {
	s32 x;
	s32 y;
} dn_vector2i_t;


f32          dn_math_step(f32 edge, f32 x);
f32          dn_math_lerp(f32 a, f32 b, f32 t);
dn_vector4_t dn_math_lerp4(dn_vector4_t a, dn_vector4_t b, f32 t);
dn_vector4_t dn_math_abs4(dn_vector4_t v);
dn_vector4_t dn_math_fract4(dn_vector4_t v);
dn_vector4_t dn_math_clamp4(dn_vector4_t v, float minVal, float maxVal);


//   ██████╗ ██████╗ ██╗      ██████╗ ██████╗ ███████╗
//  ██╔════╝██╔═══██╗██║     ██╔═══██╗██╔══██╗██╔════╝
//  ██║     ██║   ██║██║     ██║   ██║██████╔╝███████╗
//  ██║     ██║   ██║██║     ██║   ██║██╔══██╗╚════██║
//  ╚██████╗╚██████╔╝███████╗╚██████╔╝██║  ██║███████║
//   ╚═════╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝
typedef dn_vector4_t dn_color_t;
#define dn_rgb_255(red, green, blue) (dn_color_t) { .r = (red) / 255.f, .g = (green) / 255.f, .b = (blue) / 255.f, .a = 1.0 }
#define dn_rgb_01(red, green, blue) (dn_color_t) { .r = red, .g = green, .b = blue, .a = 1.0 }

typedef struct {
  dn_color_t indian_red;
	dn_color_t tyrian_purple;
	dn_color_t cardinal;
  dn_color_t celadon;
	dn_color_t spring_green;     
	dn_color_t mindaro;          
	dn_color_t light_green;      
  dn_color_t zomp;
	dn_color_t midnight_green;   
	dn_color_t prussian_blue;    
	dn_color_t orange;           
	dn_color_t sunglow;          
	dn_color_t selective_yellow; 
  dn_color_t gunmetal;
	dn_color_t paynes_gray;
	dn_color_t cadet_gray;
	dn_color_t charcoal;
	dn_color_t cool_gray;
	dn_color_t cream;            
	dn_color_t misty_rose;       
	dn_color_t taupe;            
	dn_color_t dark_green;       
	dn_color_t rich_black;       
	dn_color_t white;
} dn_colors_t;

dn_colors_t dn_colors = (dn_colors_t) {
  .indian_red       = dn_rgb_255(180, 101, 111), // RED
	.tyrian_purple    = dn_rgb_255(95,  26,  55),
	.cardinal         = dn_rgb_255(194, 37,  50),
  .celadon          = dn_rgb_255(183, 227, 204), // GREEN
	.spring_green     = dn_rgb_255(89,  255, 160),
	.mindaro          = dn_rgb_255(188, 231, 132),
	.light_green      = dn_rgb_255(161, 239, 139),
  .zomp             = dn_rgb_255(99,  160, 136), 
	.midnight_green   = dn_rgb_255(25,  83,  95),
	.prussian_blue    = dn_rgb_255(16,  43,  63),  // BLUE
	.orange           = dn_rgb_255(249, 166, 32),  // ORANGE
	.sunglow          = dn_rgb_255(255, 209, 102), 
  .selective_yellow = dn_rgb_255(250, 188, 42),  
  .gunmetal         = dn_rgb_255(43,  61,  65),  // GRAY
	.paynes_gray      = dn_rgb_255(76,  95,  107),
	.cadet_gray       = dn_rgb_255(131, 160, 160),
	.charcoal         = dn_rgb_255(64,  67,  78),
	.cool_gray        = dn_rgb_255(140, 148, 173),
	.cream            = dn_rgb_255(245, 255, 198),  // OFF WHITE
	.misty_rose       = dn_rgb_255(255, 227, 227),
	.taupe            = dn_rgb_255(68,  53,  39),   // BROWN
	.dark_green       = dn_rgb_255(4,   27,  21),   // BLACK
	.rich_black       = dn_rgb_255(4,   10,  15),
	.white            = dn_rgb_255(255, 255, 255),
};

dn_color_t dn_color_rgb_to_hsv(dn_color_t color);
dn_color_t dn_color_hsv_to_rgb(dn_color_t color);


//  ███╗   ███╗███████╗███╗   ███╗ ██████╗ ██████╗ ██╗   ██╗
//  ████╗ ████║██╔════╝████╗ ████║██╔═══██╗██╔══██╗╚██╗ ██╔╝
//  ██╔████╔██║█████╗  ██╔████╔██║██║   ██║██████╔╝ ╚████╔╝ 
//  ██║╚██╔╝██║██╔══╝  ██║╚██╔╝██║██║   ██║██╔══██╗  ╚██╔╝  
//  ██║ ╚═╝ ██║███████╗██║ ╚═╝ ██║╚██████╔╝██║  ██║   ██║   
//  ╚═╝     ╚═╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   
typedef enum {
	DN_ALLOCATOR_MODE_ALLOC,
	DN_ALLOCATOR_MODE_FREE,
	DN_ALLOCATOR_MODE_RESIZE,
} dn_allocator_mode_t;

struct dn_allocator_t;

dn_typedef_fn(
	void*, 
	dn_alloc_fn_t, 
	struct dn_allocator_t* allocator, dn_allocator_mode_t mode, u32 size, void* previous_allocation
);

typedef struct dn_allocator_t {
	dn_alloc_fn_t on_alloc;
} dn_allocator_t;

typedef struct {
  dn_allocator_t allocator;
	u8* buffer;
	u32 capacity;
	u32 bytes_used;
  gs_hash_table(u32, u32) allocations;
} dn_bump_allocator_t;

typedef struct {
  dn_allocator_t allocator;
} dn_standard_allocator_t;

typedef struct {
	dn_standard_allocator_t standard;
	dn_bump_allocator_t bump;
  gs_hash_table(dn_hash_t, dn_allocator_t*) allocators;
} dn_allocators_t;
dn_allocators_t dn_allocators;

DN_API void             dn_allocator_add(const char* name, dn_allocator_t* allocator);
DN_API dn_allocator_t*  dn_allocator_find(const char* name);
DN_API void*            dn_allocator_alloc(dn_allocator_t* allocator, u32 size);
DN_API void*            dn_allocator_realloc(dn_allocator_t* allocator, void* memory, u32 size);
DN_API void             dn_allocator_free(dn_allocator_t* allocator, void* buffer);
DN_IMP void             dn_bump_allocator_init(dn_bump_allocator_t* allocator, u32 capacity);
DN_IMP void             dn_bump_allocator_clear(dn_bump_allocator_t* allocator);
DN_IMP void*            dn_bump_allocator_on_alloc(dn_allocator_t* allocator, dn_allocator_mode_t mode, u32 size, void* buffer);
DN_IMP void             dn_standard_allocator_init(dn_standard_allocator_t* allocator);
DN_IMP void*            dn_standard_allocator_on_alloc(dn_allocator_t* allocator, dn_allocator_mode_t mode, u32 size, void* buffer);
DN_IMP void             dn_allocators_init();
DN_IMP void             dn_allocators_update();


//  ███████╗████████╗██████╗ ██╗███╗   ██╗ ██████╗ 
//  ██╔════╝╚══██╔══╝██╔══██╗██║████╗  ██║██╔════╝ 
//  ███████╗   ██║   ██████╔╝██║██╔██╗ ██║██║  ███╗
//  ╚════██║   ██║   ██╔══██╗██║██║╚██╗██║██║   ██║
//  ███████║   ██║   ██║  ██║██║██║ ╚████║╚██████╔╝
//  ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝ 
typedef struct {
  u32 len;
  u8* data;
} dn_string_t;

// dn_string_buffer_t is for defining fixed size, stack allocated strings that have a length attached. Then, you
// can use dn_str_buffer_view() or dn_str_buffer_view_ptr() to turn it into a dn_string_t and use the normal
// string APIs.
#define dn_str_buffer_t(n) struct { u32 len; u8 data [n]; }
#define dn_str_buffer_capacity(buffer) (sizeof((buffer)->data))
#define dn_str_buffer_view(buffer) (dn_string_t) { .len = (buffer).len, .data = (buffer).data }
#define dn_str_buffer_view_ptr(buffer) ((dn_string_t*)(buffer))
#define dn_string_copy_to_str_buffer(str, buffer) { dn_string_copy_to((str), (buffer)->data, dn_str_buffer_capacity(buffer)); (buffer)->len = (str).len; }

typedef char* dn_tstring_t;
#define DN_MAX_PATH_LEN 256
typedef char dn_path_t [DN_MAX_PATH_LEN];

typedef struct {
  struct {
    u8* data;
    u32 count;
    u32 capacity;
  } buffer;

  dn_allocator_t* allocator;
} dn_string_builder_t;

#define dn_string_cstr(s) (dn_string_t){ .data = (u8*)(s), .len = strlen(s)}
#define dn_string_literal(s) (dn_string_t){ .data = (u8*)(s), .len = sizeof(s) - 1}
#define dn_tstring_builder() (dn_string_builder_t){ .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump.allocator }

DN_API void        dn_string_builder_grow(dn_string_builder_t* builder, u32 requested_capacity);
DN_API void        dn_string_builder_append(dn_string_builder_t* builder, dn_string_t str);
DN_API void        dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str);
DN_API void        dn_string_builder_append_fmt(dn_string_builder_t* builder, dn_string_t fmt, ...);
DN_API dn_string_t dn_string_builder_write(dn_string_builder_t* builder);
DN_API char*       dn_string_builder_write_cstr(dn_string_builder_t* builder);

DN_IMP char*       dn_cstr_copy(const char* str, dn_allocator_t* allocator);
DN_IMP char*       dn_cstr_copy_n(const char* str, u32 length, dn_allocator_t* allocator);
DN_IMP char*       dn_cstr_copy_u8(const u8* str, u32 length, dn_allocator_t* allocator);
DN_API void        dn_cstr_copy_to(const char* str, char* buffer, u32 buffer_length);
DN_API void        dn_cstr_copy_to_n(const char* str, u32 length, char* buffer, u32 buffer_length);
DN_API bool        dn_cstr_equal(const char* a, const char* b);
DN_API u32         dn_cstr_len(const char* str);

DN_API char*       dn_string_to_cstr(dn_string_t str);
DN_API char*       dn_string_to_cstr_ex(dn_string_t str, dn_allocator_t* allocator);
DN_API dn_string_t dn_string_copy(dn_string_t str, dn_allocator_t* allocator);
DN_API dn_string_t dn_string_copy_cstr_n(const char* str, u32 length, dn_allocator_t* allocator);
DN_API dn_string_t dn_string_copy_cstr(const char* str, dn_allocator_t* allocator);
DN_API void        dn_string_copy_to_str(dn_string_t str, dn_string_t* dest, u32 capacity);
DN_API void        dn_string_copy_to(dn_string_t str, u8* buffer, u32 capacity);
DN_API dn_string_t dn_string_alloc(u32 capacity, dn_allocator_t* allocator);
DN_API bool        dn_string_equal(dn_string_t a, dn_string_t b);
DN_API bool        dn_string_equal_cstr(dn_string_t a, const char* b);

//   ██████╗ ███████╗
//  ██╔═══██╗██╔════╝
//  ██║   ██║███████╗
//  ██║   ██║╚════██║
//  ╚██████╔╝███████║
//   ╚═════╝ ╚══════╝
typedef enum {
  DN_OS_FILE_ATTR_NONE = 0,
  DN_OS_FILE_ATTR_REGULAR_FILE = 1,
  DN_OS_FILE_ATTR_DIRECTORY = 2,
} dn_os_file_attr_t;

typedef struct {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  int millisecond;
} dn_os_date_time_t;

typedef struct {
  dn_string_t file_path;
  dn_string_t file_name;
  dn_os_file_attr_t attributes;
} dn_os_directory_entry_t;

typedef struct {
  dn_os_directory_entry_t* data;
  u32 count;
} dn_os_directory_entry_list_t;

DN_API bool                         dn_os_does_path_exist(dn_string_t path);
DN_API bool                         dn_os_is_regular_file(dn_string_t path);
DN_API bool                         dn_os_is_directory(dn_string_t path);
DN_API void                         dn_os_create_directory(dn_string_t path);
DN_API void                         dn_os_remove_directory(dn_string_t path);
DN_API void                         dn_os_create_file(dn_string_t path);
DN_API void                         dn_os_remove_file(dn_string_t path);
DN_API dn_os_directory_entry_list_t dn_os_scan_directory(dn_string_t path);
DN_API dn_os_directory_entry_list_t dn_os_scan_directory_recursive(dn_string_t path);
DN_API dn_os_date_time_t            dn_os_get_date_time();
DN_API f64                          dn_os_file_mod_time(dn_string_t path);
DN_API void                         dn_os_memory_copy(const void* source, void* dest, u32 num_bytes);
DN_API bool                         dn_os_is_memory_equal(const void* a, const void* b, size_t len);
DN_API void                         dn_os_fill_memory(void* buffer, u32 buffer_size, void* fill, u32 fill_size);
DN_API void                         dn_os_fill_memory_u8(void* buffer, u32 buffer_size, u8 fill);
DN_API void                         dn_os_zero_memory(void* buffer, u32 buffer_size);
DN_IMP dn_os_file_attr_t            dn_os_winapi_attr_to_dn_attr(u32 attr);

#define dn_os_arr_copy(source, dest) { static_assert(sizeof(source) == sizeof(dest), "dn_os_arr_copy expects two C arrays of the same size"); dn_os_memory_copy(source, dest, sizeof(dest)); }


//  ██╗  ██╗ █████╗ ███████╗██╗  ██╗██╗███╗   ██╗ ██████╗ 
//  ██║  ██║██╔══██╗██╔════╝██║  ██║██║████╗  ██║██╔════╝ 
//  ███████║███████║███████╗███████║██║██╔██╗ ██║██║  ███╗
//  ██╔══██║██╔══██║╚════██║██╔══██║██║██║╚██╗██║██║   ██║
//  ██║  ██║██║  ██║███████║██║  ██║██║██║ ╚████║╚██████╔╝
//  ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝ 
// Thank you to John Jackson's framework Gunslinger, from which I took much inspiration but also
// ripped this piece of hashing code off wholesale.
#define GS_SIZE_T_BITS  ((sizeof(size_t)) * 8)
#define GS_SIPHASH_C_ROUNDS 1
#define GS_SIPHASH_D_ROUNDS 1
#define gs_rotate_left(__V, __N)   (((__V) << (__N)) | ((__V) >> (GS_SIZE_T_BITS - (__N))))
#define gs_rotate_right(__V, __N)  (((__V) >> (__N)) | ((__V) << (GS_SIZE_T_BITS - (__N))))

#define DN_HASH_SEED 0x9de341c9

dn_hash_t dn_hash_siphash_bytes(const void *p, size_t len, size_t seed);
dn_hash_t dn_hash_bytes(const void *p, size_t len);
dn_hash_t dn_hash_bytes_ex(const void *p, size_t len, size_t seed);
dn_hash_t dn_combine_hashes(dn_hash_t a, dn_hash_t b);
dn_hash_t dn_hash_cstr_dumb(const char* str);
dn_hash_t dn_hash_str_dumb(dn_string_t str);
dn_hash_t dn_hash_string(dn_string_t str);
void      dn_hash_encode_hex(char* destination, const char* data, size_t len);
void      dn_hash_encode_base64(char* destination, const char* source, size_t len);

#define dn_hash_type(t) dn_hash_string_dumb((const char*)(#t))


//   ██████╗ ██████╗ ███╗   ██╗████████╗ █████╗ ██╗███╗   ██╗███████╗██████╗ ███████╗
//  ██╔════╝██╔═══██╗████╗  ██║╚══██╔══╝██╔══██╗██║████╗  ██║██╔════╝██╔══██╗██╔════╝
//  ██║     ██║   ██║██╔██╗ ██║   ██║   ███████║██║██╔██╗ ██║█████╗  ██████╔╝███████╗
//  ██║     ██║   ██║██║╚██╗██║   ██║   ██╔══██║██║██║╚██╗██║██╔══╝  ██╔══██╗╚════██║
//  ╚██████╗╚██████╔╝██║ ╚████║   ██║   ██║  ██║██║██║ ╚████║███████╗██║  ██║███████║
//   ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝╚══════╝

/////////////////
// FIXED ARRAY //
/////////////////
typedef struct {
  u8* data;
  u32 size;
  u32 capacity;
  u32 element_size;
  dn_allocator_t* allocator;
} dn_fixed_array_t;

#define dn_fixed_array(t, n) dn_fixed_array_t

DN_API void dn_fixed_array_init(dn_fixed_array_t* fixed_array, u32 max_vertices, u32 element_size, dn_allocator_t* allocator);
DN_API u8*  dn_fixed_array_push(dn_fixed_array_t* fixed_array, void* data, u32 count);
DN_API u8*  dn_fixed_array_reserve(dn_fixed_array_t* fixed_array, u32 count);
DN_API void dn_fixed_array_clear(dn_fixed_array_t* fixed_array);
DN_API u32  dn_fixed_array_byte_size(dn_fixed_array_t* fixed_array);
DN_API u8*  dn_fixed_array_at(dn_fixed_array_t* fixed_array, u32 index);


///////////////////
// DYNAMIC ARRAY //
///////////////////
typedef struct {
  u8* data;
  u32 size;
  u32 capacity;
  u32 element_size;
  dn_allocator_t* allocator;
} dn_dynamic_array_t;

#define dn_dynamic_array(t) dn_dynamic_array_t

DN_API void dn_dynamic_array_init(dn_dynamic_array_t* dynamic_array, u32 element_size, dn_allocator_t* allocator);
DN_API u8*  dn_dynamic_array_push(dn_dynamic_array_t* dynamic_array, void* data);
DN_API u8*  dn_dynamic_array_push_n(dn_dynamic_array_t* dynamic_array, void* data, u32 count);
DN_API u8*  dn_dynamic_array_reserve(dn_dynamic_array_t* dynamic_array, u32 count);
DN_API void dn_dynamic_array_clear(dn_dynamic_array_t* dynamic_array);
DN_API u32  dn_dynamic_array_byte_size(dn_dynamic_array_t* dynamic_array);
DN_API u8*  dn_dynamic_array_at(dn_dynamic_array_t* dynamic_array, u32 index);
DN_API void dn_dynamic_array_grow(dn_dynamic_array_t* dynamic_array, u32 capacity);

//////////
// POOL //
//////////
typedef struct {
	u32 index;
	u32 generation;
} dn_pool_handle_t;

typedef struct {
  s32 next_free;
  u32 generation;
  bool occupied;
} dn_pool_slot_t;

typedef struct {
  u8* data;
  dn_pool_slot_t* slots;
  u32 element_size;
  u32 capacity;
  s32 free_list;
} dn_pool_t;

#define dn_pool(t) dn_pool_t

void             dn_pool_init(dn_pool_t* pool, u32 capacity, u32 element_size);
dn_pool_handle_t dn_pool_insert(dn_pool_t* pool, void* value);
dn_pool_handle_t dn_pool_reserve(dn_pool_t* pool);
void             dn_pool_remove(dn_pool_t* pool, dn_pool_handle_t handle);
bool             dn_pool_contains(dn_pool_t* pool, dn_pool_handle_t handle);
void             dn_pool_clear(dn_pool_t* pool);
dn_pool_handle_t dn_pool_invalid_handle();
bool             dn_pool_is_handle_valid(dn_pool_handle_t handle);
bool             dn_pool_slot_has_next_free(dn_pool_slot_t* slot);
bool             dn_pool_slot_is_match(dn_pool_slot_t* slot, dn_pool_handle_t handle);
#define dn_pool_at(T, POOL, HANDLE) ((T*)((POOL)->data + ((HANDLE).index * (POOL)->element_size)))

/////////////////
// RING BUFFER //
/////////////////
typedef struct {
	u8* data;
  u32 element_size;
	u32 head;
	u32 size;
	u32 capacity;
} dn_ring_buffer_t;

#define dn_ring_buffer(t) dn_ring_buffer_t

typedef struct {
	u32 index;
	bool reverse;
	dn_ring_buffer_t* buffer;
} dn_ring_buffer_iterator_t;

void* dn_ring_buffer_at(dn_ring_buffer_t* buffer, u32 index);
void  dn_ring_buffer_init(dn_ring_buffer_t* buffer, u32 capacity, u32 element_size);
void* dn_ring_buffer_back(dn_ring_buffer_t* buffer);
void* dn_ring_buffer_push(dn_ring_buffer_t* buffer, void* data);
void* dn_ring_buffer_push_zero(dn_ring_buffer_t* buffer);
void* dn_ring_buffer_push_overwrite(dn_ring_buffer_t* buffer, void* data);
void* dn_ring_buffer_push_overwrite_zero(dn_ring_buffer_t* buffer);
void* dn_ring_buffer_pop(dn_ring_buffer_t* buffer);
u32   dn_ring_buffer_bytes(dn_ring_buffer_t* buffer);
void  dn_ring_buffer_clear(dn_ring_buffer_t* buffer);
bool  dn_ring_buffer_is_full(dn_ring_buffer_t* buffer);
bool  dn_ring_buffer_is_empty(dn_ring_buffer_t* buffer);
void* dn_ring_buffer_iter_deref(dn_ring_buffer_iterator_t* it);
void  dn_ring_buffer_iter_next(dn_ring_buffer_iterator_t* it);
void  dn_ring_buffer_iter_prev(dn_ring_buffer_iterator_t* it);
bool  dn_ring_buffer_iter_done(dn_ring_buffer_iterator_t* it);
dn_ring_buffer_iterator_t dn_ring_buffer_iter(dn_ring_buffer_t* buffer);
dn_ring_buffer_iterator_t dn_ring_buffer_riter(dn_ring_buffer_t* buffer);

#define dn_ring_buffer_for(rb, it)  for (dn_ring_buffer_iterator_t (it) = dn_ring_buffer_iter((&rb));  !dn_ring_buffer_iter_done(&(it)); !dn_ring_buffer_iter_next(&(it)))
#define dn_ring_buffer_rfor(rb, it) for (dn_ring_buffer_iterator_t (it) = dn_ring_buffer_riter((&rb)); !dn_ring_buffer_iter_done(&(it)); !dn_ring_buffer_iter_prev(&(it)))


//  ██╗      ██████╗  ██████╗ 
//  ██║     ██╔═══██╗██╔════╝ 
//  ██║     ██║   ██║██║  ███╗
//  ██║     ██║   ██║██║   ██║
//  ███████╗╚██████╔╝╚██████╔╝
//  ╚══════╝ ╚═════╝  ╚═════╝ 
typedef enum {
	DN_LOG_FLAG_CONSOLE = 1,
	DN_LOG_FLAG_FILE = 2,
	DN_LOG_FLAG_DEFAULT = 3,
} dn_log_flags_t;

#define DN_LOGGER_MESSAGE_BUFFER_SIZE 4096
#define DN_LOGGER_PREAMBLE_BUFFER_SIZE 512
typedef struct {
	char message_buffer [DN_LOGGER_MESSAGE_BUFFER_SIZE];
	char preamble_buffer [DN_LOGGER_PREAMBLE_BUFFER_SIZE];
} dn_log_t;
dn_log_t dn_logger;

DN_API void dn_log(const char* fmt, ...);
DN_API void dn_log_flags(dn_log_flags_t flags, const char* fmt, ...);
DN_IMP void dn_log_v(dn_log_flags_t flags, const char* fmt, va_list fmt_args);
DN_IMP void dn_log_format_str(dn_string_t fmt, ...);
DN_IMP void dn_log_str(dn_string_t message);
DN_API void dn_log_builder(dn_string_builder_t builder);
DN_IMP void dn_log_flush(dn_log_flags_t flags);
DN_IMP void dn_log_zero();
DN_IMP void dn_log_init();
#define DN_LOG(fmt, ...) dn_log("%s: " fmt, __func__, __VA_ARGS__)


//  ██╗     ██╗   ██╗ █████╗ 
//  ██║     ██║   ██║██╔══██╗
//  ██║     ██║   ██║███████║
//  ██║     ██║   ██║██╔══██║
//  ███████╗╚██████╔╝██║  ██║
//  ╚══════╝ ╚═════╝ ╚═╝  ╚═╝
#ifdef DN_LUA
#ifndef LUA_VERSION
	#error "Please include Lua before including dn.h. If you aren't using Lua, find where DN_LUA is being defined and fix it."
#endif

typedef lua_State* dn_lua_interpreter_t;

typedef struct {
	dn_lua_interpreter_t state;
} dn_lua_t;
dn_lua_t dn_lua;

void 				dn_lua_init();
bool 				dn_lua_script_file(dn_string_t file_path);
const char* dn_lua_format_file_load_error(const char* error);
s32         dn_lua_format_file_load_error_l(dn_lua_interpreter_t lua);
#endif


//  ████████╗███████╗███████╗████████╗██╗███╗   ██╗ ██████╗ 
//  ╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝██║████╗  ██║██╔════╝ 
//     ██║   █████╗  ███████╗   ██║   ██║██╔██╗ ██║██║  ███╗
//     ██║   ██╔══╝  ╚════██║   ██║   ██║██║╚██╗██║██║   ██║
//     ██║   ███████╗███████║   ██║   ██║██║ ╚████║╚██████╔╝
//     ╚═╝   ╚══════╝╚══════╝   ╚═╝   ╚═╝╚═╝  ╚═══╝ ╚═════╝ 
#define DN_TEST_COLOR_BOLD "\033[1m"
#define DN_TEST_COLOR_RESET "\033[0m"
#define DN_TEST_COLOR_SUCCESS "\033[32m"
#define DN_TEST_COLOR_DESC "\033[33m"
#define DN_TEST_COLOR_FAIL "\033[31m"
#define DN_TEST_COLOR_MIXED "\033[35m"

typedef enum {
  DN_TEST_CONTEXT_FLAG_DEFAULT = 0,
  DN_TEST_CONTEXT_FLAG_LOG_FAILURE = 1 << 0,
  DN_TEST_CONTEXT_FLAG_LOG_SUCCESS = 1 << 1,
} dn_test_context_flags_t;

typedef struct {
  dn_string_t description;
  bool success;
} dn_assertion_t;

typedef struct {
  dn_string_t name;
  bool success;
  u32 num_assertions;
  dn_dynamic_array(dn_assertion_t) assertions;
} dn_test_context_t;

typedef struct {
  dn_string_t name;
  dn_test_context_flags_t flags;
  dn_dynamic_array(dn_test_context_t) tests;
} dn_test_suite_t;

typedef struct {
  dn_test_suite_t* suite;
  dn_test_context_t* context;
} dn_tests_t;
dn_tests_t dn_tests;

#define dn_test_assert_ex(desc, condition) \
do { \
  dn_assertion_t* assertion = (dn_assertion_t*)dn_dynamic_array_push(&dn_tests.context->assertions, dn_ptr_lval(dn_assertion_t, dn_zero_initialize())); \
  assertion->description = desc; \
  assertion->success = (condition); \
  dn_tests.context->num_assertions++;  \
} while (false);

#define dn_test_assert(condition)            dn_test_assert_ex(dn_string_literal(dn_macro_str(condition)), (condition))
#define dn_test_assert_desc(desc, condition) dn_test_assert_ex(dn_string_literal(desc),                    (condition))

void dn_test_init();
void dn_test_context_init(dn_test_context_t* context);
void dn_test_suite_init(dn_test_suite_t* suite);
void dn_test_begin_suite(dn_test_suite_t* suite);
void dn_test_begin(dn_string_t name);
void dn_test_end();
void dn_test_end_suite();


//  ███╗   ██╗██╗   ██╗██╗  ██╗██╗     ███████╗ █████╗ ██████╗ 
//  ████╗  ██║██║   ██║██║ ██╔╝██║     ██╔════╝██╔══██╗██╔══██╗
//  ██╔██╗ ██║██║   ██║█████╔╝ ██║     █████╗  ███████║██████╔╝
//  ██║╚██╗██║██║   ██║██╔═██╗ ██║     ██╔══╝  ██╔══██║██╔══██╗
//  ██║ ╚████║╚██████╔╝██║  ██╗███████╗███████╗██║  ██║██║  ██║
//  ╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝
#ifdef DN_NUKLEAR
typedef struct nk_context nk_context;
typedef struct nk_color nk_color;
typedef struct nk_style_item nk_style_item;
typedef struct nk_style_button nk_style_button;

#define NK_RATIO(...) ((float []){ __VA_ARGS__ })

void          nk_dn_string(nk_context* nk, dn_string_t str, nk_flags flags);
void          nk_dn_string_wrap(nk_context* nk, dn_string_t str);
void          nk_dn_string_colored(nk_context* nk, dn_string_t str, nk_flags alignment, dn_color_t color);
void          nk_dn_string_wrap_colored(nk_context* nk, dn_string_t str, dn_color_t color);
void          nk_edit_dn_string(nk_context* nk, nk_flags flags, dn_string_t* buffer, u32 max_len, nk_plugin_filter filter);
bool          nk_selectable_dn_string(nk_context* nk, dn_string_t str, nk_flags flags, nk_bool* value);
bool          nk_button_dn_string(nk_context* nk, dn_string_t str);
bool          nk_button_dn_string_styled(nk_context* nk, nk_style_button* style, dn_string_t str);
nk_color      dn_color_to_nk_color(dn_color_t color);
nk_style_item dn_color_to_nk_style_item(dn_color_t color);
#endif



//  ██╗███╗   ███╗██████╗ ██╗     ███████╗███╗   ███╗███████╗███╗   ██╗████████╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
//  ██║████╗ ████║██╔══██╗██║     ██╔════╝████╗ ████║██╔════╝████╗  ██║╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
//  ██║██╔████╔██║██████╔╝██║     █████╗  ██╔████╔██║█████╗  ██╔██╗ ██║   ██║   ███████║   ██║   ██║██║   ██║██╔██╗ ██║
//  ██║██║╚██╔╝██║██╔═══╝ ██║     ██╔══╝  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
//  ██║██║ ╚═╝ ██║██║     ███████╗███████╗██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   ██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
//  ╚═╝╚═╝     ╚═╝╚═╝     ╚══════╝╚══════╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
#ifdef DN_IMPL
dn_color_t dn_color_rgb_to_hsv(dn_color_t color) {
  DN_BROKEN();
  dn_vector4_t k = (dn_color_t) { (0.0), (-1.0 / 3.0), (2.0 / 3.0), (-1.0) };

  dn_vector4_t pa = (dn_vector4_t) { color.b, color.g, k.w, k.z };
  dn_vector4_t pb = (dn_vector4_t) { color.g, color.b, k.x, k.y };
  dn_vector4_t p = dn_math_lerp4(pa, pb, dn_math_step(color.b, color.g));

  dn_vector4_t qa = (dn_vector4_t) { p.x, p.y, p.w, color.r };
  dn_vector4_t qb = (dn_vector4_t) { color.r, p.y, p.z, p.x };
  dn_vector4_t q = dn_math_lerp4(qa, qb, dn_math_step(p.x, color.r));

  float d = q.x - dn_min(q.w, q.y);
  float e = 1.0e-10;
  return (dn_color_t) {
    fabs(q.z + (q.w - q.y) / (6.0 * d + e)),
    d / (q.x + e),
    q.x,
    color.a
  };
}

dn_color_t dn_color_hsv_to_rgb(dn_color_t color) {
  DN_BROKEN();
  dn_vector4_t k = (dn_color_t) { (1.0), (2.0 / 3.0), (1.0 / 3.0), (3.0) };
  dn_vector4_t p = (dn_color_t) { color.x + k.x, color.x + k.y, color.x + k.z };
  return color;
}

f32 dn_math_step(f32 edge, f32 x) {
    return (x < edge) ? 0.0f : 1.0f;
}

f32 dn_math_lerp(f32 a, f32 b, f32 t) {
    return a * (1.0f - t) + b * t;
}

dn_vector4_t dn_math_lerp4(dn_vector4_t a, dn_vector4_t b, f32 t) {
  return (dn_vector4_t){
    .x = dn_math_lerp(a.x, b.x, t),
    .y = dn_math_lerp(a.y, b.y, t),
    .z = dn_math_lerp(a.z, b.z, t),
    .w = dn_math_lerp(a.w, b.w, t),
  };
}

dn_vector4_t dn_math_abs4(dn_vector4_t v) {
    dn_vector4_t result;
    result.x = fabsf(v.x);
    result.y = fabsf(v.y);
    result.z = fabsf(v.z);
    result.w = fabsf(v.w);
    return result;
}

dn_vector4_t dn_math_fract4(dn_vector4_t v) {
    dn_vector4_t result;
    result.x = v.x - floorf(v.x);
    result.y = v.y - floorf(v.y);
    result.z = v.z - floorf(v.z);
    result.w = v.w - floorf(v.w);
    return result;
}

dn_vector4_t dn_math_clamp4(dn_vector4_t v, float minVal, float maxVal) {
    dn_vector4_t result;
    result.x = fmaxf(minVal, fminf(v.x, maxVal));
    result.y = fmaxf(minVal, fminf(v.y, maxVal));
    result.z = fmaxf(minVal, fminf(v.z, maxVal));
    result.w = fmaxf(minVal, fminf(v.w, maxVal));
    return result;
}

#ifdef DN_NUKLEAR
void nk_dn_string(struct nk_context* nk, dn_string_t str, nk_flags flags) {
  nk_text(nk, (char*)str.data, (s32)str.len, flags);
}

void nk_dn_string_wrap(nk_context* nk, dn_string_t str) {
  nk_text_wrap(nk, (char*)str.data, (s32)str.len);
}

void nk_dn_string_colored(struct nk_context* nk, dn_string_t str, nk_flags alignment, dn_color_t color) {
  nk_text_colored(nk, (char*)str.data, (s32)str.len, alignment, dn_color_to_nk_color(color));
}

void nk_dn_string_wrap_colored(nk_context* nk, dn_string_t str, dn_color_t color) {
  nk_text_wrap_colored(nk, (char*)str.data, (s32)str.len, dn_color_to_nk_color(color));
}

void nk_edit_dn_string(struct nk_context* nk, nk_flags flags, dn_string_t* buffer, u32 max_len, nk_plugin_filter filter) {
  nk_edit_string(nk, flags, (char*)buffer->data, (s32*)&buffer->len, max_len, filter);
}

bool nk_selectable_dn_string(nk_context* nk, dn_string_t str, nk_flags flags, nk_bool* value) {
  return nk_selectable_text(nk, (char*)str.data, (s32)str.len, flags, value);
}

bool nk_button_dn_string(nk_context* nk, dn_string_t str) {
  return nk_button_text(nk, (char*)str.data, (s32)str.len);
}

bool nk_button_dn_string_styled(nk_context* nk, nk_style_button* style, dn_string_t str) {
  return nk_button_text_styled(nk, style, (char*)str.data, (s32)str.len);
}

nk_color dn_color_to_nk_color(dn_color_t color) {
  return nk_rgba(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
}

nk_style_item dn_color_to_nk_style_item(dn_color_t color) {
  return nk_style_item_color(dn_color_to_nk_color(color));
}

#endif


void dn_log_init() {
  dn_os_zero_memory(&dn_logger, sizeof(dn_log_t));
}

void dn_log(const char* fmt, ...) {
	va_list fmt_args;
	va_start(fmt_args, fmt);
	dn_log_v(DN_LOG_FLAG_DEFAULT, fmt, fmt_args);
	va_end(fmt_args);
}

void dn_log_format_str(dn_string_t fmt, ...) {
	va_list fmt_args;
	va_start(fmt_args, fmt);
	dn_log_v(DN_LOG_FLAG_DEFAULT, dn_string_to_cstr(fmt), fmt_args);
	va_end(fmt_args);
}

void dn_log_flags(dn_log_flags_t flags, const char* fmt, ...) {
	va_list fmt_args;
	va_start(fmt_args, fmt);
	dn_log_v(flags, fmt, fmt_args);
	va_end(fmt_args);
}

void dn_log_v(dn_log_flags_t flags, const char* fmt, va_list fmt_args) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  uint64_t ms_since_epoch = (uint64_t)(tv.tv_sec) * 1000 + (tv.tv_usec / 1000);
  time_t sec_since_epoch = (time_t)(ms_since_epoch / 1000);
  struct tm* time_info = localtime(&sec_since_epoch);

	snprintf(dn_logger.preamble_buffer, DN_LOGGER_PREAMBLE_BUFFER_SIZE, "[%04d-%02d-%02d %02d:%02d:%02d.%03" DN_FMT_U64 "]",
			 1900 + time_info->tm_year, 1 + time_info->tm_mon, time_info->tm_mday,
			 time_info->tm_hour, time_info->tm_min, time_info->tm_sec, ms_since_epoch % 1000);
	
	vsnprintf(&dn_logger.message_buffer[0], DN_LOGGER_MESSAGE_BUFFER_SIZE, fmt, fmt_args);
	
	dn_log_flush(flags);
}

void dn_log_str(dn_string_t message) {
	snprintf(dn_logger.message_buffer, DN_LOGGER_MESSAGE_BUFFER_SIZE, "%.*s", message.len, message.data);
	dn_log_flush(DN_LOG_FLAG_CONSOLE);
}

void dn_log_builder(dn_string_builder_t builder) {
	dn_log_str(dn_string_builder_write(&builder));
}

void dn_log_flush(dn_log_flags_t flags) {
	if (flags & DN_LOG_FLAG_CONSOLE) { 
		printf("%s %s\n", dn_logger.preamble_buffer, dn_logger.message_buffer); 
	}

	dn_log_zero();
}

void dn_log_zero() {
	memset(&dn_logger.preamble_buffer[0], 0, DN_LOGGER_PREAMBLE_BUFFER_SIZE);
	memset(&dn_logger.message_buffer[0], 0, DN_LOGGER_MESSAGE_BUFFER_SIZE);
}

void* dn_ring_buffer_at(dn_ring_buffer_t* buffer, u32 index) {
		return buffer->data + ((buffer->head + buffer->element_size * index) % buffer->capacity);
}

void dn_ring_buffer_init(dn_ring_buffer_t* buffer, u32 capacity, u32 element_size) {
	buffer->size = 0;
	buffer->head = 0;
	buffer->capacity = capacity;
	buffer->element_size = element_size;
	buffer->data = dn_allocator_alloc(&dn_allocators.standard.allocator, capacity * element_size);
}

void* dn_ring_buffer_back(dn_ring_buffer_t* buffer) {
	DN_ASSERT(buffer->size);
  return dn_ring_buffer_at(buffer, buffer->size - 1);
}

void* dn_ring_buffer_push(dn_ring_buffer_t* buffer, void* data) {
	DN_ASSERT(buffer->size < buffer->capacity);

	u32 index = (buffer->head + buffer->size) % buffer->capacity;
  dn_os_memory_copy(data, buffer->data + index * buffer->element_size, buffer->element_size);
	buffer->size += 1; 
	return dn_ring_buffer_back(buffer);
}

void* dn_ring_buffer_push_zero(dn_ring_buffer_t* buffer) {
	DN_ASSERT(buffer->size < buffer->capacity);

	u32 index = (buffer->head + buffer->size) % buffer->capacity;
  dn_os_zero_memory(buffer->data + index, buffer->element_size);
	buffer->size += 1;
	return dn_ring_buffer_back(buffer);
}

void* dn_ring_buffer_push_overwrite(dn_ring_buffer_t* buffer, void* data) {
	if (buffer->size == buffer->capacity) dn_ring_buffer_pop(buffer);
	return dn_ring_buffer_push(buffer, data);
}

void* dn_ring_buffer_push_overwrite_zero(dn_ring_buffer_t* buffer) {
	if (buffer->size == buffer->capacity) dn_ring_buffer_pop(buffer);
	return dn_ring_buffer_push_zero(buffer);
}

void* dn_ring_buffer_pop(dn_ring_buffer_t* buffer) {
	DN_ASSERT(buffer->size);

	void* element = buffer->data + (buffer->head * buffer->element_size);
	buffer->head = (buffer->head + 1) % buffer->capacity;
	buffer->size--;
	return element;
}

u32 dn_ring_buffer_bytes(dn_ring_buffer_t* buffer) {
	return buffer->capacity * buffer->element_size;
}

void dn_ring_buffer_clear(dn_ring_buffer_t* buffer) {
	memset(buffer->data, 0, dn_ring_buffer_bytes(buffer));
	buffer->size = 0;
	buffer->head = 0;
}

bool dn_ring_buffer_is_full(dn_ring_buffer_t* buffer) {
	return buffer->capacity == buffer->size;
}

bool dn_ring_buffer_is_empty(dn_ring_buffer_t* buffer) {
	return buffer->size == 0;
}

void* dn_ring_buffer_iter_deref(dn_ring_buffer_iterator_t* it) {
  return dn_ring_buffer_at(it->buffer, it->index);
}

void dn_ring_buffer_iter_next(dn_ring_buffer_iterator_t* it) {
  DN_ASSERT(it->index < it->buffer->size);
  it->index++;
}	
	
void dn_ring_buffer_iter_prev(dn_ring_buffer_iterator_t* it) {
  DN_ASSERT(it->index < it->buffer->size);
  it->index--;
}	
	
bool dn_ring_buffer_iter_done(dn_ring_buffer_iterator_t* it) {
  if (it->reverse) return it->index == -1;
  return it->index == it->buffer->size;
}	

dn_ring_buffer_iterator_t dn_ring_buffer_iter(dn_ring_buffer_t* buffer) {
	dn_ring_buffer_iterator_t iterator;
	iterator.index = 0;
	iterator.reverse = false;
	iterator.buffer = buffer;
	return iterator;
}

dn_ring_buffer_iterator_t dn_ring_buffer_riter(dn_ring_buffer_t* buffer) {
	dn_ring_buffer_iterator_t iterator;
	iterator.index = buffer->size - 1;
	iterator.reverse = true;
	iterator.buffer = buffer;
	return iterator;
}


void dn_pool_init(dn_pool_t* pool, u32 capacity, u32 element_size) {
	pool->capacity = capacity;
	pool->free_list = 0;
  pool->element_size = element_size;
	pool->data = (u8*)dn_allocator_alloc(&dn_allocators.standard.allocator, capacity * element_size);
	pool->slots = (dn_pool_slot_t*)dn_allocator_alloc(&dn_allocators.standard.allocator, capacity * sizeof(dn_pool_slot_t));

  dn_for(i, capacity) {
		pool->slots[i].next_free = i + 1;
  }
	pool->slots[capacity - 1].next_free = -1;
}


dn_pool_handle_t dn_pool_insert(dn_pool_t* pool, void* value) {
	assert(!pool->slots[pool->free_list].occupied);
	
  dn_pool_slot_t* slot = pool->slots + pool->free_list;
	slot->occupied = true;
	slot->generation++;

  dn_os_memory_copy(value, pool->data + pool->element_size * pool->free_list, pool->element_size);

	dn_pool_handle_t handle;
	handle.index = pool->free_list;
	handle.generation = slot->generation;
	
	pool->free_list = slot->next_free;
	
	return handle;
}

dn_pool_handle_t dn_pool_reserve(dn_pool_t* pool) {
	assert(!pool->slots[pool->free_list].occupied);
	
  dn_pool_slot_t* slot = pool->slots + pool->free_list;
	slot->occupied = true;
	slot->generation++;

  dn_os_zero_memory(pool->data + pool->element_size * pool->free_list, pool->element_size);

	dn_pool_handle_t handle;
	handle.index = pool->free_list;
	handle.generation = slot->generation;
	
	pool->free_list = slot->next_free;
	
	return handle;
}

void dn_pool_remove(dn_pool_t* pool, dn_pool_handle_t handle) {
	if (handle.index >= pool->capacity) return;
	
  dn_pool_slot_t* slot = pool->slots + handle.index;
  if (!dn_pool_slot_is_match(slot, handle)) return;
  if (!dn_pool_slot_has_next_free(slot)) return;

	slot->occupied = false;
	slot->generation++;
	slot->next_free = pool->free_list;
	pool->free_list = handle.index;
}

bool dn_pool_contains(dn_pool_t* pool, dn_pool_handle_t handle) {
	if (handle.index >= pool->capacity) return false;

  dn_pool_slot_t* slot = pool->slots + handle.index;
  return dn_pool_slot_is_match(slot, handle);
}

void dn_pool_clear(dn_pool_t* pool) {
  dn_for(i, pool->capacity) {
		pool->slots[i].next_free = i + 1;
		pool->slots[i].occupied = false;
		pool->slots[i].generation++;
	}

	pool->slots[pool->capacity - 1].next_free = -1;

	pool->free_list = 0;
}

dn_pool_handle_t dn_pool_invalid_handle() {
	return (dn_pool_handle_t){
		.index = 0,
		.generation = 0
	};
}

bool dn_pool_is_handle_valid(dn_pool_handle_t handle) {
	return handle.generation > 0;
}

bool dn_pool_slot_has_next_free(dn_pool_slot_t* slot) {
  return slot->next_free >= 0;
}

bool dn_pool_slot_is_match(dn_pool_slot_t* slot, dn_pool_handle_t handle) {
  return slot->generation == handle.generation;
}

dn_hash_t hash_siphash_bytes(const void *p, size_t len, size_t seed) {
  unsigned char *d = (unsigned char *) p;
  size_t i,j;
  size_t v0,v1,v2,v3, data;

  // hash that works on 32- or 64-bit registers without knowing which we have
  // (computes different results on 32-bit and 64-bit platform)
  // derived from siphash, but on 32-bit platforms very different as it uses 4 32-bit state not 4 64-bit
  v0 = ((((size_t) 0x736f6d65 << 16) << 16) + 0x70736575) ^  seed;
  v1 = ((((size_t) 0x646f7261 << 16) << 16) + 0x6e646f6d) ^ ~seed;
  v2 = ((((size_t) 0x6c796765 << 16) << 16) + 0x6e657261) ^  seed;
  v3 = ((((size_t) 0x74656462 << 16) << 16) + 0x79746573) ^ ~seed;

  #define gs_sipround() \
    do {                   \
      v0 += v1; v1 = gs_rotate_left(v1, 13);  v1 ^= v0; v0 = gs_rotate_left(v0,GS_SIZE_T_BITS/2); \
      v2 += v3; v3 = gs_rotate_left(v3, 16);  v3 ^= v2;                                                 \
      v2 += v1; v1 = gs_rotate_left(v1, 17);  v1 ^= v2; v2 = gs_rotate_left(v2,GS_SIZE_T_BITS/2); \
      v0 += v3; v3 = gs_rotate_left(v3, 21);  v3 ^= v0;                                                 \
    } while (0)

  for (i=0; i+sizeof(size_t) <= len; i += sizeof(size_t), d += sizeof(size_t)) {
    data = d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24);
    data |= (size_t) (d[4] | (d[5] << 8) | (d[6] << 16) | (d[7] << 24)) << 16 << 16; // discarded if size_t == 4

    v3 ^= data;
    for (j=0; j < GS_SIPHASH_C_ROUNDS; ++j)
      gs_sipround();
    v0 ^= data;
  }
  data = len << (GS_SIZE_T_BITS-8);
  switch (len - i) {
    case 7: data |= ((size_t) d[6] << 24) << 24; // fall through
    case 6: data |= ((size_t) d[5] << 20) << 20; // fall through
    case 5: data |= ((size_t) d[4] << 16) << 16; // fall through
    case 4: data |= (d[3] << 24); // fall through
    case 3: data |= (d[2] << 16); // fall through
    case 2: data |= (d[1] << 8); // fall through
    case 1: data |= d[0]; // fall through
    case 0: break;
  }
  v3 ^= data;
  for (j=0; j < GS_SIPHASH_C_ROUNDS; ++j)
    gs_sipround();
  v0 ^= data;
  v2 ^= 0xff;
  for (j=0; j < GS_SIPHASH_D_ROUNDS; ++j)
    gs_sipround();

#if 0
  return v0^v1^v2^v3;
#else
  return v1^v2^v3; // slightly stronger since v0^v3 in above cancels out final round operation? I tweeted at the authors of SipHash about this but they didn't reply
#endif
}

#define DN_HASH_SEED 0x9de341c9
dn_hash_t dn_hash_bytes(const void *p, size_t len) {
  return hash_siphash_bytes(p, len, DN_HASH_SEED);
}

dn_hash_t dn_hash_bytes_ex(const void *p, size_t len, size_t seed) {
  return hash_siphash_bytes(p,len,seed);
}

dn_hash_t dn_combine_hashes(dn_hash_t a, dn_hash_t b) {
  return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
}

dn_hash_t dn_hash_cstr_dumb(const char* str) {
  return dn_hash_str_dumb(dn_string_cstr(str));
}

dn_hash_t dn_hash_str_dumb(dn_string_t str) {
  const size_t prime = 31;
  
  size_t result = 0;
  for (int i = 0; i < str.len; i++) {
    result = str.data[i] + (result * prime);
  }
    return result;
}

dn_hash_t dn_hash_string(dn_string_t str) {
  return dn_hash_bytes(str.data, str.len);
}

void dn_hash_encode_hex(char* destination, const char* data, size_t len) {
  static char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

  for (int i = 0; i < len; i++) {
    char const byte = data[i];

    int base_index = i * 2;
    destination[base_index] = hex_chars[(byte & 0xF0) >> 4];
    destination[base_index + 1]     = hex_chars[(byte & 0x0F) >> 0];
  }
}

void dn_hash_encode_base64(char* destination, const char* source, size_t len) {
  static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  u32 val = 0;
  int valb = -6;

  int i = 0;
  for (; i < len; ++i) {
    val = (val << 8) + source[i];
    valb += 8;
    while (valb >= 0) {
      destination[i] = base64_chars[(val >> valb) & 0x3F];
      valb -= 6;
    }
  }
  if (valb > -6) {
    destination[i] = base64_chars[((val << 8) >> (valb + 8)) & 0x3F];
  }

  while (i % 4) destination[i++] = '=';
}

void dn_dynamic_array_init(dn_dynamic_array_t* buffer, u32 element_size, dn_allocator_t* allocator) {
  DN_ASSERT(buffer);

  buffer->size = 0;
  buffer->capacity = 2;
  buffer->element_size = element_size;
  buffer->data = (u8*)dn_allocator_alloc(allocator, buffer->capacity * element_size);
  buffer->allocator = allocator;
}

u8* dn_dynamic_array_at(dn_dynamic_array_t* buffer, u32 index) {
  DN_ASSERT(buffer);
  return buffer->data + (index * buffer->element_size);
}

u8*  dn_dynamic_array_push(dn_dynamic_array_t* dynamic_array, void* data) {
  return dn_dynamic_array_push_n(dynamic_array, data, 1);
}

u8* dn_dynamic_array_push_n(dn_dynamic_array_t* buffer, void* data, u32 count) {
  DN_ASSERT(buffer);

  void* reserved = dn_dynamic_array_reserve(buffer, count);
  if (data) dn_os_memory_copy(data, reserved, buffer->element_size * count);
  return reserved;
}

u8* dn_dynamic_array_reserve(dn_dynamic_array_t* buffer, u32 count) {
  DN_ASSERT(buffer);

  dn_dynamic_array_grow(buffer, buffer->size + count);

  void* element = dn_dynamic_array_at(buffer, buffer->size);
  buffer->size += count;
  return element;
}

void dn_dynamic_array_clear(dn_dynamic_array_t* buffer) {
  DN_ASSERT(buffer);

  buffer->size = 0;
}

u32 dn_dynamic_array_byte_size(dn_dynamic_array_t* buffer) {
  DN_ASSERT(buffer);

  return buffer->size * buffer->element_size;
}

void dn_dynamic_array_grow(dn_dynamic_array_t* buffer, u32 capacity) {
  DN_ASSERT(buffer);

  if (buffer->capacity >= capacity) return;
  buffer->capacity = dn_max(buffer->capacity * 2, capacity);
  buffer->data = (u8*)dn_allocator_realloc(buffer->allocator, buffer->data, buffer->capacity * buffer->element_size);
}

char* dn_cstr_copy_n(const char* str, u32 length, dn_allocator_t* allocator) {
  if (!allocator) allocator = &dn_allocators.standard.allocator;

  u32 buffer_length = length + 1;
  char* copy = (char*)dn_allocator_alloc(allocator, buffer_length);
  dn_cstr_copy_to_n(str, length, copy, buffer_length);
  return copy;
}

char* dn_cstr_copy(const char* str, dn_allocator_t* allocator) {
  return dn_cstr_copy_n(str, dn_cstr_len(str), allocator);
}

char* dn_cstr_copy_u8(const u8* str, u32 length, dn_allocator_t* allocator) {
  return dn_cstr_copy_n((const char*)str, length, allocator);
}

void dn_cstr_copy_to(const char* str, char* buffer, u32 buffer_length) {
  dn_cstr_copy_to_n(str, dn_cstr_len(str), buffer, buffer_length);
}

void dn_cstr_copy_to_n(const char* str, u32 length, char* buffer, u32 buffer_length) {
  if (!str) return;
  if (!buffer) return;
  if (!buffer_length) return;

  u32 copy_length = dn_min(length, buffer_length - 1);
  for (u32 i = 0; i < copy_length; i++) {
    buffer[i] = str[i];
  }
  buffer[copy_length] = '\0';
}

char* dn_string_to_cstr(dn_string_t str) {
  return dn_string_to_cstr_ex(str, &dn_allocators.bump.allocator);
}

char* dn_string_to_cstr_ex(dn_string_t str, dn_allocator_t* allocator) {
  return dn_cstr_copy_n((char*)str.data, str.len, allocator);
}

dn_string_t dn_string_alloc(u32 capacity, dn_allocator_t* allocator) {
  return (dn_string_t){
    .len = 0,
    .data = (u8*)dn_allocator_alloc(allocator, capacity),
  };
}

bool dn_string_equal(dn_string_t a, dn_string_t b) {
  if (a.len != b.len) return false;

  return dn_os_is_memory_equal(a.data, b.data, a.len);
}

bool dn_string_equal_cstr(dn_string_t a, const char* b) {
  u32 len = dn_cstr_len(b);
  if (a.len != len) return false;

  return dn_os_is_memory_equal(a.data, b, len);
}

bool dn_cstr_equal(const char* a, const char* b) {
  u32 len_a = dn_cstr_len(a);
  u32 len_b = dn_cstr_len(b);
  if (len_a != len_b) return false;

  return dn_os_is_memory_equal(a, b, len_a);
}

u32 dn_cstr_len(const char* str) {
  u32 len = 0;
  if (str) {
    while (str[len]) len++;
  }

  return len;
}

dn_string_t dn_string_copy_cstr_n(const char* str, u32 length, dn_allocator_t* allocator) {
  dn_string_t copy;
  copy.len = dn_cstr_len(str);
  copy.data = (u8*)dn_allocator_alloc(allocator, copy.len);

  dn_os_memory_copy(str, copy.data, copy.len);
  return copy;  
}

dn_string_t dn_string_copy_cstr(const char* str, dn_allocator_t* allocator) {
  dn_string_t copy;
  copy.len = dn_cstr_len(str);
  copy.data = (u8*)dn_allocator_alloc(allocator, copy.len);

  dn_os_memory_copy(str, copy.data, copy.len);
  return copy;
}

dn_string_t dn_string_copy(dn_string_t str, dn_allocator_t* allocator) {
  dn_string_t copy = {
    .len = str.len,
    .data = (u8*)dn_allocator_alloc(allocator, str.len),
  };

  dn_os_memory_copy(str.data, copy.data, str.len);
  return copy;
}

void dn_string_copy_to_str(dn_string_t source, dn_string_t* dest, u32 capacity) {
  dest->len = dn_min(source.len, capacity);
  dn_os_memory_copy(source.data, dest->data, dest->len);
}

void dn_string_copy_to(dn_string_t str, u8* buffer, u32 capacity) {
  dn_os_memory_copy(str.data, buffer, dn_min(str.len, capacity));
}

void dn_string_builder_grow(dn_string_builder_t* builder, u32 requested_capacity) {
  if (builder->buffer.capacity > requested_capacity) return;

  builder->buffer.data = (u8*)dn_allocator_realloc(builder->allocator, builder->buffer.data, requested_capacity);
  builder->buffer.capacity = requested_capacity;
}

void dn_string_builder_append(dn_string_builder_t* builder, dn_string_t str) {
  dn_string_builder_grow(builder, builder->buffer.count + str.len);
  dn_os_memory_copy(str.data, builder->buffer.data + builder->buffer.count, str.len);
  builder->buffer.count += str.len;
}

void dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str) {
  u32 len = dn_cstr_len(str);
  dn_string_builder_grow(builder, builder->buffer.count + len);
  dn_os_memory_copy(str, builder->buffer.data + builder->buffer.count, len);
  builder->buffer.count += len;
}

void dn_string_builder_append_fmt(dn_string_builder_t* builder, dn_string_t fmt, ...) {
  va_list args, len_args;
  va_start(args, fmt);
  va_copy(len_args, args);

  const char* format = dn_string_to_cstr(fmt);
  u32 fmt_len = vsnprintf(NULL, 0, format, len_args);
  u32 vsnprintf_len = fmt_len + 1;
  dn_string_builder_grow(builder, builder->buffer.count + vsnprintf_len); // vsnprintf() null terminates, so ensure there's enough room (even though we don't care)
  vsnprintf((char*)builder->buffer.data + builder->buffer.count, vsnprintf_len, format, args);
  builder->buffer.count += fmt_len;

  va_end(args);
  va_end(args);
}

dn_string_t dn_string_builder_write(dn_string_builder_t* builder) {
  dn_string_t string = {
    .data = (u8*)dn_allocator_alloc(builder->allocator, builder->buffer.count),
    .len = builder->buffer.count
  };

  dn_os_memory_copy(builder->buffer.data, string.data, builder->buffer.count);
  return string;
}

char* dn_string_builder_write_cstr(dn_string_builder_t* builder) {
  return dn_cstr_copy_n((char*)builder->buffer.data, builder->buffer.count, builder->allocator);
}


bool dn_os_does_path_exist(dn_string_t path) {
  DN_BROKEN();
  return false;
}

bool dn_os_is_regular_file(dn_string_t path) {
  DN_BROKEN();
  return true;
}

bool dn_os_is_directory(dn_string_t path) {
  DN_BROKEN();
  return false;
}

void dn_os_remove_directory(dn_string_t path) {
  DN_BROKEN();
}

void dn_os_create_directory(dn_string_t path) {
  DN_BROKEN();
}

void dn_os_create_file(dn_string_t path) {
  DN_BROKEN();  
}

void dn_os_remove_file(dn_string_t path) {
  DN_BROKEN();
}

dn_os_directory_entry_list_t dn_os_scan_directory(dn_string_t path) {
  DN_BROKEN();
  dn_zero_return(dn_os_directory_entry_list_t);
}

dn_os_directory_entry_list_t dn_os_scan_directory_recursive(dn_string_t path) {
  DN_BROKEN();
  dn_zero_return(dn_os_directory_entry_list_t);
}

dn_os_date_time_t dn_os_get_date_time() {
  DN_BROKEN();
  dn_zero_return(dn_os_date_time_t);
}

f64 dn_os_file_mod_time(dn_string_t file_path) {
  DN_BROKEN();
  return 0;
}

dn_os_file_attr_t dn_os_winapi_attr_to_dn_attr(u32 attr) {
  DN_BROKEN();
  return DN_OS_FILE_ATTR_REGULAR_FILE;
}

bool dn_os_is_memory_equal(const void* a, const void* b, size_t len) {
    return !memcmp(a, b, len);
}

void dn_os_memory_copy(const void* source, void* dest, u32 num_bytes) {
    memcpy(dest, source, num_bytes);
}

void dn_os_fill_memory(void* buffer, u32 buffer_size, void* fill, u32 fill_size) {
  u8* current_byte = (u8*)buffer;

  int i = 0;
  while (true) {
    if (i + fill_size > buffer_size) return;
    memcpy(current_byte + i, (u8*)fill, fill_size);
    i += fill_size;
  }
}

void dn_os_fill_memory_u8(void* buffer, u32 buffer_size, u8 fill) {
  dn_os_fill_memory(buffer, buffer_size, &fill, sizeof(u8));
}

void dn_os_zero_memory(void* buffer, u32 buffer_size) {
  dn_os_fill_memory_u8(buffer, buffer_size, 0);
}


/////////////////
// FIXED ARRAY //
/////////////////
void dn_fixed_array_init(dn_fixed_array_t* buffer, u32 max_vertices, u32 element_size, dn_allocator_t* allocator) {
  DN_ASSERT(buffer);

  buffer->size = 0;
  buffer->capacity = max_vertices;
  buffer->element_size = element_size;
  buffer->data = (u8*)dn_allocator_alloc(allocator, max_vertices * element_size);
  buffer->allocator = allocator;
}

u8* dn_fixed_array_at(dn_fixed_array_t* buffer, u32 index) {
  DN_ASSERT(buffer);
  return buffer->data + (index * buffer->element_size);
}

u8* dn_fixed_array_push(dn_fixed_array_t* buffer, void* data, u32 count) {
  DN_ASSERT(buffer);
  DN_ASSERT(buffer->size < buffer->capacity);

  void* reserved = dn_fixed_array_reserve(buffer, count);
  if (data) dn_os_memory_copy(data, reserved, buffer->element_size * count);
  return reserved;
}

u8* dn_fixed_array_reserve(dn_fixed_array_t* buffer, u32 count) {
  DN_ASSERT(buffer);
  
  void* element = dn_fixed_array_at(buffer, buffer->size);
  buffer->size += count;
  return element;
}

void dn_fixed_array_clear(dn_fixed_array_t* buffer) {
  DN_ASSERT(buffer);

  buffer->size = 0;
}

u32 dn_fixed_array_byte_size(dn_fixed_array_t* buffer) {
  DN_ASSERT(buffer);

  return buffer->size * buffer->element_size;
}

////////////////////
// BUMP ALLOCATOR //
////////////////////
void dn_bump_allocator_init(dn_bump_allocator_t* allocator, u32 capacity) {
	allocator->buffer = (u8*)dn_allocator_alloc(&dn_allocators.standard.allocator, capacity);
	allocator->capacity = capacity;
	allocator->allocator.on_alloc = &dn_bump_allocator_on_alloc;
}

void dn_bump_allocator_clear(dn_bump_allocator_t* allocator) {
	memset(allocator->buffer, 0, allocator->bytes_used);
	allocator->bytes_used = 0;
}

void* dn_bump_allocator_on_alloc(dn_allocator_t* allocator, dn_allocator_mode_t mode, u32 size, void* old_memory) {
	dn_bump_allocator_t* bump = (dn_bump_allocator_t*)allocator;
  switch (mode) {
    case DN_ALLOCATOR_MODE_ALLOC: {
      if (bump->bytes_used + size > bump->capacity) {
        DN_LOG("overflow: bytes used = %d, requested size = %d", bump->bytes_used, size);
        DN_ASSERT(bump->bytes_used + size <= bump->capacity);
      }

      u8* memory_block = bump->buffer + bump->bytes_used;
      gs_hash_table_insert(bump->allocations, bump->bytes_used, size);
      bump->bytes_used += size;
    
      return memory_block;
    }
    case DN_ALLOCATOR_MODE_FREE: {
 		  return NULL;
    }
    case DN_ALLOCATOR_MODE_RESIZE: {
      if (!old_memory) {
        return allocator->on_alloc(allocator, DN_ALLOCATOR_MODE_ALLOC, size, NULL);
      }

      u32 offset = (u32)((u8*)old_memory - (u8*)bump->buffer);
      u32 old_size = gs_hash_table_get(bump->allocations, offset);
      if (old_size >= size) {
        return old_memory;
      } 

      void* memory_block = allocator->on_alloc(allocator, DN_ALLOCATOR_MODE_ALLOC, size, NULL);
      dn_os_memory_copy(old_memory, memory_block, old_size);
      return memory_block;

    }
  }
}

////////////////////////
// STANDARD ALLOCATOR //
////////////////////////
void dn_standard_allocator_init(dn_standard_allocator_t* allocator) {
	allocator->allocator.on_alloc = &dn_standard_allocator_on_alloc;
}

void* dn_standard_allocator_on_alloc(dn_allocator_t* allocator, dn_allocator_mode_t mode, u32 size, void* old_memory) {
	if (mode == DN_ALLOCATOR_MODE_ALLOC) {
		return calloc(size, 1);
	}
	else if (mode == DN_ALLOCATOR_MODE_FREE) {
		free(old_memory);
		return NULL;
	}
	else if (mode == DN_ALLOCATOR_MODE_RESIZE) {
		return realloc(old_memory, size);
	}

	DN_UNREACHABLE();
	return NULL;
}

///////////////
// ALLOCATOR //
///////////////
void dn_allocator_add(const char* name, dn_allocator_t* allocator) {
  gs_hash_table_insert(dn_allocators.allocators, dn_hash_cstr_dumb(name), allocator);
}

dn_allocator_t* dn_allocator_find(const char* name) {
  return gs_hash_table_get(dn_allocators.allocators, dn_hash_cstr_dumb(name));
}

void* dn_allocator_alloc(dn_allocator_t* allocator, u32 size) {
	DN_ASSERT(allocator);
	return allocator->on_alloc(allocator, DN_ALLOCATOR_MODE_ALLOC, size, NULL);
}

void* dn_allocator_realloc(dn_allocator_t* allocator, void* memory, u32 size) {
	DN_ASSERT(allocator);
	return allocator->on_alloc(allocator, DN_ALLOCATOR_MODE_RESIZE, size, memory);
}

void dn_allocator_free(dn_allocator_t* allocator, void* buffer) {
	DN_ASSERT(allocator);
	DN_ASSERT(buffer);
	allocator->on_alloc(allocator, DN_ALLOCATOR_MODE_FREE, 0, buffer);
}

void dn_allocators_init() {
	dn_standard_allocator_init(&dn_allocators.standard);
	dn_bump_allocator_init(&dn_allocators.bump, 50 * 1024 * 1024);

	dn_allocator_add("bump", &dn_allocators.bump.allocator);
	dn_allocator_add("standard", &dn_allocators.standard.allocator);
}

void dn_allocators_update() {
	dn_bump_allocator_clear(&dn_allocators.bump);
}

/////////
// LUA //
/////////
#ifdef DN_LUA
void dn_lua_init() {
  dn_lua.state = luaL_newstate();

  luaL_openlibs(dn_lua.state);
}

void dn_lua_pcall(dn_string_t fn) {
	lua_pushstring(dn_lua.state, dn_string_to_cstr(fn));
  lua_gettable(dn_lua.state, -2);
  bool result = lua_pcall(dn_lua.state, 0, 0, 0);
  if (result) {
		dn_string_builder_t builder = dn_tstring_builder();
		dn_string_builder_append_fmt(&builder, dn_string_literal("%.*s() failed; error = %s"),
			fn.len, fn.data,
			lua_tostring(dn_lua.state, -1)
		);
    dn_log_builder(builder);
    exit(0);
  }

}

bool dn_lua_script_file(dn_string_t file_path) {
  dn_lua_interpreter_t l = dn_lua.state;
  s32 initial_stack_size = lua_gettop(l);

	dn_log_format_str(dn_string_literal("%s: scripting %.*s"), __func__, file_path.len, file_path.data);

  lua_pushcfunction(l, &dn_lua_format_file_load_error_l);
  
	const char* file_path_cstr = dn_string_to_cstr(file_path);
  bool result = luaL_loadfile(l, file_path_cstr);

  // In all error cases, do not return early.
  if (result) {
    // There's a syntax error in the file. Since loadfile doesn't call the
    // function we put on the stack, format the message manually.
    const char* unformatted_error = lua_tostring(l, -1);
		dn_log_format_str(dn_string_literal("%s: error loading %.*s\n%s"), 
			__func__, 
			file_path.len, file_path.data, 
			dn_lua_format_file_load_error(unformatted_error)
		);

    lua_pop(l, 2);
    goto check_stack;
  }
  else {
    // The chunk compiled OK. Run it.
    result = lua_pcall(l, 0, 0, -2);
    
    if (result) {
      // There was a runtime error running the chunk.
			const char* unformatted_error = lua_tostring(l, -1);
			dn_log_format_str(dn_string_literal("%s: error running %.*s\n%s"), 
				__func__, 
				file_path.len, file_path.data, 
				dn_lua_format_file_load_error(unformatted_error)
			);

      lua_pop(l, 2);
      goto check_stack;
    }

    // The chunk loaded successfully!
    lua_pop(l, 1);
    goto check_stack;
  }

 check_stack:
  s32 final_stack_size = lua_gettop(l);
  assert(initial_stack_size == final_stack_size);
  return !result;
}

const char* dn_lua_format_file_load_error(const char* error) {
  static char buffer [2048];
  const char* fmt = "  %s";
  snprintf(&buffer[0], 2048, fmt, error);
  
  return &buffer[0];
}

s32 dn_lua_format_file_load_error_l(dn_lua_interpreter_t l) {
  const char* error = lua_tostring(dn_lua.state, 1);
  error = dn_lua_format_file_load_error(error);
  
  lua_pop(dn_lua.state, 1);
  lua_pushstring(dn_lua.state, error);
  return 1;
}
#endif


//////////
// TEST //
//////////
void dn_test_context_init(dn_test_context_t* context) {
  dn_dynamic_array_init(&context->assertions, sizeof(dn_assertion_t), &dn_allocators.bump.allocator);
}

void dn_test_suite_init(dn_test_suite_t* suite) {
  dn_dynamic_array_init(&suite->tests, sizeof(dn_test_suite_t), &dn_allocators.bump.allocator);
}

void dn_test_begin_suite(dn_test_suite_t* suite) {
  dn_test_suite_init(suite);
  dn_tests.suite = suite;
}

void dn_test_end_suite() {
  dn_test_suite_t* suite = dn_tests.suite;
  dn_log("%s", dn_string_to_cstr(suite->name));

  dn_for(i, dn_tests.suite->tests.size) {
    dn_test_context_t* context = (dn_test_context_t*)dn_dynamic_array_at(&dn_tests.suite->tests, i);

    bool found_success = false;
    bool found_failure = false;
    dn_for(i, context->assertions.size) {
      dn_assertion_t* assertion = (dn_assertion_t*)dn_dynamic_array_at(&context->assertions, i);
      found_success |= assertion->success;
      found_failure |= !assertion->success;
    }

    const char* test_color = DN_TEST_COLOR_SUCCESS;
    if (found_failure && !found_success) test_color = DN_TEST_COLOR_FAIL;
    if (!found_failure && found_success) test_color = DN_TEST_COLOR_SUCCESS;
    if (found_failure && found_success) test_color = DN_TEST_COLOR_MIXED;

    dn_log("  %s%s%s%s", DN_TEST_COLOR_BOLD, test_color, dn_string_to_cstr(context->name), DN_TEST_COLOR_RESET);

    dn_for(i, context->assertions.size) {
      dn_assertion_t* assertion = (dn_assertion_t*)dn_dynamic_array_at(&context->assertions, i);

      bool log_assertion = 
        ((suite->flags & DN_TEST_CONTEXT_FLAG_LOG_SUCCESS) && assertion->success) ||
        ((suite->flags & DN_TEST_CONTEXT_FLAG_LOG_FAILURE) && !assertion->success) ||
        (suite->flags == DN_TEST_CONTEXT_FLAG_DEFAULT);
      const char* assertion_color = assertion->success ? DN_TEST_COLOR_SUCCESS : DN_TEST_COLOR_FAIL;  
      if (log_assertion) {
        dn_log("  %s-> %s%s", assertion_color, dn_string_to_cstr(assertion->description),  DN_TEST_COLOR_RESET);
      }
    }
  }
}


void dn_test_begin(dn_string_t name) {
  dn_test_context_t* context = (dn_test_context_t*)dn_dynamic_array_reserve(&dn_tests.suite->tests, 1);
  context->name = name;
  dn_test_context_init(context);
  dn_tests.context = context;
}

void dn_test_end() {
}

void dn_gen_arena_test() {
  dn_test_suite_t suite = {
    .name = dn_string_literal("dn_gen_arena_t"),
    .flags = DN_TEST_CONTEXT_FLAG_DEFAULT
  };
  dn_test_begin_suite(&suite);

  dn_test_begin(dn_string_literal("dn_gen_arena"));
    dn_pool(u32) arena = dn_zero_initialize();
    dn_pool_init(&arena, 32, sizeof(u32));
    dn_pool_handle_t rza = dn_pool_insert(&arena, dn_ptr_lval(u32, 69));
    dn_pool_handle_t gza = dn_pool_insert(&arena, dn_ptr_lval(u32, 420));
    dn_pool_handle_t bill = dn_pool_insert(&arena, dn_ptr_lval(u32, 7));
    
    dn_test_assert(*dn_pool_at(u32, &arena, rza) == 69);
    dn_test_assert(*dn_pool_at(u32, &arena, gza) == 420);
    dn_test_assert(*dn_pool_at(u32, &arena, bill) == 7);

    dn_pool_remove(&arena, bill);

    dn_pool_handle_t murray = dn_pool_insert(&arena, dn_ptr_lval(u32, 9001));
    dn_test_assert(!dn_pool_contains(&arena, bill));
    dn_test_assert(dn_pool_contains(&arena, murray));
    dn_test_assert(*dn_pool_at(u32, &arena, murray) == 9001);
  dn_test_end();

  dn_test_end_suite();
}

void dn_string_test() {
  typedef struct {
    dn_string_t jerry;
    dn_string_t garcia;
    const char* jerry_cstr;
  } dn_strings_t;

  dn_strings_t data = {
    .jerry  = dn_string_literal("jerry garcia"),
    .garcia = dn_string_literal("jerry garcia"),
    .jerry_cstr = "jerry garcia",
  };

  dn_test_suite_t suite = {
    .name = dn_string_literal("dn_string_t"),
  };
  dn_test_begin_suite(&suite);

  dn_test_begin(dn_string_literal("dn_string_literal"));
  dn_test_assert(data.jerry.len == strlen(data.jerry_cstr));
  dn_test_end();

  // u32                dn_cstr_len(const char* str);
  dn_test_begin(dn_string_literal("dn_cstr_len"));
  dn_test_assert(dn_cstr_len(data.jerry_cstr) == 12);
  dn_test_assert(dn_cstr_len("") == 0);
  dn_test_end();

  // bool               dn_cstr_equal(const char* a, const char* b);
  dn_test_begin(dn_string_literal("dn_cstr_equal"));
  dn_test_assert(dn_cstr_equal(data.jerry_cstr, data.jerry_cstr));
  dn_test_end();

  // DN_API bool        dn_string_equal(dn_string_t a, dn_string_t b);
  dn_test_begin(dn_string_literal("dn_string_equal"));
  dn_test_assert(dn_string_equal(data.jerry, data.garcia));
  dn_test_end();

  // DN_API bool        dn_string_equal_cstr(dn_string_t a, const char* b);
  dn_test_begin(dn_string_literal("dn_string_equal_cstr"));
  dn_test_assert(dn_string_equal_cstr(data.jerry, data.jerry_cstr));
  dn_test_end();

  // DN_API dn_string_t dn_string_copy(dn_string_t str, dn_allocator_t* allocator);
  dn_test_begin(dn_string_literal("dn_string_copy"));
  dn_string_t jerry_from_dn = dn_string_copy(data.jerry,  &dn_allocators.bump.allocator);
  dn_test_assert(dn_string_equal(data.jerry, jerry_from_dn));
  dn_test_end();

  // DN_API dn_string_t dn_string_copy_cstr(const char* str, dn_allocator_t* allocator);
  dn_test_begin(dn_string_literal("dn_string_copy_cstr"));
  dn_string_t jerry_from_cstr = dn_string_copy_cstr(data.jerry_cstr,  &dn_allocators.bump.allocator);
  dn_test_assert(dn_string_equal(data.jerry, jerry_from_cstr));
  dn_test_end();

  // DN_API char*       dn_string_to_cstr_ex(dn_string_t str, dn_allocator_t* allocator);
  dn_test_begin(dn_string_literal("dn_string_to_cstr_ex"));
  char* jerry_cstr = dn_string_to_cstr_ex(data.jerry, &dn_allocators.bump.allocator);
  dn_test_assert(dn_cstr_len(jerry_cstr) == dn_cstr_len(data.jerry_cstr));
  dn_test_assert(dn_cstr_equal(jerry_cstr, data.jerry_cstr));
  dn_test_end();

  dn_test_end_suite();

  // DN_IMP char*       dn_cstr_copy(const char* str, u32 length, dn_allocator_t* allocator = nullptr);
  // DN_IMP char*       dn_cstr_copy(const char* str, dn_allocator_t* allocator = nullptr);
  // DN_IMP char*       dn_cstr_copy(const std::string& str, dn_allocator_t* allocator = nullptr);
  // DN_IMP char*       dn_cstr_copy_u8(const u8* str, u32 length, dn_allocator_t* allocator = nullptr);
  // DN_API void        dn_cstr_copy(const char* str, char* buffer, u32 buffer_length);
  // DN_API void        dn_cstr_copy_n(const char* str, u32 length, char* buffer, u32 buffer_length);
}

void dn_string_builder_test() {
  dn_test_suite_t suite = {
    .name = dn_string_literal("dn_string_t"),
  };
  dn_test_begin_suite(&suite);

  // DN_API void        dn_string_builder_grow(dn_string_builder_t* builder);
  dn_test_begin(dn_string_literal("dn_string_builder_grow")); 
  {
    dn_string_builder_t builder = { .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump.allocator };
    dn_string_builder_grow(&builder, 32);
    dn_test_assert(builder.buffer.capacity >= 32);
    dn_string_builder_grow(&builder, 8);
    dn_test_assert(builder.buffer.capacity >= 32);
    dn_string_builder_grow(&builder, 64);
    dn_test_assert(builder.buffer.capacity >= 64);
  }
  dn_test_end();

  // DN_API void        dn_string_builder_append(dn_string_builder_t* builder, dn_string_t str);
  // DN_API void        dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str);
  // DN_API dn_string_t dn_string_builder_write(dn_string_builder_t* builder);
  dn_test_begin(dn_string_literal("dn_string_builder_append"));
  {
    dn_string_builder_t builder = { .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump.allocator };
    dn_string_builder_append(&builder, dn_string_literal("jerry"));
    dn_string_builder_append_cstr(&builder, "/");
    dn_string_builder_append(&builder, dn_string_literal("garcia"));
    dn_test_assert(dn_string_equal(dn_string_builder_write(&builder), dn_string_literal("jerry/garcia")));
  }
  dn_test_end();

  // DN_API char*       dn_string_builder_write_cstr(dn_string_builder_t* builder);
  dn_test_begin(dn_string_literal("dn_string_builder_write_cstr"));
  {
    dn_string_builder_t builder = { .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump.allocator };
    dn_string_builder_append(&builder, dn_string_literal("jerry"));
    dn_test_assert(dn_cstr_equal(dn_string_builder_write_cstr(&builder), "jerry"));
  }
  dn_test_end();

  // DN_API void        dn_string_builder_append_fmt(dn_string_builder_t* builder, dn_string_t fmt, ...);
  dn_test_begin(dn_string_literal("dn_string_builder_write_fmt"));
  {
    dn_string_builder_t builder = { .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump.allocator };
    dn_string_builder_append_fmt(&builder, dn_string_literal("%d:%.2f:%s"), 69, 420.69, "blazeit");
    dn_test_assert(dn_string_equal(dn_string_builder_write(&builder), dn_string_literal("69:420.69:blazeit")));
  }
  dn_test_end();

  dn_test_end_suite();
}

void dn_color_test() {
  dn_test_suite_t suite = {
    .name = dn_string_literal("dn_color_t"),
  };
  dn_test_begin_suite(&suite);

  dn_test_begin(dn_string_literal("dn_color_rgb_to_hsv")); {
    dn_color_t rgb = dn_rgb_255(244, 211, 94);
    dn_color_t hsv = dn_color_rgb_to_hsv(rgb);
    dn_color_t hsv_expected = dn_rgb_255(47, 61, 96);
    dn_test_assert(dn_os_is_memory_equal(&hsv, &hsv_expected, sizeof(dn_color_t)));
  }
  dn_test_end();

  dn_test_end_suite();
}
void dn_test_run_internal() {
  dn_string_test();
  dn_string_builder_test();
  dn_gen_arena_test();
  // dn_color_test();
}

void dn_init() {
  dn_allocators_init();
  dn_log_init();

	#ifdef DN_LUA
	dn_lua_init();
	#endif

  #ifdef DN_RUN_INTERNAL_TESTS
  dn_test_run_internal();
  #endif
}

void dn_update() {
  dn_allocators_update();
}

#endif