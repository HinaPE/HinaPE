#include "pcisph_solver_new.h"

void HinaPE::PCISPHSolverNew::_update_neighbor() const {
// Update Target: NeighborList
    const auto fluid_size = _data->fluid_size();
    const auto boundary_size = _data->boundary_size();
    const auto &p = _data->Fluid.predicted_positions; // note: we use predicted position, because we need to update neighbor in the sub iteration

    std::vector<mVector3> total_positions;
    total_positions.reserve(fluid_size + boundary_size);
    total_positions.insert(total_positions.end(), p.begin(), p.end());
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

void HinaPE::PCISPHSolverNew::_update_density() const
{
// Update Target: densities
    auto &d = _data->Fluid.densities;
    const auto &p = _data->Fluid.predicted_positions;
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
                    density += bm * poly6((p[i] - b[j - fluid_size]).length());
                }
            }
        }
        d[i] = density; // rho(x) = m * sum(W(x - xj))
    });
}

void HinaPE::PCISPHSolverNew::init()
{
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
    _init_boundary_particles();

    _update_neighbor();
    _update_density();
}

void HinaPE::PCISPHSolverNew::reset()
{
    _data->reset();
    init();
}

void HinaPE::PCISPHSolverNew::update(real dt)
{
    // algorithm line 5
    _accumulate_non_pressure_force();
    // algorithm line 6~7
    _initialize_pressure_and_pressure_force();
    // algorithm line 8~17
    _prediction_correction_step();
    // algorithm line 18~20
    _correct_velocity_and_position();
    // algorithm line 2~3
    _update_neighbor();
}

void HinaPE::PCISPHSolverNew::_accumulate_non_pressure_force() const
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
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        // Gravity Forces
        mVector3 gravity = m * _opt.gravity;
        f[i] = gravity;

        // Viscosity Forces
        auto &nl = _data->NeighborList;
        for (size_t j: nl[i])
        {
            real dist = (x[i] - x[j]).length();
            if (d[j] > HinaPE::Constant::Epsilon)
                f[i] += _opt.viscosity * m * m * (v[j] - v[i]) / d[j] * poly6.second_derivative(dist);
        }
    });
}

void HinaPE::PCISPHSolverNew::_initialize_pressure_and_pressure_force() const
{
    auto &p = _data->Fluid.pressures;
    auto &p_f = _data->Fluid.pressure_forces;

    const auto fluid_size = _data->fluid_size();

    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        p[i] = 0.0;
        p_f[i] = mVector3(0.0,0.0,0.0);
    });
}

void HinaPE::PCISPHSolverNew::_prediction_correction_step()
{
    int iteration = 0;
    while(((iteration < _opt.min_loop)||(_opt.density_error_too_large))&&(iteration < _opt.max_loop))
    {
        // algorithm line 9~11
        _predict_velocity_and_position();
        // algorithm line 12~13
        _predict_density();
        // algorithm line 14~15
        _update_pressure();
        // algorithm line 16~17
        _accumulate_pressure_force();
        iteration++;
    }
}

void HinaPE::PCISPHSolverNew::_predict_velocity_and_position() const
{
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

void HinaPE::PCISPHSolverNew::_predict_density() const
{
    auto &d = _data->Fluid.densities;
    auto &d_p = _data->Fluid.predicted_densities;

    auto &x = _data->Fluid.positions;
    auto &x_p = _data->Fluid.predicted_positions;

    auto &m = _data->Fluid.mass;
    const auto fluid_size = _data->fluid_size();
    StdKernel poly6(_opt.kernel_radius);

    /////////TODO
    real sum_W = 0.0;
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        auto &nl = _data->NeighborList;
        for (size_t j: nl[i])
        {
            real dist = (x_p[i] - x_p[j]).length();
            sum_W += poly6(dist);
        }
        d_p[i] = m * sum_W;
    });
}

void HinaPE::PCISPHSolverNew::_update_pressure()
{
    auto &p = _data->Fluid.pressures;
    auto &delta_p = _data->Fluid.delta_pressure;
    auto &d_p = _data->Fluid.predicted_densities;
    auto &d_e = _data->Fluid.density_errors;
    auto &x = _data->Fluid.positions;
    auto &x_p = _data->Fluid.predicted_positions;
    auto &m = _data->Fluid.mass;
    const auto &dt = _opt.current_dt;

    const auto fluid_size = _data->fluid_size();
    StdKernel ploy6(_opt.kernel_radius);

    // compute beta
    real beta = 2 * pow((dt * m / _opt.target_density),2);

    // compute delta
    real denominator = 0;
    mVector3 denominator1 = mVector3(0.0,0.0,0.0);
    real denominator2 = 0;
    mVector3 gradWij;
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        auto &nl = _data->NeighborList;
        for (size_t j: nl[i]) {
            real dist = (x[i] - x[j]).length();
            mVector3 dir = (x[j] - x[i]) / dist;
            gradWij = ploy6.gradient(dist, dir);
            denominator1 += gradWij;
            denominator2 += gradWij.dot(gradWij);
        }
    });
    denominator = -denominator1.dot(denominator1) - denominator2;
    real delta = -1 / (beta * denominator);

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
        maxDensityError = (maxDensityError * maxDensityError > d_e[i] * d_e[i]) ? maxDensityError:d_e[i];
    });

    DensityErrorRatio = maxDensityError / _opt.target_density;

    if(fabs(DensityErrorRatio) < _opt.max_density_error_ratio)
    {
        _opt.density_error_too_large = false;
    }
}

