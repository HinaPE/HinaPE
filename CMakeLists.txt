cmake_minimum_required(VERSION 3.24)
project(HinaPE VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

IF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install" CACHE PATH "HinaPE install directory" FORCE)
ENDIF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

if (APPLE)
    set(ENV{TAICHI_C_API_INSTALL_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/taichi/c_api_macOS_M1)
    add_subdirectory(taichi)
endif ()

# renderers backend
set(KASUMI_RENDERER_2D TRUE)
set(KASUMI_RENDERER_3D TRUE)
set(KASUMI_SHADER_PAINTER TRUE)
set(KASUMI_VOLUME_RENDERER TRUE)
set(KASUMI_FUNCTION_VIEWER TRUE)
set(KASUMI_DEFERRED_RENDERER_3D TRUE)
add_subdirectory(kasumi)

# physics engines
add_subdirectory(physics/cloth)
add_subdirectory(physics/rigid)
add_subdirectory(physics/rigid-2d)
add_subdirectory(physics/fluid-particles)
add_subdirectory(physics/fluid-grids)
add_subdirectory(physics/fluid-hybrid)

# tutorials
add_subdirectory(tutorials)
