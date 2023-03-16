cmake_minimum_required(VERSION 3.24)
project(HinaPE_Rigid VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

if (NOT TARGET Kasumi_Render3D)
    message(FATAL_ERROR "Kasumi_Render3D is not found")
    return()
endif ()

add_subdirectory(reactphysics3d)
if (HINAPE_DOUBLE)
    target_compile_definitions(react-physics PUBLIC IS_RP3D_DOUBLE_PRECISION_ENABLED)
endif ()

add_library(HinaPE_Rigid ${src})
file(GLOB headers solver.h)
file(GLOB sources solver.cpp)
target_sources(HinaPE_Rigid
        PRIVATE
        ${sources}

        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../
        FILES
        ${headers}
        )
set_target_properties(HinaPE_Rigid PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Rigid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_link_libraries(HinaPE_Rigid PUBLIC Kasumi_Render3D react-physics)

# APP EXECUTABLE
add_executable(HinaPE_Rigid_App main.cpp)
set_target_properties(HinaPE_Rigid_App PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Rigid_App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_Rigid_App PUBLIC HinaPE_Rigid)