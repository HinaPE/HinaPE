cmake_minimum_required(VERSION 3.18)
project(300-SPH_Fluid VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(300-SPH-Fluid main.cpp api.h api.cpp)
set_target_properties(300-SPH-Fluid PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(300-SPH-Fluid PUBLIC HinaPE_Fluid Kasumi_renderer)
target_compile_definitions(
        300-SPH-Fluid
        PUBLIC
        -DMyAssetDir="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)