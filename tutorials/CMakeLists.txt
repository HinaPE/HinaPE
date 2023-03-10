cmake_minimum_required(VERSION 3.18)
project(HinaPE-tutorials VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

# tutorials
add_subdirectory(000-heart-beats)
add_subdirectory(001-view-model)

# basic applications
add_subdirectory(101-freefall)

# advanced applications
add_subdirectory(401-particle-solid-coupling)