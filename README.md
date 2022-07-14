# HinaPE

[![Cmake Build](https://github.com/Xayah-Hina/HinaPE/actions/workflows/cmake.yml/badge.svg)](https://github.com/Xayah-Hina/HinaPE/actions/workflows/cmake.yml)

## Instruction

1. HinaPE is a recursive acronym for "**HinaPE is not a Physics Engine!**".

2. HinaPE is a light and easy-to-use research-oriented physics engine developed after [CMU Scotty3D](https://github.com/CMU-Graphics/Scotty3D), consisting Rigidbody Dynamics, Deformables and Fluids.
3. HinaPE contains a built-in workbench renderer, a built-in rasterizer and a built-in pathtracer.

## Build Instruction

As a cross-platform project, [Clion](https://www.jetbrains.com/clion/) is highly recommanded to build HinaPE.

### Prerequisites

- C++ 20
- cmake > 3.16

First recursively clone this repo and all submodules.

```shell
git clone --recursive https://github.com/Xayah-Hina/HinaPE.git
```

### macOS

```shell
brew install sdl2
cmake -S . -B build
```

### Linux

```shell
apt-get install libgtk-3-dev libsdl2-dev
cmake -S . -B build
```

### Windows

```shell
cmake -S . -B build
```

