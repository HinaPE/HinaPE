cmake_minimum_required(VERSION 3.16)
project(302-grid-based_Fluid VERSION 1.21 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(302-grid-based-Fluid main.cpp api.h api.cpp)
set_target_properties(302-grid-based-Fluid PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(302-grid-based-Fluid PUBLIC HinaPE_Fluid Kasumi_renderer)
target_compile_definitions(
        302-grid-based-Fluid
        PUBLIC
        -DMyAssetDir="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)