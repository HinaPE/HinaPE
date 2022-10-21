cmake_minimum_required(VERSION 3.16)
project(HinaPE_Fluid_Engine VERSION 1.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_DEPS_DIR "../deps")

add_subdirectory("${HINAPE_DEPS_DIR}/tinyobjloader/" ${CMAKE_BINARY_DIR}/tinyobjloader)

file(GLOB_RECURSE HINA_FLUID_ENGINE_ANIMATION "animation/*.cpp" "animation/*.h" "animation/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_ARRAY "array/*.cpp" "array/*.h" "array/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_EMITTER "emitter/*.cpp" "emitter/*.h" "emitter/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_FIELD "field/*.cpp" "field/*.h" "field/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_GEOMETRY "geometry/*.cpp" "geometry/*.h" "geometry/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_GRID "grid/*.cpp" "grid/*.h" "grid/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_MATH "math/*.cpp" "math/*.h" "math/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_POINT_GENERATOR "point_generator/*.cpp" "point_generator/*.h" "point_generator/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_QUERY_ENGINE "query_engine/*.cpp" "query_engine/*.h" "query_engine/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_SEARCHER "searcher/*.cpp" "searcher/*.h" "searcher/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_SOLVER "solver/*.cpp" "solver/*.h" "solver/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_UTIL "util/*.cpp" "util/*.h" "util/*.inl.h")

file(GLOB_RECURSE HINA_FLUID_ENGINE_SRC
        ${HINA_FLUID_ENGINE_ANIMATION}
        ${HINA_FLUID_ENGINE_ARRAY}
        ${HINA_FLUID_ENGINE_EMITTER}
        ${HINA_FLUID_ENGINE_FIELD}
        ${HINA_FLUID_ENGINE_GEOMETRY}
        ${HINA_FLUID_ENGINE_GRID}
        ${HINA_FLUID_ENGINE_MATH}
        ${HINA_FLUID_ENGINE_POINT_GENERATOR}
        ${HINA_FLUID_ENGINE_QUERY_ENGINE}
        ${HINA_FLUID_ENGINE_SEARCHER}
        ${HINA_FLUID_ENGINE_SOLVER}
        ${HINA_FLUID_ENGINE_UTIL}
        )

add_library(HinaPE_Fluid_Engine ${HINA_FLUID_ENGINE_SRC})
set_target_properties(HinaPE_Fluid_Engine PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
target_include_directories(HinaPE_Fluid_Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/util "${HINAPE_DEPS_DIR}/tinyobjloader")
target_link_libraries(HinaPE_Fluid_Engine PUBLIC tinyobjloader)
include_directories("util")

option(HINAPE_TBB "Enable TBB" OFF)
find_package(TBB CONFIG)
if (TBB_FOUND)
    message(STATUS ">> TBB found")
    option(HINAPE_TBB ON)
    add_definitions(-DJET_TASKING_TBB)
    target_link_libraries(HinaPE_Fluid_Engine PUBLIC TBB::tbb)
endif ()

# INSTALL
MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
FOREACH(subdir ${SUBDIRS})
    INSTALL(
            DIRECTORY ${subdir}
            DESTINATION include/${CMAKE_PROJECT_NAME}/render-engine
            FILES_MATCHING PATTERN "*.h*"
    )
ENDFOREACH()

# Fluid Engine Test Part
add_executable(HinaPE_Fluid_Engine_Test main.cpp)
set_target_properties(HinaPE_Fluid_Engine_Test PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
target_link_libraries(HinaPE_Fluid_Engine_Test PRIVATE HinaPE_Fluid_Engine)
