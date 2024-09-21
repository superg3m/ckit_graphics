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
#define MEMORY_TAG_CHARACTER_LIMIT 16

// Date: May 04, 2024
// TODO(Jovanni): This is gonna be a problem no doubt
#define PLATFORM_CHARACTER_LIMIT 200
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
// Basic

#define CKG_ASSERT_ENABLED TRUE

typedef enum CKG_LogLevel {
    CKG_LOG_LEVEL_FATAL,
    CKG_LOG_LEVEL_ERROR,
    CKG_LOG_LEVEL_WARN,
    CKG_LOG_LEVEL_DEBUG,
    CKG_LOG_LEVEL_SUCCESS,
    CKG_LOG_LEVEL_PRINT,
    CKG_LOG_LEVEL_COUNT
} CKG_LogLevel;

void MACRO_ckit_log_output(CKG_LogLevel log_level, const char* message, ...);

#define ckg_log_output(log_level, message, ...) MACRO_ckg_log_output(log_level, message, ##__VA_ARGS__)
#define CKG_LOG_PRINT(message, ...) ckg_log_output(CKG_LOG_LEVEL_PRINT, message, ##__VA_ARGS__)
#define CKG_LOG_SUCCESS(message, ...) ckg_log_output(CKG_LOG_LEVEL_SUCCESS, message, ##__VA_ARGS__)
#define CKG_LOG_DEBUG(message, ...) ckg_log_output(CKG_LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#define CKG_LOG_WARN(message, ...) ckg_log_output(CKG_LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#define CKG_LOG_ERROR(message, ...) ckg_log_output(CKG_LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#define CKG_LOG_FATAL(message, ...) ckg_log_output(CKG_LOG_LEVEL_FATAL, message, ##__VA_ARGS__)

void ckg_stack_trace_dump();

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

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define CKIT_ASSERT_ENABLED TRUE

// Date: August 14, 2024
// TODO(Jovanni): This has to get fixed because I have my own logging logic for ckit now! ${} needs to be handled here
#if CKIT_ASSERT_ENABLED == TRUE
	#define ckit_assert_msg(expression, message, ...) ckg_assert_msg(expression, message, ##__VA_ARGS__)
	#define ckit_assert(expression) ckg_assert(expression)
#else
		#define ckit_assert_msg(expression, message, ...)
		#define ckit_assert(expression) 
