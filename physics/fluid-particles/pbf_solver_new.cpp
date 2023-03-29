#include "pbf_solver_new.h"

#define S HinaPE::Math::to_string
#define Record(x, y) _data->debug_info[i].emplace_back(x + S(y))

// ============================================================================================================
// ================================================== Solver ==================================================
void HinaPE::PBFSolverNew::init()
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
	if (_sphere == nullptr)
	{
		auto sphere = std::make_shared<Kasumi::SphereObject>();
		sphere->POSE.position = mVector3(0.5, -1.0, 0);
		sphere->POSE.scale = 0.3 * mVector3::One();
		sphere->_update_surface();
		_sphere = sphere;
		_sphere->set_color(Color::RED);
	}
	if (_cube == nullptr)
	{
		auto cube = std::make_shared<Kasumi::CubeObject>();
		cube->POSE.position = mVector3(-0.5, -1.0, 0);
		cube->POSE.euler = mVector3(45, 0, 45);
		cube->POSE.scale = mVector3(0.2, 0.3, 0.2);
		cube->_update_surface();
		_cube = cube;
		_cube->set_color(Color::PURPLE);
	}

	_data->DEFAULT_SCALE = 0.5 * _opt.radius * mVector3::One();
	_emitter->_opt.spacing = 1.2 * _opt.radius;

	_init_fluid_particles();
	_init_boundary_particles();
	_init_collider();

	_update_neighbor();
	_update_density();
}

void HinaPE::PBFSolverNew::update(real dt) const
{
	// algorithm line 20~24
	_update_positions_and_velocities(); // we reorder the loop sequence for easier debugging

	// PBF Loop start here
	_reset_debug_info();

	// algorithm line 1~4
	_apply_force_and_predict_position();

	// algorithm line 5~19
	_solve_density_constraints();
}

void HinaPE::PBFSolverNew::reset()
{
	_data->reset();
	init();
}

void HinaPE::PBFSolverNew::_init_fluid_particles() const
{
	std::vector<mVector3> init_pos, init_vel;
	_emitter->emit(&init_pos, &init_vel);
	_data->add_fluid(init_pos, init_vel);

	// update mass
	std::vector<std::vector<unsigned int>> temp_neighbor_list;
	temp_neighbor_list.resize(init_pos.size());
	PointHashGridSearch3 searcher(_opt.kernel_radius);
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

	StdKernel poly6(_opt.kernel_radius);
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
		_data->Fluid.mass = std::max((_opt.target_density / max_number_density), HinaPE::Constant::Zero);
	else
		throw std::runtime_error("max_number_density is zero");
}

void HinaPE::PBFSolverNew::_init_boundary_particles() const
{
	std::vector<mVector3> target_boundary = _domain->generate_surface();
	_data->add_boundary(target_boundary, &_domain->POSE);

	// update mass
	std::vector<std::vector<unsigned int>> temp_neighbor_list;
	temp_neighbor_list.resize(target_boundary.size());
	PointHashGridSearch3 searcher(_opt.kernel_radius);
	searcher.build(target_boundary);

	Util::parallelFor(Constant::ZeroSize, target_boundary.size(), [&](size_t i)
	{
		auto origin = target_boundary[i];
		temp_neighbor_list[i].clear();
		searcher.for_each_nearby_point(origin, [&](size_t j, const mVector3 &)
		{
			if (i != j)
				temp_neighbor_list[i].push_back(j);
		});
	});

	StdKernel poly6(_opt.kernel_radius);
	real max_number_density = 0;
	for (int i = 0; i < target_boundary.size(); ++i)
	{
		real sum = poly6(0); // self density
		const auto &point = target_boundary[i];
		for (const auto &neighbor_point_id: temp_neighbor_list[i])
		{
			auto dist = (point - target_boundary[neighbor_point_id]).length();
			sum += poly6(dist);
		}
		max_number_density = std::max(max_number_density, sum);
	}

	if (max_number_density > 0)
		_data->Boundary.mass.insert(_data->Boundary.mass.end(), target_boundary.size(), 10 * std::max((_opt.target_density / max_number_density), HinaPE::Constant::Zero));
	else
		throw std::runtime_error("max_number_density is zero");
}

