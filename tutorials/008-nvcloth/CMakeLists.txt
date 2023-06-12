cmake_minimum_required(VERSION 3.24)
project(008-NVCloth VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(008-NVCloth main.cpp)
set_target_properties(008-NVCloth PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS ON)
target_link_libraries(008-NVCloth PUBLIC NVCloth)
