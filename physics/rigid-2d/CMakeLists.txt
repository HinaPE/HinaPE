cmake_minimum_required(VERSION 3.24)
project(HinaPE_Rigid2D VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

if (NOT TARGET Kasumi_Render2D)
    message(FATAL_ERROR "Kasumi_Render2D is not found")
    return()
endif ()

add_subdirectory(box2d)

add_library(HinaPE_Rigid2D STATIC)
file(GLOB headers solver.h)
file(GLOB sources solver.cpp)
target_sources(HinaPE_Rigid2D
        PRIVATE
        ${sources}

        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../
        FILES
        ${headers}
        )
set_target_properties(HinaPE_Rigid2D PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Rigid2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_link_libraries(HinaPE_Rigid2D PUBLIC Kasumi_Render2D box2d)

# APP EXECUTABLE
add_executable(HinaPE_Rigid2D_App main.cpp)
set_target_properties(HinaPE_Rigid2D_App PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Rigid2D_App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_Rigid2D_App PUBLIC HinaPE_Rigid2D)