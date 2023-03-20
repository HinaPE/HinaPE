cmake_minimum_required(VERSION 3.24)
project(201-VolumeRender VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(201-VolumeRender main.cpp)
set_target_properties(201-VolumeRender PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(201-VolumeRender PUBLIC Kasumi_ShaderPainter)
include(../../kasumi/CMakeRC.cmake)
cmrc_add_resource_library(
        VolumeRender_Rc
        NAMESPACE VolumeRender
        shaders/cloud.glsl
)
target_link_libraries(
        201-VolumeRender
        PUBLIC
        VolumeRender_Rc
)