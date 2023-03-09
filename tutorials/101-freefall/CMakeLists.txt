cmake_minimum_required(VERSION 3.18)
project(101-FreeFall VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(101-FreeFall main.cpp)
set_target_properties(101-FreeFall PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(101-FreeFall PUBLIC Kasumi_Render3D)
