cmake_minimum_required(VERSION 3.18)
project(HINA_COMMON_UNIT_TEST VERSION 1.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

if (NOT TARGET HinaPE_Common)
    return()
endif ()

add_executable(HinaPE_Common_UNIT_TEST main.cpp)
set_target_properties(HinaPE_Common_UNIT_TEST PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(HinaPE_Common_UNIT_TEST PRIVATE HinaPE_Common)

enable_testing()
target_link_libraries(HinaPE_Common_UNIT_TEST PUBLIC GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(HinaPE_Common_UNIT_TEST)