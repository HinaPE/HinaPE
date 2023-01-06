cmake_minimum_required(VERSION 3.18)
project(999-UnitTest VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_executable(999-UnitTest main.cpp test_emitter.h test_emitter.cpp test_emitter.h test_pcisph.cpp test_grid.h test_grid.cpp)
set_target_properties(999-UnitTest PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(999-UnitTest PUBLIC HinaPE_Fluid Kasumi_renderer)
target_compile_definitions(
        999-UnitTest
        PUBLIC
        -DMyAssetDir="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)