#include "pcisph_solver_xayah.h"

#define S HinaPE::Math::to_string
#define Record(x, y) _data->debug_info[i].emplace_back(x + S(y))

// ============================================================================================================
// ================================================== Solver ==================================================
void HinaPE::PCISPHSolverXayah::init()
{
	// init data
	if (_data == nullptr)
		_data = std::make_shared<Data>();
	if (_domain == nullptr)
	{
		_domain = std::make_shared<BoxDomain>();
		_domain->POSE.scale = {1, 1.5, 1};
		_domain->_update_surface();
	}
	if (_emitter == nullptr)
	{
		_emitter = std::make_shared<VolumeParticleEmitter3>();
		_emitter->_opt.multiplier = 2.2;
	}

	_data->DEFAULT_SCALE = 0.5 * Opt.radius * mVector3::One();
	_emitter->_opt.spacing = 1.2 * Opt.radius;

	_init_fluid_particles();
}
void HinaPE::PCISPHSolverXayah::update(real dt) const
{
	_update_neighbor();
	_update_density();
	_accumulate_force();
	_time_integration();
	_resolve_collision();

	// for debug
	_data->Fluid.last_positions = _data->Fluid.positions; // show the position of last frame
}
void HinaPE::PCISPHSolverXayah::reset()
{
	_data->reset();
	init();
}
void HinaPE::PCISPHSolverXayah::_accumulate_force() const
{
	auto &x = _data->Fluid.positions;
	auto &v = _data->Fluid.velocities;
	auto &f = _data->Fluid.forces;
	auto &d = _data->Fluid.densities;
	auto &p = _data->Fluid.pressures;
	const auto &m = _data->Fluid.mass;
	const auto &nl = _data->NeighborList;
	StdKernel poly6(Opt.kernel_radius);
	SpikyKernel spiky(Opt.kernel_radius);

	// Gravity
	Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
	{
		f[i] = m * Opt.gravity;
	});

