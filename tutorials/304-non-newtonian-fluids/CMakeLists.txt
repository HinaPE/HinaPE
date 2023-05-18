cmake_minimum_required(VERSION 3.24)
project(304-non-newtonian-fluids VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(304-non-newtonian-fluids main.cpp)
set_target_properties(304-non-newtonian-fluids PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(304-non-newtonian-fluids PUBLIC HinaPE_Fluid_Particles HinaPE_Rigid)
target_compile_definitions(
        304-non-newtonian-fluids
        PUBLIC
        DEFAULT_OUTPUT_DIR="${PROJECT_SOURCE_DIR}/../../output/"
)