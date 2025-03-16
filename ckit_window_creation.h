#include "./ckit/ckit.h"

#if defined(CKIT_GRAPHICS_EXTERN)
    #define CKIT_GRAPHICS_API extern
#else
    #define CKIT_GRAPHICS_API
#endif

typedef struct CKIT_Window {
    HINSTANCE instance_handle;
    HWND handle;
    HDC	hdc;
    const char* name;
    CKIT_Bitmap back_buffer;
    BITMAPINFO back_buffer_bitmap_info;
    u16 height;
    u16 width;
} CKIT_Window;


typedef struct CKIT_Bitmap {
    u32 height;
    u32 width;
    u8 bytes_per_pixel;
    u8* memory;
} CKIT_Bitmap;

typedef u8 CKIT_CursorState;
#define CKIT_CURSOR_STATE_DISABLED 0
#define CKIT_CURSOR_STATE_ENABLED 1
#define CKIT_CURSOR_STATE_POINTER 2
#define CKIT_CURSOR_STATE_GRAB 3

/**
 * @brief Must be freed with ckit_window_free()
 * 
 * @param width 
 * @param height 
 * @param name 
 * @return CKIT_Window* 
 */
CKIT_GRAPHICS_API CKIT_Window* ckit_window_create(u32 width, u32 height, const char* name);
CKIT_GRAPHICS_API void* MACRO_ckit_window_free(CKIT_Window* window);

CKIT_GRAPHICS_API void ckit_window_bind_icon(const char* resource_path);
CKIT_GRAPHICS_API void ckit_window_bind_cursor(const char* resource_path);

CKIT_GRAPHICS_API Boolean ckit_window_should_quit(CKIT_Window* window);
CKIT_GRAPHICS_API void ckit_window_clear_color(CKIT_Window* window, CKIT_Color color);
CKIT_GRAPHICS_API void ckit_window_draw_quad(CKIT_Window* window, CKIT_Rectangle2D quad, CKIT_Color color);
CKIT_GRAPHICS_API void ckit_window_draw_line(CKIT_Window* window, CKIT_Vector3 p1, CKIT_Vector3 p2);
CKIT_GRAPHICS_API void ckit_window_draw_circle(CKIT_Window* window, s32 start_x, s32 start_y, s32 radius, Boolean is_filled, CKIT_Color color);
CKIT_GRAPHICS_API void ckit_window_draw_bitmap(CKIT_Window* window, s32 start_x, s32 start_y, u32 scale_factor, CKIT_Bitmap bitmap);
CKIT_GRAPHICS_API void ckit_window_swap_buffers(CKIT_Window* window);
CKIT_GRAPHICS_API void ckit_window_get_client_mouse_position(CKIT_Window* window, s32* mouse_x, s32* mouse_y);
CKIT_GRAPHICS_API void ckit_window_set_cursor_state(CKIT_Window* window, CKIT_CursorState cursor_state);

#define ckit_window_draw_quad_custom(window, start_x, start_y, width, height, color) ckit_window_draw_quad(window, ckit_rectangle2d_create(start_x, start_y, width, height), color)
#define ckit_window_free(window) window = MACRO_ckit_window_free(window);

// only realloc the bitmap back buffer if the previous one is too small!