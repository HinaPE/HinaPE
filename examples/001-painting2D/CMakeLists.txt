cmake_minimum_required(VERSION 3.18)
project(001-Painting2D VERSION 1.50 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(001-Painting2D main.cpp)
set_target_properties(001-Painting2D PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(001-Painting2D PUBLIC Kasumi_renderer)
target_compile_definitions(
        001-Painting2D
        PUBLIC
        -DMyShaderDir="${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
)