cmake_minimum_required(VERSION 3.16)
project(002-MathPlot LANGUAGES CXX)

add_executable(002-MathPlot main.cpp)
set_target_properties(002-MathPlot PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(002-MathPlot PUBLIC Kasumi_renderer)
target_compile_definitions(
        002-MathPlot
        PUBLIC
        -DMyShaderDir="${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
)