cmake_minimum_required(VERSION 3.24)
project(302-Smoke-Fedkiw2001 VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(302-Smoke-Fedkiw2001 main.cpp)
set_target_properties(302-Smoke-Fedkiw2001 PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(302-Smoke-Fedkiw2001 PUBLIC HinaPE_Fluid_Grids)