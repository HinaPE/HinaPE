cmake_minimum_required(VERSION 3.24)
project(box2d VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

file(GLOB_RECURSE BOX2D_SRC *.h *.cpp)

add_library(box2d ${BOX2D_SRC})
set_target_properties(box2d PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(box2d PUBLIC include)