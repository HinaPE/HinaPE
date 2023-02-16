cmake_minimum_required(VERSION 3.18)
project(HinaPE_Cloth VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

if (NOT TARGET HinaPE_Common)
    message(FATAL_ERROR "HinaPE_Common is not found")
    return()
endif ()

file(GLOB_RECURSE src
        pbd/solver.h pbd/solver.cpp
        pbd/constraints.h pbd/constraints.cpp
        )
add_library(HinaPE_Cloth ${src})
set_target_properties(HinaPE_Cloth PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Cloth PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_Cloth PUBLIC HinaPE_Common)

if (NOT TARGET Kasumi_Render3D)
    message(FATAL_ERROR "Kasumi_Render3D is not found")
    return()
endif ()

# APP EXECUTABLE
add_executable(HinaPE_Cloth_App main.cpp)
set_target_properties(HinaPE_Cloth_App PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Cloth_App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_Cloth_App PUBLIC HinaPE_Cloth Kasumi_Render3D)