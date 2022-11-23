cmake_minimum_required(VERSION 3.16)
project(HINAPE_FLUID VERSION 1.21 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_COMMON_DIR "../../common")
set(HINAPE_DEPS_DIR "../deps")

if (NOT TARGET HinaPE_Common)
    add_subdirectory(${HINAPE_COMMON_DIR} HinaPE_Common)
endif ()

file(GLOB_RECURSE HINA_FLUID_ENGINE_EMITTER "emitter/*.cpp" "emitter/*.h" "emitter/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_FDM "FDM/*.cpp" "FDM/*.h" "FDM/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_POINT_GENERATOR "point_generator/*.cpp" "point_generator/*.h" "point_generator/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_SEARCHER "searcher/*.cpp" "searcher/*.h" "searcher/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_SOLVER "solver/*.cpp" "solver/*.h" "solver/*.inl.h")

file(GLOB_RECURSE HINA_FLUID_ENGINE_SRC
        ${HINA_FLUID_ENGINE_EMITTER}
        ${HINA_FLUID_ENGINE_FDM}
        ${HINA_FLUID_ENGINE_POINT_GENERATOR}
        ${HINA_FLUID_ENGINE_SEARCHER}
        ${HINA_FLUID_ENGINE_SOLVER}
        app.cpp
        app.h
        factory.cpp
        factory.h
        )

add_library(HinaPE_Fluid ${HINA_FLUID_ENGINE_SRC})
set_target_properties(HinaPE_Fluid PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
target_include_directories(HinaPE_Fluid PUBLIC ${HINAPE_COMMON_DIR} ${HINAPE_DEPS_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_include_directories(HinaPE_Fluid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_Fluid PUBLIC HinaPE_Common)
