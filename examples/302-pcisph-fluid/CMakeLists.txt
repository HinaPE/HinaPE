cmake_minimum_required(VERSION 3.16)
project(302-PCISPH_Fluid VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(302-PCISPH_Fluid main.cpp api.h api.cpp)
set_target_properties(302-PCISPH_Fluid PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(302-PCISPH_Fluid PUBLIC HinaPE_Fluid Kasumi_renderer)
target_compile_definitions(
        302-PCISPH_Fluid
        PUBLIC
        -DMyAssetDir="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)