cmake_minimum_required(VERSION 3.18)
project(101-freefall VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(101-freefall main.cpp)
set_target_properties(101-freefall PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(101-freefall PUBLIC Kasumi_Render3D)
