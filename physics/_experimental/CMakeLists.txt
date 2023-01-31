cmake_minimum_required(VERSION 3.18)
project(HINAPE_TEST VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

add_subdirectory(vendor/googletest)

if (MSVC)
    add_compile_options(/wd4819 /wd4244)
endif ()

file(GLOB_RECURSE HINAPE_TEST_SOURCES "src/*.cpp" "src/*.h" "test/*.cpp" "test/*.h")
add_executable(HINAPE_TEST ${HINAPE_TEST_SOURCES})
set_target_properties(HINAPE_TEST PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HINAPE_TEST PUBLIC "src" "vendor/eigen3")
target_compile_definitions(HINAPE_TEST PUBLIC HINAPE_ENABLE_TEST HINAPE_EIGEN)

enable_testing()
target_link_libraries(HINAPE_TEST GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(HINAPE_TEST)


# parallel lib
find_package(TBB)
if (TBB_FOUND)
    target_link_libraries(HINAPE_TEST TBB::tbb)
    target_compile_definitions(HINAPE_TEST PUBLIC HINAPE_TBB)
endif ()