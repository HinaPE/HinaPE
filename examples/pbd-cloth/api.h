#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/app.h"
#include "cloth/solver/pbd/pbd_solver.h"

class Api : public Kasumi::Api
{
public:
	void step(float dt) final;
	void ui_sidebar() final;
	void sync() const;

private:
	HinaPE::Cloth::PBDSolverPtr _cloth_solver;
	Kasumi::ModelPtr _cloth_model;
	Kasumi::ModelPtr _cloth_particle_model;
};

#endif //HINAPE_API_H
