cmake_minimum_required(VERSION 3.24)
project(201-VolumeRender VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(201-VolumeRender main.cpp)
set_target_properties(201-VolumeRender PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(201-VolumeRender PUBLIC Kasumi_ShaderPainter)
target_compile_definitions(
        201-VolumeRender
        PUBLIC
        MyShaderDir="${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
)