cmake_minimum_required(VERSION 3.24)
project(004-SakuraScattering VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(004-SakuraScattering main.cpp)
set_target_properties(004-SakuraScattering PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(004-SakuraScattering PUBLIC Kasumi_Render3D)
