cmake_minimum_required(VERSION 3.24)
project(006-TriangleMeshGeometry VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(006-TriangleMeshGeometry main.cpp)
set_target_properties(006-TriangleMeshGeometry PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(006-TriangleMeshGeometry PUBLIC Kasumi_Render3D)
