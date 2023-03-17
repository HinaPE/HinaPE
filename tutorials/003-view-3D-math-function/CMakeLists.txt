cmake_minimum_required(VERSION 3.24)
project(003-View3DMathFunction VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(003-View3DMathFunction main.cpp)
set_target_properties(003-View3DMathFunction PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(003-View3DMathFunction PUBLIC Kasumi_Render3D)
