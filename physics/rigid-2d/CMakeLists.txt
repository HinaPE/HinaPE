cmake_minimum_required(VERSION 3.18)
project(HINAPE_RIGID2D VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_COMMON_DIR "../../common")
set(HINAPE_DEPS_DIR "../deps")

if (NOT TARGET HinaPE_Common)
    add_subdirectory(${HINAPE_COMMON_DIR} HinaPE_Common)
endif ()

add_subdirectory(box2d-lite)

file(GLOB_RECURSE HINA_RIGIDBODY_2D_ENGINE_SOLVER "solver/*.cpp" "solver/*.h" "solver/*.inl.h")

file(GLOB_RECURSE HINA_RIGIDBODY_2D_ENGINE_SRC
        ${HINA_RIGIDBODY_2D_ENGINE_SOLVER}
        )

add_library(HinaPE_Rigid2D ${HINA_RIGIDBODY_2D_ENGINE_SRC})
set_target_properties(HinaPE_Rigid2D PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
target_include_directories(HinaPE_Rigid2D PUBLIC ${HINAPE_COMMON_DIR} ${HINAPE_DEPS_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_include_directories(HinaPE_Rigid2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_Rigid2D PUBLIC HinaPE_Common box2d-lite)
