# HinaPE 开发指南 - 中文

## 简介：

HinaPE是一个建立以Kasumi Renderer为后端建立起的物理模拟平台。

## Kasumi 渲染后端

### 核心架构：

![HinaPE](HinaPE.png)

### 文件结构

- `backends`：通用渲染后端，所有与OpenGL（或其他图形API）相关的功能类全部被封装在这里。
- `common`：通用数学/几何库。一个大项目只需要依赖一个common就可以了。如果Kasumi渲染器外面还有工程包含它，那么这个common是不使用的，而是会使用外面那个common。（比如HinaPE便是）
- `renderer app`：通用渲染器，一个App的实例。这个层级不关心渲染后端的实现（如OpenGL等），完全解耦，支持场景、UI等操作。

### Backends：通用渲染后端

#### 设计思想

1. 要绝对解耦。尽可能做到最大的复用性。
   - 要与外部解耦。比如Kasumi渲染器、HinaPE等。不能依赖除了数学函数之外任何外部的东西。
   - 要与自己解耦。同在backends后端下的文件之间，也要尽可能解耦，尽可能每个文件都可以单独引用。如果做不到单独引用，那么依赖链条也必须清晰简洁。
   - 要与图形Api实现解耦。在头文件里不出现任何OpenGL的东西。（注：GLFW窗口实质上是系统层级的，并不严格属于OpenGL，因为OpenGL、Vulkan、Metal等图形API都可以使用GLFW的窗口。所以GLFW是被允许在头文件中出现的。）
2. 要尽最大的可能封装起来。
   - 比如`Model`。一旦编写完成，外部的调用（比如Kasumi Renderer）就只需要在Model层级进行操作，不需要深入`UniversalMesh`这一层级，也不需要写任何GL开头的指令。
   - 再比如`Platform`。作为一个通用图形渲染后端，我们可以发现Platform的头文件没有依赖除了标准库外的任何库。这样的封装使得扩展开发变得特别容易。

#### 文件依赖关系

| File          | Dependency                   |
| ------------- | ---------------------------- |
| `camera`      | math_api                     |
| `framebuffer` | shader                       |
| `mesh`        | math_api/shader/texture      |
| `model`       | math_api/shader/texture/mesh |
| `platform`    | /                            |
| `pose`        | math_api                     |
| `shader`      | math_api                     |
| `texture`     | /                            |

可以看到，backends中的所有文件的依赖结构都简单。由于math_api可完全自定义实现（比如使用自己的Vector或者Matrix等），在这里的所有的文件均可单独作为一个完整的模块任何项目被引入。

#### Built-in资源

我们注意到，在`backends`文件夹下面还有一个`assets`文件夹。这里存储了方便开发者使用的**<u>内置</u>**资源。包括模型，场景和内置Shader。

在`backends`的根目录下的`CMakeLists.txt`的最末尾定义了四个Built-in宏

- `BuiltinSceneDir`
- `BuiltinModelDir`
- `BuiltinShaderDir`
- `BuiltinTextureDir`

这将允许我们在**<u>任何地方</u>**随时找到并调用Built-in的资源。

## 快速上手

实践是掌握Kasumi渲染后端最快的办法。我们将简单进行几个扩展练习。

一、