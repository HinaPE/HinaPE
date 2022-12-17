cmake_minimum_required(VERSION 3.16)
project(301-ParticleSystem VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(301-ParticleSystem main.cpp api.h api.cpp)
set_target_properties(301-ParticleSystem PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(301-ParticleSystem PUBLIC HINAPE_FLUID_EXPERIMENTAL Kasumi_renderer)
target_compile_definitions(
        301-ParticleSystem
        PUBLIC
        -DMyAssetDir="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)