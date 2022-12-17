cmake_minimum_required(VERSION 3.16)
project(200-RigidBody VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(200-RigidBody main.cpp api.h api.cpp)
set_target_properties(200-RigidBody PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(200-RigidBody PUBLIC HinaPE_Rigid Kasumi_renderer)
target_compile_definitions(
        200-RigidBody
        PUBLIC
        -DMyAssetDir="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)