
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

    _update_neighbor();
    _update_density();
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

    // use a regular sampling around (0,0,0)

    const auto fluid_size = _data->fluid_size();
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i){
        _data->Fluid.delta[i] = 0.0;

        mVector3 sumGradW = mVector3::Zero();
        real sumGradW2 = 0.0;
        const real supportRadius = _opt.kernel_radius;
        const real particleRadius = _opt.radius;
        const real diam = static_cast<real>(2.0) * particleRadius;
        const mVector3 xi(0,0,0);

        auto &m = _data->Fluid.mass;
        StdKernel ploy6(_opt.kernel_radius);

        mVector3 xj = {-supportRadius, -supportRadius, -supportRadius};
        while (xj[0] <= supportRadius){
            while (xj[1] <= supportRadius){
                while (xj[2] <= supportRadius){
                    // check if xj is in the support of xi
                    if ((xi - xj).length()*(xi - xj).length() < supportRadius*supportRadius)
                    {
                        const mVector3 gradW = ploy6.gradient(xi - xj);
                        sumGradW += gradW;
                        sumGradW2 += gradW.dot(gradW);
                    }
                    xj[2] += diam;
                }
                xj[1] += diam;
                xj[2] = -supportRadius;
            }
            xj[0] += diam;
            xj[1] = -supportRadius;
            xj[2] = -supportRadius;
        }
        const real beta = static_cast<real>(2.0) * (m / _opt.target_density) * (m / _opt.target_density);
        _data->Fluid.delta[i] = static_cast<real>(1.0) / (beta * (sumGradW.dot(sumGradW) + sumGradW2));
    });
}

void HinaPE::PCISPHSolverCELESTE::_update_neighbor() const {
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

void HinaPE::PCISPHSolverCELESTE::_update_density() const {
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

void HinaPE::PCISPHSolverCELESTE::update(real dt) {

    _accumulate_non_pressure_force();

    _update_neighbor();



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

void HinaPE::PCISPHSolverCELESTE::_predict_density() const {
    auto &d = _data->Fluid.densities;
    auto &d_p = _data->Fluid.predicted_densities;

    auto &x = _data->Fluid.positions;
    auto &x_p = _data->Fluid.predicted_positions;

    auto &m = _data->Fluid.mass;
    const auto fluid_size = _data->fluid_size();
    StdKernel poly6(_opt.kernel_radius);

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



