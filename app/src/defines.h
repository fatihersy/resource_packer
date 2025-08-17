#ifndef DEFINES_H
#define DEFINES_H

// Platform detection
#if defined(_WIN32) || defined(__WIN32__)
#define PLATFORM_WINDOWS 1
//#ifndef _WIN64
//#error "64-bit is required on Windows!"
//#endif
#elif defined(__linux__) || defined(__gnu_linux__)
// Linux OS
#define PLATFORM_LINUX 1
#if defined(__ANDROID__)
#define PLATFORM_ANDROID 1
#endif
#elif defined(__unix__)
// Catch anything not caught by the above.
#define PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
// Posix
#define PLATFORM_POSIX 1
#elif __APPLE__
// Apple platforms
#define PLATFORM_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#define PLATFORM_IOS 1
#define PLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define PLATFORM_IOS 1
// iOS device
#elif TARGET_OS_MAC
#define PLATFORM_MAC 1
// Other kinds of Mac OS
#else
#error "Unknown Apple platform"
#endif
#else
#error "Unknown platform!"
#endif

#define FCLAMP(value, min, max)                                                \
  (value <= min) ? min : (value >= max) ? max : value
#define FMAX(v1, v2) (v1 >= v2) ? v1 : v2
#define FMIN(v1, v2) (v1 <= v2) ? v1 : v2
#define FABS(v1) (v1 < 0) ? v1*(-1) : v1

#include <string>
#include <array>

#define MAP_PAK_FILE_NAME "map.pak"
#define ASSET_PAK_FILE_NAME "asset.pak"
#define RESOURCE_PATH "./resources/"

#define TOTAL_ALLOCATED_MEMORY 256 * 1024 * 1024
#define TOTAL_PAK_FILE_SIZE 128 * 1024 * 1024

#define MAX_TILEMAP_LAYERS 5
#define MAX_WORLDMAP_LOCATIONS 22

// Unsigned int types.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// Signed int types.
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

// Floating point types
typedef float f32;
typedef double f64;

// Boolean types
typedef int b32;

