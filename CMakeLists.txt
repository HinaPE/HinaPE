cmake_minimum_required(VERSION 3.18)
project(HinaPE VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

#option(HINAPE_CLOTH "Load HinaPE Cloth Engine" ON)
#option(HINAPE_RIGID "Load HinaPE Rigidbody Engine" ON)
#option(HINAPE_RIGID2D "Load HinaPE Rigidbody 2D Engine" ON)
#option(HINAPE_FLUID "Load HinaPE Fluid Engine" ON)
#option(HinaPE_USE_KASUMI "Use Kasumi Renderer" ON)
#option(HinaPE_EXAMPLES "Load HinaPE Examples" ON)
#
#if (HINAPE_CLOTH)
#    add_subdirectory(physics/cloth)
#endif ()
#
#if (HINAPE_RIGID)
#    add_subdirectory(physics/rigid)
#endif ()
#
#if (HINAPE_RIGID2D)
#    add_subdirectory(physics/rigid-2d)
#endif ()
#
#if (HINAPE_FLUID)
#    add_subdirectory(physics/fluid)
#    add_subdirectory(physics/_snow) # experimental, would merge into fluid as MPM target
#    add_subdirectory(physics/_experimental) # experimental target, would be remove in future
#endif ()
#
## If HinaPE is the Root Project, then enable Renderer and Examples
#get_directory_property(HINAPE_PARENT_DIR PARENT_DIRECTORY)
#if (NOT HINAPE_PARENT_DIR)
#    if (HinaPE_USE_KASUMI)
#        add_subdirectory(kasumi)
#    endif ()
#    if (HinaPE_EXAMPLES)
#        add_subdirectory(examples)
#    endif ()
#endif ()

# experimental, would be remove in future
add_subdirectory(_common)
add_subdirectory(_physics/fluid)