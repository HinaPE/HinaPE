cmake_minimum_required(VERSION 3.24)
project(HinaPE_Fluid_Particles VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

if (NOT TARGET Kasumi_Render3D)
    message(FATAL_ERROR "Kasumi_Render3D is not found")
    return()
endif ()

add_library(HinaPE_Fluid_Particles)
file(GLOB headers pbf_solver.h pcisph_solver.h sph_solver.h domain/*.h emitter/*.h neighbor_search/*.h kernel/*.h)
file(GLOB sources pbf_solver.cpp pcisph_solver.cpp sph_solver.cpp domain/*.cpp emitter/*.cpp neighbor_search/*.cpp kernel/*.cpp)
target_sources(HinaPE_Fluid_Particles
        PRIVATE
        ${sources}

        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../
        FILES
        ${headers}
        )
set_target_properties(HinaPE_Fluid_Particles PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Fluid_Particles PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_link_libraries(HinaPE_Fluid_Particles PUBLIC Kasumi_Render3D)

# APP EXECUTABLE
add_executable(HinaPE_Fluid_Particles_App main.cpp)
set_target_properties(HinaPE_Fluid_Particles_App PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Fluid_Particles_App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(HinaPE_Fluid_Particles_App PUBLIC HinaPE_Fluid_Particles)

# INSTALL TARGETS
INSTALL(
        TARGETS HinaPE_Fluid_Particles
        CONFIGURATIONS Release
        FILE_SET HEADERS
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)