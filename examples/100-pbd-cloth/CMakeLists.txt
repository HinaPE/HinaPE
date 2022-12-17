cmake_minimum_required(VERSION 3.16)
project(100-PBD-Cloth VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(100-PBD-Cloth main.cpp api.h api.cpp)
set_target_properties(100-PBD-Cloth PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(100-PBD-Cloth PUBLIC HinaPE_Cloth Kasumi_renderer)
target_compile_definitions(
        100-PBD-Cloth
        PUBLIC
        -DMyAssetDir="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)