cmake_minimum_required(VERSION 3.18)
project(HINAPE_FLUID VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

if (NOT TARGET HINA_COMMON)
    return()
endif ()

file(GLOB_RECURSE SOLVERS_SRC "sub_solver/*.cpp" "sub_solver/*.h")
file(GLOB_RECURSE EMITTERS_SRC "emitter/*.cpp" "emitter/*.h")

add_library(HINAPE_FLUID STATIC ${SOLVERS_SRC} ${EMITTERS_SRC})
set_target_properties(HINAPE_FLUID PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(HINAPE_FLUID PRIVATE HINA_COMMON)