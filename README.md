### -- GRAPHICS MODULE ONLY WORKS ON CL.exe MSVC -- windows only right now because programming is hard lmao

# ckit_graphics
- Graphics plugin for ckit
- This library will be completly cross-platform (Will be windows exclusive for a while ngl)

# How to build
1. ./bootstrap.ps1
2. ./build.ps1 or ./build.ps1 -debug
3. ./debug.ps1
4. ./run.ps1

### key
- '[x]': Finished
- '[/]': Started working on it
- '[ ]': Haven't started working on it


### Next Steps
1. - [ ] Resolution independent bitmap
2. - [ ] Standardize the coordinate system (normalized coordinates -1 to 1)
		- [ ] ckit_window_normalized_to_pixel_coordinates(float xt, float yt) 
			- // inbetween -1 and 1 
			- // gives back width * xt and height * yt

3. - [ ] Optimize the software rendering backend using SIMD
4. - [ ] Text rendering

5. - [ ] SIMD optimization on the render

6. - [ ] EventSystem
7. - [ ] Input System
8. - [ ] IMGUI
		- [ ] Button
		- [ ] Slider
		- [ ] Panel
			- [ ] RenderGroup 

9. - [ ] Networking module make pvp pong
10. - [ ] Write a bunch of tests validating and verifying all the utilities and logic.
		- [ ] ckit_printf("%iv\n") // (int vector)
		- [ ] ckit_printf("%fv\n") // (float vector)
		- [ ] ckit_printf("%sv\n") // (string vector)
		- [ ] ckit_printf("%cv\n") // (char vector)
		
11. - [ ] OpenGL Module


# Dev Notes (for me):
- Tiny software renderer: https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling

- Software Renderer
	- ckit_window_creation (Required)
		- dropFileCallback()
	- ckit_software (Frame buffer platform agnostic) (I want to have full 3d capability)