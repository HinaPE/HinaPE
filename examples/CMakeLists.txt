cmake_minimum_required(VERSION 3.16)
project(HinaPE-examples VERSION 1.21 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

# tutorial series
add_subdirectory(000-kasumi-from-scratch)
add_subdirectory(001-painting2D)
add_subdirectory(002-math-plot)

# cloth & deformable series
add_subdirectory(100-pbd-cloth)

# rigid body series
add_subdirectory(200-rigid)

# fluid series
add_subdirectory(300-sph-fluid)
add_subdirectory(301-particle-system)

#experimental series
add_subdirectory(999-fluid-experimental)