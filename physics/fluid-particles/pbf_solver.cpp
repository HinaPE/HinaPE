#include "pbf_solver.h"

#define S HinaPE::Math::to_string
#define Record(x, y) _debug[i].emplace_back(x + S(y))


// ============================== Solver ==============================
void HinaPE::PBFSolver::init()
{
	_emit_particles();
	_data->_forces.resize(_data->_positions.size(), mVector3::Zero());
	_data->_densities.resize(_data->_positions.size(), 0);
	_data->_predicted_position = _data->_positions; // copy
	_data->_lambdas.resize(_data->_positions.size(), 0);
	_data->_delta_p.resize(_data->_positions.size(), mVector3::Zero());
	_update_neighbor();
	VALID_CHECK();
	_opt.inited = true;
}

void HinaPE::PBFSolver::update(real dt) const
{
	_data->_debug_info.clear();
	_data->_debug_info.resize(_data->_positions.size());

	// algorithm line 1~4
	_apply_force_and_predict_position();

	// algorithm line 5~7
	_update_neighbor();

	// algorithm line 8~19
	_solve_density_constraints();

	// algorithm line 20~24
	_update_positions_and_velocities();

	// simple collision handling here (temporary)
	_resolve_collision();
}

void HinaPE::PBFSolver::_emit_particles() const
{
	_emitter->emit(&_data->_positions, &_data->_velocities);
}

void HinaPE::PBFSolver::_apply_force_and_predict_position() const
{
	_data->_predicted_position = _data->_positions; // copy
	_data->_forces.resize(_data->_positions.size(), mVector3::Zero());

	auto &p = _data->_predicted_position;
	auto &v = _data->_velocities;
	auto &f = _data->_forces;
	const auto &m = _data->_mass;
	const auto g = _opt.gravity;
	const auto dt = _opt.current_dt;

	// for debug
	auto &_debug = _data->_debug_info;

	// Gravity Forces
	Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&p, &v, &f, &m, &g, &dt, &_debug](size_t i)
	{
		mVector3 gravity = m * g;
		f[i] = gravity;

		v[i] += dt * f[i] / m;
		p[i] += dt * v[i];
	});

    _boundary_data->_predicted_position = _boundary_data->_positions;
    auto &p_b = _boundary_data->_predicted_position;
    auto &v_b = _boundary_data->_velocities;

    Util::parallelFor(Constant::ZeroSize, _boundary_data->_positions.size(), [&p_b, &v_b, &dt](size_t i)
    {
        v_b[i] += mVector3(0.0,0.0,0.0);
        p_b[i] += dt * v_b[i];
    });
}

void HinaPE::PBFSolver::_update_neighbor() const
{
	_data->_densities.resize(_data->_positions.size(), 0);
	_data->_update_neighbor();

    _boundary_data->_densities.resize(_boundary_data->_positions.size(), 0);
    _boundary_data->_update_neighbor();
}

