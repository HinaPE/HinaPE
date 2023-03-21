cmake_minimum_required(VERSION 3.24)
project(301-PBF-Akinci2012 VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(301-PBF-Akinci2012 main.cpp)
set_target_properties(301-PBF-Akinci2012 PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(301-PBF-Akinci2012 PUBLIC HinaPE_Fluid_Particles)