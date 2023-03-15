cmake_minimum_required(VERSION 3.24)
project(HinaPE_Fluid_Hybrid VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

if (NOT TARGET Kasumi_Render3D)
    message(FATAL_ERROR "Kasumi_Render3D is not found")
    return()
endif ()

add_library(HinaPE_Fluid_Hybrid)
file(GLOB headers flip_solver.h apic_solver.h)
file(GLOB sources flip_solver.cpp apic_solver.cpp)
target_sources(HinaPE_Fluid_Hybrid
        PRIVATE
        ${sources}

        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../
        FILES
        ${headers}
        )
set_target_properties(HinaPE_Fluid_Hybrid PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Fluid_Hybrid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_link_libraries(HinaPE_Fluid_Hybrid PUBLIC Kasumi_Render3D)

# APP EXECUTABLE
add_executable(HinaPE_Fluid_Hybrid_App main.cpp)
set_target_properties(HinaPE_Fluid_Hybrid_App PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Fluid_Hybrid_App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_Fluid_Hybrid_App PUBLIC HinaPE_Fluid_Hybrid)

# INSTALL TARGETS
INSTALL(
        TARGETS HinaPE_Fluid_Hybrid
        CONFIGURATIONS Release
        FILE_SET HEADERS
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)