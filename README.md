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

Rendering Features
- Backends: HinaPE is designed with OpenGL & Vulkan & DirectX 12 & Apple Metal backends.
- Rendering Engine: Built-in PBR renderer & GLSL pathtracer.

## Build Instruction

Prerequisites

- C++ 20
- cmake > 3.16

All dependencies is included in HinaPE.

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
