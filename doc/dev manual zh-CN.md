# HinaPE Dev Manual - Tutorial by Examples

[TOC]

## HinaPE Common

HinaPE 通用模块

### HinaPE::Math

### HinaPE::Geom

#### Surface

#### Collider

## Kasumi Renderer3D

Kasumi 3D渲染器是一个面向快速实现3D图形学算法的基础渲染平台，包含简单的内置物体以及场景操作。

### Objects List

#### [Abstract] Object3D

一个通用3D Object类，用于管理一切物体。

##### FIELD: `ID`

- 唯一ID，不可变。

##### FIELD: `NAME`

- Object name，任意时刻可修改，实时更新。

##### FIELD: `POSE`

- Transform，任意时刻可修改，实时更新。

#### [Abstract] ObjectMesh3D

##### METHOD: `sync_opt()`

- 改变Object的Opt内容之后，一定要手动执行sync_opt

#### Cube

- 

### Scene3D

#### Functions

##### Add Object

- （可选）设置物体Transform
- （可选）设置物体的特有opt，并保证sync

```c++
auto sphere = std::make_shared<Kasumi::SphereObject>();

// edit pose
sphere->_pose.position = {0, 1, 0};
sphere->_pose.euler = {0, 1, 0};
sphere->_pose.scale = {2, 3, 1};

// edit opt (remenber to sync_opt after any altering)
sphere->_opt.radius = 1;
sphere->sync_opt();

_scene->add(sphere);
```

##### Remove Object

```c++
// delete with id
_scene->remove(sphere->ID);
```

##### Draw Scene

```c++
_scene->draw()
```

