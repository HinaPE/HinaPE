cmake_minimum_required(VERSION 3.18)
project(HinaPE-examples VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

# tutorial series
add_subdirectory(000-kasumi-from-scratch)
add_subdirectory(001-painting2D)
add_subdirectory(002-math-plot)
add_subdirectory(003-surface)
add_subdirectory(004-xyz-viewer)

# cloth & deformable series
add_subdirectory(100-pbd-cloth)

# rigid body series
add_subdirectory(200-rigid)

# fluid series
add_subdirectory(300-sph-fluid)
add_subdirectory(301-particle-system)
add_subdirectory(302-grid-based-fluid)
add_subdirectory(303-grid-based-fluid2D)

# unit test
add_subdirectory(999-unit-test)