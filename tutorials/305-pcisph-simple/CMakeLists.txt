cmake_minimum_required(VERSION 3.24)
project(305-pcisph-simple VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(305-pcisph-simple main.cpp)
set_target_properties(305-pcisph-simple PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(305-pcisph-simple PUBLIC HinaPE_Fluid_Particles HinaPE_Rigid)
target_compile_definitions(
        305-pcisph-simple
        PUBLIC
        DEFAULT_OUTPUT_DIR="${PROJECT_SOURCE_DIR}/../../output/"
)