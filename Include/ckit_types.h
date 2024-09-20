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

#include "./ckg_types.h"
#include "./ckg_logger.h"
#include "./ckg_stack_trace.h"

//========================== Begin Types ==========================
#define CKG_ASSERT_ENABLED TRUE
//=========================== End Types ===========================

#include "../Include/ckg_logger.h"
#include "../Include/ckg_memory.h"

#if defined(_MSC_VER )
    #include <windows.h>
    #include <DbgHelp.h>
    #pragma comment(lib, "dbghelp")
    void ckg_stack_trace_dump() {
        CKG_LOG_PRINT("------------------ Error Stack Trace ------------------\n");
        // Date: July 02, 2024
        // NOTE(Jovanni): This only works for windows and when debug symbols are compiled into the program
        void *stack[100];
        unsigned short number_of_captured_frames;
        SYMBOL_INFO *symbol;
        HANDLE process;

        process = GetCurrentProcess();
        SymInitialize(process, NULL, TRUE);

        number_of_captured_frames = CaptureStackBackTrace(0, 100, stack, NULL);
        symbol = (SYMBOL_INFO *)ckg_alloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char));
        symbol->MaxNameLen = 255;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        int count = 0;
        for (int i = number_of_captured_frames - 4; i > 0; i--) {
            DWORD64 displacement = 0;
            if (SymFromAddr(process, (DWORD64)(stack[i]), &displacement, symbol)) {
                DWORD displacementLine = 0;
                IMAGEHLP_LINE64 line;
                line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
                if (SymGetLineFromAddr64(process, (DWORD64)(stack[i]), &displacementLine, &line)) {
                    printf("%d: %s - %s:%d\n", count, symbol->Name, line.FileName, line.LineNumber);
                } else {
                    printf("%d: %s\n", count, symbol->Name);
                }
            }
            count++;
        }

        ckg_free(symbol);
        CKG_LOG_PRINT("------------------ Error Stack Trace End ------------------\n");
    }
#elif defined(__GNUC__)
    void ckg_stack_trace_dump() {
        CKG_LOG_PRINT("------------------ Error Stack Trace ------------------\n");
        // backtrace
        CKG_LOG_PRINT("------------------ Error Stack Trace End ------------------\n");
    }
#endif

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#if CKG_ASSERT_ENABLED == TRUE	   
    #define ckg_assert(expression)                            \
    do {                                                      \
        if (!(expression)) {                                  \
            ckg_stack_trace_dump();                           \
            char msg[] = "Func: %s, File: %s:%d\n";           \
            CKG_LOG_FATAL(msg, __func__, __FILE__, __LINE__); \
            CRASH;                                            \
        }                                                     \
    } while (FALSE)

    #define ckg_assert_msg(expression, message, ...)	\
        do {                                            \
            if (!(expression)) {                        \
                ckg_stack_trace_dump();                 \
                CKG_LOG_FATAL(message, ##__VA_ARGS__);  \
                CRASH;                                  \
            }                                           \
        } while (FALSE)
#else
        #define ckg_assert(expression)
        #define ckg_assert_msg(expression, message, ...)
#endif
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++