void HinaPE::PBFSolverNew::_init_collider() const
{
	{
		// generate sphere surface points
		std::vector<mVector3> target_boundary = _sphere->generate_surface();
		_data->add_boundary(target_boundary, &_sphere->POSE);

		// update mass
		std::vector<std::vector<unsigned int>> temp_neighbor_list;
		temp_neighbor_list.resize(target_boundary.size());
		PointHashGridSearch3 searcher(_opt.kernel_radius);
		searcher.build(target_boundary);

		Util::parallelFor(Constant::ZeroSize, target_boundary.size(), [&](size_t i)
		{
			auto origin = target_boundary[i];
			temp_neighbor_list[i].clear();
			searcher.for_each_nearby_point(origin, [&](size_t j, const mVector3 &)
			{
				if (i != j)
					temp_neighbor_list[i].push_back(j);
			});
		});

		StdKernel poly6(_opt.kernel_radius);
		real max_number_density = 0;
		for (int i = 0; i < target_boundary.size(); ++i)
		{
			real sum = poly6(0); // self density
			const auto &point = target_boundary[i];
			for (const auto &neighbor_point_id: temp_neighbor_list[i])
			{
				auto dist = (point - target_boundary[neighbor_point_id]).length();
				sum += poly6(dist);
			}
			max_number_density = std::max(max_number_density, sum);
		}

		if (max_number_density > 0)
			_data->Boundary.mass.insert(_data->Boundary.mass.end(), target_boundary.size(), 5 * std::max((_opt.target_density / max_number_density), HinaPE::Constant::Zero));
		else
			throw std::runtime_error("max_number_density is zero");
	}
	{
		// generate sphere surface points
		std::vector<mVector3> target_boundary = _cube->generate_surface();
		_data->add_boundary(target_boundary, &_cube->POSE);

		// update mass
		std::vector<std::vector<unsigned int>> temp_neighbor_list;
		temp_neighbor_list.resize(target_boundary.size());
		PointHashGridSearch3 searcher(_opt.kernel_radius);
		searcher.build(target_boundary);

		Util::parallelFor(Constant::ZeroSize, target_boundary.size(), [&](size_t i)
		{
			auto origin = target_boundary[i];
			temp_neighbor_list[i].clear();
			searcher.for_each_nearby_point(origin, [&](size_t j, const mVector3 &)
			{
				if (i != j)
					temp_neighbor_list[i].push_back(j);
			});
		});

		StdKernel poly6(_opt.kernel_radius);
		real max_number_density = 0;
		for (int i = 0; i < target_boundary.size(); ++i)
		{
			real sum = poly6(0); // self density
			const auto &point = target_boundary[i];
			for (const auto &neighbor_point_id: temp_neighbor_list[i])
			{
				auto dist = (point - target_boundary[neighbor_point_id]).length();
				sum += poly6(dist);
			}
			max_number_density = std::max(max_number_density, sum);
		}

		if (max_number_density > 0)
			_data->Boundary.mass.insert(_data->Boundary.mass.end(), target_boundary.size(), 5 * std::max((_opt.target_density / max_number_density), HinaPE::Constant::Zero));
		else
			throw std::runtime_error("max_number_density is zero");
	}
}

void HinaPE::PBFSolverNew::_apply_force_and_predict_position() const
{
	// Update Target: predicted_position, velocities, forces
	_data->Fluid.predicted_position = _data->Fluid.positions;

	auto &p = _data->Fluid.predicted_position;
	auto &v = _data->Fluid.velocities;
	auto &f = _data->Fluid.forces;
	const auto fluid_size = _data->fluid_size();
	const auto &m = _data->Fluid.mass;
	const auto &g = _opt.gravity;
	const auto &dt = _opt.current_dt;

	Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
	{
		// Gravity Forces
		mVector3 gravity = m * g;
		f[i] = gravity;

		// semi-implicit Euler
		v[i] += dt * f[i] / m;
		p[i] += dt * v[i];
	});
}

