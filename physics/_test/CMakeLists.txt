cmake_minimum_required(VERSION 3.18)
project(HINAPE_TEST VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_subdirectory(vendor/googletest)

file(GLOB_RECURSE HINAPE_TEST_SOURCES "src/*.cpp" "src/*.h")
add_executable(HINAPE_TEST ${HINAPE_TEST_SOURCES})
set_target_properties(HINAPE_TEST PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HINAPE_TEST PUBLIC "src")