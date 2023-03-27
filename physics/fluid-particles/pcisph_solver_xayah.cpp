#include "pcisph_solver_xayah.h"

// ============================================================================================================
// ================================================== Solver ==================================================
void HinaPE::PCISPHSolverXayah::init()
{
}
void HinaPE::PCISPHSolverXayah::update(real dt) const
{
}
void HinaPE::PCISPHSolverXayah::reset()
{
	_data->reset();
	init();
}
void HinaPE::PCISPHSolverXayah::INSPECT()
{
	// Solver Parameters
	ImGui::Text("SOLVER INSPECTOR");
	ImGui::Text("Fluids: %zu", _data->fluid_size());
}
// ================================================== Solver ==================================================
// ============================================================================================================


// ==========================================================================================================
// ================================================== Data ==================================================
HinaPE::PCISPHSolverXayah::Data::Data()
{
	track(&Fluid.positions);
	track_colormap(&color_map);
}
void HinaPE::PCISPHSolverXayah::Data::add_fluid(const std::vector<mVector3> &positions, const std::vector<mVector3> &velocities)
{
	if (positions.size() != velocities.size())
		throw std::runtime_error("positions.size() != velocities.size()");

	auto size = positions.size();
	Fluid.positions.insert(Fluid.positions.end(), positions.begin(), positions.end());
	Fluid.velocities.insert(Fluid.velocities.end(), velocities.begin(), velocities.end());
	Fluid.predicted_position.insert(Fluid.predicted_position.end(), positions.begin(), positions.end());
	Fluid.forces.insert(Fluid.forces.end(), size, mVector3::Zero());
	Fluid.densities.insert(Fluid.densities.end(), size, 0.0);
	Fluid.pressures.insert(Fluid.pressures.end(), size, 0.0);

	color_map.insert(color_map.end(), size, Color::ORANGE);
	debug_info.insert(debug_info.end(), size, std::vector<std::string>());
}
auto HinaPE::PCISPHSolverXayah::Data::fluid_size() const -> size_t { return Fluid.positions.size(); }
void HinaPE::PCISPHSolverXayah::Data::reset()
{
	Fluid.positions.clear();
	Fluid.velocities.clear();
	Fluid.predicted_position.clear();
	Fluid.forces.clear();
	Fluid.densities.clear();
	Fluid.pressures.clear();
	Fluid.mass = 1e-3;
	color_map.clear();
	debug_info.clear();
}
