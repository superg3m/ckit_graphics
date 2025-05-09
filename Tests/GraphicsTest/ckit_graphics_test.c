#include "../../ckit_graphics.h"

#if defined(PLATFORM_WINDOWS)
	void set_bitmap_gradient(CKIT_Window* window, u32 x_offset, u32 y_offset) {
		int stride = window->bitmap.width * window->bitmap.bytes_per_pixel;

		u8* row = window->bitmap.memory;    
		for(u32 y = 0; y < window->bitmap.height; y++)
		{
			u32* pixel = (u32*)row;
			for(u32 x = 0; x < window->bitmap.width; x++)
			{
				const u32 red = ((100 + x + x_offset) << 16);
				const u32 green = (10 << 8);
				const u32 blue = ((100 + y + y_offset) << 0);
				
				const u32 rgb = red|green|blue;

				*pixel++ = rgb;
			}
			row += stride;
		}
	}
#elif defined(PLATFORM_LINUX)
	void set_bitmap_gradient(CKIT_Window* window, u32 x_offset, u32 y_offset) {
		
	}
#endif

int main() {
	ckit_init();

	u32 width = 800;
	u32 height = 800;
	u32 width_padding = 18;
	u32 height_padding = 42;
	u32 width_with_padding = width - width_padding;
	u32 height_with_padding = height - height_padding;

	LOG_FATAL("TESTING\n");
	LOG_ERROR("TESTING\n");
	LOG_WARN("TESTING\n");
	LOG_DEBUG("TESTING\n");
	LOG_SUCCESS("TESTING\n");

	ckit_window_bind_icon("../../../../assets/c_original_logo_icon_146611.ico");
	CKIT_Window* window = ckit_window_create(width, height, "GameEngine");
	CKSGL graphics = cksgl_create(&window->bitmap.memory, (u16*)&window->bitmap.width, (u16*)&window->bitmap.height);

	float x_pos = 40;
	float y_pos = 40;

	float x_velocity = 2.0;
	float y_velocity = 1.5;

	u32 player_width = 64;
	u32 player_height = 64;
	u32 half_player_width = player_width / 2;
	u32 half_player_height = player_height / 2;

	u32 center_width = 10;
	u32 center_height = 10;
	u32 half_center_width = center_width / 2;
	u32 half_center_height = center_height / 2;

	u32 border_size = 5; 
	u32 close_factor = 0;

	size_t file_size = 0;
	u8* ckit_sword_bmp_data = ckit_os_read_entire_file("../../../../assets/Sword.bmp", &file_size);
	CKIT_Bitmap sword_bitmap = ckit_parser_load_bmp(ckit_sword_bmp_data, file_size);
	
	s32 mouse_x = 0;
	s32 mouse_y = 0;

	double start_counter = ckit_os_query_performance_counter();

	while (!ckit_window_should_quit(window)) {
		{ // UPDATE
			s32 left_threshold   = close_factor + (player_width / 2);
			s32 right_threshold  = (width_with_padding - close_factor) + (player_width / 2);

			s32 top_threshold    = close_factor + (player_height / 2);
			s32 bottom_threshold = (height_with_padding - close_factor) + (player_height / 2);

			Boolean left_check   = x_pos < left_threshold - 1;
			Boolean right_check  = (x_pos + player_width) > right_threshold + 1;
			Boolean top_check    = (y_pos) < top_threshold - 1;
			Boolean bottom_check = (y_pos + player_height) > bottom_threshold + 1;


			if (left_check || right_check) {
				x_velocity *= -1.025f;
				close_factor += 2;
				x_pos += x_velocity * 5;
			}

			if (bottom_check || top_check) {
				y_velocity *= -1.025f;
				close_factor += 2;
				y_pos += y_velocity * 5;
			}

			x_pos += x_velocity;
			y_pos += y_velocity;

			ckit_window_get_client_mouse_position(window, &mouse_x, &mouse_y);
		}

		{ // RENDER
			CKIT_Color grey = {55, 55, 55, 255};
			CKIT_Color black = {20, 20, 20, 150};
			CKIT_Color red = {255, 0, 0, 105};
			CKIT_Color green = {0, 255, 0, 75};

			cksgl_clear_color(graphics, grey);

			cksgl_draw_bitmap(graphics, sword_bitmap.width * (mouse_x / 16), sword_bitmap.height * (mouse_x / 32), (mouse_x / 16), sword_bitmap);
			cksgl_draw_circle(graphics, mouse_x, mouse_y, (mouse_x / 2), TRUE, red);

			float pixel_offset_from_the_top = ((float)border_size / 2.0);
			s32 center_x = width_with_padding / 2;
			s32 center_y = (height_with_padding / 2) + (s32)pixel_offset_from_the_top;
			float adjusted_width = (float)((s32)(width_with_padding + border_size) - (s32)(close_factor * 2));
			float adjusted_height = (float)((s32)(height_with_padding + border_size) - (s32)(close_factor * 2));

			s32 right_x = width_with_padding - close_factor;
			s32 top_y = close_factor + (s32)pixel_offset_from_the_top;
			s32 bottom_y = (s32)(height_with_padding - close_factor + (s32)pixel_offset_from_the_top);

			cksgl_draw_quad_custom(graphics, (u32)close_factor, (u32)center_y, (s32)border_size, (s32)adjusted_height, CKIT_COLOR_GREEN); // left
			cksgl_draw_quad_custom(graphics, (u32)right_x, (u32)center_y, (s32)border_size, (s32)adjusted_height, CKIT_COLOR_BLUE); // right
			cksgl_draw_quad_custom(graphics, (u32)center_x, (u32)top_y, (s32)adjusted_width, (s32)border_size, CKIT_COLOR_PURPLE); // top
			cksgl_draw_quad_custom(graphics, (u32)center_x, (u32)bottom_y, (s32)adjusted_width, (s32)border_size, CKIT_COLOR_RED); // bottom

			cksgl_draw_quad_custom(graphics, (u32)width / 2, (u32)height / 2, 400, 200, black);
			cksgl_draw_quad_custom(graphics, (u32)x_pos, (u32)y_pos, (s32)player_width, (s32)player_height, green);
			cksgl_draw_quad_custom(graphics, (u32)x_pos, (u32)y_pos, (s32)center_width, (s32)center_height, CKIT_COLOR_PURPLE);

			ckit_window_swap_buffers(window);
		}

		double end_counter = ckit_os_query_performance_counter();
		double milliseconds_per_frame = end_counter - start_counter;
		double seconds_per_frame = milliseconds_per_frame / 1000.0;
		u64 fps = (u64)(1.0 / seconds_per_frame);

		LOG_SUCCESS("%llfms / FPS: %lld\n", milliseconds_per_frame, fps);

		start_counter = end_counter;
	}

	ckit_window_free(window); // hmm how can I make this safer?
	ckit_free(ckit_sword_bmp_data);

	ckit_cleanup(TRUE);
	return 0;
}