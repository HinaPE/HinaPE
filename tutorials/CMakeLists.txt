cmake_minimum_required(VERSION 3.24)
project(HinaPE-tutorials VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

# tutorials
add_subdirectory(000-heart-beats)
add_subdirectory(001-view-model)
add_subdirectory(002-view-2D-math-function)
add_subdirectory(003-view-3D-math-function)
add_subdirectory(004-sakura-scattering)
add_subdirectory(005-deferred-rendering)
add_subdirectory(006-triangle-mesh-geometry)

# physics applications
add_subdirectory(101-simple-freefall)
add_subdirectory(102-simple-particle-system)

# rendering applications
add_subdirectory(201-volume-render)
add_subdirectory(202-screen-space-fluids)

# fluid applications
add_subdirectory(301-pbf-akinci2012)
add_subdirectory(302-smoke-fedkiw2001)
add_subdirectory(303-pcipsh-akinci2012)
add_subdirectory(304-non-newtonian-fluids)

# advanced applications
add_subdirectory(401-impl-cloud-doba2000)

# digital image processing applications
add_subdirectory(501-image-orthogonal-transform)