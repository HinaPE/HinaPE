cmake_minimum_required(VERSION 3.24)
project(000-HeartBeats VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(000-HeartBeats main.cpp icon.rc)
set_target_properties(000-HeartBeats PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(000-HeartBeats PUBLIC Kasumi_ShaderPainter)
include(../../kasumi/CMakeRC.cmake)
cmrc_add_resource_library(
        HeartBeats_Rc
        NAMESPACE HeartBeats
        shaders/heart.glsl
)
target_link_libraries(
        000-HeartBeats
        PUBLIC
        HeartBeats_Rc
)