#endif
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
/*===========================================================
 * File: ckit_logger.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
//========================== Begin Types ==========================
#define LOGGING_ENABLED TRUE
#define LOG_LEVEL_CHARACTER_LIMIT 11

typedef enum CKIT_LogLevel {
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_SUCCESS,
    LOG_LEVEL_PRINT,
    LOG_LEVEL_COUNT
} CKIT_LogLevel;

//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    void MACRO_ckit_log_output(CKIT_LogLevel log_level, const char* message, ...);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
// #define log_output(log_level, message, ...) MACRO_log_output(log_level, message, ##__VA_ARGS__)
#define ckit_log_output(log_level, message, ...) MACRO_ckit_log_output(log_level, message, ##__VA_ARGS__)
#define LOG_PRINT(message, ...) ckit_log_output(LOG_LEVEL_PRINT, message, ##__VA_ARGS__)
#define LOG_SUCCESS(message, ...) ckit_log_output(LOG_LEVEL_SUCCESS, message, ##__VA_ARGS__)
#define LOG_DEBUG(message, ...) ckit_log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#define LOG_WARN(message, ...) ckit_log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...) ckit_log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#define LOG_FATAL(message, ...) ckit_log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

//========================== Begin Types ==========================
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

typedef struct CKG_LinkedList CKG_LinkedList;
typedef struct CKG_Node CKG_Node;

typedef struct CKIT_AllocationInfo {
    const char* file_name;
    s64 line;
    const char* function_name;
    size_t allocation_size;
} CKIT_AllocationInfo;

typedef struct CKIT_MemoryTag {
    CKIT_MemoryTagID tag_id;
    const char* tag_name;
    CKIT_AllocationInfo allocation_info;
} CKIT_MemoryTag;

typedef struct CKIT_MemoryTagPool {
    CKIT_MemoryTagID tag_id;
    const char* pool_name;
    CKG_LinkedList* allocated_headers;
    size_t total_pool_allocation_size;
} CKIT_MemoryTagPool;

typedef struct CKIT_MemoryHeader {
    CKIT_MemoryTag tag;
    CKG_Node* linked_list_address;
    const char* magic;
} CKIT_MemoryHeader;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    //
    // Memory Tracker
    //
    void ckit_tracker_init();
	void ckit_tracker_register_tag_pool(CKIT_MemoryTagID tag_id, const char* name);
    void* MACRO_ckit_tracker_insert_header(void* data, CKIT_MemoryHeader header);

    CKIT_MemoryHeader ckit_tracker_header_create(CKIT_MemoryTagID tag_id, size_t allocation_size, const char* file_name, const u64 line, const char* function_name);
	void* memory_insert_header(void* data, CKIT_MemoryHeader header);
    void ckit_tracker_add(CKIT_MemoryHeader* header);
    void ckit_tracker_remove(CKIT_MemoryHeader* header);
    CKIT_MemoryHeader* ckit_tracker_get_header(void* data);

    void ckit_tracker_print_header(CKIT_MemoryHeader* header, CKIT_LogLevel log_level);
    void ckit_tracker_print_pool(CKIT_MemoryTagPool* pool, CKIT_LogLevel log_level);

    void ckit_tracker_print_all_pools(CKIT_LogLevel log_level);

    // CKIT_MemoryHeader** ckit_tracker_get_all_headers();
    // CKIT_MemoryTagPool** ckit_tracker_get_all_pools();

    //
    // Memory
    //
    void memory_init();
    void* MACRO_ckit_alloc(size_t number_of_bytes, CKIT_MemoryTagID tag_id, const char* file, const u32 line, const char* function);
    void* MACRO_ckit_realloc(void* data, u64 new_allocation_size, const char* file, const u32 line, const char* function);
    void* MACRO_ckit_free(void* data);

    void ckit_memory_report(CKIT_LogLevel log_level);

    Boolean ckit_memory_compare(const void* buffer_one, const void* buffer_two, u32 b1_allocation_size, u32 b2_allocation_size);
    void ckit_memory_copy(const void* source, void* destination, size_t source_size, size_t destination_capacity);
    void ckit_memory_move(const void* source, void* destination, size_t source_payload_size);
    void ckit_memory_zero(void* data, size_t data_size_in_bytes);

    void MACRO_ckit_memory_delete_index(void* data, u32 number_of_elements, u32 data_capacity, size_t element_size_in_bytes, u32 index);
    void MACRO_ckit_memory_insert_index(void* data, u32 number_of_elements, u32 data_capacity, size_t element_size_in_bytes, u32 index);


#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_alloc(number_of_bytes) MACRO_ckit_alloc(number_of_bytes, TAG_USER_UNKNOWN, __FILE__, __LINE__, __func__)
#define ckit_alloc_custom(number_of_bytes, tag_id) MACRO_ckit_alloc(number_of_bytes, tag_id, __FILE__, __LINE__, __func__)
#define ckit_realloc(data, new_allocation_size) MACRO_ckit_realloc(data, new_allocation_size, __FILE__, __LINE__, __func__)
#define ckit_free(data) data = MACRO_ckit_free(data)

#define ckit_memory_delete_index(data, number_of_elements, data_capacity, index) MACRO_ckit_memory_delete_index(data, number_of_elements, data_capacity, sizeof(data[0]), index)
#define ckit_memory_insert_index(data, number_of_elements, data_capacity, element, index) MACRO_ckit_memory_delete_index(data, number_of_elements, data_capacity, sizeof(data[0]), index); data[index] = element;

#define ckit_tracker_insert_header(data, header) data = MACRO_ckit_tracker_insert_header(data, header)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

//========================== Begin Types ==========================
typedef enum CKIT_ArenaFlag {
  CKIT_ARENA_FLAG_FIXED,
  CKIT_ARENA_FLAG_CIRCULAR,
  CKIT_ARENA_FLAG_EXTENDABLE_PAGES,
  CKIT_ARENA_FLAG_COUNT
} CKIT_ArenaFlag;

typedef struct CKIT_Arena CKIT_Arena;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Arena* MACRO_ckit_arena_create(size_t allocation_size, const char* name, CKIT_ArenaFlag flag, u8 alignment);
	void* MACRO_ckit_arena_push(CKIT_Arena* arena, size_t element_size);	
	
	CKIT_Arena* MACRO_ckit_arena_free(CKIT_Arena* arena);
	void ckit_arena_clear(CKIT_Arena* arena);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_arena_create(allocation_size, name) MACRO_ckit_arena_create(allocation_size, name, CKIT_ARENA_FLAG_EXTENDABLE_PAGES, 0)
#define ckit_arena_create_custom(allocation_size, name, flags, alignment) MACRO_ckit_arena_create(allocation_size, name, flags, alignment)

#define ckit_arena_free(arena) arena = MACRO_ckit_arena_free(arena)

#define ckit_arena_push(arena, type) ((type*)MACRO_ckit_arena_push(arena, sizeof(type)))
#define ckit_arena_push_array(arena, type, element_count) ((type*)MACRO_ckit_arena_push(arena, sizeof(type) * element_count))
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++


#define ckit_char_is_digit(c) ckg_char_is_digit(c)
#define ckit_char_is_upper(c) ckg_char_is_upper(c)
#define ckit_char_is_lower(c) ckg_char_is_lower(c)
#define ckit_char_is_alpha(c) ckg_char_is_alpha(c)
#define ckit_char_is_alpha_numeric(c) ckg_char_is_alpha_numeric(c)

//========================== Begin Types ==========================
typedef char* String;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	String ckit_str_create_custom(const char* c_str, size_t capacity);
	u32 ckit_cstr_length(const char* str);
	u32 ckit_str_length(const String str);
	Boolean ckit_str_equal(const char* str1, const char* str2);
	void ckit_str_copy(String str, const char* source);

	String MACRO_ckit_str_append(String str, const char* source);
	String MACRO_ckit_str_append_char(String str, const char source);

	String MACRO_ckit_str_insert(String str, const char* to_insert, const u32 index);
	String MACRO_ckit_str_insert_char(String str, const char to_insert, const u32 index);
	void ckit_str_clear(char* str1);

	// If you are copying data to the string and need to update the header state specifically for length
	void ckit_str_recanonicalize_header_length(String str);

	String ckit_substring(const char* string_buffer, u32 start_range, u32 end_range);

	// Little bit tricky. This method returns a vector of strings so 
	// ckit_vector_count(): to get the number of strings it returned
	//
	//  for (u32 i = 0; i < ckit_vector_count(string_vector); i++) {
	//     LOG_PRINT("%s\n", string_vector[i]);
	//  }
	//  ckit_vector_free(string_vector);
	String* ckit_str_split(const char* string_buffer, const char* delimitor);
	// Date: September 09, 2024
	// TODO(Jovanni): String* ckit_str_split_with_char(const char* string_buffer, const char delimitor);

	Boolean ckit_str_contains(const char* string_buffer, const char* contains);
	s32 ckit_str_index_of(const char* string_buffer, const char* sub_string);
	s32 ckit_str_last_index_of(const char* string_buffer, const char* sub_string);
	Boolean ckit_str_starts_with(const char* string_buffer, const char* starts_with);
	Boolean ckit_str_ends_with(const char* string_buffer, const char* ends_with);
	String ckit_str_reverse(const char* string_buffer);
	String ckit_str_int_to_str(int number);
	int ckit_str_to_int(const char* ascii_number);
	String ckit_str_between_delimiters(const char* str, const char* start_delimitor, const char* end_delimitor);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_str_create(str) ckit_str_create_custom(str, 0)

#define ckit_str_insert(str, source, index) str = MACRO_ckit_str_insert(str, source, index);
#define ckit_str_insert_char(str, source, index) str = MACRO_ckit_str_insert_char(str, source, index);

#define ckit_str_append(str, source) str = MACRO_ckit_str_append(str, source);
#define ckit_str_append_char(str, source) str = MACRO_ckit_str_append_char(str, source);
//++++++++++++++++++++++++++++ End Macros ++++++++++++++++++++++++++

#include <math.h>

//========================== Begin Types ==========================
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

typedef struct Mat2x2 {
	CKIT_Vector2 data[2];
} Mat2x2;

typedef struct Mat3x3 {
	CKIT_Vector3 data[3];
} Mat3x3;

typedef struct Mat4x4 {
	CKIT_Vector4 data[4];
} Mat4x4;

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

typedef struct CKIT_Circle2D {
	CKIT_Vector2 position;
	u32 radius;
} CKIT_Circle2D;

typedef struct CKIT_Circle3D {
	CKIT_Vector3 position;
	u32 radius;
} CKIT_Circle3D;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	int int_abs(int a);
	double float_abs(double a);
	double lerp(double a, double b, double t);
	CKIT_Vector2 CKIT_Vector2_lerp(CKIT_Vector2 a, CKIT_Vector2 b, double t);
	CKIT_Vector3 CKIT_Vector3_lerp(CKIT_Vector3 a, CKIT_Vector3 b, double t);
	CKIT_Vector4 CKIT_Vector4_lerp(CKIT_Vector4 a, CKIT_Vector4 b, double t);

	void CKIT_Vector2_print(CKIT_Vector2 point);
	CKIT_Vector2 CKIT_Vector2_spline_point(CKIT_Vector2* spline_points, u32 spline_points_count, double t);

	Mat2x2 ckit_mat2x2_projection_mat();
	Mat2x2 ckit_mat2x2_rotation_mat();
	Mat2x2 ckit_mat2x2_transposition();
	Mat2x2 ckit_mat2x2_mult();
	Mat2x2 ckit_mat2x2_add();

	Mat3x3 ckit_mat3x3_projection_mat();
	Mat3x3 ckit_mat3x3_rotation_mat();
	Mat3x3 ckit_mat3x3_transposition();
	Mat3x3 ckit_mat3x3_mult();
	Mat3x3 ckit_mat3x3_add();

	Mat4x4 ckit_mat4x4_projection_mat();
	Mat4x4 ckit_mat4x4_rotation_mat();
	Mat4x4 ckit_mat4x4_transposition();
	Mat4x4 ckit_mat4x4_mult();
	Mat4x4 ckit_mat4x4_add();

	CKIT_Rectangle2D ckit_rectangle2d_create(s32 x, s32 y, u32 width, u32 height);
	Boolean ckit_rectangle_check_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2);
	CKIT_Rectangle2D ckit_rectangle_get_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2);

	CKIT_Rectangle3D ckit_rectangle3d_create(s32 x, s32 y, s32 z, u32 length, u32 width, u32 height);

	CKIT_Circle2D ckit_circle2d_create(s32 x, s32 y, u32 radius);
	CKIT_Circle3D ckit_circle3d_create(s32 x, s32 y, s32 z, u32 radius);

#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

// This is going to be windows only for a bit

//========================== Begin Types ==========================
typedef struct CKIT_SystemObjectInfo {
	// last modified
	// created
	int a;
} CKIT_SystemObjectInfo;

typedef struct CKIT_File {
	char* name;
	size_t size;
	size_t cursor;
} CKIT_File;

typedef struct CKIT_Directory {
	struct CKIT_Directory* parent_directory;
	struct CKIT_Directory** sub_directories;
	CKIT_File* files;
	size_t size;
} CKIT_Directory;

typedef char* String;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	String ckit_os_get_cwd();
	void ckit_os_ls();
	void ckit_os_get_items();
	void ckit_os_chdir();
	void ckit_os_mkdir();
	void ckit_os_create_file(const char* path);
	Boolean ckit_os_path_exists(const char* path);
	void ckit_os_run_subprocess();
	void ckit_os_get_file_info();
	String ckit_os_path_join(char* path, const char* to_join);
	void ckit_os_system(const char* command);

	/**
	 * @brief returns the number of counts in a millisecond
	 * double start_counter = ckit_os_query_performance_counter();
	 * 
	 * ... do some work
	 * 
	 * double end_counter = ckit_os_query_performance_counter();
	 * double milliseconds_per_frame = end_counter - start_counter
	 * double seconds_per_frame = milliseconds_per_frame / 1000.0
	 * u64 fps = 1.0 / seconds_per_frame;
	 * 
	 * @return double 
	 */
	double ckit_os_query_performance_counter();

	CKIT_File* ckit_os_file_open(const char* path);
	String ckit_os_file_read_next_line(CKIT_File* file);
	String ckit_os_file_read_next_integer(CKIT_File* file);
	String ckit_os_file_read_next_float(CKIT_File* file);

	CKIT_File* ckit_os_file_close(CKIT_File* file);

	CKIT_File* MACRO_ckit_os_close_file(CKIT_File* file);
	u8* ckit_os_read_entire_file(const char* path, size_t* returned_file_size);

	void ckit_os_get_mouse_position(int* mouse_x, int* mouse_y);

	void ckit_os_push(char* path);
	void ckit_os_pop();

	void ckit_os_dir_append_subdir(CKIT_Directory* directory, CKIT_Directory* sub_directory);
	void ckit_os_dir_append_file(CKIT_Directory* directory, CKIT_File* file);

	// CKIT_Time ckit_os_timer_start()
	// CKIT_Time ckit_os_timer_end()
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
/*===========================================================
 * File: platform_services.h
 * Date: May 10, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

//========================== Begin Types ==========================
#define TEXT_CLEAR     0x0000 // text color default clear
#define TEXT_BLUE      0x0001 // text color contains blue.
#define TEXT_GREEN     0x0002 // text color contains green.
#define TEXT_RED       0x0004 // text color contains red.
#define TEXT_CYAN      TEXT_BLUE|TEXT_GREEN // text color contains cyan.
#define TEXT_PURPLE    TEXT_RED|TEXT_BLUE // text color contains purple.
#define TEXT_WHITE     TEXT_RED|TEXT_GREEN|TEXT_BLUE // text color contains white.

#define BACK_BLUE      0x0010 // background color contains blue.
#define BACK_GREEN     0x0020 // background color contains green.
#define BACK_RED       0x0040 // background color contains red.
//=========================== End Types ===========================

//************************* Begin Functions *************************
// Supported Platform Operations

#ifdef __cplusplus
extern "C" {
#endif
  void* platform_allocate(size_t number_of_bytes);
  void platform_free(void* data);
  void platform_console_write(const char* message, unsigned char color);
  void platform_console_init();
  void platform_console_shutdown();
  // void _platform_open_window();
  // void _platform_open_file();
  // void _platform_close_file();
  // void _platform_sleep();
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************
// Collections

//========================== Begin Types ==========================
typedef struct CKIT_VectorHeader {
	u32 count;
	u32 capacity;
	char* magic;
	// Boolean is_pointer_type (if I know this then I can use arenas way way smarter, but actually maybe it doesn't matter maybe I need to rework arenas s
	// it adds pages of memory and it links those together instead of copying and reallocing.)
} CKIT_VectorHeader;

//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    void* ckit_vector_grow(void* vector, size_t element_size, Boolean force_grow, const char* file, const u32 line, const char* function);
    void* MACRO_ckit_vector_reserve(size_t element_size, u32 capacity, const char* file, const u32 line, const char* function);
	void* MACRO_ckit_vector_free(void* vector);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

// Date: July 13, 2024
// NOTE(Jovanni): This doesn't have very good checking if the vector is NULLPTR
// Attemptying to get the length/cap/pop of a NULLPTR will result in a uncontrolled crash

// Date: September 12, 2024
// TODO(Jovanni): MAKE SURE YOU TEST ckit_vector_remove_at() and ckit_vector_insert_at()
//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_vector_base(vector) ((CKIT_VectorHeader*)(((u8*)vector) - sizeof(CKIT_VectorHeader)))
#define ckit_vector_count(vector) (*ckit_vector_base(vector)).count
#define ckit_vector_capacity(vector) (*ckit_vector_base(vector)).capacity
#define ckit_vector_push(vector, element) vector = ckit_vector_grow(vector, sizeof(element), FALSE, __FILE__, __LINE__, __func__); vector[ckit_vector_base(vector)->count++] = element
#define ckit_vector_reserve(capactiy, type) (type*)MACRO_ckit_vector_reserve(sizeof(type), capactiy, __FILE__, __LINE__, __func__)
#define ckit_vector_pop(vector) vector[--ckit_vector_base(vector)->count]
#define ckit_vector_remove_at(vector, index) ckit_memory_delete_index(vector, ckit_vector_count(vector), ckit_vector_capacity(vector), index); ckit_vector_base(vector)->count--
#define ckit_vector_insert_at(vector, element, index) ckit_memory_insert_index(vector, ckit_vector_count(vector), ckit_vector_capacity(vector), element, index); ckit_vector_base(vector)->count++
#define ckit_vector_free(vector) vector = MACRO_ckit_vector_free(vector)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

/*===========================================================
 * File: ckit_hashmap.h
 * Date: May 14, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

#define CKIT_HASHMAP_DEFAULT_LOAD_FACTOR 0.75
//========================== Begin Types ==========================

typedef struct CKIT_HashMapEntry {
  	String key;
	void* value;
} CKIT_HashMapEntry;

typedef struct CKIT_HashMap {
	u32 capacity;
	u32 count;
	CKIT_HashMapEntry* entries; // not a vector
	size_t element_size;
	Boolean is_pointer_type;
} CKIT_HashMap;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
u32 ckit_hash_value(char *str);
CKIT_HashMap* MACRO_ckit_hashmap_create(u32 hashmap_capacity, size_t element_size, Boolean is_pointer_type);
CKIT_HashMap* MACRO_ckit_hashmap_free(CKIT_HashMap* hashmap);

// Returns previous value
void* ckit_hashmap_put(CKIT_HashMap* hashmap, char* key, void* value);
void* ckit_hashmap_get(CKIT_HashMap* hashmap, char* key);
Boolean ckit_hashmap_has(CKIT_HashMap* hashmap, char* key);

#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_hashmap_create(capacity, type, is_pointer_type) MACRO_ckit_hashmap_create(capacity, sizeof(type), is_pointer_type)
#define ckit_hashmap_free(hashmap) hashmap = MACRO_ckit_hashmap_free(hashmap)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
/*===========================================================
 * File: ckit_hashset.h
 * Date: May 14, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

#define CKIT_HASHSET_DEFAULT_LOAD_FACTOR 0.75
//========================== Begin Types ==========================

typedef struct CKIT_HashSet {
	// CKIT_HashFunction* hash_func;
	u32 capacity;
	u32 count;
	char** entries; // not a vector
} CKIT_HashSet;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
u32 ckit_hash_value(char *str);
CKIT_HashSet* MACRO_ckit_hashset_create(u32 hashset_capacity);
CKIT_HashSet* MACRO_ckit_hashset_free(CKIT_HashSet* hashset);

// Returns previous value
void ckit_hashset_put(CKIT_HashSet* hashset, char* key);
Boolean ckit_hashset_has(CKIT_HashSet* hashset, char* key);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_hashset_create(capacity) MACRO_ckit_hashset_create(capacity)
#define ckit_hashset_free(hashset) hashset = MACRO_ckit_hashset_free(hashset)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

//========================== Begin Types ==========================
typedef struct CKIT_Node {
    struct CKIT_Node* next;
    struct CKIT_Node* prev;
    size_t element_size_in_bytes;
    void* data;
} CKIT_Node;

typedef struct CKIT_LinkedList {
    CKIT_Node* head;
    CKIT_Node* tail;
    size_t element_size_in_bytes;
    u32 count;
    Boolean is_pointer_type;
} CKIT_LinkedList;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    CKIT_LinkedList* MACRO_ckit_linked_list_create(size_t element_size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function);
    CKIT_Node* ckit_linked_list_insert(CKIT_LinkedList* linked_list, u32 index, void* data);
    CKIT_Node* ckit_linked_list_get_node(CKIT_LinkedList* linked_list, u32 index);
    void* ckit_linked_list_get(CKIT_LinkedList* linked_list, u32 index);
    void* ckit_linked_list_head(CKIT_LinkedList* linked_list);
    void* ckit_linked_list_tail(CKIT_LinkedList* linked_list);
    CKIT_Node* ckit_linked_list_push(CKIT_LinkedList* linked_list, void* data);
    CKIT_Node ckit_linked_list_pop(CKIT_LinkedList* linked_list);
    CKIT_Node ckit_linked_list_remove(CKIT_LinkedList* linked_list, u32 index);
    void* MACRO_ckit_linked_list_free(CKIT_LinkedList* linked_list);
    u32 ckit_linked_list_node_to_index(CKIT_LinkedList* linked_list, CKIT_Node* address);
    void* ckit_linked_list_peek_tail(CKIT_LinkedList* linked_list);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_linked_list_create(type, is_pointer_type) MACRO_ckit_linked_list_create(sizeof(type), is_pointer_type, __FILE__, __LINE__, __func__)
#define ckit_linked_list_free(linked_list) linked_list = MACRO_ckit_linked_list_free(linked_list)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
// used a circular buffer

// read, next_read. and write pointers

// if the write pointer is greater than the next_read pointer then assert
// if read pointer is on next read pointer and you try to read again assert because you will be ahead of the write pointer

//========================== Begin Types ==========================
typedef struct CKIT_Queue {
	u32 capacity;
	u32 count;
	void* data;
	size_t element_size_in_bytes;

	u32 read_index;
	Boolean has_next_to_read;
	u32 write_index;
	Boolean is_pointer_type;
} CKIT_Queue;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Queue* MACRO_ckit_queue_create(u32 inital_capacity, size_t element_size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function);
	CKIT_Queue* MACRO_ckit_queue_free(CKIT_Queue* queue);
	void ckit_enqueue(CKIT_Queue* queue, void* element);
	/*
	 You are responsible for freeing the result, if not a pointer type, I think arenas is gonna be pretty big for this whole thing.
	*/
	void* ckit_dequeue(CKIT_Queue* queue);
	u32 ckit_queue_capacity(CKIT_Queue* queue);
	u32 ckit_queue_count(CKIT_Queue* queue);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_queue_create(inital_capacity, type, is_pointer_type) MACRO_ckit_queue_create(inital_capacity, sizeof(type), is_pointer_type, __FILE__, __LINE__, __func__);
