cmake_minimum_required(VERSION 3.18)
project(HinaPE_Common VERSION 1.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

option(HINAPE_DOUBLE "use double float precision" OFF)
option(HINAPE_UNIT_TEST "build unit test" ON)

if (MSVC)
    add_compile_options(/wd4819 /wd4244)
endif ()

file(GLOB_RECURSE
        HinaPE_Common_SRC
        "common.h"
        "animation/*.cpp" "animation/*.h"
        "geom/*.cpp" "geom/*.h"
        "math/*.cpp" "math/*.h"
        "util/*.cpp" "util/*.h"
        )

add_library(HinaPE_Common STATIC ${HinaPE_Common_SRC})
set_target_properties(HinaPE_Common PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(HinaPE_Common PUBLIC HINAPE_DEBUG)
if (HINAPE_DOUBLE)
    target_compile_definitions(HinaPE_Common PUBLIC HINAPE_DOUBLE)
endif ()

# builtin dependency: Eigen3
target_include_directories(HinaPE_Common PUBLIC vendor/eigen3)
target_compile_definitions(HinaPE_Common PUBLIC HINA_EIGEN)

# builtin dependency: google test
if (HINAPE_UNIT_TEST)
    add_subdirectory(vendor/googletest)
    add_subdirectory(_unit_test)
endif ()

# optional dependency: TBB
if (APPLE)
    set(TBB_DIR /opt/homebrew/Cellar/tbb@2020/2020_U3/lib/cmake/TBB)
endif ()
find_package(TBB CONFIG)
if (TBB_FOUND)
    message(STATUS ">> TBB found")
    target_compile_definitions(HinaPE_Common PUBLIC HINAPE_TBB)
    target_link_libraries(HinaPE_Common PUBLIC TBB::tbb)
endif ()

# optional dependency:
find_package(OpenVDB CONFIG)
if (OpenVDB_FOUND)
    message(STATUS ">> OpenVDB found")
    target_compile_definitions(HinaPE_Common PUBLIC HINAPE_OPENVDB)
    target_link_libraries(HinaPE_Common PUBLIC OpenVDB::openvdb)
endif ()