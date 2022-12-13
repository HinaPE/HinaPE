cmake_minimum_required(VERSION 3.16)
project(HinaPE-examples VERSION 1.21 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_subdirectory(pbd-cloth)
add_subdirectory(rigid)
add_subdirectory(sph-fluid)
add_subdirectory(painting2D)

add_subdirectory(kasumi-from-scratch)