#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/api.h"
#include "fluid/solver/particle/sph/sph_solver3.h"

class Api : public Kasumi::Api
{
public:
	void step(float dt) final;
	void ui_sidebar() final;
	void sync();

private:
	HinaPE::Fluid::SphSolver3Ptr _fluid_solver;
};

#endif //HINAPE_API_H
