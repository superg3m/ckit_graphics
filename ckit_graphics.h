#pragma once

#if defined(CKIT_GRAPHICS_EXTERN)
    #define CKIT_GRAPHICS_API extern
#else
    #define CKIT_GRAPHICS_API
#endif

#define CKIT_EXTERN
#include "./ckit/ckit.h"

#if defined(CKIT_GRAPHICS_IMPL)
    #define CKIT_GRAPHICS_IMPL_NATIVE_WINDOW
    #define CKIT_GRAPHICS_IMPL_RENDERER
    // Date: September 25, 2024
    // TODO(Jovanni): CKIT_GRAPHICS_IMPL_GRAPH
    // TODO(Jovanni): CKIT_GRAPHICS_IMPL_IMGUI
#endif

#define CKIT_GRAPHICS_INCLUDE_NATIVE_WINDOW
#define CKIT_GRAPHICS_INCLUDE_RENDERER

#if defined(CKIT_GRAPHICS_INCLUDE_NATIVE_WINDOW)
    typedef u8 CKIT_CursorState;
    #define CKIT_CURSOR_STATE_DISABLED 0
    #define CKIT_CURSOR_STATE_ENABLED 1
    #define CKIT_CURSOR_STATE_POINTER 2
    #define CKIT_CURSOR_STATE_GRAB 3

    #if defined(PLATFORM_WINDOWS)
        typedef struct CKIT_Window {
            HINSTANCE instance_handle;
            HWND handle;
            HDC	hdc;
            const char* name;
            CKIT_Bitmap bitmap;
            BITMAPINFO bitmap_info;
            u16 height;
            u16 width;
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
#endif

#if defined(CKIT_GRAPHICS_INCLUDE_RENDERER)
    #define CKSGL_DEFAULT_BYTE_PER_PIXEL 4

    typedef struct CKSGL {
        u8** framebuffer;
        u16* width; 
        u16* height;
    } CKSGL;

    CKIT_GRAPHICS_API CKSGL cksgl_create(u8** framebuffer, u16* framebuffer_width, u16* framebuffer_height);
    CKIT_GRAPHICS_API void cksgl_draw_pixel(CKSGL inst, s32 x, s32 y, CKIT_Color color);
    CKIT_GRAPHICS_API void cksgl_draw_quad(CKSGL inst, CKIT_Rectangle2D quad, CKIT_Color color);
    CKIT_GRAPHICS_API void cksgl_draw_line(CKSGL inst, CKIT_Vector3 p0, CKIT_Vector3 p1, CKIT_Color color);
    CKIT_GRAPHICS_API void cksgl_draw_triangle(CKSGL inst, CKIT_Vector3 p0, CKIT_Vector3 p1, CKIT_Vector3 p2, Boolean is_filled, CKIT_Color color);
    CKIT_GRAPHICS_API void cksgl_draw_circle(CKSGL inst, s32 start_x, s32 start_y, s32 radius, Boolean is_filled, CKIT_Color color);
    CKIT_GRAPHICS_API void cksgl_draw_bitmap(CKSGL inst, s32 start_x, s32 start_y, u32 scale_factor, CKIT_Bitmap bitmap);
    CKIT_GRAPHICS_API void cksgl_clear_color(CKSGL inst, CKIT_Color color);
    CKIT_GRAPHICS_API void cksgl_flip_vertically(CKSGL inst);

    #define cksgl_draw_quad_custom(inst, start_x, start_y, width, height, color) cksgl_draw_quad(inst, ckit_rectangle2d_create(start_x, start_y, width, height), color)


    //============================== OpenGL ==============================

    #include "./External_Libraries/glad/include/glad/glad.h"
    #include "./External_Libraries/stb_image.h"

    #define TEXTURE_MAX 32
    typedef enum CKIT_ShaderType{
        CKIT_VERTEX_SHADER,
        CKIT_FRAGMENT_SHADER
    } CKIT_ShaderType;

    typedef struct CKIT_ShaderDescriptor {
        CKIT_ShaderType type;
        const char* path; 
    } CKIT_ShaderDescriptor;

    typedef struct CKIT_Shader {
        u32* textures;
        u32 id;
    } CKIT_Shader;

    CKIT_GRAPHICS_API void ckit_shader_check_compile_erros(u32 shaderID, const char* type);
    CKIT_GRAPHICS_API CKIT_Shader ckit_shader_create(CKIT_ShaderDescriptor* shader_descriptor, u32 shader_descriptor_count);
    CKIT_GRAPHICS_API void ckit_shader_add_texture(CKIT_Shader* shader, const char* texture_path);
    CKIT_GRAPHICS_API void ckit_shader_use(CKIT_Shader* shader);
    CKIT_GRAPHICS_API void ckit_shader_bind_textures(CKIT_Shader* shader);
    CKIT_GRAPHICS_API void ckit_wgl_context_create(HDC dc_handle);
#endif

//
// ===================================================== CKIT_GRAPHICS_IMPL =====================================================
//

#if defined(CKIT_GRAPHICS_IMPL_NATIVE_WINDOW)
    #if defined(PLATFORM_WINDOWS)
        typedef struct CKIT_WindowEntry {
            HWND WINAPI_handle;
            CKIT_Window* ckit_window;
        } CKIT_WindowEntry;

        internal HICON icon_handle = NULLPTR;
        internal HCURSOR cursor_handle = NULLPTR;
        internal Boolean interacting_with_left_menu = FALSE;
        internal CKIT_WindowEntry* registered_windows = NULLPTR;

        internal CKIT_Window* find_ckit_window_by_handle(HWND handle) {
            if (!registered_windows) {
                return NULLPTR;
            }

            for (u32 i = 0; i < ckit_vector_count(registered_windows); i++) {
                if (registered_windows[i].WINAPI_handle == handle) {
                    return registered_windows[i].ckit_window;
                }
            }

            ckit_assert(FALSE); // SHOULD NEVER GET HERE!
            return NULLPTR;
        }

        // Date: March 16, 2025
        // NOTE(Jovanni): 
        // Yeah not sure if this is correct tbh.
        // I try not to reallocate if the previous bitmap size is big enough.
        internal void ckit_window_resize(CKIT_Window* window) {
            if (!window) {
                return;
            }

            // Date: March 16, 2025
            // NOTE(Jovanni): Note sure if I have to release the hdc and get a new one every time I resize?
            if (window->hdc) {
                ReleaseDC(window->handle, window->hdc);
                window->hdc = NULLPTR;
            }
            window->hdc = GetDC(window->handle);

            RECT windowRect;
            GetWindowRect(window->handle, &windowRect);

            window->width = (u16)(windowRect.right - windowRect.left);
            window->height = (u16)(windowRect.bottom - windowRect.top);
            
            u32 bits_per_pixel = 32;
            u32 bytes_per_pixel = bits_per_pixel / 8;
            window->bitmap.bytes_per_pixel = bytes_per_pixel;

            RECT client_rect;
            GetClientRect(window->handle, &client_rect);
            size_t previous_memory_size = window->bitmap.bytes_per_pixel * window->bitmap.width * window->bitmap.height;
            size_t memory_size = window->bitmap.bytes_per_pixel * (u16)(client_rect.right - client_rect.left) * (u16)(client_rect.bottom - client_rect.top);
            if (memory_size > previous_memory_size) {
                window->bitmap.width = (u16)(client_rect.right - client_rect.left);
                window->bitmap.height = (u16)(client_rect.bottom - client_rect.top);
            }

            BITMAPINFO bitmap_info;
            bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bitmap_info.bmiHeader.biWidth = window->bitmap.width;
            bitmap_info.bmiHeader.biHeight = -((s32)window->bitmap.height);
            bitmap_info.bmiHeader.biPlanes = 1;
            bitmap_info.bmiHeader.biBitCount = bits_per_pixel;
            bitmap_info.bmiHeader.biCompression = BI_RGB;
            bitmap_info.bmiHeader.biSizeImage = 0;
            bitmap_info.bmiHeader.biXPelsPerMeter = 0;
            bitmap_info.bmiHeader.biYPelsPerMeter = 0;
            bitmap_info.bmiHeader.biClrUsed = 0;
            bitmap_info.bmiHeader.biClrImportant = 0;

            window->bitmap_info = bitmap_info;

            if (window->bitmap.memory && (memory_size != 0) && (memory_size > previous_memory_size)) {
                ckit_free(window->bitmap.memory);
            }

            if ((memory_size != 0) && (memory_size > previous_memory_size)) {
                window->bitmap.memory = ckit_alloc(memory_size);
            }
        }

        void ckit_window_swap_buffers(CKIT_Window* window) {
            if (window->bitmap.width == 0|| window->bitmap.height == 0) {
                return;
            }

            StretchDIBits(window->hdc, 
                            0, 0, window->bitmap.width, window->bitmap.height, 
                            0, 0, window->bitmap.width, window->bitmap.height,
                            window->bitmap.memory, &window->bitmap_info, DIB_RGB_COLORS, SRCCOPY);
        }

        LRESULT CALLBACK custom_window_procedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
            LRESULT result = 0;
            switch (message) {
                case WM_CREATE: {
                    LOG_SUCCESS("Window Created!\n");
                } break;

                case WM_SIZE: { // Resize
                    RECT client_rect;
                    GetClientRect(handle, &client_rect);
                    u32 width = client_rect.right - client_rect.left;
                    u32 height = client_rect.bottom - client_rect.top;

                    CKIT_Window* window = find_ckit_window_by_handle(handle);
                    ckit_window_resize(window);
                } break;

                case WM_CLOSE: {
                    LOG_SUCCESS("Window is Closed!\n");
                    PostQuitMessage(0);
                } break;

                case WM_DESTROY: {
                    PostQuitMessage(0);
                } break;

                case WM_NCLBUTTONDBLCLK: {
                    // Handle the double-click on the top-left icon
                    if (wParam == HTSYSMENU) {
                        interacting_with_left_menu = TRUE;
                    }

                    return DefWindowProcA(handle, message, wParam, lParam);
                } break;

                case WM_SYSCOMMAND: {
                    // Handle the double-click on the top-left icon
                    if ((wParam & 0xFFF0) == SC_MOUSEMENU) {
                        // you can full screen and minimize here like normal behaviour
                        return 0;
                    }
                    
                    if (((wParam & 0xFFF0) == SC_CLOSE) && interacting_with_left_menu) {
                        interacting_with_left_menu = FALSE;
                        return 0;
                    }

                    return DefWindowProcA(handle, message, wParam, lParam);
                } break;

                case WM_PAINT: { // Repaint window when its dirty
                    PAINTSTRUCT paint;
                    HDC hdc = BeginPaint(handle, &paint);

                    // SetPropA(handle, "", something_here)
                    // ckit_window_update_callback(handle);
                    // ckit_window_render_callback(handle);

                    EndPaint(handle, &paint);
                } break;
                
                default: {
                    result = DefWindowProcA(handle, message, wParam, lParam);
                } break;
            }

            return result;
        }

        void ckit_window_bind_icon(const char* resource_path) {
            ckit_assert(ckit_os_path_exists(resource_path));
            icon_handle = (HICON)LoadImageA(GetModuleHandle(NULL), resource_path, IMAGE_ICON, 0, 0, LR_LOADFROMFILE|LR_DEFAULTSIZE);
        }

        void ckit_window_bind_cursor(const char* resource_path) {
            ckit_assert(ckit_os_path_exists(resource_path));
            cursor_handle = (HCURSOR)LoadImageA(GetModuleHandle(NULL), resource_path, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE|LR_DEFAULTSIZE);
        }

        CKIT_Window* ckit_window_create(u32 width, u32 height, const char* name) {
            CKIT_Window* ret_window = ckit_alloc_custom(sizeof(CKIT_Window), TAG_CKIT_EXPECTED_USER_FREE);

            ret_window->instance_handle = GetModuleHandle(NULL);
            ret_window->width = width;
            ret_window->height = height;
            ret_window->name = name;

            WNDCLASSA window_class = {0};
            window_class.style = CS_HREDRAW|CS_VREDRAW;
            window_class.lpfnWndProc = custom_window_procedure;
            window_class.cbClsExtra = 0;
            window_class.cbWndExtra = 0;
            window_class.hInstance = ret_window->instance_handle;
            window_class.hIcon = icon_handle;
            window_class.hCursor = cursor_handle;
            window_class.hbrBackground = NULLPTR;
            window_class.lpszMenuName = NULLPTR;
            window_class.lpszClassName = name;

            RegisterClassA(&window_class);

            // Date: May 04, 2024
            // TODO(Jovanni): Extended Window Styles (look into them you can do cool stuff)
            // WS_EX_ACCEPTFILES 0x00000010L (The window accepts drag-drop files.)
            DWORD dwStyle = WS_OVERLAPPEDWINDOW|WS_VISIBLE;
            ret_window->handle = CreateWindowExA(0, name, name, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULLPTR, NULLPTR, ret_window->instance_handle, NULLPTR);
            ret_window->hdc = GetDC(ret_window->handle);

            ckit_window_resize(ret_window);

            CKIT_WindowEntry window_entry = {0};
            window_entry.WINAPI_handle = ret_window->handle;
            window_entry.ckit_window = ret_window; 

            ckit_vector_push(registered_windows, window_entry);
            return ret_window;
        }

        void* MACRO_ckit_window_free(CKIT_Window* window) {
            for (u32 i = 0; i < ckit_vector_count(registered_windows); i++) {
                if (window == registered_windows[i].ckit_window) {
                    ckit_vector_remove_at(registered_windows, i);
                }
            }

            if (ckit_vector_count(registered_windows) == 0) {
                ckit_vector_free(registered_windows);
            }

            ckit_free(window->bitmap.memory);
            ckit_free(window);

            return window;
        }

        Boolean ckit_window_should_quit(CKIT_Window* window) {
            MSG msg;
            while (PeekMessageA(&msg, NULLPTR, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    ReleaseDC(window->handle, window->hdc);
                    window->hdc = NULLPTR;
                    return TRUE;
                }

                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }

            return FALSE;
        }

        void ckit_window_get_client_mouse_position(CKIT_Window* window, s32* mouse_x, s32* mouse_y) {
            POINT point;
            ckit_os_get_mouse_position(mouse_x, mouse_y);
            point.x = *mouse_x;
            point.y = *mouse_y;
            ckit_assert(ScreenToClient(window->handle, &point));
            *mouse_x = point.x;
            *mouse_y = point.y;
        }

        void ckit_window_set_cursor_state(CKIT_Window* window, CKIT_CursorState cursor_state) {
            switch (cursor_state) {
                case CKIT_CURSOR_STATE_ENABLED: {
                    ShowCursor(TRUE);
                }

                case CKIT_CURSOR_STATE_DISABLED: {
                    ShowCursor(FALSE);
                }
            }
        }
    #elif defined(PLATFORM_LINUX)
        // #include "./Source/Native_Window/Platforms/ckit_graphics_linux_native_window.c"
    #elif defined(PLATFORM_APPLE)
        // #include "./Source/Native_Window/Platforms/ckit_graphics_apple_native_window.c"
    #endif