void HinaPE::PCISPHSolverNew::_accumulate_pressure_force() const
{
    auto &x = _data->Fluid.positions;
    auto &p_f = _data->Fluid.pressure_forces;
    auto &delta_p = _data->Fluid.delta_pressure;
    auto &m = _data->Fluid.mass;
    const auto fluid_size = _data->fluid_size();
    StdKernel poly6(_opt.kernel_radius);

    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        auto &nl = _data->NeighborList;
        for (size_t j: nl[i])
        {
            real dist = (x[i] - x[j]).length();
            mVector3 dir = (x[j] - x[i]) / dist;
            p_f[i] = -m * m * (2 * delta_p[i]/(_opt.target_density * _opt.target_density)) * poly6.gradient(dist, dir);
        }
    });
}

void HinaPE::PCISPHSolverNew::_correct_velocity_and_position() const
{
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

void HinaPE::PCISPHSolverNew::_init_fluid_particles() const {
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

void add_wall(const mVector3 &minX, const mVector3 &maxX, real radius, std::vector<mVector3> *target_boundary)
{
    const real diam = 1.4 * radius;
    const int stepsX = (int) ((maxX.x() - minX.x()) / diam) + 1;
    const int stepsY = (int) ((maxX.y() - minX.y()) / diam) + 1;
    const int stepsZ = (int) ((maxX.z() - minX.z()) / diam) + 1;

    for (int i = 0; i < stepsX; ++i)
    {
        for (int j = 0; j < stepsY; ++j)
        {
            for (int k = 0; k < stepsZ; ++k)
            {
                const real x = minX.x() + i * diam;
                const real y = minX.y() + j * diam;
                const real z = minX.z() + k * diam;
                target_boundary->emplace_back(x, y, z);
            }
        }
    }
}

void HinaPE::PCISPHSolverNew::_init_boundary_particles() const
{
    std::vector<mVector3> target_boundary;
    target_boundary.clear();

    const auto half_width = _domain->_extent.x();
    const auto half_height = _domain->_extent.y();
    const auto half_depth = _domain->_extent.z();

    const real x1 = -half_width;
    const real x2 = half_width;
    const real y1 = -half_height;
    const real y2 = half_height;
    const real z1 = -half_depth;
    const real z2 = half_depth;

    add_wall(mVector3(x1, y1, z1), mVector3(x2, y1, z2), _opt.radius, &target_boundary); // floor
    add_wall(mVector3(x1, y2, z1), mVector3(x2, y2, z2), _opt.radius, &target_boundary); // top
    add_wall(mVector3(x1, y1, z1), mVector3(x1, y2, z2), _opt.radius, &target_boundary); // left
    add_wall(mVector3(x2, y1, z1), mVector3(x2, y2, z2), _opt.radius, &target_boundary); // right
    add_wall(mVector3(x1, y1, z1), mVector3(x2, y2, z1), _opt.radius, &target_boundary); // back
    add_wall(mVector3(x1, y1, z2), mVector3(x2, y2, z2), _opt.radius, &target_boundary); // front

    _data->add_boundary(target_boundary);
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
        _data->Boundary.mass = std::max((_opt.target_density / max_number_density), HinaPE::Constant::Zero);
    else
        throw std::runtime_error("max_number_density is zero");

    _data->Boundary.mass *= 10; // ???why?
}

void HinaPE::PCISPHSolverNew::INSPECT() {
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

void HinaPE::PCISPHSolverNew::_reset_debug_info() const {
    _data->debug_info.clear();
    _data->debug_info.resize(_data->fluid_size());
    _data->p_debug.clear();
    _data->delta_p_debug.clear();
}

// ==========================================================================================================
// ================================================== Data ==================================================
HinaPE::PCISPHSolverNew::Data::Data() {
    track(&Fluid.positions);
    _color_map = &color_map;
}
auto HinaPE::PCISPHSolverNew::Data::fluid_size() const -> size_t { return Fluid.positions.size(); }
auto HinaPE::PCISPHSolverNew::Data::boundary_size() const -> size_t { return Boundary.positions.size(); }

void HinaPE::PCISPHSolverNew::Data::add_fluid(const std::vector<mVector3> &positions, const std::vector<mVector3> &velocities)
{
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
void HinaPE::PCISPHSolverNew::Data::add_boundary(const std::vector<mVector3> &positions) {
    Boundary.positions.insert(Boundary.positions.end(), positions.begin(), positions.end());
}

void HinaPE::PCISPHSolverNew::Data::reset() {
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
