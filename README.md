# HinaPE is not a Physics Engine

[![Ubuntu Build](https://github.com/HinaPE/HinaPE/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/HinaPE/HinaPE/actions/workflows/ubuntu.yml)
[![macOS Build](https://github.com/HinaPE/HinaPE/actions/workflows/macos.yml/badge.svg)](https://github.com/HinaPE/HinaPE/actions/workflows/macos.yml)
[![Windows Build](https://github.com/HinaPE/HinaPE/actions/workflows/windows.yml/badge.svg)](https://github.com/HinaPE/HinaPE/actions/workflows/windows.yml)

BREAKING: 🎉 HinaPE reached 1000 commits! 🎉

## Introduction
HinaPE is a cross-platform and portable research-oriented physics-based animation infrastructure. HinaPE is a recursive acronym for "**HinaPE is not a Physics Engine**".

## Build Instruction

Prerequisites & dependencies:

- C++ 20
- Cmake > 3.24
- [intel tbb](https://github.com/oneapi-src/oneTBB.git): we use intel tbb for parallel computing, so make sure you have installed it.

Built-in dependencies

- [assimp](https://github.com/assimp/assimp.git): various 3d-model-formats loader
- [glfw3](https://github.com/glfw/glfw.git): windowing & input
- [stb](https://github.com/nothings/stb.git): texture loader
- [glad](https://github.com/Dav1dde/glad.git): OpenGL loader
- [nfd](https://github.com/EmbarkStudios/nfd2.git): file dialog
- [imgui](https://github.com/ocornut/imgui.git): GUI (include [implot](https://github.com/epezent/implot.git) & [imguizmo](https://github.com/CedricGuillemet/ImGuizmo.git))
- [libigl](https://github.com/libigl/libigl.git): geometry processing
- [eigen3](https://gitlab.com/libeigen/eigen.git): linear algebra
- [compactNsearch](https://github.com/InteractiveComputerGraphics/CompactNSearch.git) (optional): fast nearest neighbor search
- [cppnumericalsolvers](https://github.com/PatWie/CppNumericalSolvers.git) (optional): nonlinear solvers
- [ffmpeg]() (optional): video encoder

Optional dependencies:
- [OpenVDB](https://github.com/AcademySoftwareFoundation/openvdb.git): sparse volumetric data
- [CGAL](https://github.com/CGAL/cgal.git): geometry processing

First **<u>recursively</u>** clone this repo and all submodules.

```shell
git clone --recursive https://github.com/HinaPE/HinaPE.git
cd HinaPE
```

For all platform (MacOS, Ubuntu, Windows)

```shell
cmake -S . -B Build
cmake --build Build --config Release
```

## Features

Physics Features
- Fluid Engine
  - SPH solver
  - PciSPH solver
  - PBF solver
  - Flip solver
  - PIC solver
  - APIC solver
  - Level set solver
  - MPM snow
- Cloth Engine
  - PBD/XPBD solver
  - FMS/PD solver
  - FEM solver
  - powered by [NVCloth](https://github.com/NVIDIAGameWorks/NvCloth.git)
- Rigidbody Engine
  - powered by [reactphysics3D](https://github.com/DanielChappuis/reactphysics3d.git)
  - powered by [box2D](https://github.com/erincatto/box2d.git)
  - powered by [PhysX 5.1](https://github.com/NVIDIA-Omniverse/PhysX.git) (optional)

Rendering Features
- Backends: HinaPE is designed with OpenGL & Vulkan & DirectX 12 & Apple Metal backends.
- Rendering Engine: Built-in PBR renderer & GLSL pathtracer.

## Dev Instruction

You need to first **<u>recursively</u>** update all submodules
```git
git submodule update --init --recursive
git submodule foreach --recursive git pull origin master
git submodule foreach --recursive git checkout master
```