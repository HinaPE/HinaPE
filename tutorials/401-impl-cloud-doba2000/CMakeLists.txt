cmake_minimum_required(VERSION 3.24)
project(401-CloudDoba2000 VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(401-CloudDoba2000 main.cpp cloud_solver.cpp cloud_solver.h)
set_target_properties(401-CloudDoba2000 PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(401-CloudDoba2000 PUBLIC HinaPE_Fluid_Grids)
