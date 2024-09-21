#pragma once

#include "../../../External_Libraries/ckg_ckit_core/ckit_extern.h"
//========================== Begin Types ==========================
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	void ckit_graphics_software_backend_draw_quad(u8* framebuffer, u32 framebuffer_width, u32 framebuffer_height, CKIT_Rectangle2D quad, CKIT_Color color);
	void ckit_graphics_software_backend_draw_bitmap(u8* framebuffer, u32 framebuffer_width, u32 framebuffer_height, s32 start_x, s32 start_y, u32 scale_factor, CKIT_Bitmap bitmap);
	void ckit_graphics_software_backend_draw_circle(u8* framebuffer, u32 framebuffer_width, u32 framebuffer_height, s32 start_x, s32 start_y, s32 radius, Boolean is_filled, CKIT_Color color);
	void ckit_graphics_software_backend_clear_color(u8* framebuffer, u32 framebuffer_width, u32 framebuffer_height, CKIT_Color color);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++