cmake_minimum_required(VERSION 3.16)
project(Fluid_Experimental VERSION 1.21 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(Example-Fluid-Experimental main.cpp api.h api.cpp)
set_target_properties(Example-Fluid-Experimental PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(Example-Fluid-Experimental PUBLIC HINAPE_FLUID_EXPERIMENTAL Kasumi_renderer)
target_compile_definitions(
        Example-Fluid-Experimental
        PUBLIC
        -DMyAssetDir="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)