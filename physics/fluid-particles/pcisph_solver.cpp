#include "pcisph_solver.h"

void HinaPE::PCISPHSolver::init()
{
	_emit_particles();
    _opt.inited = true;
}

void HinaPE::PCISPHSolver::update(real dt) const
{
	// accumulate external forces, viscosity force
    _accumulate_non_pressure_force();

    // prediction_correction_step
    _prediction_correction_step();

    //update velocity and position
    _update_velocity_and_position();

	// emit particle to data, and rebuild data
	_emit_particles();
}

void HinaPE::PCISPHSolver::_emit_particles() const
{
    _emitter->emit(&_data->_positions, &_data->_velocities);
    _data->_forces.resize(_data->_positions.size(), mVector3::Zero());
    _data->_densities.resize(_data->_positions.size(), 0);
    _data->_pressures.resize(_data->_positions.size(), 0);

    _data->_temp_positions.resize(_data->_positions.size(), mVector3::Zero());
    _data->_temp_velocities.resize(_data->_positions.size(), mVector3::Zero());
    _data->_pressures_forces.resize(_data->_positions.size(), mVector3::Zero());
    _data->_density_errors.resize(_data->_positions.size(), 0);
    _data->_predict_densities.resize(_data->_positions.size(), 0);

    _data->_update_neighbor();
    _data->_update_density();

    //_data->_update_pressure();
}

void HinaPE::PCISPHSolver::_prediction_correction_step() const
{
    int iteration = 0;

    while((iteration < _data->min_loop)||((_data->density_error_too_large)&&(iteration < _data->max_loop)))
    {
        //predict velocity and position
        _predict_velocity_and_position();

        // deal with collision (particle-solid)
        _resolve_collision();

        // accumulate pressure forces
        _accumulate_pressure_force();

        iteration++;
    }
}

void HinaPE::PCISPHSolver::_accumulate_non_pressure_force() const
{
    auto &x = _data->_positions;
    auto &v = _data->_velocities;
    auto &f = _data->_forces;
    auto &d = _data->_densities;
    auto &p = _data->_pressures;
    const auto &m = _data->_mass;

    // Gravity Forces
    Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
    {
        mVector3 gravity = m * _opt.gravity;
        f[i] = gravity;
    });

    // Viscosity Forces
    Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
    {
        const auto &neighbors = _data->_neighbor_lists[i];
        for (size_t j: neighbors)
        {
            real dist = (x[i] - x[j]).length();
            if (d[j] > HinaPE::Constant::Epsilon)
                f[i] += _data->viscosity_coefficient * m * m * (v[j] - v[i]) / d[j] * (*_data->kernel).second_derivative(dist);
        }
    });
}

