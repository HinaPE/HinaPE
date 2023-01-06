cmake_minimum_required(VERSION 3.18)
project(PIC-Fluid VERSION 1.21 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(Example-PIC-Fluid main.cpp api.h api.cpp)
set_target_properties(Example-PIC-Fluid PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(Example-PIC-Fluid PUBLIC HinaPE_Fuild Kasumi_renderer)
target_compile_definitions(
        Example-PIC-Fluid
        PUBLIC
        -DMyAssetDir="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)