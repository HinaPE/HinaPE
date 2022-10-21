cmake_minimum_required(VERSION 3.16)
project(HinaPE_Fluid_Engine VERSION 1.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_DEPS_DIR "../deps")

add_subdirectory("${HINAPE_DEPS_DIR}/tinyobjloader/" ${CMAKE_BINARY_DIR}/tinyobjloader)

file(GLOB_RECURSE HINA_FLUID_SRC "*.cpp" "*.h" "*.inl.h")
add_executable(HinaPE_Fluid_Engine ${HINA_FLUID_SRC})
include_directories("util")
target_include_directories(HinaPE_Fluid_Engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} "${HINAPE_DEPS_DIR}/tinyobjloader")
target_link_libraries(HinaPE_Fluid_Engine PRIVATE tinyobjloader)
set_target_properties(HinaPE_Fluid_Engine PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)

option(HINAPE_TBB "Enable TBB" OFF)
find_package(TBB CONFIG)
if (TBB_FOUND)
    message(STATUS ">> TBB found")
    option(HINAPE_TBB ON)
    add_definitions(-DJET_TASKING_TBB)
    target_link_libraries(HinaPE_Fluid_Engine PRIVATE TBB::tbb)
endif ()
