#include "./ckit_types.h"

//************************** Start Functions **************************
extern void* ckg_alloc(size_t allocation_size);
extern void* ckg_realloc(void* data, size_t old_allocation_size, size_t new_allocation_size);
extern void* MACRO_ckg_free(void* data);

#define ckg_free(data) data = MACRO_ckg_free(data);
extern void ckg_stack_trace_dump();
extern void MACRO_ckg_log_output(CKG_LogLevel log_level, const char* message, ...);

extern void* MACRO_ckit_alloc(size_t number_of_bytes, CKIT_MemoryTagID tag_id, const char* file, const u32 line, const char* function);
extern void* MACRO_ckit_realloc(void* data, u64 new_allocation_size, const char* file, const u32 line, const char* function);
extern void* MACRO_ckit_free(void* data);
extern void MACRO_ckit_memory_delete_index(void* data, u32 number_of_elements, u32 data_capacity, size_t element_size_in_bytes, u32 index);
extern void MACRO_ckit_memory_insert_index(void* data, u32 number_of_elements, u32 data_capacity, size_t element_size_in_bytes, u32 index);

extern CKIT_Rectangle2D ckit_rectangle2d_create(s32 x, s32 y, u32 width, u32 height);
extern u8* ckit_os_read_entire_file(const char* path, size_t* returned_file_size);
extern CKIT_Bitmap ckit_graphics_load_bmp(u8* bmp_file_data, size_t file_size);
extern u32 ckit_color_to_u32(CKIT_Color color);
extern CKIT_Color ckit_color_from_u32(u32 color);
extern CKIT_Color ckit_color_alpha_blend(CKIT_Color front_color, CKIT_Color back_color);
extern CKIT_Color ckit_color_u32_alpha_blend(u32 front_color_u32, u32 back_color_u32);

extern void MACRO_ckit_log_output(CKIT_LogLevel log_level, const char* message, ...);

extern void* ckit_vector_grow(void* vector, size_t element_size, Boolean force_grow, const char* file, const u32 line, const char* function);
extern void* MACRO_ckit_vector_reserve(size_t element_size, u32 capacity, const char* file, const u32 line, const char* function);
extern void* MACRO_ckit_vector_free(void* vector);

extern double ckit_os_query_performance_counter();
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

//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckg_log_output(log_level, message, ...) MACRO_ckg_log_output(log_level, message, ##__VA_ARGS__)
#define CKG_LOG_PRINT(message, ...) ckg_log_output(CKG_LOG_LEVEL_PRINT, message, ##__VA_ARGS__)
#define CKG_LOG_SUCCESS(message, ...) ckg_log_output(CKG_LOG_LEVEL_SUCCESS, message, ##__VA_ARGS__)
#define CKG_LOG_DEBUG(message, ...) ckg_log_output(CKG_LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#define CKG_LOG_WARN(message, ...) ckg_log_output(CKG_LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#define CKG_LOG_ERROR(message, ...) ckg_log_output(CKG_LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#define CKG_LOG_FATAL(message, ...) ckg_log_output(CKG_LOG_LEVEL_FATAL, message, ##__VA_ARGS__)

#define CKG_ASSERT_ENABLED TRUE

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

#define ckit_vector_base(vector) ((CKIT_VectorHeader*)(((u8*)vector) - sizeof(CKIT_VectorHeader)))
#define ckit_vector_count(vector) (*ckit_vector_base(vector)).count
#define ckit_vector_capacity(vector) (*ckit_vector_base(vector)).capacity
#define ckit_vector_push(vector, element) vector = ckit_vector_grow(vector, sizeof(element), FALSE, __FILE__, __LINE__, __func__); vector[ckit_vector_base(vector)->count++] = element
#define ckit_vector_reserve(capactiy, type) (type*)MACRO_ckit_vector_reserve(sizeof(type), capactiy, __FILE__, __LINE__, __func__)
#define ckit_vector_pop(vector) vector[--ckit_vector_base(vector)->count]
#define ckit_vector_remove_at(vector, index) ckit_memory_delete_index(vector, ckit_vector_count(vector), ckit_vector_capacity(vector), index); ckit_vector_base(vector)->count--
#define ckit_vector_insert_at(vector, element, index) ckit_memory_insert_index(vector, ckit_vector_count(vector), ckit_vector_capacity(vector), element, index); ckit_vector_base(vector)->count++
#define ckit_vector_free(vector) vector = MACRO_ckit_vector_free(vector)


#define ckit_alloc(number_of_bytes) MACRO_ckit_alloc(number_of_bytes, TAG_USER_UNKNOWN, __FILE__, __LINE__, __func__)
#define ckit_alloc_custom(number_of_bytes, tag_id) MACRO_ckit_alloc(number_of_bytes, tag_id, __FILE__, __LINE__, __func__)
#define ckit_realloc(data, new_allocation_size) MACRO_ckit_realloc(data, new_allocation_size, __FILE__, __LINE__, __func__)
#define ckit_free(data) data = MACRO_ckit_free(data)
#define ckit_memory_delete_index(data, number_of_elements, data_capacity, index) MACRO_ckit_memory_delete_index(data, number_of_elements, data_capacity, sizeof(data[0]), index)
#define ckit_memory_insert_index(data, number_of_elements, data_capacity, element, index) MACRO_ckit_memory_delete_index(data, number_of_elements, data_capacity, sizeof(data[0]), index); data[index] = element;
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++