cmake_minimum_required(VERSION 3.18)
project(302-grid-smoke VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(302-grid-smoke main.cpp)
set_target_properties(302-grid-smoke PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(302-grid-smoke PUBLIC Kasumi_renderer)