void HinaPE::PBFSolverNew::_update_neighbor() const
{
	// Update Target: NeighborList
	const auto fluid_size = _data->fluid_size();
	const auto boundary_size = _data->boundary_size();
	const auto &p = _data->Fluid.predicted_position; // note: we use predicted position, because we need to update neighbor in the sub iteration

	std::vector<mVector3> total_positions;
	total_positions.reserve(fluid_size + boundary_size);
	total_positions.insert(total_positions.end(), p.begin(), p.end());
	_data->update_boundary();
	total_positions.insert(total_positions.end(), _data->Boundary.positions.begin(), _data->Boundary.positions.end());

	PointHashGridSearch3 searcher(_opt.kernel_radius);
	searcher.build(total_positions);
	auto &nl = _data->NeighborList;
	Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
	{
		auto origin = p[i];
		nl[i].clear();
		searcher.for_each_nearby_point(origin, [&](size_t j, const mVector3 &)
		{
			if (i != j)
				nl[i].push_back(j);
		});
	});

	// ==================== Debug ====================
	_data->neighbor_list_debug = nl; // copy neighbor list to debug
}

void HinaPE::PBFSolverNew::_update_density() const
{
	// Update Target: densities
	auto &d = _data->Fluid.densities;
	const auto &p = _data->Fluid.predicted_position;
	const auto &b = _data->Boundary.positions;
	const auto &m = _data->Fluid.mass;
	const auto &bm = _data->Boundary.mass;
	const auto &nl = _data->NeighborList;
	const auto fluid_size = _data->fluid_size();
	const auto boundary_size = _data->boundary_size();

	StdKernel poly6(_opt.kernel_radius);
	Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
	{
		real density = m * poly6(0); // self density
		for (const auto j: nl[i])
		{
			if (j < fluid_size)
			{
				real dist = (p[i] - p[j]).length();
				density += m * poly6(dist);
			} else
			{
				if (_opt.use_akinci2012_collision)
				{
					density += bm[j - fluid_size] * poly6((p[i] - b[j - fluid_size]).length());
				}
			}
		}
		d[i] = density; // rho(x) = m * sum(W(x - xj))
	});
}

void HinaPE::PBFSolverNew::_solve_density_constraints() const
{
	// Update Target: Lambdas, Delta P
	for (int i = 0; i < _opt.constraint_solver_iterations; ++i)
	{
		// Note:
		// "i" is the index of the current particle,
		// "j" is the index of the neighbor particle

		_update_neighbor();
		_update_density();

		const auto fluid_size = _data->fluid_size();
		const auto boundary_size = _data->boundary_size();
		const auto d0 = _opt.target_density;
		const auto h = _opt.radius;
		const auto eps = 1e-6;

		const auto &p = _data->Fluid.predicted_position;
		const auto &b = _data->Boundary.positions;
		const auto &d = _data->Fluid.densities;
		const auto &nl = _data->NeighborList;
		const auto &m = _data->Fluid.mass;
		const auto &bm = _data->Boundary.mass;
		StdKernel poly6(_opt.kernel_radius);

		auto &lambdas = _data->Fluid.lambdas;
		Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
		{
			real d_i = d[i];
			real C_i = d_i / d0 - 1;

			if (C_i > 0) // if density is bigger than water density, do constraints projection
			{
				real sum_grad_C_i_squared = 0;
				mVector3 grad_C_i = mVector3::Zero();

				for (const auto j: nl[i])
				{
					if (j < fluid_size)
					{
						const auto p_i = p[i];
						const auto p_j = p[j];
						const mVector3 grad_C_j = -(m / d0) * poly6.gradient(p_i - p_j);

						// Equation (8)
						sum_grad_C_i_squared += grad_C_j.length_squared();
						grad_C_i -= grad_C_j;
					} else
					{
						if (_opt.use_akinci2012_collision)
						{
							const auto p_i = p[i];
							const auto b_j = b[j - fluid_size];
							const mVector3 grad_C_j = -(bm[j - fluid_size] / d0) * poly6.gradient(p_i - b_j);

							sum_grad_C_i_squared += grad_C_j.length_squared();
							grad_C_i -= grad_C_j;
						}
					}
				}

				sum_grad_C_i_squared += grad_C_i.length_squared();

				// Equation (11): compute lambda
				real lambda = -C_i / (sum_grad_C_i_squared + eps); // eps is for soft constraint
				lambdas[i] = lambda; // thread safe write

				// for debug
				Record("C: ", C_i);
				Record("Sum Grad C Squared: ", sum_grad_C_i_squared);
				Record("Grad C: ", grad_C_i);
				Record("Lambda: ", lambda);
			} else
				lambdas[i] = 0;
		});

		auto &dp = _data->Fluid.delta_p;
		Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
		{
			const auto &lambda_i = lambdas[i];

			auto k_corr = m * 1.0e-04;
			auto n_corr = 4.0;
			auto q_corr = 0.1;

			// Equation (12): compute delta p
			mVector3 delta_p_i = mVector3::Zero();
			for (const auto j: nl[i])
			{
				if (j < fluid_size)
				{
					const auto &lambda_j = lambdas[j];
					const auto p_i = p[i];
					const auto p_j = p[j];

					const auto w_corr = poly6(q_corr * h);
					const auto ratio = poly6((p_i - p_j).length()) / w_corr;
					const auto s_corr = -k_corr * pow(ratio, n_corr);

					const mVector3 grad_C_j = -(m / d0) * poly6.gradient(p_i - p_j);
					delta_p_i -= (lambda_i + lambda_j + s_corr) * grad_C_j;
				} else // Boundary: Akinci2012
				{
					if (_opt.use_akinci2012_collision)
					{
						const auto p_i = p[i];
						const auto b_j = b[j - fluid_size];

						const mVector3 grad_C_j = -(bm[j - fluid_size] / d0) * poly6.gradient(p_i - b_j);
						delta_p_i -= (lambda_i) * grad_C_j;
					}
				}
			}
			dp[i] = delta_p_i; // thread safe write

			Record("delta p: ", delta_p_i);
		});

		// Finally, apply delta p to all particles
		auto &p_to_write = _data->Fluid.predicted_position;
		Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
		{
			p_to_write[i] -= dp[i];
		});

		if (!_opt.use_akinci2012_collision)
		{
			// Dead simple collision
			const auto &v = _data->Fluid.velocities;
			Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
			{
				auto temp_v = v[i]; // we don't need to update velocity here
				_domain->resolve_collision(_opt.radius, _opt.restitution, &p_to_write[i], &temp_v);
				_sphere->resolve_collision(_opt.radius, _opt.restitution, &p_to_write[i], &temp_v);
				_cube->resolve_collision(_opt.radius, _opt.restitution, &p_to_write[i], &temp_v);
			});
		}

		// ==================== Debug ====================
		_data->p_debug.push_back(p);
		_data->lambdas_debug.push_back(lambdas);
		_data->delta_p_debug.push_back(dp);
	}
}

