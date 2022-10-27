cmake_minimum_required(VERSION 3.16)
project(HinaPE_Common VERSION 1.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_DEPS_DIR "../deps")

file(GLOB_RECURSE HINAPE_COMMON_SRC "*.cpp" "*.h" "*.inl.h")

add_library(HinaPE_Common STATIC ${HINAPE_COMMON_SRC})
set_target_properties(HinaPE_Common PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
target_include_directories(HinaPE_Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# add dependencies
add_subdirectory("${HINAPE_DEPS_DIR}/tinyobjloader/" ${CMAKE_BINARY_DIR}/tinyobjloader)
target_link_libraries(HinaPE_Common PUBLIC tinyobjloader)

set(TBB_DIR /opt/homebrew/Cellar/tbb@2020/2020_U3/lib/cmake/TBB)
find_package(TBB CONFIG)
if (TBB_FOUND)
    message(STATUS ">> TBB found")
    set(HINAPE_TBB TRUE CACHED BOOL "TBB is found")
    target_compile_definitions(HinaPE_Common PUBLIC JET_TASKING_TBB)
    target_link_libraries(HinaPE_Common PUBLIC TBB::tbb)
endif ()
