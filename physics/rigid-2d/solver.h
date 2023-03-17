#ifndef HINAPE_RIGID2D_SOLVER_H
#define HINAPE_RIGID2D_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "renderer2D/scene.h"
#include "box2d/box2d.h"

namespace HinaPE
{
enum class RigidType
{
	Static,
	Dynamic,
	Kinematic
};
class Rigid2DSolver
{
public:
	void add(const Kasumi::Object2DPtr &object, RigidType type = RigidType::Dynamic);
	void update(real dt);
	Rigid2DSolver();

private:
	std::vector<std::pair<Kasumi::Object2DPtr, b2Body *>> _objects;
	b2World _world;
};
}

#endif //HINAPE_RIGID2D_SOLVER_H