void HinaPE::PBFSolverNew::_update_positions_and_velocities() const
{
	const auto &p = _data->Fluid.predicted_position;
	const auto &d = _data->Fluid.densities;
	const auto &nl = _data->NeighborList;
	const auto m = _data->Fluid.mass;
	const auto fluid_size = _data->fluid_size();
	const auto dt = _opt.current_dt;
	StdKernel poly6(_opt.kernel_radius);

	auto &x = _data->Fluid.positions;
	auto &v = _data->Fluid.velocities;
	// First, update velocities
	Util::parallelFor(Constant::ZeroSize, x.size(), [&](size_t i)
	{
		v[i] = (p[i] - x[i]) / dt;
	});

	if (_opt.enable_viscosity)
	{
		// Apply XSPH viscosity
		const auto c = _opt.viscosity;
		Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
		{
			const auto &p_i = p[i];
			const auto &v_i = v[i];

			mVector3 sum_value = mVector3::Zero();
			for (auto const &j: nl[i])
			{
				if (j < fluid_size)
				{
					const real d_j = d[j];
					const auto &p_j = p[j];
					const auto &v_j = v[j];
					mVector3 tmp = v_i - v_j;
					tmp *= poly6((p_i - p_j).length()) * (m / d_j);
					sum_value += tmp;
				} else
				{
					if (_opt.use_akinci2012_collision) {}
				}
			}

			v[i] = v_i - c * sum_value;

			Record("XSPH Vis: ", c * sum_value);
		});
	}

	if (_opt.enable_vorticity)
	{
		Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
		{
			const auto &p_i = p[i];
			const auto &v_i = v[i];

			mVector3 f_vorticity = mVector3::Zero();
			mVector3 N = mVector3::Zero();
			mVector3 curl = mVector3::Zero();
			mVector3 curl_x = mVector3::Zero();
			mVector3 curl_y = mVector3::Zero();
			mVector3 curl_z = mVector3::Zero();

			for (auto const &j: nl[i])
			{
				if (j < fluid_size)
				{
					const auto &p_j = p[j];
					const auto &v_j = v[j];
					mVector3 tmp = v_j - v_i;
					curl += tmp.cross(poly6.gradient(p_i - p_j));
					curl_x += tmp.cross(poly6.gradient(p_i + mVector3(0.01, 0, 0) - p_j));
					curl_y += tmp.cross(poly6.gradient(p_i + mVector3(0, 0.01, 0) - p_j));
					curl_z += tmp.cross(poly6.gradient(p_i + mVector3(0, 0, 0.01) - p_j));
				} else
				{
					if (_opt.use_akinci2012_collision) {}
				}
			}

			real curlLen = curl.length();
			N.x() = curl_x.length() - curlLen;
			N.y() = curl_y.length() - curlLen;
			N.z() = curl_z.length() - curlLen;
			N = N.normalized();
			f_vorticity = _opt.vorticity * N.cross(curl);

			v[i] = v_i + f_vorticity * dt;

			Record("Vorticity: ", f_vorticity * dt);
		});
	}

	// Finally, update positions
	Util::parallelFor(Constant::ZeroSize, x.size(), [&](size_t i)
	{
		x[i] = p[i];
	});
}

