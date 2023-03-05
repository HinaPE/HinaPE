cmake_minimum_required(VERSION 3.18)
project(HinaPE VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_subdirectory(kasumi)
add_subdirectory(physics/rigid/reactphysics3d)
add_subdirectory(physics/cloth)
add_subdirectory(physics/fluid)
add_subdirectory(tutorials)
