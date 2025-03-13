# -------------------------------- GENERATED BY C_BUILD --------------------------------
from c_build.source.Utilities import *
from c_build.source.Manager import *
# --------------------------------------------------------------------------------------s

compiler_name = C_BUILD_COMPILER_NAME() if C_BUILD_IS_DEPENDENCY() else "INVALID_COMPILER"

if IS_WINDOWS():
    compiler_name = "cl"
if IS_DARWIN():
    compiler_name = "clang"
elif IS_LINUX():
    compiler_name = "gcc"
    
compiler_warning_level = ""
compiler_disable_specific_warnings = []
compiler_treat_warnings_as_errors = True
compiler_disable_warnings = False
compiler_disable_sanitizer = True

project_name = "ckit_graphics"
project_dependencies = ["ckit"]
project_debug_with_visual_studio = True
project_rebuild_project_dependencies = False
project_executable_procedures = ["ckit_graphics_test.exe", "ckit_pong.exe"]

# Do different things depending on the platform
if compiler_name == "cl":
	compiler_warning_level = "2"
	compiler_disable_specific_warnings = ["5105", "4668", "4820", "4996"]
elif compiler_name in ["gcc", "cc", "clang"]:
	compiler_warning_level = "all"
	compiler_disable_specific_warnings = ["deprecated", "pointer-sign", "parentheses", "unused-variable", "missing-braces"]

executable_procedure_libs = [
	f"../../../build_{compiler_name}/{C_BUILD_LIB("ckit_graphics", compiler_name)}", 
	f"../../../ckit/build_{compiler_name}/{C_BUILD_LIB("ckit", compiler_name)}"
]

if IS_WINDOWS():
	windows_libs = ["User32.lib", "Gdi32.lib", "Opengl32.lib"] if compiler_name == "cl" else ["-lUser32", "-lGdi32", "-lOpengl32"]
	executable_procedure_libs += windows_libs

procedures_config = {
    "ckit_graphics_lib": {
        "build_directory": f"./build_{compiler_name}",
        "output_name": C_BUILD_LIB("ckit_graphics", compiler_name),
        "source_files": ["../ckit_graphics.c", "../External_Libraries/stb_image.c", "../External_Libraries/glad/src/glad.c"],
        "additional_libs": [],
        "compile_time_defines": ["CKIT_WSL"],
        "compiler_inject_into_args": [],
        "include_paths": [""],
    },
	"ckit_graphics_test": {
        "build_directory": f"./Tests/GraphicsTest/build_{compiler_name}",
        "output_name": "ckit_graphics_test.exe",
        "source_files": ["../*.c"],
        "additional_libs": executable_procedure_libs,
		"compile_time_defines": ["CKIT_WSL"],
        "compiler_inject_into_args": [],
        "include_paths": [""],
    },
    "ckg_pong": {
        "build_directory": f"./Tests/PongTest/build_{compiler_name}",
        "output_name": "ckit_pong.exe",
        "source_files": ["../*.c"],
        "additional_libs": executable_procedure_libs,
        "compile_time_defines": ["CKIT_WSL"],
        "compiler_inject_into_args": [],
        "include_paths": [""],
    },
}

compiler_config = {key: value for key, value in locals().items() if key.startswith('compiler_')}
project_config = {key: value for key, value in locals().items() if key.startswith('project_')}

manager: Manager = Manager(compiler_config, project_config, procedures_config)
manager.build_project()
# -------------------------------------------------------------------------------------