#define ckit_queue_free(queue) MACRO_ckit_queue_free(queue);
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

//========================== Begin Types ==========================
typedef struct CKIT_Stack CKIT_Stack;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Stack* MACRO_ckit_stack_create(size_t size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function);
	void* ckit_stack_push(CKIT_Stack* stack, void* data);
	// Date: July 22, 2024
	// NOTE(Jovanni): If pointer type is true you must free this yourself, trivially this should be able to go into an arena.
	void* ckit_stack_pop(CKIT_Stack* stack);
	void* MACRO_ckit_stack_free(CKIT_Stack* stack);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_stack_create(type, is_pointer_type) MACRO_ckit_stack_create(sizeof(type), is_pointer_type, __FILE__, __LINE__, __func__)
#define ckit_stack_free(stack) MACRO_ckit_stack_free(stack)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
// Utilities

typedef struct FileSystem {
	String file_name;
	FILE* handle;
	u8* data;
	size_t file_size;
	Boolean reachedEOF;
} FileSystem;

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	FileSystem file_system_create(String file_name);
	void file_open(FileSystem* file_system);
	void file_close(FileSystem* file_system);
	size_t file_size(FileSystem* file_system);
	String file_get_next_line(FileSystem* file_system);
	char file_get_next_char(FileSystem* file_system);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//========================== Begin Types ==========================