void HinaPE::PBFSolver::_solve_density_constraints() const
{
	for (int i = 0; i < _opt.constraint_solver_iterations; ++i)
	{
		// Note:
		// "i" is the index of the current particle,
		// "j" is the index of the neighbor particle

		_data->_update_density();

		const size_t size = _data->_predicted_position.size();
		const real d0 = _data->target_density;
		const real eps = 1e-6;

		const auto &p = _data->_predicted_position;
		const auto &d = _data->_densities;
		const auto &nl = _data->_neighbor_lists;
		const auto &kernel = _data->poly6_kernel;
		const auto &m = _data->_mass;

		// for debug
		auto &_debug = _data->_debug_info;

		// First, we compute all lambdas
		auto &lambdas = _data->_lambdas;
		lambdas.resize(size, 0); // initialize lambdas to zero
		Util::parallelFor(Constant::ZeroSize, size, [&lambdas, &p, &d, &m, &nl, &kernel, d0, eps, &_debug](size_t i)
		{
			real d_i = d[i];
			real C_i = d_i / d0 - 1;

			if (C_i > 0) // if density is bigger than water density, do constraints projection
			{
				real sum_grad_C_i_squared = 0;
				mVector3 grad_C_i = mVector3::Zero();

				std::string debug_sum_grad_C_i_squared;
				std::string debug_grad_C_i;

				for (const auto j: nl[i])
				{
					const auto p_i = p[i];
					const auto p_j = p[j];
					const mVector3 grad_C_j = -(m / d0) * (*kernel).gradient(p_i - p_j);

					// Equation (8)
					sum_grad_C_i_squared += grad_C_j.length_squared();
					grad_C_i -= grad_C_j;
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
			}
		});


		// Second, we compute all correction delta p
		auto &dp = _data->_delta_p;
		dp.resize(size, mVector3::Zero()); // initialize delta p to zero vector
        auto &h = _data->kernel_radius;

		Util::parallelFor(Constant::ZeroSize, size, [&dp, &h, &lambdas, &p, &m, d0, &nl, &kernel, &_debug](size_t i)
		{
			const auto &lambda_i = lambdas[i];

            auto k_corr = m * 1.0e-04;
            //auto k_corr = 0.1;
            auto n_corr = 4.0;
            auto q_corr = 0.1;

			// Equation (12): compute delta p
			mVector3 delta_p_i = mVector3::Zero();
			for (const auto j: nl[i])
			{
				const auto &lambda_j = lambdas[j];
				const auto p_i = p[i];
				const auto p_j = p[j];

                const auto w_corr = (*kernel).operator()(q_corr * h);
                const auto ratio = (*kernel).operator()((p_i - p_j).length()) / w_corr;
                const auto s_corr = -k_corr * pow(ratio,n_corr);

				const mVector3 grad_C_j = -(m / d0) * (*kernel).gradient(p_i - p_j);
				delta_p_i -= (lambda_i + lambda_j + s_corr) * grad_C_j;
			}
			dp[i] = delta_p_i; // thread safe write

			Record("delta p: ", delta_p_i);
		});


		// Finally, apply delta p to all particles
		auto &p_to_write = _data->_predicted_position;
		Util::parallelFor(Constant::ZeroSize, size, [&p_to_write, &dp](size_t i)
		{
			p_to_write[i] -= dp[i];
		});
	}
}

void HinaPE::PBFSolver::_update_positions_and_velocities() const
{
	auto &x = _data->_positions;
	auto &v = _data->_velocities;

	const auto &p = _data->_predicted_position;
	const auto &d = _data->_densities;
	const auto &m = _data->_mass;
	const auto &nl = _data->_neighbor_lists;
	const auto size = p.size();
	const auto dt = _opt.current_dt;
	const auto &kernel = _data->poly6_kernel;

	// First, update velocities
	Util::parallelFor(Constant::ZeroSize, size, [&x, &p, &v, &dt](size_t i)
	{
		v[i] = (p[i] - x[i]) / dt;
	});

	// Apply XSPH viscosity
	auto c = _data->viscosity_coeff;

	Util::parallelFor(Constant::ZeroSize, size, [&p, &d, &m, &nl, &v, &c, &kernel](size_t i)
	{
		const auto &p_i = p[i];
		const auto &v_i = v[i];

		mVector3 sum_value = mVector3::Zero();

		for (auto const &j: nl[i])
		{
			const real d_j = d[j];
			const auto &p_j = p[j];
			const auto &v_j = v[j];
			mVector3 tmp = v_i - v_j;
			tmp *= (*kernel)((p_i - p_j).length()) * (m / d_j);
            sum_value += tmp;
        }

		v[i] = v_i - c * sum_value;
	});

    // Apply Vorticity Confinement

    Util::parallelFor(Constant::ZeroSize, size, [&p, &nl, &v, &dt, &kernel](size_t i)
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
            const auto &p_j = p[j];
            const auto &v_j = v[j];
            mVector3 tmp = v_j - v_i;
            curl += tmp.cross((*kernel).gradient(p_i - p_j));
            curl_x += tmp.cross((*kernel).gradient(p_i + mVector3(0.01,0,0) - p_j));
            curl_y += tmp.cross((*kernel).gradient(p_i + mVector3(0,0.01,0) - p_j));
            curl_z += tmp.cross((*kernel).gradient(p_i + mVector3(0,0,0.01) - p_j));
        }

        real curlLen = curl.length();
        N.x() = curl_x.length() - curlLen;
        N.y() = curl_y.length() - curlLen;
        N.z() = curl_z.length() - curlLen;
        N = N.normalized();
        f_vorticity = 0.00001 * N.cross(curl);

        v[i] = v_i + f_vorticity * dt;

    });

	// Finally, update positions
	Util::parallelFor(Constant::ZeroSize, size, [&x, &p](size_t i)
	{
		x[i] = p[i];
	});
}

