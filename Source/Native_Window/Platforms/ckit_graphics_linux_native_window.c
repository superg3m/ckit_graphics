/*
#include <X11/Xlib.h>
#include <stdlib.h>

int main() {
	Display *display = XOpenDisplay(NULL);
	if (!display) return 1;

	int screen = DefaultScreen(display);
	Window root = RootWindow(display, screen);
	Window win = XCreateSimpleWindow(display, root, 0, 0, 800, 600, 1, BlackPixel(display, screen), WhitePixel(display, screen));
	XMapWindow(display, win);

	// Bitmap memory setup
	int width = 800;
	int height = 600;
	XImage *ximage = XCreateImage(display, DefaultVisual(display, screen), DefaultDepth(display, screen),
								ZPixmap, 0, malloc(width * height * 4), width, height, 32, 0);

	// Write to the bitmap
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			XPutPixel(ximage, x, y, (x ^ y) & 0xff);  // Simple pattern
		}
	}

	// Display the image
	GC gc = XCreateGC(display, win, 0, NULL);
	XPutImage(display, win, gc, ximage, 0, 0, 0, 0, width, height);
	XFlush(display);

	// Event loop to keep the window open
	XEvent event;
	while (1) {
		XNextEvent(display, &event);
		if (event.type == Expose) {
			XPutImage(display, win, gc, ximage, 0, 0, 0, 0, width, height);
		}
	}

	XDestroyImage(ximage);
	XFreeGC(display, gc);
	XDestroyWindow(display, win);
	XCloseDisplay(display);
	return 0;
}
*/
#include "../../../Include/Native_Window/ckit_graphics_native_window.h"

CKIT_Window* ckit_window_create(u32 width, u32 height, const char* name) {
	CKIT_Window* ret_window = ckit_alloc(sizeof(CKIT_Window));

	Display* display = XOpenDisplay(NULL);
	if (!display) {
		return 1;
	}

	int screen = DefaultScreen(display);
	Window root = RootWindow(display, screen);
	Window win = XCreateSimpleWindow(display, root, 0, 0, 800, 600, 1, BlackPixel(display, screen), WhitePixel(display, screen));
	XMapWindow(display, win);

	// Bitmap memory setup
	CKIT_Bitmap* bitmap = sizeof(CKIT_Bitmap);
	bitmap->bytes_per_pixel = 4;
	bitmap->memory = XCreateImage(display, DefaultVisual(display, screen), DefaultDepth(display, screen),
								ZPixmap, 0, ckit_alloc(width * height * bytes_per_pixel), width, height, 32, 0);

	// Write to the bitmap
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			XPutPixel(bitmap->memory, x, y, (x ^ y) & 0xff);  // Simple pattern
		}
	}

	// Display the image
	bitmap->gc = XCreateGC(display, win, 0, NULL);
	XPutImage(display, win, bitmap->gc, bitmap->memory, 0, 0, 0, 0, width, height);
	XFlush(display);

	ret_window->x11_display = display;
	ret_window->x11_window = win;
	ret_window->bitmap = bitmap;
}

void* MACRO_ckit_window_free(CKIT_Window* window) {
	//XDestroyImage(window->bitmap->memory);
	XDestroyImage(window->bitmap->memory);
	XFreeGC(window->x11_display, window->x11_gc);
	XDestroyWindow(window->x11_display, window->x11_window);
	XCloseDisplay(window->x11_display);
}

void ckit_window_bind_icon(const char* resource_path) {
	ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");
}

void ckit_window_bind_cursor(const char* resource_path) {
	ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");
}

Boolean ckit_window_should_quit(CKIT_Window* window) {
	XNextEvent(display, &event);
	if (event.type == Expose) {
		XPutImage(window->x11_display, window->x11_window, window->bitmap->x11_gc, window->bitmap->x11_ximage, 0, 0, 0, 0, window->bitmap->x11_width, window->bitmap->x11_height);
	}
}

void ckit_window_clear_color(CKIT_Window* window, CKIT_Color color) {
	// Write to the bitmap
	for (int y = 0; y < window->memory->bitmap->height; ++y) {
		for (int x = 0; x < window->memory->bitmap->width; ++x) {
			XPutPixel(bitmap->memory, x, y, ckit_color_to_u32(color));  // might have to endian swap the color
		}
	}
}

void ckit_window_draw_quad(CKIT_Window* window, s32 start_x, s32 start_y, u32 width, u32 height, CKIT_Color color) {
	const u32 VIEWPORT_WIDTH = window->bitmap->width;
	const u32 VIEWPORT_HEIGHT = window->bitmap->height;

	u32 left = CLAMP(start_x, 0, VIEWPORT_WIDTH);
	u32 right = CLAMP(start_x + width, 0, VIEWPORT_WIDTH);

	u32 bottom = CLAMP(start_y, 0, VIEWPORT_HEIGHT);
	u32 top = CLAMP(start_y + height, 0, VIEWPORT_HEIGHT);

	u32 true_quad_width = right - left;
	u32 true_quad_height = top - bottom;

	Boolean should_draw = (true_quad_width != 0) && (true_quad_height != 0);
	if (!should_draw) {
		return;
	}

	for (u32 y = 0; y < true_quad_height; y++) {
		for (u32 x = 0; x < true_quad_width; x++) {
			XPutPixel(bitmap->memory, x, y, ckit_color_to_u32(color));
		}
	}
}

void ckit_window_draw_bitmap(CKIT_Window* window) {
	ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");
}

void ckit_window_swap_buffers(CKIT_Window* window) {
	ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");
}