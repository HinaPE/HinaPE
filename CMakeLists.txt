cmake_minimum_required(VERSION 3.16)
project(HinaPE VERSION 1.0.1 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_DEPS_DIR "deps")

add_subdirectory(render-engine)
add_subdirectory(fluid-engine)

add_executable(HinaPE HinaPE.cpp)
set_target_properties(HinaPE PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(HinaPE PUBLIC HinaPE_Render_Engine HinaPE_Fluid_Engine)
if (APPLE)
    find_library(APPKIT_LIB AppKit)
    target_link_libraries(HinaPE PRIVATE ${APPKIT_LIB})
endif ()
# fixed by: https://github.com/libMesh/libmesh/issues/1396
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(HinaPE_Render_Engine PUBLIC -fno-omit-frame-pointer)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
endif ()
