#include "solver.h"
void HinaPE::SPHSolver::INSPECT()
{
	ImGui::Text("Solver");
	ImGui::Text("Solver %zu", _data->_positions.size());
	ImGui::Separator();
	INSPECT_VEC3(_opt.gravity, "gravity");
	INSPECT_REAL(_opt.eos_exponent, "eos exponent");
	INSPECT_REAL(_opt.negative_pressure_scale, "negative pressure scale");
	INSPECT_REAL(_opt.viscosity_coefficient, "viscosity coefficient");
	INSPECT_REAL(_opt.pseudo_viscosity_coefficient, "pseudo viscosity coefficient");
	INSPECT_REAL(_opt.speed_of_sound, "speed of sound");
	INSPECT_REAL(_opt.time_step_limit_scale, "time step limit scale");
//	_emitter->INSPECT();

	if (!_opt.inited)
	{
		if (ImGui::Button("Generate"))
		{
			_opt.inited = true;
		}
	}
	ImGui::Separator();
}
void HinaPE::SPHSolver::VALID_CHECK() const
{
	if (_domain == nullptr) throw std::runtime_error("SPHSolver::_domain is nullptr");
}
void HinaPE::SPHSolver::update(real dt)
{
}
