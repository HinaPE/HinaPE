# 谈复杂系统的解耦与测试

在软件工程中，解耦是一个非常重要的系统设计概念。一个良好的解耦系统设计能极大地方便我们进行分模块测试，而不至于牵一发而动全身。

这篇memo就主要谈谈如何对一个设计良好的模拟系统（Fluid Engine Dev）进行单元测试。

## 粒子的生成：Emitter

### 为什么是Emitter

对于一个模拟项目而言，核心数据流动可以说是一件最为重要的事情。数据从哪里产生，哪些模块持有这些数据，哪些地方会产生大量计算，数据最终流向哪里等是在单元测试时不可避免的问题。

对于流体系统而言，Emitter是一切粒子生成的源头。所以，我们第一个测试就选择测试Emitter。

特殊的，由于SPH的例子的对步长的要求高，且参数影响过大调参困难（p138, 2.3.3 Results and Limitations），很容易产生diverge的后果（p140, 第二行）因此我们直接采用PCISPH作为基础测试模板，因为PCISPH模拟起来比较稳定。

### 观察ParticleEmitter3类

继承层级：

- ParticleEmitter3
  - VolumeParticleEmitter3
  - PointParticleEmitter3

jet给了一个Emitter的一个基类两个实现类。我们决定首先测试比较常用的体积粒子Emitter。

通过观察