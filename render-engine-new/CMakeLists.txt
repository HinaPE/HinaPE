cmake_minimum_required(VERSION 3.16)
project(HinaPE_Render_Engine_Pro VERSION 1.1.2 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_DEPS_DIR "../deps")
set(HINAPE_COMMON_DIR "../common")

if (NOT TARGET HinaPE_Common)
    add_subdirectory(${HINAPE_COMMON_DIR} HinaPE_Common)
endif ()

set(HINA_RENDER_ENGINE_PRO_OpenGL
        "graphics_api/OpenGL/mesh.cpp"
        "graphics_api/mesh.h"
#        "platform/platform.cpp"
#        "platform/platform.h"
        "${HINAPE_DEPS_DIR}/imgui/imgui_impl_opengl3.cpp"
        "${HINAPE_DEPS_DIR}/imgui/imgui_impl_opengl3.h"
        "${HINAPE_DEPS_DIR}/imgui/imgui_impl_sdl.cpp"
        "${HINAPE_DEPS_DIR}/imgui/imgui_impl_sdl.h")

add_library(HinaPE_Render_Engine_Pro ${HINA_RENDER_ENGINE_PRO_OpenGL} math_api.h)
set_target_properties(HinaPE_Render_Engine_Pro PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
target_include_directories(HinaPE_Render_Engine_Pro PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(HinaPE_Render_Engine_Pro PUBLIC HINAPE_SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shader/") # TODO: Error Prone
target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC HinaPE_Common)


# setup OS-specific options
if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()
if (APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "-framework AppKit")
    find_package(SDL2 REQUIRED)
    target_include_directories(HinaPE_Render_Engine_Pro PUBLIC ${SDL2_INCLUDE_DIRS}/..)
    target_include_directories(HinaPE_Render_Engine_Pro PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_directories(HinaPE_Render_Engine_Pro PUBLIC ${SDL2_LIBRARY_DIRS})
    add_definitions(${SDL2_CFLAGS_OTHER})
endif ()
if (LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    target_include_directories(HinaPE_Render_Engine_Pro PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_directories(HinaPE_Render_Engine_Pro PUBLIC ${SDL2_LIBRARY_DIRS})
    add_definitions(${SDL2_CFLAGS_OTHER})
endif ()
if (MSVC)
    #    target_compile_options(HinaPE_Render_Engine_Pro PUBLIC /MP /W4 /WX /wd4201 /wd4840 /wd4100 /wd4505 /fp:fast)
    #    target_compile_options(HinaPE_Render_Engine_Pro PUBLIC /wd4127 /wd4819 /wd4189 /wd4459 /wd5054 /wd4458 /MP /W4 /WX /wd4201 /wd4840 /wd4100 /wd4505 /fp:fast)
else ()
    #    target_compile_options(HinaPE_Render_Engine_Pro PUBLIC -Wall -Wextra -Werror -Wno-reorder -Wno-unused-function -Wno-unused-parameter -Wordered-compare-function-pointers)
    #    target_compile_options(HinaPE_Render_Engine_Pro PUBLIC -Wall -Wextra -Wno-reorder -Wno-unused-function -Wno-unused-parameter)
endif ()
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(HinaPE_Render_Engine_Pro PUBLIC -fno-omit-frame-pointer)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
endif ()

# build dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC Threads::Threads) # std::thread need pthread lib to be linked
add_subdirectory("${HINAPE_DEPS_DIR}/imgui/" "${CMAKE_CURRENT_BINARY_DIR}/imgui")
add_subdirectory("${HINAPE_DEPS_DIR}/glad/" "${CMAKE_CURRENT_BINARY_DIR}/glad")
add_subdirectory("${HINAPE_DEPS_DIR}/nfd/" "${CMAKE_CURRENT_BINARY_DIR}/nfd")
add_subdirectory("${HINAPE_DEPS_DIR}/sf_libs/" "${CMAKE_CURRENT_BINARY_DIR}/sf_libs")
set(ASSIMP_BUILD_COLLADA_IMPORTER TRUE)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)
set(ASSIMP_BUILD_PLY_IMPORTER TRUE)
set(ASSIMP_BUILD_FBX_IMPORTER TRUE)
set(ASSIMP_BUILD_GLTF_IMPORTER TRUE)
set(ASSIMP_BUILD_3DS_IMPORTER TRUE)
set(ASSIMP_BUILD_STL_IMPORTER TRUE)
set(ASSIMP_BUILD_BLEND_IMPORTER TRUE)
set(ASSIMP_BUILD_COLLADA_EXPORTER TRUE)
add_subdirectory("${HINAPE_DEPS_DIR}/assimp/" "${CMAKE_CURRENT_BINARY_DIR}/assimp")
target_include_directories(HinaPE_Render_Engine_Pro PUBLIC ${ASSIMP_INCLUDE_DIRS})
target_include_directories(glad PUBLIC ${HINAPE_DEPS_DIR})
#target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(imgui PUBLIC ../render-engine/)

# define include paths
target_include_directories(HinaPE_Render_Engine_Pro PUBLIC "${HINAPE_DEPS_DIR}" "${HINAPE_DEPS_DIR}/assimp/include" "${HINAPE_DEPS_DIR}/eigen3")
target_include_directories(HinaPE_Render_Engine_Pro PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(HinaPE_Render_Engine_Pro PUBLIC "${HINAPE_COMMON_DIR}")

# link libraries
if (WIN32)
    target_include_directories(HinaPE_Render_Engine_Pro PUBLIC "${HINAPE_DEPS_DIR}/win")
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC ${HINAPE_DEPS_DIR}/win/SDL2/SDL2main)
    target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC ${HINAPE_DEPS_DIR}/win/SDL2/SDL2)
    target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC Winmm)
    target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC Version)
    target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC Setupapi)
    target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC Shcore)
endif ()
if (LINUX)
    target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC SDL2)
endif ()
if (APPLE)
    target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC ${SDL2_LIBRARIES})
endif ()
target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC assimp)
target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC nfd)
target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC sf_libs)
target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC imgui)
target_link_libraries(HinaPE_Render_Engine_Pro PUBLIC glad)
