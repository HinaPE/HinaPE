cmake_minimum_required(VERSION 3.18)
project(HINA_COMMON VERSION 1.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

option(HINAPE_DOUBLE "use double float precision" OFF)
option(HINAPE_UNIT_TEST "build unit test" ON)

if (MSVC)
    add_compile_options(/wd4819 /wd4244)
endif ()

file(GLOB_RECURSE
        HINA_COMMON_SOURCES
        "animation/*.cpp" "animation/*.h"
        "geom/*.cpp" "geom/*.h"
        "math/*.cpp" "math/*.h"
        "util/*.cpp" "util/*.h"
        )

add_library(HINA_COMMON STATIC ${HINA_COMMON_SOURCES})
set_target_properties(HINA_COMMON PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HINA_COMMON PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(HINA_COMMON PUBLIC HINAPE_DEBUG)
if (HINAPE_DOUBLE)
    target_compile_definitions(HINA_COMMON PUBLIC HINAPE_DOUBLE HINAPE_DEBUG)
endif ()

# builtin dependency: Eigen3
target_include_directories(HINA_COMMON PUBLIC vendor/eigen3)
target_compile_definitions(HINA_COMMON PUBLIC HINA_EIGEN)

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
    target_compile_definitions(HINA_COMMON PUBLIC HINAPE_TBB)
    target_link_libraries(HINA_COMMON PUBLIC TBB::tbb)
endif ()

# optional dependency:
find_package(OpenVDB CONFIG)
if (OpenVDB_FOUND)
    message(STATUS ">> OpenVDB found")
    target_compile_definitions(HINA_COMMON PUBLIC HINAPE_OPENVDB)
    target_link_libraries(HINA_COMMON PUBLIC OpenVDB::openvdb)
endif ()