// Ensure all types are of the correct size.
static_assert(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
static_assert(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
static_assert(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
static_assert(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

static_assert(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
static_assert(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
static_assert(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
static_assert(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

static_assert(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
static_assert(sizeof(f64) == 8, "Expected float to be 8 bytes.");


#define I64_MAX 9223372036854775807
#define U64_MAX 18446744073709551615U
#define F64_MAX 1.7976931348623157e+308
#define I32_MAX 2147483647
#define U32_MAX 4294967295U
#define F32_MAX 3.402823466e+38F
#define I16_MAX 32767
#define U16_MAX 65535U
#define  I8_MAX 127
#define  U8_MAX 255
/**
 * @brief Any id set to this should be considered invalid,
 * and not actually pointing to a real object.
 */
#define INVALID_ID32 U32_MAX
#define INVALID_ID16 U16_MAX

typedef enum data_type {
  DATA_TYPE_UNRESERVED,
  
  DATA_TYPE_I64,
  DATA_TYPE_U64,
  DATA_TYPE_F64,
  DATA_TYPE_I32,
  DATA_TYPE_U32,
  DATA_TYPE_F32,
  DATA_TYPE_I16,
  DATA_TYPE_U16,
  DATA_TYPE_I8,
  DATA_TYPE_U8,
  DATA_TYPE_C,
  DATA_TYPE_SAMPLER2D,

  DATA_TYPE_MAX,
}data_type;
typedef union data64 {
  i64 i64;
  u64 u64;
  f64 f64;
 
  i32 i32[2];
  u32 u32[2];
  f32 f32[2];
 
  i16 i16[4];
  u16 u16[4];

  i8 i8[8];
  u8 u8[8];
  char c[8];

  void* address;

  data64(void) {
    this->u64 = 0u;
  }
  
  data64(::i64 value1) {
    this->i64 = value1;
  }
  data64(::u64 value1){
    this->u64 = value1;
  }
  data64(::f64 value1){
    this->f64 = value1;
  }
  data64(::i32 value1, ::i32 value2 = 0){
    this->i32[0] = value1;
    this->i32[1] = value2;
  }
  data64(::u32 value1, ::u32 value2 = 0){
    this->u32[0] = value1;
    this->u32[1] = value2;
  }
  data64(::f32 value1, ::f32 value2 = 0){
    this->f32[0] = value1;
    this->f32[1] = value2;
  }
  data64(::u16 value1, ::u16 value2 = 0, ::u16 value3 = 0, ::u16 value4 = 0){
    this->u16[0] = value1;
    this->u16[1] = value2;
    this->u16[2] = value3;
    this->u16[3] = value4;
  }
  data64(::i16 value1, ::i16 value2 = 0, ::i16 value3 = 0, ::i16 value4 = 0){
    this->i16[0] = value1;
    this->i16[1] = value2;
    this->i16[2] = value3;
    this->i16[3] = value4;
  }
  data64(::u16* value, ::u16 len){
    if (len < 4) {
      for (int i=0; i<len; ++i) {
        this->u16[i] = value[i];
      }
    }
  }
  data64(::i16* value, ::u16 len){
    if (len < 4) {
      for (int i=0; i<len; ++i) {
        this->i16[i] = value[i];
      }
    }
  }
  data64(::u8* value, ::u16 len){
    if (len < 8) {
      for (int i=0; i<len; ++i) {
        this->u8[i] = value[i];
      }
    }
  }
  data64(::i8* value, ::u16 len){
    if (len < 8) {
      for (int i=0; i<len; ++i) {
        this->i8[i] = value[i];
      }
    }
  }
  data64(char* value, ::u16 len){
    if (len < 8) {
      for (int i=0; i<len; ++i) {
        this->c[i] = value[i];
      }
      this->c[len-1] = '\0';
    }
  }
  data64(const char* value, ::u16 len){
    if (len < 8) {
      for (int i=0; i<len; ++i) {
        this->c[i] = value[i];
      }
      this->c[len-1] = '\0';
    }
  }
  data64(void* address1){
    this->address = address1;
  }
} data64;

typedef union data128 {
  i64 i64[2];
  u64 u64[2];
  f64 f64[2];
 
  i32 i32[4];
  u32 u32[4];
  f32 f32[4];
 
  i16 i16[8];
  u16 u16[8];

  i8 i8[16];
  u8 u8[16];
  char c[16];

  void* address[2];

  data128(void) {
    this->u64[0] = 0u;
    this->u64[1] = 0u;
  }
  
  data128(::i64 value1, ::i64 value2 = 0) {
    this->i64[0] = value1;
    this->i64[1] = value2;
  }
  data128(::u64 value1, ::u64 value2 = 0){
    this->u64[0] = value1;
    this->u64[1] = value2;
  }
  data128(::f64 value1, ::f64 value2 = 0){
    this->f64[0] = value1;
    this->f64[1] = value2;
  }
  data128(::i32 value1, ::i32 value2 = 0, ::i32 value3 = 0, ::i32 value4 = 0){
    this->i32[0] = value1;
    this->i32[1] = value2;
    this->i32[2] = value3;
    this->i32[3] = value4;
  }
  data128(::u32 value1, ::u32 value2 = 0, ::u32 value3 = 0, ::u32 value4 = 0){
    this->u32[0] = value1;
    this->u32[1] = value2;
    this->u32[2] = value3;
    this->u32[3] = value4;
  }
  data128(::f32 value1, ::f32 value2 = 0, ::f32 value3 = 0, ::f32 value4 = 0){
    this->f32[0] = value1;
    this->f32[1] = value2;
    this->f32[2] = value3;
    this->f32[3] = value4;
  }
  data128(::u16 value1, ::u16 value2 = 0, ::u16 value3 = 0, ::u16 value4 = 0, ::u16 value5 = 0, ::u16 value6 = 0, ::u16 value7 = 0, ::u16 value8 = 0){
    this->u16[0] = value1;
    this->u16[1] = value2;
    this->u16[2] = value3;
    this->u16[3] = value4;
    this->u16[4] = value5;
    this->u16[5] = value6;
    this->u16[6] = value7;
    this->u16[7] = value8;
  }
  data128(::i16 value1, ::i16 value2 = 0, ::i16 value3 = 0, ::i16 value4 = 0, ::i16 value5 = 0, ::i16 value6 = 0, ::i16 value7 = 0, ::i16 value8 = 0){
    this->i16[0] = value1;
    this->i16[1] = value2;
    this->i16[2] = value3;
    this->i16[3] = value4;
    this->i16[4] = value5;
    this->i16[5] = value6;
    this->i16[6] = value7;
    this->i16[7] = value8;
  }
  data128(::u16* value, ::u16 len){
    if (len < 8) {
      for (int i=0; i<len; ++i) {
        this->u16[i] = value[i];
      }
    }
  }
  data128(::i16* value, ::u16 len){
    if (len < 8) {
      for (int i=0; i<len; ++i) {
        this->i16[i] = value[i];
      }
    }
  }
  data128(::u8* value, ::u16 len){
    if (len < 16) {
      for (int i=0; i<len; ++i) {
        this->u8[i] = value[i];
      }
    }
  }
  data128(::i8* value, ::u16 len){
    if (len < 16) {
      for (int i=0; i<len; ++i) {
        this->i8[i] = value[i];
      }
    }
  }
  data128(char* value, ::u16 len){
    if (len < 16) {
      for (int i=0; i<len; ++i) {
        this->c[i] = value[i];
      }
      this->c[len-1] = '\0';
    }
  }
  data128(const char* value, ::u16 len){
    if (len < 16) {
      for (int i=0; i<len; ++i) {
        this->c[i] = value[i];
      }
      this->c[len-1] = '\0';
    }
  }
  data128(void* address1, void* address2 = nullptr){
    this->address[0] = address1;
    this->address[1] = address2;
  }
} data128;

  // 256 bytes
typedef union data256 {
  i64 i64[4];
  u64 u64[4];
  f64 f64[4];
 
  i32 i32[8];
  u32 u32[8];
  f32 f32[8];
 
  i16 i16[16];
  u16 u16[16];

  i8 i8[32];
  u8 u8[32];

  char c[32];
  void* address[4];

  data256(void){
    this->u64[0] = 0u;
    this->u64[1] = 0u;
    this->u64[2] = 0u;
    this->u64[3] = 0u;
  }
  
  data256(::i64 value1, ::i64 value2 = 0, ::i64 value3 = 0, ::i64 value4 = 0){
    this->i64[0] = value1;
    this->i64[1] = value2;
    this->i64[2] = value3;
    this->i64[3] = value4;
  }
  data256(::u64 value1, ::u64 value2 = 0, ::u64 value3 = 0, ::u64 value4 = 0){
    this->u64[0] = value1;
    this->u64[1] = value2;
    this->u64[2] = value3;
    this->u64[3] = value4;
  }
  data256(::f64 value1, ::f64 value2 = 0, ::f64 value3 = 0, ::f64 value4 = 0){
    this->f64[0] = value1;
    this->f64[1] = value2;
    this->f64[2] = value3;
    this->f64[3] = value4;
  }
  data256(::i32 value1, ::i32 value2 = 0, ::i32 value3 = 0, ::i32 value4 = 0, ::i32 value5 = 0, ::i32 value6 = 0, ::i32 value7 = 0, ::i32 value8 = 0){
    this->i32[0] = value1;
    this->i32[1] = value2;
    this->i32[2] = value3;
    this->i32[3] = value4;
    this->i32[4] = value5;
    this->i32[5] = value6;
    this->i32[6] = value7;
    this->i32[7] = value8;
  }
  data256(::u32 value1, ::u32 value2 = 0, ::u32 value3 = 0, ::u32 value4 = 0, ::u32 value5 = 0, ::u32 value6 = 0, ::u32 value7 = 0, ::u32 value8 = 0){
    this->u32[0] = value1;
    this->u32[1] = value2;
    this->u32[2] = value3;
    this->u32[3] = value4;
    this->u32[4] = value5;
    this->u32[5] = value6;
    this->u32[6] = value7;
    this->u32[7] = value8;
  }
  data256(::f32 value1, ::f32 value2 = 0, ::f32 value3 = 0, ::f32 value4 = 0, ::f32 value5 = 0, ::f32 value6 = 0, ::f32 value7 = 0, ::f32 value8 = 0){
    this->f32[0] = value1;
    this->f32[1] = value2;
    this->f32[2] = value3;
    this->f32[3] = value4;
    this->f32[4] = value5;
    this->f32[5] = value6;
    this->f32[6] = value7;
    this->f32[7] = value8;
  }
  data256(::u16* value, ::u16 len){
    if (len < 16) {
      for (int i=0; i<len; ++i) {
        this->u16[i] = value[i];
      }
    }
  }
  data256(::i16* value, ::u16 len){
    if (len < 16) {
      for (int i=0; i<len; ++i) {
        this->i16[i] = value[i];
      }
    }
  }
  data256(::u8* value, ::u16 len){
    if (len < 32) {
      for (int i=0; i<len; ++i) {
        this->u8[i] = value[i];
      }
    }
  }
  data256(::i8* value, ::u16 len){
    if (len < 32) {
      for (int i=0; i<len; ++i) {
        this->i8[i] = value[i];
      }
    }
  }
  data256(char* value, ::u16 len){
    if (len < 32) {
      for (int i=0; i<len; ++i) {
        this->c[i] = value[i];
      }
      this->c[len-1] = '\0';
    }
  }
  data256(const char* value, ::u16 len){
    if (len < 32) {
      for (int i=0; i<len; ++i) {
        this->c[i] = value[i];
      }
      this->c[len-1] = '\0';
    }
  }
  data256(void* address1, void* address2 = nullptr, void* address3 = nullptr, void* address4 = nullptr){
    this->address[0] = address1;
    this->address[1] = address2;
    this->address[2] = address3;
    this->address[3] = address4;
  }
} data256;

typedef enum file_extension {
  FILE_EXT_UNDEFINED,
  FILE_EXT_PNG,
  FILE_EXT_WAV,
  FILE_EXT_TTF,
  FILE_EXT_TXT,
  FILE_EXT_MAX
}file_extension;

typedef enum worldmap_stage_file_content {
  WSF_CONTENT_UNDEFINED,
  WSF_CONTENT_COLLISION,
  WSF_CONTENT_LAYER,
  WSF_CONTENT_PROP,
  WSF_CONTENT_MAX
}worldmap_stage_file_content;

typedef struct filename_offset_data {
  size_t filename_offset;
  size_t path_length;

  bool is_success;
  filename_offset_data(void) {
    this->filename_offset = 0u;
    this->path_length = 0u;
    this->is_success = false;
  }
}filename_offset_data;

typedef struct file_data {
  std::string file_name;
  std::string file_extension;
  size_t offset;
  i32 total_size;
  bool is_success;
  file_data(void) {
    this->file_name = std::string();
    this->file_extension = std::string();
    this->offset = 0u;
    this->total_size = 0;
    this->is_success = false;
  }
}file_data;

typedef struct worldmap_stage_file {
  i32 stage_index;
  std::array<std::string, MAX_TILEMAP_LAYERS> file_data;
  std::string file_collision;
  std::string file_prop;
  bool is_success;
  worldmap_stage_file(void) {
    this->stage_index = 0;
    this->file_data.fill(std::string(""));
    this->file_collision = std::string("");
    this->file_prop = std::string("");
    this->is_success = false;
  }
} worldmap_stage_file;

#endif
