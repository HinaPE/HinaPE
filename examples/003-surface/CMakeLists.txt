cmake_minimum_required(VERSION 3.16)
project(003-Surface LANGUAGES CXX)

add_executable(003-Surface main.cpp)
set_target_properties(003-Surface PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(003-Surface PUBLIC HINAPE_FLUID_EXPERIMENTAL Kasumi_renderer)
target_compile_definitions(
        003-Surface
        PUBLIC
        -DMyShaderDir="${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
)