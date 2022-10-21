cmake_minimum_required(VERSION 3.16)
project(HinaPE VERSION 1.0.1 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_subdirectory(render-engine)
add_subdirectory(fluid_engine)

add_library(HinaPE HinaPE.cpp)
target_link_directories(HinaPE PRIVATE HinaPE_Render_Engine HinaPE_Fluid_Engine)
