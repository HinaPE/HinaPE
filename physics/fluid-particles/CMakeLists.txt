cmake_minimum_required(VERSION 3.18)
project(HinaPE_Fluid_Particles VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

if (NOT TARGET Kasumi_Render3D)
    message(FATAL_ERROR "Kasumi_Render3D is not found")
    return()
endif ()

file(GLOB_RECURSE src
        solver.cpp solver.h
        domain/*.cpp domain/*.h
        )
add_library(HinaPE_Fluid_Particles ${src})
set_target_properties(HinaPE_Fluid_Particles PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Fluid_Particles PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_Fluid_Particles PUBLIC Kasumi_Render3D)

# APP EXECUTABLE
add_executable(HinaPE_Fluid_Particles_App main.cpp)
set_target_properties(HinaPE_Fluid_Particles_App PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Fluid_Particles_App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_Fluid_Particles_App PUBLIC HinaPE_Fluid_Particles)