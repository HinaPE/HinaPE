enable_testing()

add_executable(test01 test01.cpp)
target_link_libraries(test01 PRIVATE gtest_main igl_copyleft_cgal)
add_executable(test02 test02.cpp)
target_link_libraries(test02 PRIVATE gtest_main igl_copyleft_cgal)

include(GoogleTest)
gtest_discover_tests(test01)
gtest_discover_tests(test02)
