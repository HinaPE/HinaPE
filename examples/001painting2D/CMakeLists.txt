cmake_minimum_required(VERSION 3.16)
project(Painting2D VERSION 1.21 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(Example-Painting2D main.cpp)
set_target_properties(Example-Painting2D PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(Example-Painting2D PUBLIC Kasumi_renderer)
target_compile_definitions(
        Example-Painting2D
        PUBLIC
        -DMyShaderDir="${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
)