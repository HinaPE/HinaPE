cmake_minimum_required(VERSION 3.24)
project(102-SimpleParticleSystem VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(102-SimpleParticleSystem main.cpp)
set_target_properties(102-SimpleParticleSystem PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(102-SimpleParticleSystem PUBLIC Kasumi_Render3D)
