# -------------------------------- GENERATED BY C_BUILD --------------------------------
import argparse
from c_build.new_stuff.globals import *
from c_build.new_stuff.new_project import *

parser = argparse.ArgumentParser(description='c_build_script')
parser.add_argument('--build_type', type=str, required=True, help='Build type (e.g. debug, release)')
args = parser.parse_args()
build_type = args.build_type
# --------------------------------------------------------------------------------------

COMPILER = os.getenv("COMPILER", "gcc")
if os.name != "nt":
    COMPILER = "gcc"
	
project = Project("ckit_graphics", COMPILER)

# Do different things depending on the platform
if COMPILER == "cl":
	project.set_compiler_warning_level("1")
	project.disable_specific_warnings(["5105", "4668", "4820"])
elif COMPILER in ["gcc", "cc", "clang"]:
	project.disable_specific_warnings(["deprecated", "pointer-sign", "parentheses"])

project.set_treat_warnings_as_errors(True)
project.set_debug_with_visual_studio(True)
project.set_rebuild_project_dependencies(True)

project.set_project_dependencies([""])
# -------------------------------------------------------------------------------------

executable_procedure_libs = [
	f"../../../build_{COMPILER}/ckit_graphics.lib" if COMPILER == "cl" else f"../../../build_{COMPILER}/libckit_graphics.a", 
	f"../../../ckit/build_{COMPILER}/ckit.lib" if COMPILER == "cl" else f"../../../ckit/build_{COMPILER}/libckit.a"
]

if os.name == "nt":
	windows_libs = ["User32.lib", "Gdi32.lib"] if COMPILER == "cl" else ["-lUser32", "-lGdi32"]
	executable_procedure_libs += windows_libs

procedures = {
    "ckit_graphics_lib": {
        "build_directory": f"./build_{COMPILER}",
        "output_name": "ckit_graphics.lib" if COMPILER == "cl" else "libckit_graphics.a",
        "source_files": ["../ckit_graphics.c"],
        "additional_libs": [],
        "compile_time_defines": ["CKIT_WSL"],
        "include_paths": [],
    },
	"ckit_graphics_test": {
        "build_directory": f"./Tests/GraphicsTest/build_{COMPILER}",
        "output_name": "ckit_graphics_test.exe",
        "source_files": ["../*.c"],
        "additional_libs": executable_procedure_libs,
		"compile_time_defines": ["CKIT_WSL"],
        "include_paths": [],
    },
    "ckg_pong": {
        "build_directory": f"./Tests/PongTest/build_{COMPILER}",
        "output_name": "ckit_pong.exe",
        "source_files": ["../*.c"],
        "additional_libs": executable_procedure_libs,
        "compile_time_defines": ["CKIT_WSL"],
        "include_paths": [],
    },
}

for procedure_name, procedure_data in procedures.items():
	print(os.getenv("IS_DEPENDENCY", "False"))
	if (procedure_name in ["ckit_graphics_test", "ckg_pong"]) and os.getenv("IS_DEPENDENCY", "False") == "True" and os.name != "nt":
		continue

	procedure = project.add_procedure(procedure_data["build_directory"])
	procedure.set_output_name(procedure_data["output_name"])
	procedure.set_source_files(procedure_data["source_files"])
	procedure.set_include_paths(procedure_data["include_paths"])
	procedure.set_compile_time_defines(procedure_data["compile_time_defines"])
	procedure.set_additional_libs(procedure_data["additional_libs"])

# -------------------------------------------------------------------------------------
project.set_executables_to_run(["ckit_graphics.lib" if COMPILER == "cl" else f"libckit_graphics.a"])

project.build(build_type)