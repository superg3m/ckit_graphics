#include "../../ckit_graphics.h"
#include "./obj_model.c"

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
		model = obj_model_create("../african_head.obj_wave");
	}

	while (!ckit_window_should_quit(window)) {
		{ // UPDATE
			ckit_window_get_client_mouse_position(window, &mouse_x, &mouse_y);
		}

		{ // RENDER
			cksgl_clear_color(graphics, CKIT_COLOR_BLACK);

			for (int i = 0; i < ckit_vector_count(model.faces) - 1; i++) {
				u32 f_index0 = model.faces[i][0];
				u32 f_index1 = model.faces[i + 1][0];

				CKIT_Vector3 v0 = model.verts[f_index0];
				CKIT_Vector3 v1 = model.verts[f_index1];

				u32 y_offset = 0;

				int x0 = (int)((v0.x + 1.0) * width / 2.0);
				int y0 = (int)((v0.y + 1.0) * height / 2.0) + y_offset;
				int x1 = (int)((v1.x + 1.0) * width / 2.0);
				int y1 = (int)((v1.y + 1.0) * height / 2.0) + y_offset;

				CKIT_Vector3 p0 = {x0, y0, 0};
				CKIT_Vector3 p1 = {x1, y1, 0};
				cksgl_draw_line(graphics, p0, p1, CKIT_COLOR_WHITE);
			}

			cksgl_flip_vertically(graphics);

			ckit_window_swap_buffers(window);
		}
	}

	obj_model_free(&model);
	ckit_window_free(window); // hmm how can I make this safer?
	ckit_cleanup(TRUE);
	return 0;
}