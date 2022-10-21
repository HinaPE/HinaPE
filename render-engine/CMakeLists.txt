cmake_minimum_required(VERSION 3.16)
project(HinaPE_Render_Engine VERSION 1.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_DEPS_DIR "../deps")

file(GLOB_RECURSE HinaRendererGUI "gui/*.cpp" "gui/*.h")
file(GLOB_RECURSE HinaRendererGEOM "geometry/*.cpp" "geometry/*.h" "geometry/*.inl")
file(GLOB_RECURSE HinaRendererRAYS "rays/*.cpp" "rays/*.h")
file(GLOB_RECURSE HinaRendererUTIL "util/*.cpp" "util/*.h")
file(GLOB_RECURSE HinaRendererSCENE "scene/*.cpp" "scene/*.h")
file(GLOB_RECURSE HinaRendererLIB "lib/*.cpp" "lib/*.h")
file(GLOB_RECURSE HinaRendererTOIMPL "reference/*.cpp" "reference/*.h" "reference/*.inl")
file(GLOB_RECURSE HinaRendererPHYSICSAPI "physics-API/*.cpp" "physics-API/*.h" "physics-API/*.inl")
set(HinaRendererPLATFORM
        "platform/gl.cpp"
        "platform/platform.cpp"
        "platform/gl.h"
        "platform/platform.h"
        "${HINAPE_DEPS_DIR}/imgui/imgui_impl_opengl3.cpp"
        "${HINAPE_DEPS_DIR}/imgui/imgui_impl_opengl3.h"
        "${HINAPE_DEPS_DIR}/imgui/imgui_impl_sdl.cpp"
        "${HINAPE_DEPS_DIR}/imgui/imgui_impl_sdl.h")

set(HINA_RENDER_ENGINE_SRC
        ${HinaRendererGUI}
        ${HinaRendererGEOM}
        ${HinaRendererRAYS}
        ${HinaRendererUTIL}
        ${HinaRendererSCENE}
        ${HinaRendererLIB}
        ${HinaRendererTOIMPL}
        ${HinaRendererPLATFORM}
        ${HinaRendererPHYSICSAPI}
        app.cpp
        app.h
        main.cpp
        platform/icon.rc
        )

add_executable(HinaPE_Render_Engine ${HINA_RENDER_ENGINE_SRC})
set_target_properties(HinaPE_Render_Engine PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)

# setup OS-specific options
if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()
if (APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "-framework AppKit")
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS}/..)
    include_directories(${SDL2_INCLUDE_DIRS})
    link_directories(${SDL2_LIBRARY_DIRS})
    add_definitions(${SDL2_CFLAGS_OTHER})
endif ()
if (LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    include_directories(${SDL2_INCLUDE_DIRS})
    link_directories(${SDL2_LIBRARY_DIRS})
    add_definitions(${SDL2_CFLAGS_OTHER})
endif ()
if (MSVC)
    #    target_compile_options(HinaPE_Render_Engine PRIVATE /MP /W4 /WX /wd4201 /wd4840 /wd4100 /wd4505 /fp:fast)
    #    target_compile_options(HinaPE_Render_Engine PRIVATE /wd4127 /wd4819 /wd4189 /wd4459 /wd5054 /wd4458 /MP /W4 /WX /wd4201 /wd4840 /wd4100 /wd4505 /fp:fast)
else ()
    #    target_compile_options(HinaPE_Render_Engine PRIVATE -Wall -Wextra -Werror -Wno-reorder -Wno-unused-function -Wno-unused-parameter -Wordered-compare-function-pointers)
    #    target_compile_options(HinaPE_Render_Engine PRIVATE -Wall -Wextra -Wno-reorder -Wno-unused-function -Wno-unused-parameter)
endif ()
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(HinaPE_Render_Engine PRIVATE -fno-omit-frame-pointer)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
endif ()

# build dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(HinaPE_Render_Engine PRIVATE Threads::Threads) # std::thread need pthread lib to be linked
add_subdirectory("${HINAPE_DEPS_DIR}/imgui/" "${CMAKE_CURRENT_BINARY_DIR}/imgui")
add_subdirectory("${HINAPE_DEPS_DIR}/glad/" "${CMAKE_CURRENT_BINARY_DIR}/glad")
add_subdirectory("${HINAPE_DEPS_DIR}/nfd/" "${CMAKE_CURRENT_BINARY_DIR}/nfd")
add_subdirectory("${HINAPE_DEPS_DIR}/sf_libs/" "${CMAKE_CURRENT_BINARY_DIR}/sf_libs")
add_subdirectory("${HINAPE_DEPS_DIR}/libigl/" "${CMAKE_CURRENT_BINARY_DIR}/libigl")
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
include_directories(${ASSIMP_INCLUDE_DIRS})
target_include_directories(glad PRIVATE ${HINAPE_DEPS_DIR})
target_include_directories(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# define include paths
include_directories("${HINAPE_DEPS_DIR}" "${HINAPE_DEPS_DIR}/assimp/include" "${HINAPE_DEPS_DIR}/eigen3" "${HINAPE_DEPS_DIR}/libigl/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# link libraries
if (WIN32)
    target_include_directories(HinaPE_Render_Engine PRIVATE "${HINAPE_DEPS_DIR}/win")
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    target_link_libraries(HinaPE_Render_Engine PRIVATE "${HINAPE_DEPS_DIR}/win/SDL2/SDL2main")
    target_link_libraries(HinaPE_Render_Engine PRIVATE "${HINAPE_DEPS_DIR}/win/SDL2/SDL2")
    target_link_libraries(HinaPE_Render_Engine PRIVATE Winmm)
    target_link_libraries(HinaPE_Render_Engine PRIVATE Version)
    target_link_libraries(HinaPE_Render_Engine PRIVATE Setupapi)
    target_link_libraries(HinaPE_Render_Engine PRIVATE Shcore)
endif ()
if (LINUX)
    target_link_libraries(HinaPE_Render_Engine PRIVATE SDL2)
endif ()
if (APPLE)
    target_link_libraries(HinaPE_Render_Engine PRIVATE ${SDL2_LIBRARIES})
endif ()
target_link_libraries(HinaPE_Render_Engine PRIVATE assimp)
target_link_libraries(HinaPE_Render_Engine PRIVATE nfd)
target_link_libraries(HinaPE_Render_Engine PRIVATE sf_libs)
target_link_libraries(HinaPE_Render_Engine PRIVATE imgui)
target_link_libraries(HinaPE_Render_Engine PRIVATE glad)