void HinaPE::PBFSolver::_resolve_collision() const
{
	// collide with domain
	Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
	{
		_domain->resolve_collision(_data->_radius, _opt.restitution, &_data->_positions[i], &_data->_velocities[i]);
	});
}

// ============================== Solver Data ==============================

HinaPE::PBFSolver::Data::Data() { track(&_positions); DEFAULT_SCALE = _radius * mVector3::One(); }

void HinaPE::PBFSolver::Data::_update_neighbor()
{
	_neighbor_search->build(_predicted_position);
	_neighbor_lists.resize(_predicted_position.size());

	for (size_t i = 0; i < _predicted_position.size(); ++i)
	{
		mVector3 origin = _predicted_position[i];
		_neighbor_lists[i].clear();

		_neighbor_search->for_each_nearby_point(origin, [&](size_t j, const mVector3 &)
		{
			if (i != j)
				_neighbor_lists[i].push_back(j);
		});
	}
}

void HinaPE::PBFSolver::Data::_update_density()
{
	if (!_mass_inited)
		_update_mass(); // update mass to ensure the initial density is 1000

	auto &x = _predicted_position;
	auto &d = _densities;
	const auto &m = _mass;

	Util::parallelFor(Constant::ZeroSize, _positions.size(), [&](size_t i)
	{
		real sum = (*poly6_kernel)(0); // self density
		for (int j = 0; j < _neighbor_lists[i].size(); ++j)
		{
			real dist = (x[i] - x[_neighbor_lists[i][j]]).length();
			sum += (*poly6_kernel)(dist);
		}
		d[i] = m * sum; // rho(x) = m * sum(W(x - xj))
	});
}

void HinaPE::PBFSolver::Data::_update_mass()
{
	// TODO: to be rewritten
	_mass = 1.0;

	real max_number_density = 0;
	for (int i = 0; i < _positions.size(); ++i)
	{
		real sum = (*poly6_kernel)(0); // self density
		const auto &point = _positions[i];
		for (const auto &neighbor_point_id: _neighbor_lists[i])
		{
			auto dist = (point - _positions[neighbor_point_id]).length();
			sum += (*poly6_kernel)(dist);
		}
		max_number_density = std::max(max_number_density, sum);
	}

	if (max_number_density > 0)
		_mass = std::max((target_density / max_number_density), HinaPE::Constant::Zero);

	_mass_inited = true;
}


void HinaPE::PBFSolver::INSPECT()
{
	ImGui::Text("Solver Inspector");
	ImGui::Text("Particles: %zu", _data->_positions.size());
	INSPECT_REAL(_opt.gravity[1], "g");
	ImGui::Separator();
}

void HinaPE::PBFSolver::Data::INSPECT()
{
	PoseBase::INSPECT();
	if (_inst_id >= 0 && _inst_id < _densities.size())
	{
		ImGui::Text("Inst: %d", _inst_id);
		ImGui::Text("Mass: %f", _mass);
		ImGui::Text("Force: {%.3f, %.3f, %.3f}", _forces[_inst_id].x(), _forces[_inst_id].y(), _forces[_inst_id].z());
		ImGui::Text("Density: %.3f", _densities[_inst_id]);
		ImGui::Text("Lambda: %.3f", _lambdas[_inst_id]);
		ImGui::Text("Neighbors: %zu", _neighbor_lists[_inst_id].size());
		ImGui::Separator();
		if (_debug_info.empty())
			return;
		for (auto &info: _debug_info[_inst_id])
		{
			ImGui::Text("%s\n", info.c_str());
			ImGui::Separator();
		}
	}
}

void HinaPE::PBFSolver::VALID_CHECK() const
{
	if (_data == nullptr) throw std::runtime_error("PBFSolver::_data is nullptr");
	if (_domain == nullptr) throw std::runtime_error("PBFSolver::_domain is nullptr");
	if (_emitter == nullptr) throw std::runtime_error("PBFSolver::_emitter is nullptr");

	if (_data->_positions.size() != _data->_velocities.size() &&
		_data->_positions.size() != _data->_forces.size() &&
		_data->_positions.size() != _data->_densities.size() &&
		_data->_positions.size() != _data->_predicted_position.size())
		throw std::runtime_error("PBFSolver::_data size mismatch");
}

