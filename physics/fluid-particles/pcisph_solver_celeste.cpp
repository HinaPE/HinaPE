
#include "pcisph_solver_celeste.h"

void HinaPE::PCISPHSolverCELESTE::init() {
// init data
    if (_data == nullptr)
        _data = std::make_shared<Data>();
    if (_domain == nullptr)
        _domain = std::make_shared<BoxDomain>();
    if (_emitter == nullptr)
        _emitter = std::make_shared<VolumeParticleEmitter3>();

    _data->DEFAULT_SCALE = 0.5 * _opt.radius * mVector3::One();
    _emitter->_opt.spacing = 1.2 * _opt.radius;

    _init_fluid_particles();
    //_init_boundary_particles();
}

void HinaPE::PCISPHSolverCELESTE::_init_fluid_particles() const {
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
void HinaPE::PCISPHSolverCELESTE::_update_neighbor() const
{
    // Update Target: NeighborList
    const auto fluid_size = _data->fluid_size();
    const auto boundary_size = _data->boundary_size();
    const auto &x = _data->Fluid.positions; // note: we use predicted position, because we need to update neighbor in the sub iteration

    std::vector<mVector3> total_positions;
    total_positions.reserve(fluid_size);
    total_positions.insert(total_positions.end(), x.begin(), x.end());
    //total_positions.insert(total_positions.end(), _data->Boundary.positions.begin(), _data->Boundary.positions.end());

    PointHashGridSearch3 searcher(_opt.kernel_radius);
    searcher.build(total_positions);

    auto &nl = _data->NeighborList;
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

    // ==================== Debug ====================
    _data->neighbor_list_debug = nl; // copy neighbor list to debug
}
void HinaPE::PCISPHSolverCELESTE::_update_density() const {
// Update Target: densities
    auto &d = _data->Fluid.densities;
    const auto &x = _data->Fluid.positions;
    //const auto &b = _data->Boundary.positions;
    const auto &m = _data->Fluid.mass;
    //const auto &bm = _data->Boundary.mass;
    const auto &nl = _data->NeighborList;
    const auto fluid_size = _data->fluid_size();
    const auto boundary_size = _data->boundary_size();

    StdKernel poly6(_opt.kernel_radius);
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

void HinaPE::PCISPHSolverCELESTE::update(real dt)
{
    // algorithm line 2~3
    _update_neighbor();
    _update_density();
    // algorithm line 5
    _accumulate_non_pressure_force();
    // algorithm line 6~7
    _initialize_pressure_and_pressure_force();
    // algorithm line 8~17
    _prediction_correction_step();
    // algorithm line 18~20
    _correct_velocity_and_position();
}

void HinaPE::PCISPHSolverCELESTE::_accumulate_non_pressure_force() const
{
    // Update Target: predicted_position, velocities, forces
    _data->Fluid.predicted_positions = _data->Fluid.positions;
    _data->Fluid.predicted_velocities = _data->Fluid.velocities;

    auto &f = _data->Fluid.non_pressure_forces;
    auto &x = _data->Fluid.positions;
    auto &v = _data->Fluid.velocities;
    auto &d = _data->Fluid.densities;
    StdKernel poly6(_opt.kernel_radius);

    const auto fluid_size = _data->fluid_size();
    const auto boundary_size = _data->boundary_size();

    const auto &m = _data->Fluid.mass;
    // Gravity Forces
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        mVector3 gravity = m * _opt.gravity;
        f[i] = gravity;
    });

    // Viscosity Forces
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        auto &nl = _data->NeighborList;
        for (size_t j: nl[i])
        {
            real dist = (x[i] - x[j]).length();
            if (d[j] > HinaPE::Constant::Epsilon)
                f[i] += _opt.viscosity * m * m * (v[j] - v[i]) / d[j] * poly6.second_derivative(dist);
        }
    });
}
void HinaPE::PCISPHSolverCELESTE::_prediction_correction_step() {
    int iteration = 0;
    while(((iteration < _opt.min_loop)||(_opt.density_error_too_large))&&(iteration < _opt.max_loop))
    {
        // algorithm line 9~11
        _predict_velocity_and_position();

        _resolve_collision();

        // algorithm line 12~13
        _predict_density();
        // algorithm line 14~15
        _update_pressure();
        // algorithm line 16~17
        _accumulate_pressure_force();
        iteration++;
    }
}
void HinaPE::PCISPHSolverCELESTE::_initialize_pressure_and_pressure_force() const {
    auto &p = _data->Fluid.pressures;
    auto &p_f = _data->Fluid.pressure_forces;
    auto &d_e = _data->Fluid.density_errors;
    auto &d = _data->Fluid.densities;
    auto &p_d = _data->Fluid.predicted_densities;

    const auto fluid_size = _data->fluid_size();

    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        p[i] = 0.0;
        p_f[i] = mVector3(0.0,0.0,0.0);
        ///Add
        d_e[i] = 0.0;
        p_d[i] = d[i];
    });
}
void HinaPE::PCISPHSolverCELESTE::_resolve_collision() const {
    auto &x = _data->Fluid.positions;
    auto &v = _data->Fluid.velocities;

    // Collision
    Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
    {
        _domain->resolve_collision(_opt.radius, _opt.restitution, &x[i], &v[i]);
    });
}
void HinaPE::PCISPHSolverCELESTE::_predict_velocity_and_position() const {
    auto &x = _data->Fluid.positions;
    auto &v = _data->Fluid.velocities;
    auto &f = _data->Fluid.non_pressure_forces;
    auto &f_p = _data->Fluid.pressure_forces;

    auto &x_p = _data->Fluid.predicted_positions;
    auto &v_p = _data->Fluid.predicted_velocities;
    const auto &m = _data->Fluid.mass;
    const auto &dt = _opt.current_dt;
    const auto fluid_size = _data->fluid_size();
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        v_p[i] = v[i] + dt * (f[i] + f_p[i]) / m;
        x_p[i] = x[i] + dt * v_p[i];
    });
}
void HinaPE::PCISPHSolverCELESTE::_predict_density() const
{
    auto &d = _data->Fluid.densities;
    auto &d_p = _data->Fluid.predicted_densities;

    auto &x = _data->Fluid.positions;
    auto &x_p = _data->Fluid.predicted_positions;

    auto &m = _data->Fluid.mass;
    const auto fluid_size = _data->fluid_size();
    StdKernel poly6(_opt.kernel_radius);

    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        real weight_sum = poly6(0);
        auto &nl = _data->NeighborList;
        for (size_t j: nl[i])
        {
            real dist = (x_p[i] - x_p[j]).length();
            weight_sum += poly6(dist);
        }
        d_p[i] = m * weight_sum;
    });
}
void HinaPE::PCISPHSolverCELESTE::_update_pressure() {
    auto &p = _data->Fluid.pressures;
    auto &delta_p = _data->Fluid.delta_pressure;
    auto &d_p = _data->Fluid.predicted_densities;
    auto &d_e = _data->Fluid.density_errors;
    auto &x = _data->Fluid.positions;
    auto &x_p = _data->Fluid.predicted_positions;
    auto &m = _data->Fluid.mass;
    const auto &dt = _opt.current_dt;

    const auto fluid_size = _data->fluid_size();
    SpikyKernel spiky(_opt.kernel_radius);

    // compute beta
    real beta = 2 * Math::square(dt * m / _opt.target_density);

    // compute delta
    mBBox3 bound(mVector3::Zero(), mVector3::Zero());
    bound.expand(1.5 * _opt.kernel_radius);

    real denominator = 0;
    mVector3 denominator1 = mVector3(0.0,0.0,0.0);
    real denominator2 = 0;
    mVector3 gradWij;

    std::vector<mVector3> points;
    real spacing = _opt.kernel_radius;
    real half_spacing = 0.5 * _opt.kernel_radius;
    real box_w = bound.width();
    real box_h = bound.height();
    real box_d = bound.depth();

    mVector3 point;
    bool has_offset = false;
    for (int i = 0; i * half_spacing <= box_d; ++i)
    {
        point.z() = i * half_spacing + bound._lower_corner.z();

        real offset = (has_offset) ? half_spacing : 0;

        for (int j = 0; j * spacing + offset <= box_h; ++j)
        {
            point.y() = j * spacing + offset + bound._lower_corner.y();
            for (int k = 0; k * spacing + offset <= box_w; ++k)
            {
                point.x() = k * spacing + offset + bound._lower_corner.x();
                points.push_back(point);
            }
        }
        has_offset = !has_offset;
    }

    for (const auto &p_i: points)
    {
        real dist = p_i.length();
        if ((dist * dist) < (_opt.kernel_radius * _opt.kernel_radius))
        {
            mVector3 dir = (dist > 0) ? p_i / dist : mVector3::Zero();
            auto gradWij = spiky.gradient(dist, dir);
            denominator1 += gradWij;
            denominator2 += gradWij.dot(gradWij);
        }
    }
    denominator = -denominator1.dot(denominator1) - denominator2;
    real delta = (std::fabs(denominator) > 0) ? -1 / (beta * denominator) : 0;

    // update pressure
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        // compute density error
        // algorithm line 14
        d_e[i] = d_p[i] - _opt.target_density;
        // compute delta pressure
        delta_p[i] = delta * d_e[i];
        // compute pressure force
        // algorithm line 15
        p[i] += delta * d_e[i];
    });

    // Compute max density error
    real maxDensityError = 0.0;
    real DensityErrorRatio;

    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        maxDensityError = std::max(maxDensityError, std::abs(d_e[i]));
    });

    DensityErrorRatio = maxDensityError / _opt.target_density;

    if(fabs(DensityErrorRatio) < _opt.max_density_error_ratio)
    {
        _opt.density_error_too_large = false;
    }
}
void HinaPE::PCISPHSolverCELESTE::_accumulate_pressure_force() const
{
    auto &x = _data->Fluid.positions;
    auto &p_f = _data->Fluid.pressure_forces;
    auto &p = _data->Fluid.pressures;
    auto &d = _data->Fluid.densities;
    auto &p_d = _data->Fluid.predicted_densities;
    auto &delta_p = _data->Fluid.delta_pressure;
    auto &m = _data->Fluid.mass;
    const auto fluid_size = _data->fluid_size();
    SpikyKernel spiky(_opt.kernel_radius);
    std::fill(p_f.begin(), p_f.end(), mVector3::Zero());

    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        auto &nl = _data->NeighborList;
        for (size_t j: nl[i])
        {
            real dist = (x[i] - x[j]).length();
            if (dist > HinaPE::Constant::Epsilon && d[j] > HinaPE::Constant::Epsilon)
            {
                mVector3 dir = (x[j] - x[i]) / dist;
                p_f[i] -= m * m * (p[i] / (p_d[i] * p_d[i]) + p[j] / (d[j] * d[j])) * spiky.gradient(dist, dir);
            }
        }
    });
}
void HinaPE::PCISPHSolverCELESTE::_correct_velocity_and_position() const {
    auto &x = _data->Fluid.positions;
    auto &v = _data->Fluid.velocities;
    auto &x_p = _data->Fluid.predicted_positions;
    auto &v_p = _data->Fluid.predicted_velocities;
    auto &f = _data->Fluid.non_pressure_forces;
    auto &p_f = _data->Fluid.pressure_forces;
    auto &m = _data->Fluid.mass;
    const auto &dt = _opt.current_dt;
    const auto fluid_size = _data->fluid_size();

    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        v[i] += dt * (f[i] + p_f[i]) / m;
        x[i] += dt * v[i];
    });
}
void HinaPE::PCISPHSolverCELESTE::reset() {
    _data->reset();
    init();
}
void HinaPE::PCISPHSolverCELESTE::INSPECT()
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
    ImGui::DragScalar("Gravity", ImGuiDataType_Real, &_opt.gravity[1], 0.1, nullptr, nullptr, "%.2f");
    static real min_multiplier = 1e-1, max_multiplier = 3;
    ImGui::DragScalar("Particles Multiplier", ImGuiDataType_Real, &_emitter->_opt.multiplier, 0.1, &min_multiplier, &max_multiplier, "%.2f");
    static real min_radius = 1e-3, max_radius = 9e-1;
    if (ImGui::DragScalar("Radius", ImGuiDataType_Real, &_opt.radius, 1e-3, &min_radius, &max_radius, "%.3f")) { _opt.kernel_radius = _opt.radius * _opt.relative_kernel_radius; }
    static real min_relative_radius = 1, max_relative_radius = 5;
    if (ImGui::DragScalar("Relative Kernel Radius", ImGuiDataType_Real, &_opt.relative_kernel_radius, 1e-1, &min_relative_radius, &max_relative_radius, "%.3f")) { _opt.kernel_radius = _opt.radius * _opt.relative_kernel_radius; }

    ImGui::Checkbox("Akinci2012 Collision", &_opt.use_akinci2012_collision);
    ImGui::Separator();

    // Debug Info
    auto inst_id = _data->_inst_id;
    if (inst_id >= 0 && inst_id < _data->fluid_size())
    {
        ImGui::Text("Inst: %d", inst_id);
        ImGui::Text("Mass: %f", _data->Fluid.mass);
        ImGui::Text("Non Pressure Force: {%.3f, %.3f, %.3f}", _data->Fluid.non_pressure_forces[inst_id].x(), _data->Fluid.non_pressure_forces[inst_id].y(), _data->Fluid.non_pressure_forces[inst_id].z());
        ImGui::Text("Pressure Force: {%.3f, %.3f, %.3f}", _data->Fluid.pressure_forces[inst_id].x(), _data->Fluid.pressure_forces[inst_id].y(), _data->Fluid.pressure_forces[inst_id].z());
        ImGui::Text("Density: %.3f", _data->Fluid.densities[inst_id]);
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

HinaPE::PCISPHSolverCELESTE::Data::Data() {
    track(&Fluid.positions);
    _color_map = &color_map;
}
auto HinaPE::PCISPHSolverCELESTE::Data::fluid_size() const -> size_t { return Fluid.positions.size(); }
auto HinaPE::PCISPHSolverCELESTE::Data::boundary_size() const -> size_t { return Boundary.positions.size(); }

void HinaPE::PCISPHSolverCELESTE::Data::add_fluid(const std::vector<mVector3> &positions,
                                                  const std::vector<mVector3> &velocities) {
    if (positions.size() != velocities.size())
        throw std::runtime_error("positions.size() != velocities.size()");

    auto size = positions.size();
    Fluid.positions.insert(Fluid.positions.end(), positions.begin(), positions.end());
    Fluid.velocities.insert(Fluid.velocities.end(), velocities.begin(), velocities.end());
    Fluid.predicted_positions.insert(Fluid.predicted_positions.end(), positions.begin(), positions.end());
    Fluid.predicted_velocities.insert(Fluid.predicted_velocities.end(), velocities.begin(), velocities.begin());
    Fluid.densities.insert(Fluid.densities.end(), size, 0.0);
    Fluid.predicted_densities.insert(Fluid.predicted_densities.end(), size, 0.0);
    Fluid.density_errors.insert(Fluid.density_errors.end(), size, 0.0);
    Fluid.pressures.insert(Fluid.pressures.end(), size, 0.0);
    Fluid.delta_pressure.insert(Fluid.delta_pressure.end(), size, 0.0);

    NeighborList.insert(NeighborList.end(), size, std::vector<unsigned int>());
    color_map.insert(color_map.end(), size, Color::ORANGE);
    debug_info.insert(debug_info.end(), size, std::vector<std::string>());
}

void HinaPE::PCISPHSolverCELESTE::Data::reset() {
    Fluid.positions.clear();
    Fluid.velocities.clear();

    Fluid.predicted_positions.clear();
    Fluid.predicted_velocities.clear();

    Fluid.non_pressure_forces.clear();
    Fluid.pressure_forces.clear();

    Fluid.densities.clear();
    Fluid.predicted_densities.clear();
    Fluid.density_errors.clear();

    Fluid.pressures.clear();
    Fluid.delta_pressure.clear();
    Fluid.mass = 1e-3;

    Boundary.positions.clear();
    Boundary.mass = 1e-3;

    NeighborList.clear();
    color_map.clear();
    debug_info.clear();
    neighbor_list_debug.clear();
    p_debug.clear();
    delta_p_debug.clear();
}



