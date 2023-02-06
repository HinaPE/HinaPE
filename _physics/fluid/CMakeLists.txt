cmake_minimum_required(VERSION 3.18)
project(HINAPE_FLUID VERSION 1.50 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

if (NOT TARGET HinaPE_Common)
    message(FATAL_ERROR "HinaPE_Common is not found. Please build HinaPE_Common first.")
    return()
endif ()

file(GLOB_RECURSE SUB_SOLVERS_SRC "sub_solver/*.cpp" "sub_solver/*.h")
file(GLOB_RECURSE EMITTERS_SRC "emitter/*.cpp" "emitter/*.h")
file(GLOB SOLVERS_SRC "*.cpp" "*.h")

add_library(HinaPE_fluid STATIC ${SUB_SOLVERS_SRC} ${EMITTERS_SRC} ${SOLVERS_SRC})
set_target_properties(HinaPE_fluid PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_fluid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_fluid PUBLIC HinaPE_Common)