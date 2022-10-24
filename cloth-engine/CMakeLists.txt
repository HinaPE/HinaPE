cmake_minimum_required(VERSION 3.16)
project(HinaPE_Cloth_Engine VERSION 1.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_DEPS_DIR "../deps")

file(GLOB_RECURSE HINA_CLOTH_ENGINE_SOLVER "solver/*.cpp" "solver/*.h" "solver/*.inl.h")

file(GLOB_RECURSE HINA_CLOTH_ENGINE_SRC
        ${HINA_CLOTH_ENGINE_SOLVER}
        )

add_library(HinaPE_Cloth_Engine ${HINA_CLOTH_ENGINE_SRC})
set_target_properties(HinaPE_Cloth_Engine PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
target_include_directories(HinaPE_Cloth_Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/util)
include_directories("util")

get_directory_property(HINA_CLOTH_ENGINE_PARENT_DIR PARENT_DIRECTORY)
if (NOT HINA_CLOTH_ENGINE_PARENT_DIR)
    add_executable(HinaPE_Cloth_Engine_Test main.cpp)
    set_target_properties(HinaPE_Cloth_Engine_Test PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
    target_link_libraries(HinaPE_Cloth_Engine_Test PUBLIC HinaPE_Cloth_Engine)
endif ()
