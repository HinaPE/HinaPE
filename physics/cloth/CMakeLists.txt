cmake_minimum_required(VERSION 3.16)
project(HINAPE_CLOTH VERSION 1.21 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_COMMON_DIR "../../common")
set(HINAPE_DEPS_DIR "../deps")

if (NOT TARGET HinaPE_Common)
    add_subdirectory(${HINAPE_COMMON_DIR} HinaPE_Common)
endif ()

file(GLOB_RECURSE HINA_CLOTH_ENGINE_FACTORY "factory/*.cpp" "factory/*.h" "factory/*.inl.h")
file(GLOB_RECURSE HINA_CLOTH_ENGINE_SOLVER "solver/*.cpp" "solver/*.h" "solver/*.inl.h")
file(GLOB_RECURSE HINA_CLOTH_ENGINE_UTIL "util/*.cpp" "util/*.h" "util/*.inl.h")

file(GLOB_RECURSE HINA_CLOTH_ENGINE_SRC
        ${HINA_CLOTH_ENGINE_SOLVER}
        ${HINA_CLOTH_ENGINE_FACTORY}
        ${HINA_CLOTH_ENGINE_UTIL}
        math_api.h
        )

add_library(HinaPE_Cloth ${HINA_CLOTH_ENGINE_SRC})
set_target_properties(HinaPE_Cloth PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
target_include_directories(HinaPE_Cloth PUBLIC ${HINAPE_COMMON_DIR} ${HINAPE_DEPS_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_include_directories(HinaPE_Cloth PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_Cloth PUBLIC HinaPE_Common)