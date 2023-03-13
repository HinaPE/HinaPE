#include "renderer3D/renderer3D.h"

struct SimpleParticlesObject
{
	std::vector<mVector3> positions;
	std::vector<mVector3> velocities;
	std::vector<mVector3> forces;
	real mass = 1;
};
struct Fireworks : public SimpleParticlesObject, public Kasumi::ObjectParticles3D
{
	Fireworks() { track(&positions); _random_color = true; }
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

auto main() -> int
{
	auto solver = std::make_shared<SimpleFreeFallSolver>();

	auto fireworks = std::make_shared<Fireworks>();
	size_t particles = 5000;
	fireworks->positions.resize(particles);
	fireworks->velocities.resize(particles);
	fireworks->forces.resize(particles);
	// initial positions
	real size = 0.3;
	for (auto &p: fireworks->positions)
		p = {HinaPE::Math::random_real(-size, size), HinaPE::Math::random_real(-size, size), HinaPE::Math::random_real(-size, size)};
	// initial velocities
	real speed = 2;
	for (auto &v: fireworks->velocities)
		v = {HinaPE::Math::random_real(-speed, speed), 4* speed, HinaPE::Math::random_real(-speed, speed)};

	solver->add(fireworks);


	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene) { scene->add(fireworks); };
	Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt) { solver->update(dt); };
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();

	return 0;
}