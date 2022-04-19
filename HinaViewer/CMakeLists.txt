# Detects whether this is a top-level project
get_directory_property(HINAVIEWER_PARENT_DIR PARENT_DIRECTORY)
if (NOT HINAVIEWER_PARENT_DIR)
    set(HINAVIEWER_TOPLEVEL_PROJECT ON)
else ()
    set(HINAVIEWER_TOPLEVEL_PROJECT OFF)
endif ()

set(REQUIRED_CMAKE_VERSION "3.16.0")
if (HINAVIEWER_TOPLEVEL_PROJECT)
    cmake_minimum_required(VERSION ${REQUIRED_CMAKE_VERSION})
else ()
    if (${CMAKE_VERSION} VERSION_LESS ${REQUIRED_CMAKE_VERSION})
        message(FATAL_ERROR "CMake required version to build HinaViewer is ${REQUIRED_CMAKE_VERSION}")
    endif ()
endif ()

option(HINA_TEST "WHETHER ENABLE TEST FOR HINAPE" ON)

project(HinaViewer VERSION 2.0.0)

option(LIBIGL_OPENGL "Use OpenGL" ON)
option(LIBIGL_GLFW "Use GLFW" ON)
option(LIBIGL_IMGUI "Use ImGui" ON)
include(cmake/libigl.cmake)

if (HINA_TEST)
    include(cmake/googletest.cmake)
endif ()

file(GLOB HinaViewerSrc
        *.h *.cpp
        api/*.h api/*.cpp
        )

add_library(HinaViewer ${HinaViewerSrc})
target_link_libraries(
        HinaViewer
        PUBLIC
        igl::core
        igl::opengl
        igl::glfw
        igl::imgui
        igl_copyleft_core
        igl_copyleft_cgal
        pmp
)
