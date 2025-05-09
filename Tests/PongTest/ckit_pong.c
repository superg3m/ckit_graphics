#include "../../ckit_graphics.h"

u32 half_player_width = 0;
u32 half_player_height = 0;

CKIT_Rectangle2D create_player() {
	const s32 player_x_pos = 100;
	const s32 player_y_pos = 200;
	const u32 player_width = 12;
	const u32 player_height = 50;

	half_player_width = player_width / 2;
	half_player_height = player_height / 2;

	return ckit_rectangle2d_create(player_x_pos + half_player_width, player_y_pos - half_player_height, player_width, player_height);
}

CKIT_Rectangle2D create_ball(u32 window_width, u32 window_height) {
	const s32 ball_x_pos = window_width / 2;
	const s32 ball_y_pos = window_height / 2;
	const u32 ball_radius = 5;
	const u32 ball_diameter = ball_radius * 2;

	return ckit_rectangle2d_create(ball_x_pos, ball_y_pos, ball_diameter, ball_diameter);
}

CKIT_Rectangle2D create_ai(u32 window_width, u32 window_height) {
	
	const s32 ai_x_pos = window_width - 100;
	const s32 ai_y_pos = window_height / 2;
	const u32 ai_width = 12;
	const u32 ai_height = 50;

	const u32 half_ai_width = ai_width / 2;
	const u32 half_ai_height = ai_height / 2;
	return ckit_rectangle2d_create(ai_x_pos, ai_y_pos, ai_width, ai_height);
}

int main() {
	ckit_init();

	u32 width = 1200;
	u32 height = 800;
	u32 width_padding = 2;
	u32 height_padding = 2;

	LOG_FATAL("TESTING\n");
	LOG_ERROR("TESTING\n");
	LOG_WARN("TESTING\n");
	LOG_DEBUG("TESTING\n");
	LOG_SUCCESS("TESTING\n");

	ckit_window_bind_icon("../../../../assets/c_original_logo_icon_146611.ico");
	CKIT_Window* window = ckit_window_create(width, height, "CKIT Pong");
	CKSGL graphics = cksgl_create(&window->bitmap.memory, (u16*)&window->bitmap.width, (u16*)&window->bitmap.height);

	u32 width_with_padding = window->bitmap.width - width_padding;
	u32 height_with_padding = window->bitmap.height - height_padding;

	CKIT_Rectangle2D player_rect = create_player();
	CKIT_Rectangle2D ball_rect = create_ball(width, height);

	CKIT_Rectangle2D ai_rect = create_ai(width, height);
	float ball_x_velocity = 1.0;
	float ball_y_velocity = 1.0;
	u32 player_score = 0;
	u32 ai_score = 0;

	Boolean ai_hit = FALSE;
	Boolean player_hit = FALSE;

	CKIT_Color player_color = CKIT_COLOR_BLUE;
	CKIT_Color ai_color = CKIT_COLOR_RED;
	CKIT_Color default_color = CKIT_COLOR_WHITE;

	// ckit_window_set_cursor_state(window, DISABLED);

	const CKIT_Rectangle2D x_axis_rect = ckit_rectangle2d_create(window->bitmap.width/2, height_with_padding, window->bitmap.width, 1);
	const CKIT_Rectangle2D y_axis_rect = ckit_rectangle2d_create(width_with_padding, window->bitmap.height/2, 1, window->bitmap.height);


	int mouse_x, mouse_y = 0;
	const float OFFSET = 2.0f;
	while (!ckit_window_should_quit(window)) {
		{ // UPDATE
			int steps = 4;
			float step_x = ball_x_velocity / steps;
			float step_y = ball_y_velocity / steps;

			for (int i = 0; i < steps; i++) {
				ball_rect.position.x += step_x;
				ball_rect.position.y += step_y;

				// Collision with walls
				Boolean left_check   = ball_rect.position.x <= 0;
				Boolean right_check  = (ball_rect.position.x + ball_rect.width) >= width_with_padding;
				Boolean bottom_check = ball_rect.position.y <= 0;
				Boolean top_check    = (ball_rect.position.y + ball_rect.height) >= height_with_padding;

				if (left_check || right_check) {
					ball_x_velocity *= -1;
					ai_score += left_check;
					player_score += right_check;
					LOG_DEBUG("Player Score: %d\n", player_score);
					LOG_DEBUG("AI Score: %d\n\n", ai_score);

					break; // Avoid multiple bounces in one frame
				}

				if (bottom_check || top_check) {
					ball_y_velocity *= -1;

					break;
				}

				// Paddle collision
				if (ckit_rectangle_check_aabb_collision(player_rect, ball_rect)) {
					player_hit = TRUE;
					ai_hit = FALSE;
					ball_x_velocity *= -1;

					break;
				}

				if (ckit_rectangle_check_aabb_collision(ai_rect, ball_rect)) {
					player_hit = FALSE;
					ai_hit = TRUE;
					ball_x_velocity *= -1;

					break;
				}
			}

			ai_rect.position.y = ball_rect.position.y;
			ball_rect.position.x += ball_x_velocity;
			ball_rect.position.y += ball_y_velocity;


			ckit_window_get_client_mouse_position(window, &mouse_x, &mouse_y);
			player_rect.position.y = mouse_y;
		}


		{ // RENDER
			cksgl_clear_color(graphics, CKIT_COLOR_BLACK);

			cksgl_draw_quad(graphics, x_axis_rect, CKIT_COLOR_RED); // x axis
			cksgl_draw_quad(graphics, y_axis_rect, CKIT_COLOR_BLUE); // y axis
			cksgl_draw_quad(graphics, player_rect, player_color); // ACTUAL PLAYER_PADDLE
			cksgl_draw_quad(graphics, player_rect, player_color); // ACTUAL PLAYER_PADDLE

			CKIT_Color ball_color = default_color;

			if (ai_hit) {
				ball_color = ai_color;
			} else if (player_hit) {
				ball_color = player_color;
			}

			CKIT_Vector3 p0 = {200, 200, 0};
			CKIT_Vector3 p1 = {400, 200, 0};
			CKIT_Vector3 p2 = {mouse_x, mouse_y, 0};
			cksgl_draw_triangle(graphics, p0, p1, p2, FALSE, CKIT_COLOR_YELLOW);

			cksgl_draw_quad(graphics, ball_rect, ball_color); // ball
			cksgl_draw_quad(graphics, ai_rect, ai_color); // ai_paddle

			ckit_window_swap_buffers(window);
		}
	}

	ckit_window_free(window); // hmm how can I make this safer?

	ckit_cleanup(TRUE);
	return 0;
}