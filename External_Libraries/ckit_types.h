#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

//========================== Begin Types ==========================
typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u8 Boolean;
//=========================== End Types ===========================

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define FALSE 0
#define TRUE 1
#define NULLPTR 0
#define PI 3.14159265359

#define stringify(entry) #entry
#define glue(a, b) a##b

#define KiloBytes(value) ((u64)(value) * 1024L)
#define MegaBytes(value) ((u64)KiloBytes(value) * 1024L)
#define GigaBytes(value) ((u64)MegaBytes(value) * 1024L)

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define CLAMP(value, min_value, max_value) (MIN(MAX(value, min_value), max_value))

#define local_persist static
#define internal static

// Date: July 12, 2024
// TODO(Jovanni): Test this to make sure its actually works but it makes sense to me
#define OFFSET_OF(type, member) sizeof((u64)(&(((type*)0)->member)))

#define FIRST_DIGIT(number) ((int)number % 10);

void U32_EndianSwap(u32* number_to_endian_swap);
void U64_EndianSwap(u64* number_to_endian_swap);
#define GET_BIT(number, bit_to_check) ((number & (1 << bit_to_check)) >> bit_to_check)
#define SET_BIT(number, bit_to_set) number |= (1 << bit_to_set);
#define UNSET_BIT(number, bit_to_unset) number &= (~(1 << bit_to_unset));

/**
 * @brief This only works on static arrays not buffers
 * 
 */
#define ArrayCount(array) (sizeof(array) / sizeof(array[0]))

#define PLATFORM_MAX_PATH 256

#if defined(_WIN32)
    #define PLATFORM_WINDOWS
    #define OS_DELIMITER '\\'
    #define CRASH __debugbreak()
#elif defined(__APPLE__)
    #define PLATFORM_APPLE
    #define OS_DELIMITER '/'
    #define CRASH __builtin_trap()
#elif defined(__linux__) || defined(__unix__) || defined(__POSIX__)
    #define PLATFORM_LINUX
    #define OS_DELIMITER '/'
    #define CRASH __builtin_trap()
#else
    #error "Unknown Platform???"
#endif
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
typedef struct CKIT_Vector2 {
	union {
		struct {
			double x;
			double y;
		};
		double v[2];
	};
} CKIT_Vector2;

typedef struct CKIT_Vector3 {
	union {
		struct {
			double x;
			double y;
			double z;
		};
		struct {
			double r;
			double g;
			double b;
		};
		double v[3];
	};
} CKIT_Vector3;

typedef struct CKIT_Vector4 {
	union {
		struct {
			double x;
			double y;
			double z;
			double w;
		};
		struct {
			double r;
			double g;
			double b;
			double a;
		};
		double v[4];
	};
} CKIT_Vector4;

typedef CKIT_Vector4 CKIT_Color;

typedef struct CKIT_Bitmap {
	u32 height;
	u32 width;
	u8 bytes_per_pixel;
	u8* memory;
} CKIT_Bitmap;

typedef struct CKIT_Rectangle2D {
	CKIT_Vector2 position;
	u32 width;
	u32 height;
} CKIT_Rectangle2D;

typedef struct CKIT_Rectangle3D {
	CKIT_Vector3 position;
	u32 length;
	u32 width;
	u32 height;
} CKIT_Rectangle3D;

typedef enum CKG_LogLevel {
    CKG_LOG_LEVEL_FATAL,
    CKG_LOG_LEVEL_ERROR,
    CKG_LOG_LEVEL_WARN,
    CKG_LOG_LEVEL_DEBUG,
    CKG_LOG_LEVEL_SUCCESS,
    CKG_LOG_LEVEL_PRINT,
    CKG_LOG_LEVEL_COUNT
} CKG_LogLevel;

typedef enum CKIT_LogLevel {
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_SUCCESS,
    LOG_LEVEL_PRINT,
    LOG_LEVEL_COUNT
} CKIT_LogLevel;

typedef enum CKIT_MemoryTagID { // Reserved tags
    TAG_USER_UNKNOWN,

    TAG_CKIT_TEMP,
    TAG_CKIT_CORE_STRING,
    TAG_CKIT_CORE_ARENA,
    TAG_CKIT_CORE_VECTOR,
    TAG_CKIT_CORE_STACK,
    TAG_CKIT_CORE_LINKED_LIST,
    TAG_CKIT_CORE_QUEUE,
    TAG_CKIT_CORE_IO,
    TAG_CKIT_CORE_HASHMAP,
    TAG_CKIT_CORE_HASHSET,

    TAG_CKIT_MODULE_LEXER,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_BMP,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_OBJ,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_PNG,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_JSON,

    TAG_CKIT_EXPECTED_USER_FREE,

    TAG_CKIT_RESERVED_COUNT
} CKIT_MemoryTagID;

typedef struct CKIT_VectorHeader {
	u32 count;
	u32 capacity;
	char* magic;
	// Boolean is_pointer_type (if I know this then I can use arenas way way smarter, but actually maybe it doesn't matter maybe I need to rework arenas s
	// it adds pages of memory and it links those together instead of copying and reallocing.)
} CKIT_VectorHeader;

#define CKIT_COLOR_BLACK (CKIT_Color){0, 0, 0, 255}
#define CKIT_COLOR_RED (CKIT_Color){255, 0, 0, 255}
#define CKIT_COLOR_BLUE (CKIT_Color){0, 0, 255, 255}
#define CKIT_COLOR_GREEN (CKIT_Color){0, 255, 0, 255}
#define CKIT_COLOR_WHITE (CKIT_Color){255, 255, 255, 255}
#define CKIT_COLOR_PINK (CKIT_Color){255, 105, 180, 255}
#define CKIT_COLOR_LIME (CKIT_Color){0, 255, 128, 255}
#define CKIT_COLOR_CYAN (CKIT_Color){0, 255, 255, 255}
#define CKIT_COLOR_PURPLE (CKIT_Color){128, 0, 128, 255}
#define CKIT_COLOR_YELLOW (CKIT_Color){255, 255, 0, 255}