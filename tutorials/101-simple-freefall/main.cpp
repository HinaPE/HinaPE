#include "renderer3D/renderer3D.h"

struct SimpleObject
{
	mVector3 x; // position
	mVector3 v; // velocity
	mVector3 f; // force
	real m = 1; // mass
};
struct Ball : public SimpleObject, public Kasumi::SphereObject { Ball() { track(&x); }};

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

auto main() -> int
{
	auto solver = std::make_shared<SimpleFreeFallSolver>();

	auto ball = std::make_shared<Ball>();
	ball->x = {0, 5, 0};
	solver->add(ball);

	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene) { scene->add(ball); };
	Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt) { solver->update(dt); };
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}