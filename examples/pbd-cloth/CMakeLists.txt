cmake_minimum_required(VERSION 3.16)
project(PBD-Cloth VERSION 1.21 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(Example-PBD-Cloth main.cpp api.h api.cpp)
set_target_properties(Example-PBD-Cloth PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(Example-PBD-Cloth PUBLIC HinaPE_Cloth Kasumi_renderer)
