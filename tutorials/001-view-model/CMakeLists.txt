cmake_minimum_required(VERSION 3.24)
project(001-ViewModel VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(001-ViewModel main.cpp)
set_target_properties(001-ViewModel PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(001-ViewModel PUBLIC Kasumi_Render3D)
