cmake_minimum_required(VERSION 3.18)
project(HinaPE_Fluid VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

if (NOT TARGET HinaPE_Common)
    message(FATAL_ERROR "HinaPE_Common is not found")
    return()
endif ()

if (NOT TARGET Kasumi_Render3D)
    message(FATAL_ERROR "Kasumi_Render3D is not found")
    return()
endif ()

file(GLOB_RECURSE src
        lagrangian/*.h lagrangian/*.cpp
        euler/*.h euler/*.cpp
        hybrid/*.h hybrid/*.cpp
        )
add_library(HinaPE_Fluid ${src})
set_target_properties(HinaPE_Fluid PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Fluid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_Fluid PUBLIC HinaPE_Common Kasumi_Render3D)

# APP EXECUTABLE
add_executable(HinaPE_Fluid_App main.cpp)
set_target_properties(HinaPE_Fluid_App PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Fluid_App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_Fluid_App PUBLIC HinaPE_Fluid)