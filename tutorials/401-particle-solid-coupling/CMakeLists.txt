cmake_minimum_required(VERSION 3.24)
project(401-ParticleSolidCoupling VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(401-ParticleSolidCoupling main.cpp)
set_target_properties(401-ParticleSolidCoupling PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(401-ParticleSolidCoupling PUBLIC Kasumi_Render3D HinaPE_Rigid HinaPE_Fluid_Particles)
