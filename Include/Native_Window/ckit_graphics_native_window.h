#pragma once

#include "ckit_extern.h"
//========================== Begin Types ==========================
typedef enum CKIT_CursorState {
	DISABLED,
	ENABLED,
	POINTER,
	GRAB,
} CKIT_CursorState;

#if defined(PLATFORM_WINDOWS)
	#include <windows.h>

	typedef struct CKIT_Window {
		HINSTANCE instance_handle;
		HWND handle;
		HDC	hdc;
		u16 height;
		u16 width;
		const char* name;
		BITMAPINFO bitmap_info;
		CKIT_Bitmap bitmap;
	} CKIT_Window;
#elif defined(PLATFORM_LINUX)
	#include <x11/xlib.h>
	// https://www.youtube.com/watch?v=d2E7ryHCK08
	// https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html
	// https://www.youtube.com/watch?v=u2F_Lif4KGA&list=PLyxjkYF62ii8ZgaRBPlj9nVIoOeIaszw-
	// https://www.youtube.com/watch?v=qZmJwk2xrJ0
	
	typedef struct CKIT_Window {
		u16 height;
		u16 width;
		Display* x11_display;
		Window* x11_window;
		XImage* memory;
		GC x11_gc;
	} CKIT_Window;
#elif defined(PLATFORM_DARWIN)
	typedef struct CKIT_Window {
		u16 height;
		u16 width;
		const char* name;
		CKIT_Bitmap bitmap;
	} CKIT_Window;
#endif
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	/**
	 * @brief Must be freed with ckit_window_free()
	 * 
	 * @param width 
	 * @param height 
	 * @param name 
	 * @return CKIT_Window* 
	 */
	CKIT_Window* ckit_window_create(u32 width, u32 height, const char* name);
	void* MACRO_ckit_window_free(CKIT_Window* window);
	void ckit_window_bind_icon(const char* resource_path);
	void ckit_window_bind_cursor(const char* resource_path);
	Boolean ckit_window_should_quit(CKIT_Window* window);
	void ckit_window_clear_color(CKIT_Window* window, CKIT_Color color);
	void ckit_window_draw_quad(CKIT_Window* window, CKIT_Rectangle2D quad, CKIT_Color color);
	void ckit_window_draw_line(CKIT_Window* window, CKIT_Vector3 p1, CKIT_Vector3 p2);
	void ckit_window_draw_circle(CKIT_Window* window, s32 start_x, s32 start_y, s32 radius, Boolean is_filled, CKIT_Color color);
	void ckit_window_draw_bitmap(CKIT_Window* window, s32 start_x, s32 start_y, u32 scale_factor, CKIT_Bitmap bitmap);
	void ckit_window_swap_buffers(CKIT_Window* window);
	void ckit_window_get_mouse_position(CKIT_Window* window, s32* mouse_x, s32* mouse_y);
	void ckit_window_set_cursor_state(CKIT_Window* window, CKIT_CursorState cursor_state);
#ifdef __cplusplus
}
#endif
//************************* End Functions *************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_window_draw_quad_custom(window, start_x, start_y, width, height, color) ckit_window_draw_quad(window, ckit_rectangle_create(start_x, start_y, width, height), color)
#define ckit_window_free(window) window = MACRO_ckit_window_free(window);
#define ckit_window_free(window) window = MACRO_ckit_window_free(window);
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

