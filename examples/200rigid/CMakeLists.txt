cmake_minimum_required(VERSION 3.16)
project(RigidBody VERSION 1.21 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(Example-RigidBody main.cpp api.h api.cpp)
set_target_properties(Example-RigidBody PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(Example-RigidBody PUBLIC HinaPE_Rigid Kasumi_renderer)
target_compile_definitions(
        Example-RigidBody
        PUBLIC
        -DMyAssetDir="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)