//	// Viscosity Forces
//	Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
//	{
//		const auto &neighbors = nl[i];
//		for (size_t j: neighbors)
//		{
//			real dist = (x[i] - x[j]).length();
//			if (d[j] > Constant::Epsilon)
//				f[i] += Opt.viscosity * m * m * (v[j] - v[i]) / d[j] * poly6.second_derivative(dist);
//		}
//	});

	// Pressure Forces
	std::vector<real> ds(_data->fluid_size()); // predicted density

	auto &tx = _data->Fluid.temp_positions;
	auto &tv = _data->Fluid.temp_velocities;
	auto &tpf = _data->Fluid.temp_pressure_forces;
	auto &tde = _data->Fluid.temp_density_errors;
	const auto dt = Opt.current_dt;
	auto delta = _compute_delta();

	Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
	{
		p[i] = Constant::Zero;
		tpf[i] = mVector3::Zero();
		tde[i] = Constant::Zero;
		ds[i] = d[i];
	});

	real max_density_err = 0;
	real density_error_ratio = 0;

	for (int i = 0; i < Opt.max_iterations; ++i)
	{
		// Predict velocity and position
		Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
		{
			tv[i] = v[i] + dt * (f[i] + tpf[i]) / m;
			tx[i] = x[i] + dt * tv[i];
		});

		// Resolve collisions
		Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
		{
			_domain->resolve_collision(Opt.radius, Opt.restitution, &tx[i], &tv[i]);
		});

		// Compute pressure from density error
		Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
		{
			real weight_sum = poly6(0); // self
			for (size_t j: nl[i])
			{
				real dist = (tx[i] - tx[j]).length();
				weight_sum += poly6(dist);
			}

			real density = m * weight_sum;
			real density_err = density - Opt.target_density;
			real pressure = 0.01 * delta * density_err;

			if (pressure < 0)
			{
				pressure *= Opt.nps;
				density_err *= Opt.nps;
			}

			p[i] += pressure;
			ds[i] = density;
			tde[i] = density_err;
		});

		// Compute pressure gradient force
		std::fill(tpf.begin(), tpf.end(), mVector3::Zero());
		Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
		{
			const auto &neighbors = _data->NeighborList[i];
			for (size_t j: neighbors)
			{
				real dist = (x[i] - x[j]).length();
				if (dist > HinaPE::Constant::Epsilon && d[j] > HinaPE::Constant::Epsilon)
				{
					mVector3 dir = (x[j] - x[i]) / dist;
					tpf[i] -= m * m * (p[i] / (ds[i] * ds[i]) + p[j] / (ds[j] * ds[j])) * spiky.gradient(dist, dir);
				}
			}
		});

		// Compute max density error
		for (int j = 0; j < _data->fluid_size(); ++j)
			max_density_err = std::max(max_density_err, std::abs(tde[j]));

		density_error_ratio = max_density_err / Opt.target_density;

		if (std::abs(density_error_ratio) < Opt.max_density_error_ratio)
			break;
	}

	// Accumulate pressure force
	Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
	{
		f[i] += tpf[i];
	});
}
void HinaPE::PCISPHSolverXayah::_time_integration() const
{
	auto &x = _data->Fluid.positions;
	auto &v = _data->Fluid.velocities;
	auto &f = _data->Fluid.forces;
	const auto &m = _data->Fluid.mass;
	const auto &dt = Opt.current_dt;

	// semi-euler integration
	Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
	{
		v[i] += dt * f[i] / m;
		x[i] += dt * v[i];
	});
}
void HinaPE::PCISPHSolverXayah::_resolve_collision() const
{
	auto &x = _data->Fluid.positions;
	auto &v = _data->Fluid.velocities;

	// Collision
	Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
	{
		_domain->resolve_collision(Opt.radius, Opt.restitution, &x[i], &v[i]);
	});
}
void HinaPE::PCISPHSolverXayah::_update_neighbor() const
{
	// Update Target: NeighborList
	auto &nl = _data->NeighborList;
	const auto fluid_size = _data->fluid_size();
	const auto &x = _data->Fluid.positions;


	std::vector<mVector3> total_positions;
	total_positions.reserve(fluid_size);
	total_positions.insert(total_positions.end(), x.begin(), x.end());

	PointHashGridSearch3 searcher(Opt.kernel_radius);
	searcher.build(total_positions);

	Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
	{
		auto origin = x[i];
		nl[i].clear();
		searcher.for_each_nearby_point(origin, [&](size_t j, const mVector3 &)
		{
			if (i != j)
				nl[i].push_back(j);
		});
	});
}
void HinaPE::PCISPHSolverXayah::_update_density() const
{
	auto &d = _data->Fluid.densities;
	const auto &x = _data->Fluid.positions;
	const auto &m = _data->Fluid.mass;
	const auto &nl = _data->NeighborList;

	StdKernel poly6(Opt.kernel_radius);
	Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
	{
		real sum = poly6(0);
		for (size_t j: nl[i])
		{
			real dist = (x[i] - x[j]).length();
			sum += poly6(dist);
		}
		d[i] = m * sum; // rho(x) = m * sum(W(x - xj))
	});
}
void HinaPE::PCISPHSolverXayah::_init_fluid_particles() const
{
	std::vector<mVector3> init_pos, init_vel;
	_emitter->emit(&init_pos, &init_vel);
	_data->add_fluid(init_pos, init_vel);

	// update mass
	std::vector<std::vector<unsigned int>> temp_neighbor_list;
	temp_neighbor_list.resize(init_pos.size());
	PointHashGridSearch3 searcher(Opt.kernel_radius);
	searcher.build(init_pos);

	Util::parallelFor(Constant::ZeroSize, init_pos.size(), [&](size_t i)
	{
		auto origin = init_pos[i];
		temp_neighbor_list[i].clear();
		searcher.for_each_nearby_point(origin, [&](size_t j, const mVector3 &)
		{
			if (i != j)
				temp_neighbor_list[i].push_back(j);
		});
	});

	StdKernel poly6(Opt.kernel_radius);
	real max_number_density = 0;
	for (int i = 0; i < init_pos.size(); ++i)
	{
		real sum = poly6(0); // self density
		const auto &point = init_pos[i];
		for (const auto &neighbor_point_id: temp_neighbor_list[i])
		{
			auto dist = (point - init_pos[neighbor_point_id]).length();
			sum += poly6(dist);
		}
		max_number_density = std::max(max_number_density, sum);
	}

	if (max_number_density > 0)
		_data->Fluid.mass = std::max((Opt.target_density / max_number_density), HinaPE::Constant::Zero);
	else
		throw std::runtime_error("max_number_density is zero");
}
auto HinaPE::PCISPHSolverXayah::_compute_delta() const -> real
{
	mBBox3 bound(mVector3::Zero(), mVector3::Zero());
	bound.expand(1.5 * Opt.kernel_radius);

	SpikyKernel spiky(Opt.kernel_radius);
	real denom = 0;
	mVector3 denom1 = mVector3::Zero();
	real denom2 = 0;

	std::vector<mVector3> points;
	real spacing = Opt.kernel_radius;
	real half_spacing = 0.5 * Opt.kernel_radius;
	real box_w = bound.width();
	real box_h = bound.height();
	real box_d = bound.depth();

	mVector3 p;
	bool has_offset = false;
	for (int i = 0; i * half_spacing <= box_d; ++i)
	{
		p.z() = i * half_spacing + bound._lower_corner.z();

		real offset = (has_offset) ? half_spacing : 0;

		for (int j = 0; j * spacing + offset <= box_h; ++j)
		{
			p.y() = j * spacing + offset + bound._lower_corner.y();
			for (int k = 0; k * spacing + offset <= box_w; ++k)
			{
				p.x() = k * spacing + offset + bound._lower_corner.x();
				points.push_back(p);
			}
		}

		has_offset = !has_offset;
	}

	for (const auto &p_i: points)
	{
		real dist = p_i.length();

		if ((dist * dist) < (Opt.kernel_radius * Opt.kernel_radius))
		{
			mVector3 dir = (dist > 0) ? p_i / dist : mVector3::Zero();

			auto gradWij = spiky.gradient(dist, dir);
			denom1 += gradWij;
			denom2 += gradWij.dot(gradWij);
		}
	}

	denom += -denom1.dot(denom1) - denom2;
	real beta = 2 * Math::square(_data->Fluid.mass * Opt.current_dt / Opt.target_density);

	return (std::fabs(denom) > 0) ? -1 / (beta * denom) : 0;
}
void HinaPE::PCISPHSolverXayah::INSPECT()
{
	// Solver Parameters
	ImGui::Text("SOLVER INSPECTOR");
	ImGui::Text("Fluids: %zu", _data->fluid_size());
	ImGui::Separator();

	// Debug Info
	auto inst_id = _data->_inst_id;
	if (inst_id >= 0 && inst_id < _data->fluid_size())
	{
		ImGui::Text("Inst: %d", inst_id);
		ImGui::Text("Mass: %f", _data->Fluid.mass);
		ImGui::Text("Force: {%.3f, %.3f, %.3f}", _data->Fluid.forces[inst_id].x(), _data->Fluid.forces[inst_id].y(), _data->Fluid.forces[inst_id].z());
		ImGui::Text("Density: %.3f", _data->Fluid.densities[inst_id]);
		ImGui::Text("Pressure: %.3f", _data->Fluid.pressures[inst_id]);
		ImGui::Text("Neighbors: %zu", _data->NeighborList[inst_id].size());
		ImGui::Separator();
		if (_data->debug_info.empty())
			return;
		for (auto &info: _data->debug_info[inst_id])
		{
			ImGui::Text("%s\n", info.c_str());
			ImGui::Separator();
		}
	}
}
// ================================================== Solver ==================================================
// ============================================================================================================


// ==========================================================================================================
// ================================================== Data ==================================================
HinaPE::PCISPHSolverXayah::Data::Data()
{
	track(&Fluid.last_positions);
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
	NeighborList.insert(NeighborList.end(), size, std::vector<unsigned int>());

	color_map.insert(color_map.end(), size, Color::ORANGE);
	debug_info.insert(debug_info.end(), size, std::vector<std::string>());

	Fluid.temp_positions.insert(Fluid.temp_positions.end(), size, mVector3::Zero());
	Fluid.temp_velocities.insert(Fluid.temp_velocities.end(), size, mVector3::Zero());
	Fluid.temp_pressure_forces.insert(Fluid.temp_pressure_forces.end(), size, mVector3::Zero());
	Fluid.temp_density_errors.insert(Fluid.temp_density_errors.end(), size, Constant::Zero);

	Fluid.last_positions.insert(Fluid.last_positions.end(), positions.begin(), positions.end());
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
