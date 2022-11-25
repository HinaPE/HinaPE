#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/api.h"
#include "renderer/scene/scene.h"
#include "rigid/solver/pbd/pbd_solver.h"

namespace HinaPE::Rigid
{
class Api : public Kasumi::Api
{
public:
	void step(float dt) final;

	void ui_sidebar() final;

	void sync();

private:
	std::map<Kasumi::TexturedMeshPtr, RigidSolver> _solvers;
};
}


#endif //HINAPE_API_H
