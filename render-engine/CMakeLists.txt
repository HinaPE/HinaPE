cmake_minimum_required(VERSION 3.16)
project(HinaPE_Render_Engine VERSION 1.1.1 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

option(FluidEngineOption "Enable Fluid Engine(Default: ON)" ON)
option(ClothEngineOption "Enable Cloth Engine(Default: ON)" ON)
option(RigidBodyEngineOption "Enable RigidBody Engine(Default: ON)" ON)

set(HINAPE_COMMON_DIR "../common")
set(HINAPE_PHYSICS_DIR "../physics-engine")

if (NOT TARGET HinaPE_Common)
    add_subdirectory(${HINAPE_COMMON_DIR} HinaPE_Common)
endif ()

file(GLOB_RECURSE HINA_RENDER_ENGINE_GUI "gui/*.cpp" "gui/*.h")
file(GLOB_RECURSE HINA_RENDER_ENGINE_GEOM "geometry/*.cpp" "geometry/*.h" "geometry/*.inl")
file(GLOB_RECURSE HINA_RENDER_ENGINE_RAYS "rays/*.cpp" "rays/*.h")
file(GLOB_RECURSE HINA_RENDER_ENGINE_UTIL "util/*.cpp" "util/*.h")
file(GLOB_RECURSE HINA_RENDER_ENGINE_SCENE "scene/*.cpp" "scene/*.h")
file(GLOB_RECURSE HINA_RENDER_ENGINE_LIB "lib/*.cpp" "lib/*.h")
file(GLOB_RECURSE HINA_RENDER_ENGINE_TO_IMPL "reference/*.cpp" "reference/*.h" "reference/*.inl")
set(HINA_RENDER_ENGINE_PLATFORM
        "platform/gl.cpp"
        "platform/gl.h"
        "platform/platform.cpp"
        "platform/platform.h"
        "deps/imgui/imgui_impl_opengl3.cpp"
        "deps/imgui/imgui_impl_opengl3.h"
        "deps/imgui/imgui_impl_sdl.cpp"
        "deps/imgui/imgui_impl_sdl.h")

set(HINA_RENDER_ENGINE_SRC
        ${HINA_RENDER_ENGINE_GUI}
        ${HINA_RENDER_ENGINE_GEOM}
        ${HINA_RENDER_ENGINE_RAYS}
        ${HINA_RENDER_ENGINE_UTIL}
        ${HINA_RENDER_ENGINE_SCENE}
        ${HINA_RENDER_ENGINE_LIB}
        ${HINA_RENDER_ENGINE_TO_IMPL}
        ${HINA_RENDER_ENGINE_PLATFORM}
        physics-system-api.h
        app.cpp
        app.h
        platform/icon.rc
        )

add_library(HinaPE_Render_Engine ${HINA_RENDER_ENGINE_SRC})
target_compile_definitions(HinaPE_Render_Engine PUBLIC HINAPE_SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shader/")
set_target_properties(HinaPE_Render_Engine PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
target_link_libraries(HinaPE_Render_Engine PUBLIC HinaPE_Common)

# setup OS-specific options
if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()
if (APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "-framework AppKit")
    find_package(SDL2 REQUIRED)
    target_include_directories(HinaPE_Render_Engine PUBLIC ${SDL2_INCLUDE_DIRS}/..)
    target_include_directories(HinaPE_Render_Engine PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_directories(HinaPE_Render_Engine PUBLIC ${SDL2_LIBRARY_DIRS})
    add_definitions(${SDL2_CFLAGS_OTHER})
endif ()
if (LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    target_include_directories(HinaPE_Render_Engine PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_directories(HinaPE_Render_Engine PUBLIC ${SDL2_LIBRARY_DIRS})
    add_definitions(${SDL2_CFLAGS_OTHER})
endif ()
if (MSVC)
    #    target_compile_options(HinaPE_Render_Engine PUBLIC /MP /W4 /WX /wd4201 /wd4840 /wd4100 /wd4505 /fp:fast)
    #    target_compile_options(HinaPE_Render_Engine PUBLIC /wd4127 /wd4819 /wd4189 /wd4459 /wd5054 /wd4458 /MP /W4 /WX /wd4201 /wd4840 /wd4100 /wd4505 /fp:fast)
else ()
    #    target_compile_options(HinaPE_Render_Engine PUBLIC -Wall -Wextra -Werror -Wno-reorder -Wno-unused-function -Wno-unused-parameter -Wordered-compare-function-pointers)
    #    target_compile_options(HinaPE_Render_Engine PUBLIC -Wall -Wextra -Wno-reorder -Wno-unused-function -Wno-unused-parameter)
endif ()
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(HinaPE_Render_Engine PUBLIC -fno-omit-frame-pointer)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
endif ()

# build dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(HinaPE_Render_Engine PUBLIC Threads::Threads) # std::thread need pthread lib to be linked
if (NOT TARGET imgui)
    add_subdirectory("deps/imgui/" "${CMAKE_CURRENT_BINARY_DIR}/imgui")
    target_include_directories(imgui PUBLIC ../render-engine/)
endif ()
if (NOT TARGET glad)
    add_subdirectory("deps/glad/" "${CMAKE_CURRENT_BINARY_DIR}/glad")
    target_include_directories(glad PUBLIC deps)
endif ()
if (NOT TARGET nfd)
    add_subdirectory("deps/nfd/" "${CMAKE_CURRENT_BINARY_DIR}/nfd")
endif ()
if (NOT TARGET sf_libs)
    add_subdirectory("deps/sf_libs/" "${CMAKE_CURRENT_BINARY_DIR}/sf_libs")
endif ()
if (NOT TARGET assimp)
    set(ASSIMP_BUILD_COLLADA_IMPORTER TRUE)
    set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)
    set(ASSIMP_BUILD_PLY_IMPORTER TRUE)
    set(ASSIMP_BUILD_FBX_IMPORTER TRUE)
    set(ASSIMP_BUILD_GLTF_IMPORTER TRUE)
    set(ASSIMP_BUILD_3DS_IMPORTER TRUE)
    set(ASSIMP_BUILD_STL_IMPORTER TRUE)
    set(ASSIMP_BUILD_BLEND_IMPORTER TRUE)
    set(ASSIMP_BUILD_COLLADA_EXPORTER TRUE)
    add_subdirectory("deps/assimp/" "${CMAKE_CURRENT_BINARY_DIR}/assimp")
    if (APPLE)
        target_compile_options(assimp PUBLIC -Wno-error)
    endif ()
endif ()
target_include_directories(HinaPE_Render_Engine PUBLIC ${ASSIMP_INCLUDE_DIRS})

# define include paths
target_include_directories(HinaPE_Render_Engine PUBLIC "deps" "deps/assimp/include" "deps/eigen3")
target_include_directories(HinaPE_Render_Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(HinaPE_Render_Engine PUBLIC "${HINAPE_COMMON_DIR}")

# link libraries
if (WIN32)
    target_include_directories(HinaPE_Render_Engine PUBLIC "deps/win")
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    target_link_libraries(HinaPE_Render_Engine PUBLIC deps/win/SDL2/SDL2main)
    target_link_libraries(HinaPE_Render_Engine PUBLIC deps/win/SDL2/SDL2)
    target_link_libraries(HinaPE_Render_Engine PUBLIC Winmm)
    target_link_libraries(HinaPE_Render_Engine PUBLIC Version)
    target_link_libraries(HinaPE_Render_Engine PUBLIC Setupapi)
    target_link_libraries(HinaPE_Render_Engine PUBLIC Shcore)
endif ()
if (LINUX)
    target_link_libraries(HinaPE_Render_Engine PUBLIC SDL2)
endif ()
if (APPLE)
    target_link_libraries(HinaPE_Render_Engine PUBLIC ${SDL2_LIBRARIES})
endif ()
target_link_libraries(HinaPE_Render_Engine PUBLIC assimp)
target_link_libraries(HinaPE_Render_Engine PUBLIC nfd)
target_link_libraries(HinaPE_Render_Engine PUBLIC sf_libs)
target_link_libraries(HinaPE_Render_Engine PUBLIC imgui)
target_link_libraries(HinaPE_Render_Engine PUBLIC glad)

# Load Fluid Engine
if (FluidEngineOption)
    add_subdirectory(${HINAPE_PHYSICS_DIR}/fluid-engine ${CMAKE_BINARY_DIR}/physics-engine/fluid-engine)
    target_include_directories(HinaPE_Render_Engine PUBLIC "${HINAPE_PHYSICS_DIR}/fluid-engine")
    target_link_libraries(HinaPE_Render_Engine PUBLIC HinaPE_Fluid_Engine)
endif ()

# Load Cloth Engine
if (ClothEngineOption)
    add_subdirectory(${HINAPE_PHYSICS_DIR}/cloth-engine ${CMAKE_BINARY_DIR}/physics-engine/cloth-engine)
    target_include_directories(HinaPE_Render_Engine PUBLIC "${HINAPE_PHYSICS_DIR}/cloth-engine")
    target_link_libraries(HinaPE_Render_Engine PUBLIC HinaPE_Cloth_Engine)
endif ()

# Load RigidBody Engine
if (RigidBodyEngineOption)
    add_subdirectory(${HINAPE_PHYSICS_DIR}/rigidbody-engine ${CMAKE_BINARY_DIR}/physics-engine/rigidbody-engine)
    target_include_directories(HinaPE_Render_Engine PUBLIC "${HINAPE_PHYSICS_DIR}/rigidbody-engine")
    target_link_libraries(HinaPE_Render_Engine PUBLIC HinaPE_RigidBody_Engine)
endif ()

# Render Engine Test Part
#add_executable(HinaPE_Render_Engine_Test main.cpp)
#set_target_properties(HinaPE_Render_Engine_Test PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
#target_link_libraries(HinaPE_Render_Engine_Test PUBLIC HinaPE_Render_Engine)
