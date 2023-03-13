## HinaPE Dev Manual - Physics Module, Totorials by Examples

### 前提准备

- 目标：快速实现小型物理算法，并在出错时通过可视化GUI快速定位出错点。
- 特性：实现算法时不考虑任何渲染过程，完全屏蔽Backends图形后端一切东西。

---

### Example101: Simple Free Fall

>  目标：实现球体自由落体动画。
>
> 依赖文件
>
> - kasumi/renderer3D/renderer3D.h
> - kasumi/backends/objects/sphere.h

首先，定义基本的物理物体数据结构，以及简单的运动解算器。

由于是自由落体运动，我们可以视作质点运动，因此只需要记录物体质心。

 ```c++
struct SimpleObject
{
	mVector3 x; // position
	mVector3 v; // velocity
	mVector3 f; // force
	real m = 1; // mass
};
class SimpleFreeFallSolver
{
public:
	void add(const std::shared_ptr<SimpleObject> &o) { _objects.push_back(o); }
	void update(real dt)
	{
		for (auto &o: _objects)
		{
			auto &x = o->x;
			auto &v = o->v;
			auto &f = o->f;
			auto &m = o->m;

			f = _gravity;

			// apply semi-implicit Euler
			v += dt * f / m;
			x += dt * v;
		}
	}

private:
	std::vector<std::shared_ptr<SimpleObject>> _objects;
	mVector3 _gravity = {0, -9.8, 0};
};
 ```

现在，我们需要将SimpleObject的运动轨迹渲染出来。所以，首先，我们需要包含我们的3D渲染器。对于所有质心运动的物体，我们只需要让其继承一些基础的形状，并使用这些基础形状内置的`track`方法链接质心位置即可完成渲染需求。

```c++
#include "renderer3D/renderer3D.h"

struct Ball : public SimpleObject, public Kasumi::SphereObject
{
    Ball()
    {
        track(&x);
    }
};
```

最后，我们只需调用渲染器，将物理系统初始化，将所有需要被渲染的物体加入到场境之内，最后跟随每一帧前进更新物理系统即可。

```c++
auto solver = std::make_shared<SimpleFreeFallSolver>();

auto ball = std::make_shared<Ball>();
ball->x = {0, 5, 0};
solver->add(ball);

Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
{ 
    scene->add(ball); 
};
Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
{ 
    solver->update(dt);
};
Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
```

---

Example102: Simple Particle System

> 目标：实现粒子群自由落体动画
>
>  依赖文件
>
> - kasumi/renderer3D/renderer3D.h
> - kasumi/backends/objects/object3D.h

对于一群粒子物体，我们当然可以按照Example101一样，为每个物体指定一个渲染形状并将其渲染出来。但当粒子数量成百上千累积上来，会遭遇较大的性能挑战。

对于这类粒子动画需求，我们需要借助`ObjectParticles3D`来实现。

如同Example101一样，我们首先写出一个简单的粒子重力Solver。

```c++
struct SimpleParticlesObject
{
	std::vector<mVector3> positions;
	std::vector<mVector3> velocities;
	std::vector<mVector3> forces;
	real mass = 1;
};
class SimpleFreeFallSolver
{
public:
	void add(const std::shared_ptr<SimpleParticlesObject> &o) { _objects.push_back(o); }
	void update(real dt)
	{
		for (auto &o: _objects)
		{
			const auto size = o->positions.size();
			for (size_t i = 0; i < size; ++i)
			{
				auto &x = o->positions[i];
				auto &v = o->velocities[i];
				auto &f = o->forces[i];
				auto &m = o->mass;

				// apply semi-implicit Euler
				f = _gravity;
				v += dt * f / m;
				x += dt * v;
			}

		}
	}

private:
	std::vector<std::shared_ptr<SimpleParticlesObject>> _objects;
	mVector3 _gravity = {0, -9.8, 0};
};
```

现在，我们需要追踪渲染所有物体的位置，那么我们我们可以这样做。

```c++
struct Fireworks : public SimpleParticlesObject, public Kasumi::ObjectParticles3D
{
	Fireworks() { track(&positions); }
};
```

最后，同样的，我们将Solver嵌入Renderer中，并作简单的初始化。

```c++

```

