# HinaPE is not a Physics Engine

[![Ubuntu Build](https://github.com/HinaPE/HinaPE/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/HinaPE/HinaPE/actions/workflows/ubuntu.yml)
[![macOS Build](https://github.com/HinaPE/HinaPE/actions/workflows/macos.yml/badge.svg)](https://github.com/HinaPE/HinaPE/actions/workflows/macos.yml)
[![Windows Build](https://github.com/HinaPE/HinaPE/actions/workflows/windows.yml/badge.svg)](https://github.com/HinaPE/HinaPE/actions/workflows/windows.yml)

## Introduction

1. HinaPE is a recursive acronym for "**HinaPE is not a Physics Engine**".
2. HinaPE is a cross-platform and portable research-oriented physics-based animation infrastructure.
3. HinaPE is proudly powered by [Kasumi-renderer](https://github.com/HinaPE/Kasumi-renderer.git).

## Features

Physics Features
- Cloth Engine
  - FEM solver
  - FMS solver
  - PBD solver
  - PD solver
  - XPBD solver
- Rigidbody Engine
  - powered by [reactphysics3D](https://github.com/DanielChappuis/reactphysics3d.git)
  - powered by [PhysX 5.1](https://github.com/NVIDIA-Omniverse/PhysX.git) (optional)
- Fluid Engine
  - SPH solver
  - PciSPH solver
  - Flip solver
  - PIC solver
  - APIC solver
  - Level set solver
  - MPM snow

Rendering Features
- Backends: HinaPE is designed with OpenGL & Vulkan & DirectX 12 & Apple Metal backends.
- Rendering Engine: Built-in PBR renderer & GLSL pathtracer.

## Gallery

![Kasumi](https://github.com/Xayah-Hina/HinaPE-Resource/blob/master/image-bed/fluid.png?raw=true)
![Kasumi](https://github.com/Xayah-Hina/HinaPE-Resource/blob/master/image-bed/cloth.PNG?raw=true)
![Kasumi](https://github.com/Xayah-Hina/HinaPE-Resource/blob/master/image-bed/painting2d.PNG?raw=true)
![Kasumi](https://github.com/Xayah-Hina/HinaPE-Resource/blob/master/image-bed/conellbox.PNG?raw=true)

## Build Instruction

Prerequisites & dependencies: 

(**NOTE: all external dependencies are built-in**)

- C++ 20
- cmake > 3.16
- [assimp](https://github.com/assimp/assimp.git): various 3d-model-formats loader
- [glfw3](https://github.com/glfw/glfw.git): windowing & input
- [stb](https://github.com/nothings/stb.git): texture loader
- [glad](https://github.com/Dav1dde/glad.git): OpenGL loader
- [nfd](https://github.com/EmbarkStudios/nfd2.git): file dialog
- [imgui](https://github.com/ocornut/imgui.git): GUI (include [implot](https://github.com/epezent/implot.git) & [imguizmo](https://github.com/CedricGuillemet/ImGuizmo.git))
- [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader.git): obj model loader (to be removed soon)
- [fpm](https://github.com/MikeLankamp/fpm.git): fixed-point math library (not used)

First recursively clone this repo and all submodules.

```shell
git clone --recursive https://github.com/HinaPE/HinaPE.git
cd HinaPE
```
For all platform (MacOS, Ubuntu, Windows)

```shell
cmake -S . -B Build
cmake --build Build --config Release
./Build/HinaPE
```

## Documentation
on dev...

## Q&A
update all submodules
```git
git submodule foreach git pull origin master
git submodule foreach git checkout master
```

## TODOs
- ray-marching: https://www.youtube.com/watch?v=hUaYxqkrfjA
- MLS-MPM:
  - https://github.com/cb-geo/mpm
  - https://github.com/WindQAQ/MPM
