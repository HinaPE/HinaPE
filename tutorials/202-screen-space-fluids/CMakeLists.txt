cmake_minimum_required(VERSION 3.24)
project(202-ScreenSpaceFluids VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(202-ScreenSpaceFluids main.cpp ssf_renderer.h ssf_renderer.cpp simple_fluid.h simple_fluid.cpp)
set_target_properties(202-ScreenSpaceFluids PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(202-ScreenSpaceFluids PUBLIC HinaPE_Fluid_Particles)
include(../../kasumi/CMakeRC.cmake)
cmrc_add_resource_library(
        ScreenSpaceFluids_Rc
        NAMESPACE ScreenSpaceFluids
        shaders/blur.frag
        shaders/blur.vert
        shaders/curvatureflow.frag
        shaders/particle.geo
        shaders/particle.vert
        shaders/particleDepth.frag
        shaders/particleNoise.frag
        shaders/particleThickness.frag
        shaders/quad.frag
        shaders/quad.vert
        shaders/quadThickness.frag
        shaders/quadtrans.frag
        shaders/quadtrans.vert
)
target_link_libraries(
        202-ScreenSpaceFluids
        PUBLIC
        ScreenSpaceFluids_Rc
)