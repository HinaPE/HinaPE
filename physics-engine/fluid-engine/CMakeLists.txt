cmake_minimum_required(VERSION 3.16)
project(HinaPE_Fluid_Engine VERSION 1.1.1 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_COMMON_DIR "../../common")
set(HINAPE_DEPS_DIR "../../deps")

if (NOT TARGET HinaPE_Common)
    add_subdirectory(${HINAPE_COMMON_DIR} HinaPE_Common)
endif ()

file(GLOB_RECURSE HINA_FLUID_ENGINE_EMITTER "emitter/*.cpp" "emitter/*.h" "emitter/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_POINT_GENERATOR "point_generator/*.cpp" "point_generator/*.h" "point_generator/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_SEARCHER "searcher/*.cpp" "searcher/*.h" "searcher/*.inl.h")
file(GLOB_RECURSE HINA_FLUID_ENGINE_SOLVER "solver/*.cpp" "solver/*.h" "solver/*.inl.h")

file(GLOB_RECURSE HINA_FLUID_ENGINE_SRC
        ${HINA_FLUID_ENGINE_EMITTER}
        ${HINA_FLUID_ENGINE_POINT_GENERATOR}
        ${HINA_FLUID_ENGINE_SEARCHER}
        ${HINA_FLUID_ENGINE_SOLVER}
        app.cpp
        app.h
        factory.cpp
        factory.h
        )

add_library(HinaPE_Fluid_Engine ${HINA_FLUID_ENGINE_SRC} solver/grid/grid_fluid_solver3.h solver/grid/grid_system_data3.h solver/grid/grid_system_data3.cpp emitter/grid_emitter3.h emitter/grid_emitter3.cpp solver/grid/grid_pressure_solver3.h solver/grid/grid_boundary_condition_solver3.h solver/grid/grid_boundary_condition_solver3.cpp solver/grid/grid_pressure_solver3.cpp solver/grid/grid_diffusion_solver3.h solver/grid/grid_diffusion_solver3.cpp solver/advection/advection_solver3.h solver/advection/advection_solver3.cpp solver/grid/grid_fluid_solver3.cpp solver/grid/grid_fractional_single_phase_pressure_solver3.h solver/grid/grid_blocked_boundary_condition_solver3.h solver/grid/grid_fractional_single_phase_pressure_solver3.cpp solver/grid/grid_fractional_boundary_condition_solver3.h solver/grid/grid_blocked_boundary_condition_solver.cpp solver/grid/grid_fractional_boundary_condition_solver3.cpp solver/grid/grid_backward_euler_diffusion_solver3.h solver/grid/grid_backward_euler_diffusion_solver3.cpp FDM/fdm_linear_system_solver3.h FDM/fdm_linear_system_solver3.cpp FDM/fdm_linear_system3.h FDM/fdm_linear_system3.cpp FDM/fdm_utils.h FDM/fdm_utils.cpp FDM/fdm_iccg_solver3.h FDM/fdm_cg_solver3.h FDM/fdm_cg_solver3.cpp FDM/cg.h FDM/blas.h FDM/blas-inl.h FDM/cg-inl.h FDM/fdm_iccg_solver3.cpp FDM/fdm_mg_solver3.h FDM/fdm_mg_linear_system3.h FDM/mg.h FDM/mg-inl.h FDM/fdm_mg_linear_system3-inl.h FDM/fdm_mg_linear_system3.cpp FDM/fdm_mg_solver3.cpp FDM/fdm_gauss_seidel_solver3.h FDM/fdm_gauss_seidel_solver3.cpp solver/particle/pcisph/pci_sph_solver3.h solver/particle/pcisph/pci_sph_solver3.cpp solver/grid/pic/pic_solver3.h solver/grid/pic/pic_solver3.cpp solver/grid/flip/flip_solver3.h solver/grid/flip/flip_solver3.cpp)
set_target_properties(HinaPE_Fluid_Engine PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
target_include_directories(HinaPE_Fluid_Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${HINAPE_COMMON_DIR})
target_link_libraries(HinaPE_Fluid_Engine PUBLIC HinaPE_Common)

# Fluid Engine Test Part
#get_directory_property(HINA_FLUID_ENGINE_PARENT_DIR PARENT_DIRECTORY)
#if (NOT HINA_FLUID_ENGINE_PARENT_DIR)
#    add_executable(HinaPE_Fluid_Engine_Test main.cpp)
#    set_target_properties(HinaPE_Fluid_Engine_Test PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF)
#    target_link_libraries(HinaPE_Fluid_Engine_Test PUBLIC HinaPE_Fluid_Engine)
#endif ()