void HinaPE::PBFSolverNew::INSPECT()
{
	// Solver Parameters
	ImGui::Text("SOLVER INSPECTOR");
	ImGui::Text("Fluids: %zu", _data->fluid_size());
	ImGui::Text("Boundaries: %zu", _data->boundary_size());
	static real min_dt = 0, max_dt = 1;
	ImGui::DragScalar("Time Step", ImGuiDataType_Real, &_opt.current_dt, 0.01, &min_dt, &max_dt, "%.3f");
	static real min_restitution = 0, max_restitution = 1;
	ImGui::DragScalar("Restitution", ImGuiDataType_Real, &_opt.restitution, 0.01, &min_restitution, &max_restitution, "%.2f");
	static int min_solver_iteration = 1, max_solver_iteration = 15;
	ImGui::DragScalar("Constraint Solver Iterations", ImGuiDataType_S32, &_opt.constraint_solver_iterations, 1, &min_solver_iteration, &max_solver_iteration, "%d");
	ImGui::DragScalar("Gravity", ImGuiDataType_Real, &_opt.gravity[1], 0.1, nullptr, nullptr, "%.2f");
	static real min_multiplier = 1e-1, max_multiplier = 3;
	ImGui::DragScalar("Particles Multiplier", ImGuiDataType_Real, &_emitter->_opt.multiplier, 0.1, &min_multiplier, &max_multiplier, "%.2f");
	static real min_radius = 1e-3, max_radius = 9e-1;
	if (ImGui::DragScalar("Radius", ImGuiDataType_Real, &_opt.radius, 1e-3, &min_radius, &max_radius, "%.3f")) { _opt.kernel_radius = _opt.radius * _opt.relative_kernel_radius; }
	static real min_relative_radius = 1, max_relative_radius = 5;
	if (ImGui::DragScalar("Relative Kernel Radius", ImGuiDataType_Real, &_opt.relative_kernel_radius, 1e-1, &min_relative_radius, &max_relative_radius, "%.3f")) { _opt.kernel_radius = _opt.radius * _opt.relative_kernel_radius; }
	ImGui::Checkbox("Surface Tension", &_opt.enable_surface_tension);
	ImGui::Checkbox("XSPH Viscosity", &_opt.enable_viscosity);
	if (_opt.enable_viscosity)
	{
		static real viscosity_max = 1;
		ImGui::SliderScalar("Viscosity", ImGuiDataType_Real, &_opt.viscosity, &Constant::Zero, &viscosity_max);
	}
	ImGui::Checkbox("Vorticity", &_opt.enable_vorticity);
	if (_opt.enable_vorticity)
	{
		static real vorticity_max = 0.0001;
		ImGui::SliderScalar("Vorticity", ImGuiDataType_Real, &_opt.vorticity, &Constant::Zero, &vorticity_max);
	}
	ImGui::Checkbox("Akinci2012 Collision", &_opt.use_akinci2012_collision);
	ImGui::Separator();

	// Debug Info
	auto inst_id = _data->_inst_id;
	if (inst_id >= 0 && inst_id < _data->fluid_size())
	{
		ImGui::Text("Inst: %d", inst_id);
		ImGui::Text("Mass: %f", _data->Fluid.mass);
		ImGui::Text("Force: {%.3f, %.3f, %.3f}", _data->Fluid.forces[inst_id].x(), _data->Fluid.forces[inst_id].y(), _data->Fluid.forces[inst_id].z());
		ImGui::Text("Density: %.3f", _data->Fluid.densities[inst_id]);
		ImGui::Text("Lambda: %.3f", _data->Fluid.lambdas[inst_id]);
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
void HinaPE::PBFSolverNew::_reset_debug_info() const
{
	_data->debug_info.clear();
	_data->debug_info.resize(_data->fluid_size());
	_data->p_debug.clear();
	_data->lambdas_debug.clear();
	_data->delta_p_debug.clear();
}
// ================================================== Solver ==================================================
// ============================================================================================================


// ==========================================================================================================
// ================================================== Data ==================================================
HinaPE::PBFSolverNew::Data::Data()
{
	track(&Fluid.positions);
	track_colormap(&color_map);
}
auto HinaPE::PBFSolverNew::Data::fluid_size() const -> size_t { return Fluid.positions.size(); }
auto HinaPE::PBFSolverNew::Data::boundary_size() const -> size_t { return Boundary.positions.size(); }
void HinaPE::PBFSolverNew::Data::add_fluid(const std::vector<mVector3> &positions, const std::vector<mVector3> &velocities)
{
	if (positions.size() != velocities.size())
		throw std::runtime_error("positions.size() != velocities.size()");

	auto size = positions.size();
	Fluid.positions.insert(Fluid.positions.end(), positions.begin(), positions.end());
	Fluid.velocities.insert(Fluid.velocities.end(), velocities.begin(), velocities.end());
	Fluid.predicted_position.insert(Fluid.predicted_position.end(), positions.begin(), positions.end());
	Fluid.forces.insert(Fluid.forces.end(), size, mVector3::Zero());
	Fluid.densities.insert(Fluid.densities.end(), size, 0.0);
	Fluid.lambdas.insert(Fluid.lambdas.end(), size, 0.0);
	Fluid.delta_p.insert(Fluid.delta_p.end(), size, mVector3::Zero());
	NeighborList.insert(NeighborList.end(), size, std::vector<unsigned int>());

	color_map.insert(color_map.end(), size, Color::ORANGE);
	debug_info.insert(debug_info.end(), size, std::vector<std::string>());
}
void HinaPE::PBFSolverNew::Data::add_boundary(const std::vector<mVector3> &positions, const Kasumi::Pose *pose)
{
	size_t start = Boundary.positions_origin.size();
	Boundary.positions_origin.insert(Boundary.positions_origin.end(), positions.begin(), positions.end());
	size_t end = Boundary.positions_origin.size();

	Boundary.poses.push_back(pose);
	Boundary.boundary_sizes.emplace_back(start, end);
	Boundary.positions.resize(Boundary.positions_origin.size());
	update_boundary();
}
void HinaPE::PBFSolverNew::Data::update_boundary()
{
	for (int i = 0; i < Boundary.poses.size(); ++i)
	{
		std::vector<mVector3> res;
		const auto [start, end] = Boundary.boundary_sizes[i];
		const auto m = Boundary.poses[i]->get_model_matrix();

		for (size_t j = start; j < end; ++j)
			Boundary.positions[j] = (m * mVector4(Boundary.positions_origin[j].x(), Boundary.positions_origin[j].y(), Boundary.positions_origin[j].z(), 1)).xyz();
	}
}
void HinaPE::PBFSolverNew::Data::reset()
{
	Fluid.positions.clear();
	Fluid.velocities.clear();
	Fluid.predicted_position.clear();
	Fluid.forces.clear();
	Fluid.densities.clear();
	Fluid.lambdas.clear();
	Fluid.delta_p.clear();
	Fluid.mass = 1e-3;
	Boundary.positions.clear();
	Boundary.positions_origin.clear();
	Boundary.mass.clear();
	Boundary.poses.clear();
	Boundary.boundary_sizes.clear();
	NeighborList.clear();
	color_map.clear();
	debug_info.clear();
	neighbor_list_debug.clear();
	p_debug.clear();
	lambdas_debug.clear();
	delta_p_debug.clear();
}
// ================================================== Data ==================================================
// ==========================================================================================================