#endif

#if defined(CKIT_GRAPHICS_IMPL_RENDERER)
    CKSGL cksgl_create(u8** framebuffer, u16* framebuffer_width, u16* framebuffer_height) {
        CKSGL ret;
        ret.framebuffer = framebuffer;
        ret.width = framebuffer_width;
        ret.height = framebuffer_height;

        return ret;
    }

    void cksgl_draw_pixel(CKSGL inst, s32 x, s32 y, CKIT_Color color) {
        const u16 VIEWPORT_WIDTH = *inst.width;
        const u16 VIEWPORT_HEIGHT = *inst.height;
    
        if (x < 0 || x >= VIEWPORT_WIDTH || y < 0 || y >= VIEWPORT_HEIGHT) {
            return;
        }
    
        size_t final_pixel_index = x + (y * VIEWPORT_WIDTH);
        u32* dest = (u32*)(*inst.framebuffer);
    
        CKIT_Color new_back_buffer_color = ckit_color_u32_alpha_blend(dest[final_pixel_index], ckit_color_to_u32(color));
        dest[final_pixel_index] = ckit_color_to_u32(new_back_buffer_color);
    }
    

    void cksgl_draw_quad(CKSGL inst, CKIT_Rectangle2D quad, CKIT_Color color) {
        const u16 VIEWPORT_WIDTH = *inst.width;
        const u16 VIEWPORT_HEIGHT = *inst.height;

        s32 true_x = (s32)quad.position.x - (quad.width / 2); 
        s32 true_y = (s32)quad.position.y - (quad.height / 2); 

        u32 left = (u32)CLAMP(true_x, 0, VIEWPORT_WIDTH);
        u32 right = (u32)CLAMP(true_x + (s32)quad.width, 0, VIEWPORT_WIDTH);
        u32 top = (u32)CLAMP(true_y, 0, VIEWPORT_HEIGHT);
        u32 bottom = (u32)CLAMP(true_y + (s32)quad.height, 0, VIEWPORT_HEIGHT);

        u32* dest = (u32*)(*inst.framebuffer);

        for (u32 y = top; y < bottom; y++) {
            for (u32 x = left; x < right; x++) {
                cksgl_draw_pixel(inst, x, y, color);
            }
        }
    }

    internal void fswap(double* a, double* b) {
        double temp = *a;
        *a = *b;
        *b = temp;
    }

    internal void draw_line_vertical(CKSGL inst, CKIT_Vector3 p0, CKIT_Vector3 p1, CKIT_Color color) {
        if (p0.y > p1.y) {
            fswap(&p0.x, &p1.x);
            fswap(&p0.y, &p1.y);
        }
        
        int dx = (int)(p1.x - p0.x); 
        int dy = (int)(p1.y - p0.y); 

        int direction = dx < 0 ? -1 : 1;
        dx *= direction;

        if (dy == 0) {
            return;
        }

        int x = (int)p0.x;
        int p = (2 * dx) - dy;

        for (s32 i = 0; i < dy + 1; i++) {
            cksgl_draw_pixel(inst, x, (s32)p0.y + i, color);

            if (p >= 0) {
                x += direction;
                p = p - (2 * dy);
            }

            p = p + (2 * dx);
        }
    }

    internal void draw_line_horizontal(CKSGL inst, CKIT_Vector3 p0, CKIT_Vector3 p1, CKIT_Color color) {
        if (p0.x > p1.x) {
            fswap(&p0.x, &p1.x);
            fswap(&p0.y, &p1.y);
        }
        
        int dx = (int)(p1.x - p0.x); 
        int dy = (int)(p1.y - p0.y); 

        int direction = dy < 0 ? -1 : 1;
        dy *= direction;

        if (dx == 0) {
            return;
        }

        int y = (int)p0.y;
        int p = (2 * dy) - dx;

        for (s32 i = 0; i < dx + 1; i++) {
            cksgl_draw_pixel(inst, (s32)p0.x + i, y, color);

            if (p >= 0) {
                y += direction;
                p = p - (2 * dx);
            }

            p = p + (2 * dy);
        }
    }

    void cksgl_draw_line(CKSGL inst, CKIT_Vector3 p0, CKIT_Vector3 p1, CKIT_Color color) {
        if (fabs(p1.x - p0.x) > fabs(p1.y - p0.y)) {
            draw_line_horizontal(inst, p0, p1, color);
        } else {
            draw_line_vertical(inst, p0, p1, color);
        }
    }    

    void cksgl_draw_triangle(CKSGL inst, CKIT_Vector3 p0, CKIT_Vector3 p1, CKIT_Vector3 p2, Boolean is_filled, CKIT_Color color) {
        if (is_filled) {
            cksgl_draw_line(inst, p0, p1, color);
            cksgl_draw_line(inst, p1, p2, color);
            cksgl_draw_line(inst, p2, p0, color);
            // test if point is inside triangle
            // https://jtsorlinis.github.io/rendering-tutorial/
            // https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
        } else {
            cksgl_draw_line(inst, p0, p1, color);
            cksgl_draw_line(inst, p1, p2, color);
            cksgl_draw_line(inst, p2, p0, color);
        }
    }

    void cksgl_draw_bitmap(CKSGL inst, s32 start_x, s32 start_y, u32 scale_factor, CKIT_Bitmap bitmap) {
        const u16 VIEWPORT_WIDTH = *inst.width;
        const u16 VIEWPORT_HEIGHT = *inst.height;

        const s32 scaled_bmp_width = bitmap.width * scale_factor;
        const s32 scaled_bmp_height = bitmap.height * scale_factor;

        s32 true_x = start_x - (scaled_bmp_width / 2);
        s32 true_y = start_y - (scaled_bmp_height / 2);

        u32 left = (u32)CLAMP(true_x, 0, VIEWPORT_WIDTH);
        u32 right = (u32)CLAMP(true_x + scaled_bmp_width, 0, VIEWPORT_WIDTH);
        u32 top = (u32)CLAMP(true_y, 0, VIEWPORT_HEIGHT);
        u32 bottom = (u32)CLAMP(true_y + scaled_bmp_height, 0, VIEWPORT_HEIGHT);

        u32* dest = (u32*)(*inst.framebuffer);
        u32* bmp_memory = (u32*)bitmap.memory + ((bitmap.height - 1) * bitmap.width);

        // Date: August 31, 2024
        // TODO(Jovanni): SIMD for optimizations
        for (u32 y = top; y < bottom; y++) { 
            for (u32 x = left; x < right; x++) {
                const s64 bmp_x = (x - true_x) / scale_factor;
                const s64 bmp_y = (y - true_y) / scale_factor;

                s64 color_index = bmp_x - (bmp_y * bitmap.width);
                u32 color32 = bmp_memory[color_index];
                u8 alpha = (color32 >> 24);
                if (alpha == 0) {
                    continue;
                }

                cksgl_draw_pixel(inst, x, y, ckit_color_from_u32(color32));
            }
        }
    }

    internal Boolean is_pixel_inside_circle(s32 test_point_x, s32 test_point_y, s32 center_x, s32 center_y, u32 radius) {
        double dx = center_x - test_point_x;
        double dy = center_y - test_point_y;
        dx *= dx;
        dy *= dy;
        double distanceSquared = dx + dy;
        double radiusSquared = radius * radius;
        return distanceSquared < radiusSquared;
    }

    internal Boolean is_pixel_on_circle_line(s32 test_point_x, s32 test_point_y, s32 center_x, s32 center_y, u32 radius) {
        double dx = center_x - test_point_x;
        double dy = center_y - test_point_y;
        dx *= dx;
        dy *= dy;
        double distanceSquared = dx + dy;
        double radiusSquared = radius * radius;
        return distanceSquared == radiusSquared;
    }

    // Date: September 09, 2024
    // TODO(Jovanni): Switch to the midpoint circle algo because its just better
    // - https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
    // - https://noobtomaster.com/computer-graphics/circle-drawing-algorithms-midpoint-algorithm/
    // - https://www.thecrazyprogrammer.com/2016/12/bresenhams-midpoint-circle-algorithm-c-c.html
    // - https://www.youtube.com/watch?v=hpiILbMkF9w
    void cksgl_draw_circle(CKSGL inst, s32 start_x, s32 start_y, s32 radius, Boolean is_filled, CKIT_Color color) {
        if (radius <= 0) {
            return;
        }

        const u16 VIEWPORT_WIDTH = *inst.width;
        const u16 VIEWPORT_HEIGHT = *inst.height;

        const u32 diameter = radius * 2;

        s32 true_x = start_x - (radius);
        s32 true_y = start_y - (radius);

        u32 left = CLAMP(true_x, 0, VIEWPORT_WIDTH);
        u32 right = CLAMP(true_x + (s32)diameter, 0, VIEWPORT_WIDTH);
        u32 top = CLAMP(true_y, 0, VIEWPORT_HEIGHT);
        u32 bottom = CLAMP(true_y + (s32)diameter, 0, VIEWPORT_HEIGHT); // kyle wuz here skool sux

        u32* dest = (u32*)(*inst.framebuffer);

        if (is_filled) {
            for (s32 y = top; y < bottom; y++) {
                for (s32 x = left; x < right; x++) {
                    size_t final_pixel_index = x + (y * VIEWPORT_WIDTH);
                    u32 center_x = true_x + radius;
                    u32 center_y = true_y + radius;

                    if (is_pixel_inside_circle(x, y, center_x, center_y, radius)) {
                        cksgl_draw_pixel(inst, x, y, color);
                    }
                }
            }
        } else {
            ckit_assert_msg(FALSE, "Non-filled circle is not implemented yet!");
            for (s32 y = top; y < bottom; y++) {
                for (s32 x = left; x < right; x++) {
                    size_t final_pixel_index = x + (y * VIEWPORT_WIDTH);
                    u32 center_x = start_x + radius;
                    u32 center_y = start_y + radius;

                    if (is_pixel_on_circle_line(x, y, center_x, center_y, radius)) {
                        cksgl_draw_pixel(inst, x, y, color);
                    }
                }
            }
        }
    }

    /**
     * @brief This needs so much work... I don't understand why set pixel doesn't work here?
     * 
     * @param inst 
     */
    void cksgl_flip_vertically(CKSGL inst) {
        u16 width = *inst.width;
        u16 height = *inst.height;
        u32* pixels = (u32*)(*inst.framebuffer);
    
        for (s32 top_y = 0; top_y < height / 2; top_y++) {
            u32 bottom_y = (height - 1) - top_y;
            for (u32 x = 0; x < width; x++) {
                u32 index_top = x + (top_y * width);
                u32 index_bottom = x + (bottom_y * width);

                u32 pixel_top = pixels[index_top];
                u32 pixel_bottom = pixels[index_bottom];

                pixels[index_top] = pixel_bottom;
                pixels[index_bottom] = pixel_top;

                // cksgl_draw_pixel(inst, x, top_y, ckit_color_from_u32(pixel_bottom));
                // cksgl_draw_pixel(inst, x, bottom_y, ckit_color_from_u32(pixel_top));
            }
        }
    }

    void cksgl_clear_color(CKSGL inst, CKIT_Color color) {
        u16 width = *inst.width;
        u16 height = *inst.height;
        int stride = width * CKSGL_DEFAULT_BYTE_PER_PIXEL;
        u8* row = (u8*)(*inst.framebuffer); 

        for(u32 y = 0; y < height; y++) {
            u32* pixel = (u32*)row;
            for(u32 x = 0; x < width; x++) {
                *pixel++ = ckit_color_to_u32(color);
            }

            row += stride;
        }
    }

    //
    // ======================================================= OPENGL ===========================================================
    //

    void ckit_shader_check_compile_erros(u32 shaderID, const char* type) {
        int success;
        char info_log[1024];
        if (ckit_str_equal((const String)type, (const String)"PROGRAM")) {
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shaderID, 1024, NULL, info_log);
                LOG_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: %s\n", type);
                LOG_ERROR("%s -- --------------------------------------------------- --\n", info_log);
            }
        } else {
            glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shaderID, 1024, NULL, info_log);
                LOG_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: %s\n", type);
                LOG_ERROR("%s -- --------------------------------------------------- --\n", info_log);
            }
        }
    }

    // Date: August 15, 2024
    // NOTE(Jovanni): You might not even need this shader_discriptors
    CKIT_Shader ckit_shader_create(CKIT_ShaderDescriptor* shader_descriptor, u32 shader_descriptor_count) {
        CKIT_Shader ret = {0};
        u32* shader_source_ids = NULLPTR; 

        ret.id = glCreateProgram();
        for (int i = 0; i < shader_descriptor_count; i++) {
            size_t file_size = 0;
            const char* shader_source = ckit_os_read_entire_file(shader_descriptor[i].path, &file_size);
            u32 source_id;

            switch (shader_descriptor[i].type) {
                case CKIT_VERTEX_SHADER: {
                    source_id = glCreateShader(GL_VERTEX_SHADER);
                    glShaderSource(source_id, 1, &shader_source, NULL);
                    glCompileShader(source_id);
                    ckit_shader_check_compile_erros(source_id, "VERTEX");
                    glAttachShader(ret.id, source_id);
                } break;

                case CKIT_FRAGMENT_SHADER: {
                    source_id = glCreateShader(GL_FRAGMENT_SHADER);
                    glShaderSource(source_id, 1, &shader_source, NULL);
                    glCompileShader(source_id);
                    ckit_shader_check_compile_erros(source_id, "FRAGMENT");
                    glAttachShader(ret.id, source_id);
                } break;
            }

            ckit_vector_push(shader_source_ids, source_id);
        }
        glLinkProgram(ret.id);

        GLint success = FALSE;
        glGetProgramiv(ret.id, GL_LINK_STATUS, &success);
        if (!success) {
            char info_log[1028] = {0};
            glGetProgramInfoLog(ret.id, 512, NULL, info_log);
            LOG_ERROR("LINKING_FAILED\n");
        }

        for (int i = 0; i < ckit_vector_count(shader_source_ids); i++) {
            glDeleteShader(shader_source_ids[i]);
        }

        return ret;
    }

    void ckit_shader_add_texture(CKIT_Shader* shader, const char* texture_path) {
        ckit_assert_msg(ckit_vector_count(shader->textures) <= TEXTURE_MAX, "Texture max hit!");

        u32 texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        u8 *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            LOG_ERROR("Failed to load texture\n");
        }
        stbi_image_free(data);

        ckit_vector_push(shader->textures, texture);
    }

    void ckit_shader_bind_textures(CKIT_Shader* shader) {
        for (u32 i = 0; i < ckit_vector_count(shader->textures); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, shader->textures[i]);
        }
    }

    void ckit_shader_use(CKIT_Shader* shader) {
        glUseProgram(shader->id);
    }

    /*
    void *GetAnyGLFuncAddress(const char *name)
    {
        void *p = (void *)wglGetProcAddress(name);
        if(p == 0 ||
            (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
            (p == (void*)-1) )
        {
            HMODULE module = LoadLibraryA("opengl32.dll");
            p = (void *)GetProcAddress(module, name);
        }

        return p;
    }
    */

    void ckit_wgl_context_create(HDC dc_handle) {
        HGLRC opengl_context = wglCreateContext(dc_handle);
        wglMakeCurrent(dc_handle, opengl_context);
    }
#endif