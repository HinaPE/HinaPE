cmake_minimum_required(VERSION 3.16)
project(HinaPE VERSION 1.0.1 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_subdirectory(render-engine)
add_subdirectory(fluid_engine)

add_executable(HinaPE HinaPE.cpp)
set_target_properties(HinaPE PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
target_link_libraries(HinaPE PRIVATE HinaPE_Render_Engine HinaPE_Fluid_Engine)
