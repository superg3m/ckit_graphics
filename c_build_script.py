# -------------------------------- GENERATED BY C_BUILD --------------------------------
import os
import sys

def FIND_C_BUILD(current_dir):
    if os.path.isdir(os.path.join(current_dir, "c_build")):
        sys.path.insert(0, current_dir)
        return

    parent_dir = os.path.dirname(current_dir)
    if parent_dir != current_dir:
        FIND_C_BUILD(parent_dir)

FIND_C_BUILD(os.path.abspath(os.path.dirname(__file__)))
from c_build.source.UserUtilities import *
from c_build.source.Manager import *
# --------------------------------------------------------------------------------------s

pc: ProjectConfig = ProjectConfig(
    project_name = "ckit_graphics",
    project_dependencies = [
        Dependency(
            name="ckit",
            branch_name="main",
        ),
    ],
    project_debug_with_visual_studio = True,
    project_rebuild_project_dependencies = True,
    project_executable_names  = ["ckit_obj.exe", "ckit_graphics_test.exe", "ckit_pong.exe"]
)

cc: CompilerConfig = CompilerConfig(
    compiler_name = C_BUILD_COMPILER_NAME() if C_BUILD_IS_DEPENDENCY() else "INVALID_COMPILER",
    compiler_std_version = "",
    compiler_warning_level = "",
    compiler_disable_specific_warnings = [""],
    compiler_treat_warnings_as_errors = True,
    compiler_disable_warnings  = False,
    compiler_disable_sanitizer = True
)

if IS_WINDOWS() and not C_BUILD_IS_DEPENDENCY():
    cc.compiler_name = "cl"
elif IS_DARWIN() and not C_BUILD_IS_DEPENDENCY():
    cc.compiler_name = "clang"
elif IS_LINUX() and not C_BUILD_IS_DEPENDENCY():
    cc.compiler_name = "gcc"

# Do different things depending on the platform
if cc.compiler_name == "cl":
    cc.compiler_warning_level = "2"
    cc.compiler_disable_specific_warnings = ["5105", "4668", "4820", "4996"]
else:
    cc.compiler_warning_level = ""
    cc.compiler_disable_specific_warnings = ["deprecated", "pointer-sign", "parentheses", "unused-variable", "missing-braces"]


build_postfix = f"build_{cc.compiler_name}/{C_BUILD_BUILD_TYPE()}"
executable_procedure_libs = [
    f"../../../../{build_postfix}/{GET_LIB_NAME(cc, 'ckit_graphics')}",
    f"../../../../ckit/{build_postfix}/{GET_LIB_NAME(cc, 'ckit')}"
]
if IS_WINDOWS():
    windows_libs = [GET_LIB_FLAG(cc, "User32"), GET_LIB_FLAG(cc, "Gdi32"), GET_LIB_FLAG(cc, "Opengl32")]
    executable_procedure_libs += windows_libs

procedures_config = {
    "ckit_graphics_lib": ProcedureConfig(
        build_directory = f"./{build_postfix}",
        output_name = GET_LIB_NAME(cc, "ckit_graphics"),
        source_files = [
            "../../ckit_graphics.c", 
            "../../External_Libraries/stb_image.c", 
            "../../External_Libraries/glad/src/glad.c"],
        additional_libs = [],
        compile_time_defines = ["CKIT_WSL"],
        compiler_inject_into_args = [],
        include_paths = [""],
    ),


    "ckit_graphics_test": ProcedureConfig(
        build_directory = f"./Tests/GraphicsTest/{build_postfix}",
        output_name = "ckit_graphics_test.exe",
        source_files = ["../../*.c"],
        additional_libs = executable_procedure_libs,
        compile_time_defines = ["CKIT_WSL"],
        compiler_inject_into_args = [],
        include_paths = [""],
    ),

    "ckit_pong": ProcedureConfig(
        build_directory = f"./Tests/PongTest/{build_postfix}",
        output_name = "ckit_pong.exe",
        source_files = ["../../*.c"],
        additional_libs = executable_procedure_libs,
        compile_time_defines = ["CKIT_WSL"],
        compiler_inject_into_args = [],
        include_paths = [""],
    ),

    "ckit_obj": ProcedureConfig(
        build_directory = f"./Tests/OBJ/{build_postfix}",
        output_name = "ckit_obj.exe",
        source_files = ["../../ckit_obj.c"],
        additional_libs = executable_procedure_libs,
        compile_time_defines = ["CKIT_WSL"],
        compiler_inject_into_args = [],
        include_paths = ["../../../../ckit"],
    ),
}

manager: Manager = Manager(cc, pc, procedures_config)
manager.build_project()
# -------------------------------------------------------------------------------------