cmake_minimum_required(VERSION 3.24)
project(007-imterm VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(007-imterm main.cpp)
set_target_properties(007-imterm PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS ON)
target_link_libraries(007-imterm PUBLIC Kasumi_Backends)
