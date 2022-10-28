cmake_minimum_required(VERSION 3.16)
project(HinaPE_Fluid_Engine VERSION 1.1.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_COMMON_DIR "../../common")
set(HINAPE_DEPS_DIR "../../deps")

if (NOT TARGET HinaPE_Common)
    add_subdirectory(${HINAPE_COMMON_DIR} HinaPE_Common)
endif ()

file(GLOB_RECURSE HINA_FLUID_ENGINE_EMITTER "emitter/*.cpp" "emitter/*.h" "emitter/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_POINT_GENERATOR "point_generator/*.cpp" "point_generator/*.h" "point_generator/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_SEARCHER "searcher/*.cpp" "searcher/*.h" "searcher/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_SOLVER "solver/*.cpp" "solver/*.h" "solver/*.inl.h")

file(GLOB_RECURSE HINA_FLUID_ENGINE_SRC
        ${HINA_FLUID_ENGINE_EMITTER}
        ${HINA_FLUID_ENGINE_POINT_GENERATOR}
        ${HINA_FLUID_ENGINE_SEARCHER}
        ${HINA_FLUID_ENGINE_SOLVER}
        app.cpp
        app.h
        factory.cpp
        factory.h
        )

add_library(HinaPE_Fluid_Engine ${HINA_FLUID_ENGINE_SRC})
set_target_properties(HinaPE_Fluid_Engine PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
target_include_directories(HinaPE_Fluid_Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${HINAPE_COMMON_DIR})
target_link_libraries(HinaPE_Fluid_Engine PUBLIC HinaPE_Common)

# Fluid Engine Test Part
get_directory_property(HINA_FLUID_ENGINE_PARENT_DIR PARENT_DIRECTORY)
if (NOT HINA_FLUID_ENGINE_PARENT_DIR)
    add_executable(HinaPE_Fluid_Engine_Test main.cpp)
    set_target_properties(HinaPE_Fluid_Engine_Test PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
    target_link_libraries(HinaPE_Fluid_Engine_Test PUBLIC HinaPE_Fluid_Engine)
endif ()
