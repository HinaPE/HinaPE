cmake_minimum_required(VERSION 3.24)
project(HinaPE VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install" CACHE PATH "HinaPE install directory" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

add_subdirectory(kasumi)
add_subdirectory(physics/cloth)
add_subdirectory(physics/rigid)
add_subdirectory(physics/rigid-2d)
add_subdirectory(physics/fluid-particles)
add_subdirectory(physics/fluid-grids)
add_subdirectory(tutorials)
