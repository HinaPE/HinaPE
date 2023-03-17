cmake_minimum_required(VERSION 3.24)
project(HinaPE-tutorials VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

# tutorials
add_subdirectory(000-heart-beats)
add_subdirectory(001-view-model)
add_subdirectory(002-view-2D-math-function)
add_subdirectory(003-view-3D-math-function)

# physics applications
add_subdirectory(101-simple-freefall)
add_subdirectory(102-simple-particle-system)

# rendering applications
add_subdirectory(201-volume-render)

# advanced applications
add_subdirectory(401-particle-solid-coupling)