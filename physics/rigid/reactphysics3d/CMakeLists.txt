cmake_minimum_required(VERSION 3.18)
project(react-physics VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

file(GLOB_RECURSE REACT_PHYSICS_SRC *.h *.cpp)

add_library(react-physics ${REACT_PHYSICS_SRC})
set_target_properties(react-physics PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(react-physics PUBLIC ../)
target_include_directories(react-physics PUBLIC .)