typedef CKIT_Vector4 CKIT_Color;

typedef struct CKIT_Bitmap {
	u32 height;
	u32 width;
	u8 bytes_per_pixel;
	u8* memory;
} CKIT_Bitmap;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	u32 ckit_color_to_u32(CKIT_Color color);
	CKIT_Color ckit_color_from_u32(u32 color);
	CKIT_Color ckit_color_alpha_blend(CKIT_Color front_color, CKIT_Color back_color);
	CKIT_Color ckit_color_u32_alpha_blend(u32 front_color_u32, u32 back_color_u32);

	/**
	 * @brief value from 0.0 to 1.0
	 * 
	 * @param color 
	 * @param value 
	 * @return CKIT_Color 
	 */
	CKIT_Color ckit_color_multiply(CKIT_Color color, float value);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
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
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

// Parser_and_Lexer

//========================== Begin Types ==========================
#pragma pack(push, 1)
typedef struct BmpHeader { 	// total 40 bytes
	char signature[2]; 			// 2 bytes
	u32 file_size; 				// 4 bytes
	u32 reserved;  				// 4 bytes
	u32 data_offset; 			// 4 bytes

	u32 size;					// 4 bytes
	u32 width;					// 4 bytes
	u32 height;					// 4 bytes
	u16 planes;					// 2 bytes
	u16 bits_per_pixel; 		// 2 bytes
	u32 compression;			// 4 bytes
	u32 compressed_image_size;	// 4 bytes
	u32 x_pixels_per_meter; 	// 4 bytes // horizontal resolution: Pixels/meter
	u32 y_pixels_per_meter; 	// 4 bytes // vertical resolution: Pixels/meter
	u32 colors_used;            // 4 bytes // Number of actually used colors. For a 8-bit / pixel bitmap this will be 100h or 256.
	u32 important_colors;		// 4 bytes // Number of important colors
} BmpHeader;
#pragma pack(pop)
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Bitmap ckit_parser_load_bmp(u8* bmp_file_data, size_t file_size);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************
#ifdef __cplusplus
extern "C" {
#endif
  void ckit_init();
  void ckit_cleanup();
#ifdef __cplusplus
}
#endif