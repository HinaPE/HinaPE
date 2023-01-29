cmake_minimum_required(VERSION 3.18)
project(HINAPE_TEST VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

file(GLOB_RECURSE HINAPE_TEST_SOURCES "src/*.cpp" "src/*.h")
add_executable(HINAPE_TEST ${HINAPE_TEST_SOURCES})