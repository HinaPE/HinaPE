cmake_minimum_required(VERSION 3.16)
project(HinaPE VERSION 1.21 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

option(HinaPE_CLOTH_ENGINE "Load HinaPE Cloth Engine" ON)
option(HinaPE_RIGIDBODY_ENGINE "Load HinaPE Rigidbody Engine" ON)
option(HinaPE_FLUID_ENGINE "Load HinaPE Fluid Engine" ON)
option(HinaPE_USE_KASUMI "Use Kasumi Renderer" ON)
option(HinaPE_EXAMPLES "Load HinaPE Examples" OFF)

if (HinaPE_CLOTH_ENGINE)
    add_subdirectory(engine/cloth)
endif ()

if (HinaPE_RIGIDBODY_ENGINE)
    add_subdirectory(engine/fluid)
endif ()

if (HinaPE_FLUID_ENGINE)
    add_subdirectory(engine/rigid)
endif ()

if (HinaPE_USE_KASUMI)
    add_subdirectory(kasumi)
endif ()

if (HinaPE_EXAMPLES)
    add_subdirectory(examples)
endif ()
