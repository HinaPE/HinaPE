cmake_minimum_required(VERSION 3.24)
project(HinaPE_Fluid_Particles VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

if (NOT TARGET Kasumi_Render3D)
    message(FATAL_ERROR "Kasumi_Render3D is not found")
    return()
endif ()

add_library(HinaPE_Fluid_Particles STATIC)
file(GLOB headers pbf_solver.h pcisph_solver.h export_to_xyz.h sph_solver.h pbf_solver_new.h pcisph_solver_new.h pcisph_solver_xayah.h pcisph_solver_celeste.h pcisph_solver_new.h domain/*.h emitter/*.h neighbor_search/*.h kernel/*.h boundary/*.h non_newtonian_pcisph_solver.h pcisph_temp.h pcisph.h pcisph_akinci.h pcisph_akinci_twoway.h pcisph_akinci_remake.h pcisph_akinci_surface_tension.h)
file(GLOB sources pbf_solver.cpp pcisph_solver.cpp sph_solver.cpp pbf_solver_new.cpp pcisph_solver_new.cpp pcisph_solver_xayah.cpp pcisph_solver_celeste.cpp pcisph_solver_new.cpp domain/*.cpp emitter/*.cpp neighbor_search/*.cpp kernel/*.cpp boundary/*.cpp non_newtonian_pcisph_solver.cpp pcisph_temp.cpp pcisph.cpp pcisph_akinci.cpp pcisph_akinci_twoway.cpp pcisph_akinci_remake.cpp pcisph_akinci_surface_tension.cpp)
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
target_compile_definitions(
        HinaPE_Fluid_Particles_App
        PUBLIC
        DEFAULT_OUTPUT_DIR="${PROJECT_SOURCE_DIR}/../../output/"
)

# INSTALL TARGETS
INSTALL(
        TARGETS HinaPE_Fluid_Particles
        CONFIGURATIONS Release
        FILE_SET HEADERS
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)