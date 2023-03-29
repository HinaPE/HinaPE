cmake_minimum_required(VERSION 3.24)
project(303-PCISPH-Akinci2012 VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(303-PCISPH-Akinci2012 main.cpp)
set_target_properties(303-PCISPH-Akinci2012 PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(303-PCISPH-Akinci2012 PUBLIC HinaPE_Fluid_Particles)
target_compile_definitions(
        303-PCISPH-Akinci2012
        PUBLIC
        DEFAULT_OUTPUT_DIR="${PROJECT_SOURCE_DIR}/../../output/"
)