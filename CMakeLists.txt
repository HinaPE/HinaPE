cmake_minimum_required(VERSION 3.16)
project(HinaPE)

set(MY_EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/../Graphics-Dependencies/install) # !Important: Determine External Install Dir
set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4005 /wd4100 /wd4244 /wd4267 /wd4819 /wd4305 /wd4996 /wd4018 /wd4389 /wd4018 /wd4127 /wd4458 /wd4459 /wd4189 /wd4456 /wd4101 /wd4125 /wd4505")
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>DLL)
endif ()
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Debug)
endif ()
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Release)
endif ()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

include_directories(${CMAKE_SOURCE_DIR} ${MY_EXTERNAL_DIR}/include)
link_directories(${MY_EXTERNAL_DIR}/bin ${MY_EXTERNAL_DIR}/lib)

add_subdirectory(HinaGeom)
add_subdirectory(HinaViewer)
