cmake_minimum_required(VERSION 3.24)
project(501-ImageOrthogonalTransform VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(501-ImageOrthogonalTransform main.cpp)
set_target_properties(501-ImageOrthogonalTransform PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(501-ImageOrthogonalTransform PUBLIC Kasumi_ShaderPainter)
