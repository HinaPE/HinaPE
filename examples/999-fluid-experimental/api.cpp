#include "api.h"

void FluidExperimental::prepare()
{
	_solver = std::make_shared<HinaPE::Fluid::ParticleSystemSolver3>();
}
void FluidExperimental::step(float dt)
{
	static HinaPE::Frame frame;
	frame.advance();
	_solver->update(frame);
}
void FluidExperimental::ui_sidebar() {}
void FluidExperimental::sync() {}
