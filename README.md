# HinaPE is not a Physics Engine

[![Ubuntu Build](https://github.com/HinaPE/HinaPE/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/HinaPE/HinaPE/actions/workflows/ubuntu.yml)
[![macOS Build](https://github.com/HinaPE/HinaPE/actions/workflows/macos.yml/badge.svg)](https://github.com/HinaPE/HinaPE/actions/workflows/macos.yml)
[![Windows Build](https://github.com/HinaPE/HinaPE/actions/workflows/windows.yml/badge.svg)](https://github.com/HinaPE/HinaPE/actions/workflows/windows.yml)

## Introduction

1. HinaPE is a recursive acronym for "**HinaPE is not a Physics Engine**".
2. HinaPE is a cross-platform, light and easy-to-use research-oriented physics engine developed after [CMU Scotty3D](https://github.com/CMU-Graphics/Scotty3D), [Fluid Engine Dev](http://github.com/doyubkim/fluid-engine-dev) and [SPlisHSPlasH](https://github.com/InteractiveComputerGraphics/SPlisHSPlasH), consisting Rigidbody Dynamics, Deformables and Fluids.
3. HinaPE is proudly powered by [Kasumi-render](https://github.com/HinaPE/Kasumi-renderer.git).

## Gallery

![HinaPE](https://github.com/Xayah-Hina/HinaPE-Resource/blob/master/image-bed/HinaPE-20220715.png?raw=true)
![HinaPE](https://github.com/Xayah-Hina/HinaPE-Resource/blob/master/image-bed/HinaPE-20220715-2.png?raw=true)
![HinaPE](https://github.com/Xayah-Hina/HinaPE-Resource/blob/master/image-bed/HinaPE-20220715-3.png?raw=true)
![HinaPE](https://github.com/Xayah-Hina/HinaPE-Resource/blob/master/image-bed/HinaPE-20221024.png?raw=true)
more illustrations in coming...

## Build Instruction

As a cross-platform project, [Clion](https://www.jetbrains.com/clion/) is highly recommended to build HinaPE.

### Prerequisites

- C++ 20
- cmake > 3.16

First recursively clone this repo and all submodules.

```shell
git clone --recursive https://github.com/HinaPE/HinaPE.git
```

### macOS

```shell
cmake -S . -B Build
cmake --build Build --config Release
./Build/HinaPE
```

### Ubuntu

```shell
cmake -S . -B Build
cmake --build Build --config Release
./Build/HinaPE
```

### Windows

```shell
cmake -S . -B Build
cmake --build Build --config Release
.\Build\Release\HinaPE.exe
```

For update all submodules: https://stackoverflow.com/questions/5828324/update-git-submodule-to-latest-commit-on-origin
```git
git submodule foreach git pull origin master
git submodule foreach git checkout master
```