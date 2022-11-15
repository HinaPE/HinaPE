cmake_minimum_required(VERSION 3.16)
project(HinaPE_RigidBody_Engine VERSION 1.1.3 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_COMMON_DIR "../../common")
set(HINAPE_DEPS_DIR "../deps")

if (NOT TARGET HinaPE_Common)
    add_subdirectory(${HINAPE_COMMON_DIR} HinaPE_Common)
endif ()

file(GLOB_RECURSE HINA_RIGIDBODY_ENGINE_SOLVER "solver/*.cpp" "solver/*.h" "solver/*.inl.h")

file(GLOB_RECURSE HINA_RIGIDBODY_ENGINE_SRC
        ${HINA_RIGIDBODY_ENGINE_SOLVER}
        )

add_library(HinaPE_RigidBody_Engine ${HINA_RIGIDBODY_ENGINE_SRC})
set_target_properties(HinaPE_RigidBody_Engine PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
target_include_directories(HinaPE_RigidBody_Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${HINAPE_COMMON_DIR})
target_link_libraries(HinaPE_RigidBody_Engine PUBLIC HinaPE_Common)

# RigidBody Engine Test Part
#get_directory_property(HINA_RIGIDBODY_ENGINE_PARENT_DIR PARENT_DIRECTORY)
#if (NOT HINA_RIGIDBODY_ENGINE_PARENT_DIR)
#    add_executable(HinaPE_RigidBody_Engine_Test main.cpp)
#    set_target_properties(HinaPE_RigidBody_Engine_Test PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
#    target_link_libraries(HinaPE_RigidBody_Engine_Test PUBLIC HinaPE_RigidBody_Engine)
#endif ()