void HinaPE::PCISPHSolver::_accumulate_pressure_force() const {
    auto &x = _data->_positions;
    auto &v = _data->_velocities;
    auto &f = _data->_forces;
    auto &d = _data->_densities;
    auto &p = _data->_pressures;
    const auto &m = _data->_mass;
    const auto &dt = _opt.current_dt;

    auto &f_p = _data->_pressures_forces;
    auto &v_t = _data->_temp_velocities;
    auto &x_t = _data->_temp_positions;
    auto &d_p = _data->_predict_densities;
    auto &d_e = _data->_density_errors;

    real dist;
    mVector3 dir;
    // compute beta
    real beta = 2 * pow((dt * m / _data->target_density),2);

    // compute delta
    real denominator;
    mVector3 denominator1;
    real denominator2 = 0;
    Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
    {
        const auto &neighbors = _data->_neighbor_lists[i];
        for (size_t j: neighbors) {
            dist = (x_t[i] - x_t[j]).length();
            dir = (x[j] - x[i]) / dist;
        }
        mVector3 gradWij = (*_data->kernel).gradient(dist, dir);
        denominator1 += gradWij;
        denominator2 += gradWij.dot(gradWij);
    });
    denominator = -denominator1.dot(denominator1) - denominator2;
    real delta = -1 / (beta * denominator);

    // Compute pressure from density error
    Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
    {
        real weightSum = 0;
        const auto &neighbors = _data->_neighbor_lists[i];
        for (size_t j: neighbors)
        {
            if (dist > HinaPE::Constant::Epsilon && d[j] > HinaPE::Constant::Epsilon)
            {
                weightSum += (*_data->kernel).operator()(dist);
                real density = m * weightSum;
                real densityError = _data->target_density - density;
                real pressure = delta * densityError;

                if(pressure < 0.0)
                {
                    pressure *= _data->negative_pressure_scale;
                    densityError *= _data->negative_pressure_scale;
                }

                p[i] += pressure;
                d_p[i] = density;
                d_e[i] = densityError;
            }
        }
    });

    // Compute pressure gradient force
    Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
    {
        const auto &neighbors = _data->_neighbor_lists[i];
        for (size_t j: neighbors)
        {
            real dist = (x[i] - x[j]).length();
            if (dist > HinaPE::Constant::Epsilon && d[j] > HinaPE::Constant::Epsilon)
            {
                mVector3 dir = (x[j] - x[i]) / dist;
                f_p[i] -= m * m * (p[i] / (d[i] * d[i]) + p[j] / (d[j] * d[j])) * (*_data->kernel).gradient(dist, dir);
            }
        }
    });

    // Compute max density error
    real maxDensityError = 0.0;
    real maxDensityErrorRatio;
    Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
    {
        maxDensityError = (maxDensityError * maxDensityError > _data->_density_errors[i] * _data->_density_errors[i]) ? maxDensityError:_data->_density_errors[i];
    });

    maxDensityErrorRatio = maxDensityError / _data->target_density;

    if(maxDensityErrorRatio < _data->max_density_error_ratio)
    {
        _data->density_error_too_large = false;
    }

    // Accumulate pressure force
    Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
    {
        f[i] += f_p[i];
    });
}

void HinaPE::PCISPHSolver::_predict_velocity_and_position() const
{
    auto &x = _data->_positions;
    auto &v = _data->_velocities;
    auto &f = _data->_forces;
    const auto &m = _data->_mass;
    const auto &dt = _opt.current_dt;

    auto &f_p = _data->_pressures_forces;
    auto &v_t = _data->_temp_velocities;
    auto &x_t = _data->_temp_positions;

    // predict velocity and position
    Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
    {
        v_t[i] = v[i] + dt * (f[i] + f_p[i]) / m;
        x_t[i] += dt * v_t[i];
    });
}

void HinaPE::PCISPHSolver::_update_velocity_and_position() const{
    auto &x = _data->_positions;
    auto &v = _data->_velocities;
    auto &f = _data->_forces;
    const auto &m = _data->_mass;
    const auto &dt = _opt.current_dt;

    auto &f_p = _data->_pressures_forces;
    auto &v_t = _data->_temp_velocities;
    auto &x_t = _data->_temp_positions;

    //  update velocity and position
    Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
    {
        v[i] += dt * f[i] / m;
        x[i] += dt * v[i];
    });
}

void HinaPE::PCISPHSolver::_resolve_collision() const
{
    // collide with domain
    Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
    {
        _domain->resolve_collision(_data->_radius, _opt.restitution, &_data->_positions[i], &_data->_velocities[i]);
    });
}

void HinaPE::PCISPHSolver::INSPECT()
{
	ImGui::Text("Solver Inspector");
	ImGui::Text("Particles: %zu", _data->_positions.size());
	INSPECT_REAL(_opt.gravity[1], "g");
	ImGui::Separator();
}

void HinaPE::PCISPHSolver::VALID_CHECK() const
{
	if (_data == nullptr) throw std::runtime_error("SPHSolver::_data is nullptr");
	if (_domain == nullptr) throw std::runtime_error("SPHSolver::_domain is nullptr");
	if (_emitter == nullptr) throw std::runtime_error("SPHSolver::_emitter is nullptr");

	if (_data->_positions.size() != _data->_velocities.size() &&
		_data->_positions.size() != _data->_forces.size() &&
		_data->_positions.size() != _data->_densities.size() &&
		_data->_positions.size() != _data->_pressures.size())
		throw std::runtime_error("SPHSolver::_data size mismatch");
}

