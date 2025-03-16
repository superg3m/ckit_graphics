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
CKIT_GRAPHICS_API void ckit_window_swap_buffers(CKIT_Window* window);
CKIT_GRAPHICS_API void ckit_window_get_client_mouse_position(CKIT_Window* window, s32* mouse_x, s32* mouse_y);
CKIT_GRAPHICS_API void ckit_window_set_cursor_state(CKIT_Window* window, CKIT_CursorState cursor_state);

#define ckit_window_free(window) window = MACRO_ckit_window_free(window);

// Callback events to handle
// Drag and Drop files callback
// I should know if I need a framebuffer or not?
// SO something like:
// ckit_window_create(width, height, "Space Invaders", CKIT_WINDOW_BACKEND_SOFTWARE) // Allocate a framebuffer!
// ckit_window_create(width, height, "Space Invaders", CKIT_WINDOW_BACKEND_OPENGL) // Don't allocate framebuffer set up opengl context!