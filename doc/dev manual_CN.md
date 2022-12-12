# HinaPE 开发指南 - 中文

## Kasumi 渲染后端

### 文件结构

- backends：通用渲染后端，所有与OpenGL（或其他图形API）相关的功能类全部被封封装在这里。
- common：通用数学/几何库。一个大项目只需要依赖一个common就可以了。如果Kasumi渲染器外面还有工程包含它，那么这个common是不使用的，而是会使用外面那个common。（比如HinaPE便是）
- renderer：通用渲染器。这个层级与渲染后端（OpenGL等）无关，完全解耦，包含场景、UI等操作。

### Backends：通用渲染后端开发与使用

