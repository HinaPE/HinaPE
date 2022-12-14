#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/app.h"
#include "rigid/solver/pbd/pbd_solver.h"

class Api : public Kasumi::Api
{
public:
	void step(float dt) final;
	void ui_sidebar() final;
	void sync();

private:
	HinaPE::Rigid::RigidSolver _rb_solver;
	Kasumi::ModelPtr _rb_model;
};


#endif //HINAPE_API_H
