cmake_minimum_required(VERSION 3.24)
project(002-View2DMathFunction VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(002-View2DMathFunction main.cpp)
set_target_properties(002-View2DMathFunction PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(002-View2DMathFunction PUBLIC Kasumi_Render2D)
