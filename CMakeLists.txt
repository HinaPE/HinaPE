cmake_minimum_required(VERSION 3.16)
project(HinaPE VERSION 1.0.1 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

file(GLOB_RECURSE HinaGUI "src/gui/*.cpp" "src/gui/*.h")
file(GLOB_RECURSE HinaGEOM "src/geometry/*.cpp" "src/geometry/*.h" "src/geometry/*.inl")
file(GLOB_RECURSE HinaRAYS "src/rays/*.cpp" "src/rays/*.h")
file(GLOB_RECURSE HinaUTIL "src/util/*.cpp" "src/util/*.h")
file(GLOB_RECURSE HinaSCENE "src/scene/*.cpp" "src/scene/*.h")
file(GLOB_RECURSE HinaLIB "src/lib/*.cpp" "src/lib/*.h")
file(GLOB_RECURSE HinaPHYSICS "src/physics/*.cpp" "src/physics/*.h" "src/physics/*.inl")
file(GLOB_RECURSE HinaTOIMPL "src/reference/*.cpp" "src/reference/*.h" "src/reference/*.inl")

set(HinaPLATFORM
        "src/platform/gl.cpp"
        "src/platform/platform.cpp"
        "src/platform/gl.h"
        "src/platform/platform.h"
        "deps/imgui/imgui_impl_opengl3.cpp"
        "deps/imgui/imgui_impl_opengl3.h"
        "deps/imgui/imgui_impl_sdl.cpp"
        "deps/imgui/imgui_impl_sdl.h")

set(Hina_SOURCES
        ${HinaGUI}
        ${HinaGEOM}
        ${HinaRAYS}
        ${HinaUTIL}
        ${HinaPLATFORM}
        ${HinaSCENE}
        ${HinaLIB}
        ${HinaPHYSICS}
        ${HinaTOIMPL}
        src/app.cpp
        src/app.h
        src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/icon.rc
        )



# setup OS-specific options

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if(APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "-framework AppKit")
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS}/..)
    include_directories(${SDL2_INCLUDE_DIRS})
    link_directories(${SDL2_LIBRARY_DIRS})
    add_definitions(${SDL2_CFLAGS_OTHER})
endif()

if(LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    include_directories(${SDL2_INCLUDE_DIRS})
    link_directories(${SDL2_LIBRARY_DIRS})
    add_definitions(${SDL2_CFLAGS_OTHER})
endif()

add_executable(HinaPE ${Hina_SOURCES})
set_target_properties(HinaPE PROPERTIES
        CXX_STANDARD 20
        CXX_EXTENSIONS OFF)

#if(MSVC)
#    target_compile_options(HinaPE PRIVATE /MP /W4 /WX /wd4201 /wd4840 /wd4100 /wd4505 /fp:fast)
#else()
##    target_compile_options(HinaPE PRIVATE -Wall -Wextra -Werror -Wno-reorder -Wno-unused-function -Wno-unused-parameter -Wordered-compare-function-pointers)
#    target_compile_options(HinaPE PRIVATE -Wall -Wextra -Wno-reorder -Wno-unused-function -Wno-unused-parameter)
#endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(HinaPE PRIVATE -fno-omit-frame-pointer)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(HinaPE PRIVATE Threads::Threads)



# define include paths

target_include_directories(HinaPE PRIVATE "deps/" "deps/assimp/include" "deps/eigen3")
target_include_directories(HinaPE PRIVATE "${CMAKE_BINARY_DIR}/deps/assimp/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/")

source_group(lib FILES ${HinaLIB})
source_group(scene FILES ${HinaSCENE})
source_group(platform FILES ${HinaPLATFORM})



# build dependencies

add_subdirectory("deps/imgui/")
add_subdirectory("deps/glad/")
add_subdirectory("deps/nfd/")
add_subdirectory("deps/sf_libs/")

set(ASSIMP_BUILD_COLLADA_IMPORTER TRUE)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)
set(ASSIMP_BUILD_PLY_IMPORTER TRUE)
set(ASSIMP_BUILD_FBX_IMPORTER TRUE)
set(ASSIMP_BUILD_GLTF_IMPORTER TRUE)
set(ASSIMP_BUILD_3DS_IMPORTER TRUE)
set(ASSIMP_BUILD_STL_IMPORTER TRUE)
set(ASSIMP_BUILD_BLEND_IMPORTER TRUE)
set(ASSIMP_BUILD_COLLADA_EXPORTER TRUE)
add_subdirectory("deps/assimp/")
include_directories(${ASSIMP_INCLUDE_DIRS})



# link libraries

if(WIN32)
    target_include_directories(HinaPE PRIVATE "deps/win")
#    if(MSVC)
#        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \"${CMAKE_CURRENT_SOURCE_DIR}/src/platform/icon.res\" /IGNORE:4098 /IGNORE:4099")
#    endif()
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    target_link_libraries(HinaPE PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/win/SDL2/SDL2main.lib")
    target_link_libraries(HinaPE PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/win/SDL2/SDL2.lib")
    target_link_libraries(HinaPE PRIVATE Winmm)
    target_link_libraries(HinaPE PRIVATE Version)
    target_link_libraries(HinaPE PRIVATE Setupapi)
    target_link_libraries(HinaPE PRIVATE Shcore)
endif()

if(LINUX)
    target_link_libraries(HinaPE PRIVATE SDL2)
endif()

if(APPLE)
    target_link_libraries(HinaPE PRIVATE ${SDL2_LIBRARIES})
endif()

target_link_libraries(HinaPE PRIVATE assimp)
target_link_libraries(HinaPE PRIVATE nfd)
target_link_libraries(HinaPE PRIVATE sf_libs)
target_link_libraries(HinaPE PRIVATE imgui)
target_link_libraries(HinaPE PRIVATE glad)
