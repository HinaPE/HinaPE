cmake_minimum_required(VERSION 3.18)
project(HINA_COMMON_UNIT_TEST VERSION 1.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

if (NOT TARGET HINA_COMMON)
    return()
endif ()

add_executable(HINA_COMMON_UNIT_TEST main.cpp)
set_target_properties(HINA_COMMON_UNIT_TEST PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(HINA_COMMON_UNIT_TEST PRIVATE HINA_COMMON)

enable_testing()
target_link_libraries(HINA_COMMON_UNIT_TEST PUBLIC GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(HINA_COMMON_UNIT_TEST)