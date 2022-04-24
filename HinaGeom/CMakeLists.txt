# Detects whether this is a top-level project
get_directory_property(HINAGEOM_PARENT_DIR PARENT_DIRECTORY)
if (NOT HINAGEOM_PARENT_DIR)
    set(HINAGEOM_TOPLEVEL_PROJECT ON)
else ()
    set(HINAGEOM_TOPLEVEL_PROJECT OFF)
endif ()

set(REQUIRED_CMAKE_VERSION "3.16.0")
if (HINAGEOM_TOPLEVEL_PROJECT)
    cmake_minimum_required(VERSION ${REQUIRED_CMAKE_VERSION})
else ()
    if (${CMAKE_VERSION} VERSION_LESS ${REQUIRED_CMAKE_VERSION})
        message(FATAL_ERROR "CMake required version to build HinaGeom is ${REQUIRED_CMAKE_VERSION}")
    endif ()
endif ()

option(HINA_TEST "WHETHER ENABLE TEST FOR HINAPE" ON)

project(HinaGeom VERSION 2.0.0)

option(LIBIGL_OPENGL "Use OpenGL" ON)
option(LIBIGL_GLFW "Use GLFW" ON)
option(LIBIGL_IMGUI "Use ImGui" ON)
include(cmake/libigl.cmake)

if (HINA_TEST)
    include(cmake/googletest.cmake)
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB HinaGeomSrc
        simple_shapes/*.h simple_shapes/*.cpp
        triangle_meshes/*.h triangle_meshes/*.cpp
        )
add_subdirectory(pmp)
add_library(HinaGeom ${HinaGeomSrc})
target_include_directories(HinaGeom PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaGeom PRIVATE pmp)

if (HINA_TEST)
    add_executable(shape_test unit_test/shape_test.cpp)
    target_link_libraries(shape_test PRIVATE gtest_main HinaGeom)
    include(GoogleTest)
    gtest_discover_tests(shape_test)
endif ()