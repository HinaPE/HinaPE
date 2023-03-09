cmake_minimum_required(VERSION 3.18)
project(000-HeartBeats VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(000-HeartBeats main.cpp icon.rc)
set_target_properties(000-HeartBeats PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(000-HeartBeats PUBLIC Kasumi_Render3D)
target_compile_definitions(
        000-HeartBeats
        PUBLIC
        -DMyShaderDir="${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
)