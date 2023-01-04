cmake_minimum_required(VERSION 3.16)
project(303-grid-based_Fluid2D VERSION 1.21 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(303-grid-based-Fluid2D main.cpp api.h api.cpp)
set_target_properties(303-grid-based-Fluid2D PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(303-grid-based-Fluid2D PUBLIC HinaPE_Fluid Kasumi_renderer)
target_compile_definitions(
        303-grid-based-Fluid2D
        PUBLIC
        -DMyAssetDir="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)