HinaPE::PCISPHSolver::Data::Data() { track(&_positions); }

void HinaPE::PCISPHSolver::Data::_update_neighbor()
{
	_neighbor_search->build(_positions);
	_neighbor_lists.resize(_positions.size());

	for (size_t i = 0; i < _positions.size(); ++i)
	{
		mVector3 origin = _positions[i];
		_neighbor_lists[i].clear();

		_neighbor_search->for_each_nearby_point(origin, [&](size_t j, const mVector3 &)
		{
			if (i != j)
				_neighbor_lists[i].push_back(j);
		});
	}
}

void HinaPE::PCISPHSolver::Data::_update_density()
{
	if (!_mass_inited)
		_update_mass(); // update mass to ensure the initial density is 1000

	auto &x = _positions;
	auto &d = _densities;
	const auto &m = _mass;

	Util::parallelFor(Constant::ZeroSize, _positions.size(), [&](size_t i)
	{
		real sum = 0;
		for (int j = 0; j < _neighbor_lists[i].size(); ++j)
		{
			real dist = (x[i] - x[_neighbor_lists[i][j]]).length();
			sum += (*kernel)(dist);
		}
		d[i] = m * sum; // rho(x) = m * sum(W(x - xj))
	});
}

void HinaPE::PCISPHSolver::Data::_update_predict_density() {
    if (!_mass_inited)
        _update_mass(); // update mass to ensure the initial density is 1000

    auto &x = _positions;
    auto &d = _densities;
    const auto &m = _mass;
    Util::parallelFor(Constant::ZeroSize, _positions.size(), [&](size_t i)
    {
        real sum = 0;
        for (int j = 0; j < _neighbor_lists[i].size(); ++j)
        {
            real dist = (x[i] - x[_neighbor_lists[i][j]]).length();
            sum += (*kernel)(dist);
        }
        d[i] = m * sum; // rho(x) = m * sum(W(x - xj))
    });
}

void HinaPE::PCISPHSolver::Data::_update_pressure()
{
	auto &d = _densities;
	auto &p = _pressures;
	const real td = target_density;
	const real es = target_density * speed_of_sound * speed_of_sound;
	const real ee = eos_exponent;
	const real nps = negative_pressure_scale;
	Util::parallelFor(Constant::ZeroSize, _positions.size(), [&](size_t i)
	{
		// See Murnaghan-Tait equation of state from
		// https://en.wikipedia.org/wiki/Tait_equation
		p[i] = es / ee * (std::pow(d[i] / td, ee) - 1.0);

		if (p[i] < 0)
			p[i] *= nps;
	});
}

void HinaPE::PCISPHSolver::Data::_update_mass()
{	_mass = 1.0;

	real max_number_density = 0;
	for (int i = 0; i < _positions.size(); ++i)
	{
		real sum = 0;
		const auto &point = _positions[i];
		for (const auto &neighbor_point_id: _neighbor_lists[i])
		{
			auto dist = (point - _positions[neighbor_point_id]).length();
			sum += (*kernel)(dist);
		}
		max_number_density = std::max(max_number_density, sum);
	}

	if (max_number_density > 0)
		_mass = std::max((target_density / max_number_density), HinaPE::Constant::Zero);

	_mass_inited = true;
}

void HinaPE::PCISPHSolver::Data::INSPECT()
{
	InstancePosesBase::INSPECT();
	if (_inst_id >= 0 && _inst_id < _densities.size())
	{
		ImGui::Text("Inst: %d", _inst_id);
		ImGui::Text("Force: {%.3f, %.3f, %.3f}", _forces[_inst_id].x(), _forces[_inst_id].y(), _forces[_inst_id].z());
		ImGui::Text("Density: %.3f", _densities[_inst_id]);
		ImGui::Text("Pressure: %.3f", _pressures[_inst_id]);
	}
}




