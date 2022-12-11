#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/api.h"
#include "renderer/scene/scene.h"
#include "cloth/solver/pbd/pbd_solver.h"

namespace HinaPE::Cloth
{
class Api : public Kasumi::Api
{
public:
	void step(float dt) final;
	void ui_sidebar() final;
	void sync();

private:
	std::map<Kasumi::ModelPtr, ClothSolverPtr> _solvers;
};
}

#endif //HINAPE_API_H