void HinaPE::PBFSolver::resizeParticles(size_t fluid_size, size_t boundary_size)
{
    /// _data ? _boundary_data
    _data->_positions.resize(fluid_size + boundary_size);
    _data->_velocities.resize(fluid_size + boundary_size);
    _data->_forces.resize(fluid_size + boundary_size);
    _data->_densities.resize(fluid_size);
    _data->_predicted_position.resize(fluid_size);
    _data->_lambdas.resize(fluid_size);
    _data->_neighbor_lists.resize(fluid_size + boundary_size);
}

// ============================== Boundary Data ==============================
HinaPE::PBFSolver::BoundaryData::BoundaryData() {
    track(&_positions); DEFAULT_SCALE = _radius * mVector3::One();
}

void HinaPE::PBFSolver::BoundaryData::add_boundary(const mVector3 &minX, const mVector3 &maxX,
                                                   std::vector<mVector3> &boundary) {
    const real diam = 2.0 * _radius;
    const int stepsX = (int) ((maxX.x() - minX.x()) / diam) + 1;
    const int stepsY = (int) ((maxX.y() - minX.y()) / diam) + 1;
    const int stepsZ = (int) ((maxX.z() - minX.z()) / diam) + 1;

    for (int i = 0; i < stepsX; ++i) {
        for (int j = 0; j < stepsY; ++j) {
            for (int k = 0; k < stepsZ; ++k) {
                const real x = minX.x() + i * diam;
                const real y = minX.y() + j * diam;
                const real z = minX.z() + k * diam;
                boundary.emplace_back(x, y, z);
            }
        }
    }
}

void HinaPE::PBFSolver::BoundaryData::init_boundary(std::vector<mVector3> &boundary)
{
    real containerWidth = (_width + 5) * _radius * static_cast<real>(4.0);
    real containerDepth = (_depth + 5) * _radius * static_cast<real>(4.0);
    real containerHeight = 1.5;

    const real x1 = -containerWidth * 0.5;
    const real x2 = containerWidth * 0.5;
    const real y1 = 0.0;
    const real y2 = containerHeight;
    const real z1 = -containerDepth * 0.5;
    const real z2 = containerDepth * 0.5;

    // Floor
    add_boundary(mVector3(x1, y1, z1), mVector3(x2, y1, z2), boundary);
    // Top
    add_boundary(mVector3(x1, y2, z1), mVector3(x2, y2, z2), boundary);
    // Left
    add_boundary(mVector3(x1, y1, z1), mVector3(x1, y2, z2), boundary);
    // Right
    add_boundary(mVector3(x2, y1, z1), mVector3(x2, y2, z2), boundary);
    // Back
    add_boundary(mVector3(x1, y1, z1), mVector3(x2, y2, z1), boundary);
    // Front
    add_boundary(mVector3(x1, y1, z2), mVector3(x2, y2, z2), boundary);
}

void HinaPE::PBFSolver::BoundaryData::_update_neighbor() {
    if (!_mass_inited)
        _update_mass(); // update mass to ensure the initial density is 1000

    auto &x_b = _predicted_position;
    auto &d_b = _densities;
    const auto &m_b = _mass;

    Util::parallelFor(Constant::ZeroSize, _positions.size(), [&](size_t i)
    {
        real sum = (*poly6_kernel)(0); // self density
        for (int j = 0; j < _neighbor_lists[i].size(); ++j)
        {
            real dist = (x_b[i] - x_b[_neighbor_lists[i][j]]).length();
            sum += (*poly6_kernel)(dist);
        }
        d_b[i] = m_b * sum; // rho(x) = m * sum(W(x - xj))
    });
}

void HinaPE::PBFSolver::BoundaryData::_update_mass()
{
    _mass = 1.0;

    real max_number_density = 0;
    for (int i = 0; i < _positions.size(); ++i)
    {
        real sum = (*poly6_kernel)(0); // self density
        const auto &point = _positions[i];
        for (const auto &neighbor_point_id: _neighbor_lists[i])
        {
            auto dist = (point - _positions[neighbor_point_id]).length();
            sum += (*poly6_kernel)(dist);
        }
        max_number_density = std::max(max_number_density, sum);
    }

    if (max_number_density > 0)
        _mass = std::max((target_density / max_number_density), HinaPE::Constant::Zero);

    _mass_inited = true;
}
