#include "../../ckit_graphics.h"
#include "./obj_model.c"

CKIT_Vector3 project_to_screen(CKIT_Vector3 ndc_point, int width, int height, int y_offset) {
	// -1 - 1
	//  0 - 2
	//  0 - width

    CKIT_Vector3 screen_pos;
    screen_pos.x = (int)((ndc_point.x + 1.0) * (width / 2.0));
    screen_pos.y = (int)((ndc_point.y + 1.0) * (height / 2.0)) + y_offset;
	screen_pos.z = 0;
    return screen_pos;
}

CKIT_Vector3 screen_to_ndc(CKIT_Vector3 screen_point, int width, int height, int y_offset) {
	//  0 - width
	//  0 - 2
	// -1 - 1

	// 632 / (920 * 2)

    CKIT_Vector3 ndc;
    ndc.x = (double)((screen_point.x * (2.0 / width)) - 1.0);
    ndc.y = (double)((screen_point.y * (2.0 / height)) - 1.0) + y_offset;
	ndc.z = 0;

    return ndc;
}

int main(int argc, char** argv) {
	ckit_init();

	u32 width = 1200;
	u32 height = 920;

	CKIT_Window* window = ckit_window_create(width, height, "CKIT OBJ Model");
	CKSGL graphics = cksgl_create(&window->bitmap.memory, (u16*)&window->bitmap.width, (u16*)&window->bitmap.height);

	int mouse_x, mouse_y = 0;
	const float OFFSET = 2.0f;

	OBJ_Model model;

	if (2 == argc) {
		model = obj_model_create(argv[1]);
	} else {
		model = obj_model_create("../../african_head.obj_wave");
	}

	while (!ckit_window_should_quit(window)) {
		{ // UPDATE
			ckit_window_get_client_mouse_position(window, &mouse_x, &mouse_y);
		}

		{ // RENDER
			cksgl_clear_color(graphics, CKIT_COLOR_BLACK);

			CKIT_Vector3 mouse_point = {mouse_x, (s32)window->bitmap.height - mouse_y, 0};
			CKIT_Vector3 ndc_mouse_point = screen_to_ndc(mouse_point, width, height, 0);
			model.verts[model.faces[0][0]] = ndc_mouse_point;

			for (int i = 0; i < ckit_vector_count(model.faces) - 2; i += 3) {
				u32 f_index0 = model.faces[i][0];
				u32 f_index1 = model.faces[i + 1][0];
				u32 f_index2 = model.faces[i + 2][0];

				CKIT_Vector3 p0 = project_to_screen(model.verts[f_index0], width, height, 0);
				CKIT_Vector3 p1 = project_to_screen(model.verts[f_index1], width, height, 0);
				CKIT_Vector3 p2 = project_to_screen(model.verts[f_index2], width, height, 0);

				cksgl_draw_triangle(graphics, p0, p1, p2, FALSE, CKIT_COLOR_WHITE);
			}

			cksgl_flip_vertically(graphics);

			cksgl_draw_quad_custom(graphics, mouse_x, mouse_y, 32, 32, CKIT_COLOR_GREEN);

			ckit_window_swap_buffers(window);
		}
	}

	obj_model_free(&model);
	ckit_window_free(window); // hmm how can I make this safer?
	ckit_cleanup(TRUE);
	return 0;
}