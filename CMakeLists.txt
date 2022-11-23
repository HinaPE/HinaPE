cmake_minimum_required(VERSION 3.16)
project(HinaPE VERSION 1.21 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)
set(HINAPE_PHYSICS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/engine)
set(HINAPE_RENDERER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/kasumi)

option(HinaPE_CLOTH_ENGINE "Load HinaPE Cloth Engine" ON)
option(HinaPE_RIGIDBODY_ENGINE "Load HinaPE Rigidbody Engine" ON)
option(HinaPE_FLUID_ENGINE "Load HinaPE Fluid Engine" ON)
option(HinaPE_USE_LEGACY_KASUMI "Use Legacy Kasumi Renderer" OFF)
option(HinaPE_EXAMPLES "Load HinaPE Examples" OFF)

if (HinaPE_CLOTH_ENGINE)
    add_subdirectory(${HINAPE_PHYSICS_DIR}/cloth)
endif ()

if (HinaPE_RIGIDBODY_ENGINE)
    add_subdirectory(${HINAPE_PHYSICS_DIR}/fluid)
endif ()

if (HinaPE_FLUID_ENGINE)
    add_subdirectory(${HINAPE_PHYSICS_DIR}/rigid)
endif ()

if (HinaPE_USE_LEGACY_KASUMI)
    add_library(${HINAPE_RENDERER_DIR})
endif ()

if (HinaPE_EXAMPLES)
    add_subdirectory(${HINAPE_COMMON_DIR}/examples)
endif ()
