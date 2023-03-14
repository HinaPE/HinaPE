cmake_minimum_required(VERSION 3.23)
project(101-SimpleFreeFall VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(101-SimpleFreeFall main.cpp)
set_target_properties(101-SimpleFreeFall PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(101-SimpleFreeFall PUBLIC Kasumi_Render3D)
