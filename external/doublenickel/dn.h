#include "stdint.h"

//  ████████╗██╗   ██╗██████╗ ███████╗███████╗
//  ╚══██╔══╝╚██╗ ██╔╝██╔══██╗██╔════╝██╔════╝
//     ██║    ╚████╔╝ ██████╔╝█████╗  ███████╗
//     ██║     ╚██╔╝  ██╔═══╝ ██╔══╝  ╚════██║
//     ██║      ██║   ██║     ███████╗███████║
//     ╚═╝      ╚═╝   ╚═╝     ╚══════╝╚══════╝                              
#ifndef DN_NO_HIJACK_NUMERIC_TYPES
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;
#endif

#ifdef DN_MATH_BACKEND_HANDMADE
#include "HandmadeMath.h"
typedef HMM_Vec2 dn_vector2_t;
typedef HMM_Vec3 dn_vector3_t;
typedef HMM_Vec4 dn_vector4_t;
#else 
typedef struct {
  float x;
  float y;
} dn_vector2_t;

typedef struct {
  float x;
  float y;
  float z;
} dn_vector3_t;

typedef struct {
  float x;
  float y;
  float z;
  float w;
} dn_vector4_t;
#endif

typedef struct {
	i32 x;
	i32 y;
} dn_vector2i_t;


typedef size_t dn_hash_t;

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
#define DN_UNTESTED() DN_ASSERT(false)
#define DN_UNREACHABLE() DN_ASSERT(false)
#define DN_BROKEN() DN_ASSERT(false)

#define dn_zero_initialize() { 0 }
#define dn_zero_return(t) { t __dn_zero_return = dn_zero_initialize(); return __dn_zero_return; }
#define dn_type_name(t) (#t)

#define dn_enum_flags(t) DEFINE_ENUM_FLAG_OPERATORS(t)

#include "gs.h"



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
  u8* data;
  u32 len;
} dn_string_t;

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

#define dn_string_literal(s) (dn_string_t){ .data = (u8*)(s), .len = sizeof(s) - 1}

DN_API void        dn_string_builder_grow(dn_string_builder_t* builder, u32 requested_capacity);
DN_API void        dn_string_builder_append(dn_string_builder_t* builder, dn_string_t str);
DN_API void        dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str);
DN_API void        dn_string_builder_append_fmt(dn_string_builder_t* builder, dn_string_t fmt, ...);
DN_API dn_string_t dn_string_builder_write(dn_string_builder_t* builder);
DN_API char*       dn_string_builder_write_cstr(dn_string_builder_t* builder);
DN_IMP char*       dn_cstr_copy_n(const char* str, u32 length, dn_allocator_t* allocator);
DN_IMP char*       dn_cstr_copy(const char* str, dn_allocator_t* allocator);
DN_IMP char*       dn_cstr_copy_u8(const u8* str, u32 length, dn_allocator_t* allocator);
DN_API void        dn_cstr_copy_to(const char* str, char* buffer, u32 buffer_length);
DN_API void        dn_cstr_copy_to_n(const char* str, u32 length, char* buffer, u32 buffer_length);
DN_API char*       dn_string_to_cstr(dn_string_t str);
DN_API char*       dn_string_to_cstr_ex(dn_string_t str, dn_allocator_t* allocator);
DN_API bool        dn_string_equal(dn_string_t a, dn_string_t b);
DN_API bool        dn_string_equal_cstr(dn_string_t a, const char* b);
DN_API bool        dn_cstr_equal(const char* a, const char* b);
DN_API u32         dn_cstr_len(const char* str);
DN_API dn_string_t dn_string_copy_cstr(const char* str, dn_allocator_t* allocator);
DN_API dn_string_t dn_string_copy(dn_string_t str, dn_allocator_t* allocator);


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

DN_API void dn_dynamic_array_init(dn_dynamic_array_t* dynamic_array, u32 element_size, dn_allocator_t* allocator);
DN_API u8*  dn_dynamic_array_push(dn_dynamic_array_t* dynamic_array, void* data, u32 count);
DN_API u8*  dn_dynamic_array_reserve(dn_dynamic_array_t* dynamic_array, u32 count);
DN_API void dn_dynamic_array_clear(dn_dynamic_array_t* dynamic_array);
DN_API u32  dn_dynamic_array_byte_size(dn_dynamic_array_t* dynamic_array);
DN_API u8*  dn_dynamic_array_at(dn_dynamic_array_t* dynamic_array, u32 index);
DN_API void dn_dynamic_array_grow(dn_dynamic_array_t* dynamic_array, u32 capacity);

//  ██╗███╗   ███╗██████╗ ██╗     ███████╗███╗   ███╗███████╗███╗   ██╗████████╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
//  ██║████╗ ████║██╔══██╗██║     ██╔════╝████╗ ████║██╔════╝████╗  ██║╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
//  ██║██╔████╔██║██████╔╝██║     █████╗  ██╔████╔██║█████╗  ██╔██╗ ██║   ██║   ███████║   ██║   ██║██║   ██║██╔██╗ ██║
//  ██║██║╚██╔╝██║██╔═══╝ ██║     ██╔══╝  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
//  ██║██║ ╚═╝ ██║██║     ███████╗███████╗██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   ██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
//  ╚═╝╚═╝     ╚═╝╚═╝     ╚══════╝╚══════╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
#ifdef DOUBLENICKEL_IMPL

size_t hash_siphash_bytes(const void *p, size_t len, size_t seed) {
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

  #ifdef STBDS_TEST_SIPHASH_2_4
  // hardcoded with key material in the siphash test vectors
  v0 ^= 0x0706050403020100ull ^  seed;
  v1 ^= 0x0f0e0d0c0b0a0908ull ^ ~seed;
  v2 ^= 0x0706050403020100ull ^  seed;
  v3 ^= 0x0f0e0d0c0b0a0908ull ^ ~seed;
  #endif

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
  return dn_hash_str_dumb(dn_string_literal(str));
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

u8* dn_dynamic_array_push(dn_dynamic_array_t* buffer, void* data, u32 count) {
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

dn_string_t dn_string_copy_cstr(const char* str, dn_allocator_t* allocator) {
  dn_string_t copy;
  copy.len = dn_cstr_len(str);
  copy.data = (u8*)dn_allocator_alloc(allocator, copy.len);

  dn_os_memory_copy(str, copy.data, copy.len);
  return copy;
}

dn_string_t dn_string_copy(dn_string_t str, dn_allocator_t* allocator) {
  dn_string_t copy = {
    (u8*)dn_allocator_alloc(allocator, str.len),
    str.len
  };

  dn_os_memory_copy(str.data, copy.data, str.len);
  return copy;
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
	if (mode == DN_ALLOCATOR_MODE_ALLOC) {
		DN_ASSERT(bump->bytes_used + size <= bump->capacity);

		u8* memory_block = bump->buffer + bump->bytes_used;
    gs_hash_table_insert(bump->allocations, bump->bytes_used, size);
		bump->bytes_used += size;
	
		return memory_block;
	}
	else if (mode == DN_ALLOCATOR_MODE_FREE) {
		return NULL;
	}
	else if (mode == DN_ALLOCATOR_MODE_RESIZE) {
    DN_BROKEN();
	}

	DN_UNREACHABLE();
	return NULL;
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

#endif