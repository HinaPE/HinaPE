cmake_minimum_required(VERSION 3.24)
project(005-DeferredRendering VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(005-DeferredRendering main.cpp)
set_target_properties(005-DeferredRendering PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(005-DeferredRendering PUBLIC Kasumi_Render3D)
