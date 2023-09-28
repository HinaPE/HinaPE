#include "pcisph_akinci_surface_tension.h"

void HinaPE::PCISPHAkinciSurface::init() {
    if (_data == nullptr)
        _data = std::make_shared<Data>();
    if (_domain == nullptr)
    {
        _domain = std::make_shared<BoxDomain>();
        _domain->POSE.scale = {1.8, 1, 0.8};
        _domain->_update_surface();
    }
    if (_emitter == nullptr)
    {
        _emitter = std::make_shared<VolumeParticleEmitter3>();
        _emitter->_opt.multiplier = 2.2;
    }
    if (_cube == nullptr)
    {
        auto cube = std::make_shared<Kasumi::CubeObject>();
        //cube->POSE.position = mVector3(0, -0.8, 0);
        cube->POSE.position = mVector3(-0.8, 0.5, 0);
        cube->POSE.scale = mVector3(0.2, 0.2, 0.2);
        cube->_update_surface();
        _cube = cube;
        _cube->set_color(Color::PURPLE);
    }
    _data->DEFAULT_SCALE = 0.5 * _opt.radius * mVector3::One();
    _emitter->_opt.spacing = 1.2 * _opt.radius;

    _init_fluid_particles();
    _init_boundary_particles();
    _init_collider();
    //_compute_rest_mass_center();
}

void HinaPE::PCISPHAkinciSurface::_init_fluid_particles() const
{
    std::vector<mVector3> init_pos, init_vel;
    _emitter->emit(&init_pos, &init_vel);
    _data->add_fluid(init_pos, init_vel);

    // update mass
    /*real diam = 2 * _opt.radius;
    real volume = static_cast<real>(0.8) * diam * diam * diam;
    _data->Fluid.mass = _opt.target_density * volume;*/
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

void HinaPE::PCISPHAkinciSurface::_init_boundary_particles() const {
    std::vector<mVector3> target_boundary = _domain->generate_surface();
    _data->add_boundary(target_boundary, &_domain->POSE);

    _data->Boundary.IsActive.insert(_data->Boundary.IsActive.end(), target_boundary.size(), false);
    // update mass
    _data->Boundary.volume.insert(_data->Boundary.volume.end(), target_boundary.size(), HinaPE::Constant::Zero);
    //_init_boundary_volume();
}

void HinaPE::PCISPHAkinciSurface::_init_boundary_volume() const {
    auto &bnl = _data->BoundaryNeighborList;
    const auto fluid_size = _data->fluid_size();
    const auto &x = _data->Fluid.positions;
    const auto boundary_size = _data->boundary_size();
    const auto &bx = _data->Boundary.positions;
    auto &b_v = _data->Boundary.volume;

    std::vector<mVector3> total_positions;
    total_positions.reserve(fluid_size + boundary_size);
    total_positions.insert(total_positions.end(), x.begin(), x.end());
    _data->update_boundary();
    total_positions.insert(total_positions.end(), bx.begin(), bx.end());

    PointHashGridSearch3 searcher(_opt.kernel_radius);
    searcher.build(total_positions);

    Util::parallelFor(Constant::ZeroSize, boundary_size, [&](size_t i)
    {
        auto origin = bx[i];
        bnl[i].clear();
        searcher.for_each_nearby_point(origin, [&](size_t j, const mVector3 &)
        {
            if (i != j)
            {
                bnl[i].push_back(j);
            }
        });
    });

    StdKernel poly6(_opt.kernel_radius);
    // compute delta
    real delta_b;
    Util::parallelFor(Constant::ZeroSize, boundary_size, [&](size_t i)
    {
        delta_b = poly6(0); // self density without mass
        for (const auto j: bnl[i])
        {
            if(j < fluid_size)
            {
                real dist = (bx[i] - x[j]).length();
                delta_b += poly6(dist);
            }else{
                real dist = (bx[i] - bx[j - fluid_size]).length();
                delta_b += poly6(dist);
            }
        }
        const real volume = static_cast<real>(1.0) / delta_b;
        b_v[i] = volume;
    });
}

void HinaPE::PCISPHAkinciSurface::_init_collider() const
{
    {
        // generate cube surface points
        std::vector<mVector3> target_boundary = _cube->generate_surface();
        _data->add_boundary(target_boundary, &_cube->POSE);

        _data->Boundary.IsActive.insert(_data->Boundary.IsActive.end(), target_boundary.size(), true);
        _data->Boundary.volume.insert(_data->Boundary.volume.end(), target_boundary.size(), HinaPE::Constant::Zero);
        //_init_boundary_volume();
    }
}

void HinaPE::PCISPHAkinciSurface::update(real dt)
{
    // algorithm line 2~3
    _update_fluid_neighbor();

    _update_boundary_neighbor();
    _update_boundary_volume();

    _update_density();
    // algorithm line 5
    _accumulate_non_pressure_force();
    // algorithm line 6~7
    _initialize_pressure_and_pressure_force();
    // algorithm line 8~17
    _prediction_correction_step();

    _compute_boundary_forces();
    _compute_rigid_forces_and_torque();

    // algorithm line 18~20
    _correct_velocity_and_position();

    _update_delta_t();

    //_solve_rigid_body();

    _resolve_collision();

    // for debug
    _data->Fluid.last_positions = _data->Fluid.positions; // show the position of last frame
}

void HinaPE::PCISPHAkinciSurface::_update_fluid_neighbor() const {
// Update Target: NeighborList
    const auto fluid_size = _data->fluid_size();
    const auto &x = _data->Fluid.positions;
    const auto boundary_size = _data->boundary_size();
    const auto &bx = _data->Boundary.positions;

    std::vector<mVector3> total_positions;
    total_positions.reserve(fluid_size + boundary_size);
    total_positions.insert(total_positions.end(), x.begin(), x.end());
    _data->update_boundary();
    total_positions.insert(total_positions.end(), bx.begin(), bx.end());

    PointParallelHashGridSearch3 searcher(_opt.kernel_radius);
    searcher.build(total_positions);

    auto &nl = _data->FluidNeighborList;
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        auto origin = x[i];
        nl[i].clear();
        searcher.for_each_nearby_point(origin, [&](size_t j, const mVector3 &)
        {
            if (i != j) // exclude self
            {
                nl[i].push_back(j);
            }
        });
    });
}

void HinaPE::PCISPHAkinciSurface::_update_boundary_neighbor() const {
    auto &bnl = _data->BoundaryNeighborList;
    const auto fluid_size = _data->fluid_size();
    const auto &x = _data->Fluid.positions;
    const auto boundary_size = _data->boundary_size();
    const auto &bx = _data->Boundary.positions;

    std::vector<mVector3> total_positions;
    total_positions.reserve(fluid_size + boundary_size);
    total_positions.insert(total_positions.end(), x.begin(), x.end());
    _data->update_boundary();
    total_positions.insert(total_positions.end(), bx.begin(), bx.end());

    PointHashGridSearch3 searcher(_opt.kernel_radius);
    searcher.build(total_positions);

    Util::parallelFor(Constant::ZeroSize, boundary_size, [&](size_t i)
    {
        auto origin = bx[i];
        bnl[i].clear();
        searcher.for_each_nearby_point(origin, [&](size_t j, const mVector3 &)
        {
            if (i != j)
            {
                bnl[i].push_back(j);
            }
        });
    });
}

void HinaPE::PCISPHAkinciSurface::_update_boundary_volume() const
{
    auto &x = _data->Fluid.positions;
    const auto &p_b = _data->Boundary.positions;
    auto &v_b = _data->Boundary.volume;
    const auto fluid_size = _data->fluid_size();
    const auto boundary_size = _data->boundary_size();
    const auto &bnl = _data->BoundaryNeighborList;

    StdKernel poly6(_opt.kernel_radius);
    Util::parallelFor(Constant::ZeroSize, boundary_size, [&](size_t i)
    {
        real delta_b = poly6(0);
        for (const auto j: bnl[i])
        {
            if(j < fluid_size)
            {
                delta_b += poly6((p_b[i] - x[j]).length());
            }
            else
            {
                delta_b += poly6((p_b[i] - p_b[j - fluid_size]).length());
            }
        }
        const real volume = static_cast<real>(1.0) / delta_b;
        v_b[i] = volume;
    });
}

void HinaPE::PCISPHAkinciSurface::_update_density() const
{
    // Update Target: fluid densities
    auto &d = _data->Fluid.densities;
    const auto &p = _data->Fluid.positions;
    const auto &m = _data->Fluid.mass;
    const auto &nl = _data->FluidNeighborList;
    const auto fluid_size = _data->fluid_size();

    const auto &b_p = _data->Boundary.positions;
    auto &b_v = _data->Boundary.volume;

    StdKernel poly6(_opt.kernel_radius);
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        real density = m * poly6(0); // self density
        for (const auto j: nl[i])
        {
            if(j < fluid_size){
                real dist = (p[i] - p[j]).length();
                density += m * poly6(dist);
            }else{
                real dist = (p[i] - b_p[j - fluid_size]).length();
                density += _opt.target_density * b_v[j - fluid_size] * poly6(dist);
            }
        }
        d[i] = density; // rho(x) = m * sum(W(x - xj))
    });
}

void HinaPE::PCISPHAkinciSurface::_accumulate_non_pressure_force() const
{
    // Update Target: predicted_position, velocities, forces
    auto &f = _data->Fluid.non_pressure_forces;
    auto &x = _data->Fluid.positions;
    auto &v = _data->Fluid.velocities;
    auto &d = _data->Fluid.densities;
    auto &n = _data->Fluid.normals;
    StdKernel poly6(_opt.kernel_radius);
    CohesionKernel cohesion(_opt.kernel_radius);

    const auto fluid_size = _data->fluid_size();

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
        auto &nl = _data->FluidNeighborList;
        for (size_t j: nl[i])
        {
            if (j < fluid_size)
            {
                real dist = (x[i] - x[j]).length();
                if (d[j] > HinaPE::Constant::Epsilon)
                    //f[i] += _opt.viscosity * m * m * (v[j] - v[i]) / d[j] * poly6.second_derivative(dist);
                    f[i] += m * 0.5 * (2 + 2) * _opt.viscosity * (m / d[j]) * (v[i] - v[j]).dot(x[i] - x[j]) / (dist * dist + 0.01 * _opt.kernel_radius * _opt.kernel_radius) * poly6.cubic_kernel_derivative(dist);
            }
        }
    });

    _compute_normal();

    // Surface Tension Forces
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        auto &nl = _data->FluidNeighborList;
        for (size_t j: nl[i])
        {
            mVector3 F_cohesion = mVector3::Zero();
            mVector3 F_curvature = mVector3::Zero();
            if (j < fluid_size)
            {
                mVector3 x_ij = x[i] - x[j];
                const real k_ij = 2.0 * _opt.target_density / (d[i] + d[j]);
                // Cohesion force
                real dist = (x[i] - x[j]).length();
                if(dist * dist > 1.0e-9)
                {
                    x_ij = 1.0 / dist * x_ij;
                    F_cohesion -= m * m * _opt._surfaceTension * x_ij * cohesion(dist);
                }
                // Curvature force
                F_curvature -= _opt._surfaceTension * (n[i] - n[j]);
                f[i] += m * k_ij * (F_cohesion + F_curvature);
            }
        }
    });
    /*const auto &b = _data->Boundary.positions;
    const auto &bv = _data->Boundary.velocities;
    const auto &bV = _data->Boundary.volume;
    auto &bf_f = _data->Boundary.friction_forces;
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        auto &nl = _data->FluidNeighborList;
        for (size_t j: nl[i])
        {
            if (j < fluid_size) {
                // real nu = _opt.viscosity(?) * _opt.kernel_radius * _opt.speed_of_sound / (d[i] + d[j]); // ? maybe equal to _opt.viscosity
                auto pi = -_opt.viscosity * (v[i] - v[j]).dot(x[i] - x[j]) /
                          ((x[i] - x[j]).squared_norm() + _opt.epsilon * _opt.kernel_radius * _opt.kernel_radius);
                real dist = (x[i] - x[j]).length();
                if (d[j] > HinaPE::Constant::Epsilon)
                    f[i] += m * m * pi * poly6.second_derivative(dist);
            }else{
                auto pi = -_opt.viscosity * (v[i] - v[j - fluid_size]).dot(x[i] - x[j - fluid_size]) /
                          ((x[i] - x[j - fluid_size]).squared_norm() + _opt.epsilon * _opt.kernel_radius * _opt.kernel_radius); // wrong
                real dist = (x[i] - b[j - fluid_size]).length();
                mVector3 boundary_viscosity_force;
                if (d[j] > HinaPE::Constant::Epsilon)
                {
                    //f[i] += _opt.viscosity * bm[j - fluid_size] * m * (v[j] - v[i]) / d[j] * poly6.second_derivative(dist);
                    boundary_viscosity_force = - bV[j - fluid_size] * _opt.target_density * m * pi * poly6.second_derivative(dist); // ??
                    f[i] += boundary_viscosity_force;
                    bf_f[j - fluid_size] += -boundary_viscosity_force;
                }
            }
        }
    });*/
}

void HinaPE::PCISPHAkinciSurface::_initialize_pressure_and_pressure_force() const {
    auto &p = _data->Fluid.pressures;
    auto &p_f = _data->Fluid.pressure_forces;
    auto &d_e = _data->Fluid.density_errors;

    const auto fluid_size = _data->fluid_size();

    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        p[i] = 0.0;
        p_f[i] = mVector3(0.0,0.0,0.0);
        d_e[i] = 0.0;
    });
}

void HinaPE::PCISPHAkinciSurface::_prediction_correction_step() {
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

void HinaPE::PCISPHAkinciSurface::_predict_velocity_and_position() const {
    auto &x = _data->Fluid.positions;
    auto &v = _data->Fluid.velocities;
    auto &f = _data->Fluid.non_pressure_forces;
    auto &p_f = _data->Fluid.pressure_forces;

    auto &x_p = _data->Fluid.predicted_positions;
    auto &v_p = _data->Fluid.predicted_velocities;
    const auto &m = _data->Fluid.mass;
    const auto &dt = _opt.current_dt;
    const auto fluid_size = _data->fluid_size();
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        v_p[i] = v[i] + dt * (f[i] + p_f[i]) / m;
        x_p[i] = x[i] + dt * v_p[i];
    });
}

void HinaPE::PCISPHAkinciSurface::_predict_density() const
{
    auto &d_p = _data->Fluid.predicted_densities;
    auto &x_p = _data->Fluid.predicted_positions;
    auto &m = _data->Fluid.mass;
    const auto fluid_size = _data->fluid_size();
    StdKernel poly6(_opt.kernel_radius);

    const auto &b_p = _data->Boundary.positions;
    auto &b_v = _data->Boundary.volume;

    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        real density = m * poly6(0); // self density
        auto &nl = _data->FluidNeighborList;
        for (size_t j: nl[i])
        {
            if(j < fluid_size)
            {
                real dist = (x_p[i] - x_p[j]).length();
                density += m * poly6(dist);
            }else{
                real dist = (x_p[i] - b_p[j - fluid_size]).length();
                density += _opt.target_density * b_v[j - fluid_size] * poly6(dist);
            }
        }
        d_p[i] = density;
    });
}

void HinaPE::PCISPHAkinciSurface::_update_pressure() {
    auto &p = _data->Fluid.pressures;
    auto &d_p = _data->Fluid.predicted_densities;
    auto &d_e = _data->Fluid.density_errors;
    const auto fluid_size = _data->fluid_size();

    // compute delta
    auto delta = _compute_delta();

    // update pressure
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        real density_err = d_p[i] - _opt.target_density;
        real pressure = delta * density_err;
        if (pressure < 0)
        {
            pressure *= _opt.nps;
            density_err *= _opt.nps;
        }
        // compute density error
        // algorithm line 14
        d_e[i] = density_err;
        // compute pressure force
        // algorithm line 15
        p[i] += pressure;
    });
}

void HinaPE::PCISPHAkinciSurface::_accumulate_pressure_force() {
    auto &x_p = _data->Fluid.predicted_positions;
    auto &p_f = _data->Fluid.pressure_forces;
    auto &p = _data->Fluid.pressures;
    auto &d_e = _data->Fluid.density_errors;
    auto &d_p = _data->Fluid.predicted_densities;
    auto &m = _data->Fluid.mass;
    auto &v = _data->Fluid.velocities;
    const auto fluid_size = _data->fluid_size();

    const auto &b = _data->Boundary.positions;
    auto &bp_f = _data->Boundary.pressure_forces;
    auto &bf_f = _data->Boundary.friction_forces;
    auto &bs_f = _data->Boundary.surface_tension_force;
    auto &b_v = _data->Boundary.volume;
    auto &vb = _data->Boundary.velocities;

    SpikyKernel spiky(_opt.kernel_radius);
    AdhesionKernel adhesion(_opt.kernel_radius);

    std::fill(p_f.begin(), p_f.end(), mVector3::Zero());
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        auto &nl = _data->FluidNeighborList;
        for (size_t j: nl[i])
        {
            if(j < fluid_size)
            {
                real dist = (x_p[i] - x_p[j]).length();
                if (dist > HinaPE::Constant::Epsilon && d_p[j] > HinaPE::Constant::Epsilon)
                {
                    mVector3 dir = (x_p[j] - x_p[i]) / dist;
                    p_f[i] -= m * m * (p[i] / (d_p[i] * d_p[i]) + p[j] / (d_p[j] * d_p[j])) * spiky.gradient(dist, dir);
                }
            }else{
                real dist = (x_p[i] - b[j - fluid_size]).length();
                mVector3 dir = (b[j - fluid_size] - x_p[i]) / dist;
                mVector3 boundary_pressure_force = -_opt.target_density * b_v[j - fluid_size] * m * (p[i] / (d_p[i] * d_p[i]) + p[i] / (d_p[i] * d_p[i])) * spiky.gradient(dist, dir);
                p_f[i] += boundary_pressure_force;
                bp_f[j - fluid_size] = -boundary_pressure_force;

                auto pi = -_opt.viscosity * (v[i] - vb[j - fluid_size]).dot(x_p[i] - b[j - fluid_size]) /
                          ((x_p[i] - b[j - fluid_size]).squared_norm() + _opt.epsilon * _opt.kernel_radius * _opt.kernel_radius);
                mVector3 boundary_friction_force = -_opt.target_density * b_v[j - fluid_size] * m * pi * (p[i] / (d_p[i] * d_p[i]) + p[i] / (d_p[i] * d_p[i])) * spiky.gradient(dist, dir);
                p_f[i] += boundary_friction_force;
                bf_f[j - fluid_size] = -boundary_friction_force;

                /*mVector3 x_ij = x_p[i] - b[j - fluid_size];
                const real length2 = x_ij.squared_norm();
                if(length2 > 1.0e-9)
                {
                    x_ij = 1.0 / sqrt(length2) * x_ij;
                    mVector3 surface_tension_force = -m * _opt._surfaceTensionBoundary * _opt.target_density * b_v[j - fluid_size] * x_ij * adhesion(dist);
                    p_f[i] += surface_tension_force;
                    bs_f[j - fluid_size] = -surface_tension_force;
                    // The definition as p_f may not be appropriate
                }*/
            }
        }
    });

    // Compute max density error
    real maxDensityError = 0.0;
    real DensityErrorRatio;

    for (int j = 0; j < fluid_size; ++j)
    {
        maxDensityError = std::max(maxDensityError, std::abs(d_e[j]));
    }

    DensityErrorRatio = maxDensityError / _opt.target_density;

    _opt._maxDensityVariation = maxDensityError;

    if(abs(DensityErrorRatio) < _opt.max_density_error_ratio)
    {
        _opt.density_error_too_large = false;
    }

    real avgDensityError = 0.0;
    for (int j = 0; j < fluid_size; ++j)
    {
        avgDensityError += std::abs(d_e[j]);
    }
    avgDensityError /= fluid_size;

    _opt._avgDensityVariation = avgDensityError;
}

void HinaPE::PCISPHAkinciSurface::_correct_velocity_and_position() {
    auto &x = _data->Fluid.positions;
    auto &v = _data->Fluid.velocities;
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

    /*_compute_boundary_forces();
    const auto boundary_size = _data->boundary_size();
    auto &b_x = _data->Boundary.positions;
    auto &b_v = _data->Boundary.velocities;
    auto &b_f = _data->Boundary.forces;
    auto &b_vo = _data->Boundary.volume;
    Util::parallelFor(Constant::ZeroSize, boundary_size, [&](size_t i)
    {
        b_v[i] += dt * b_f[i] / (b_vo[i] * _opt.target_density);
        b_x[i] += dt * b_v[i];
    });*/

    real maxForce = 0.0;
    real maxVelocity = 0.0;

    for (int i = 0; i < _data->fluid_size(); ++i)
    {
        maxForce = std::max(maxForce, std::abs((p_f[i] + f[i]).norm()));
        maxVelocity = std::max(maxVelocity, std::abs(v[i].norm()));
    }
    _opt._maxVelocity = maxVelocity;
    _opt._maxForce = maxForce;
}

void HinaPE::PCISPHAkinciSurface::_resolve_collision() const {
    auto &x = _data->Fluid.positions;
    auto &v = _data->Fluid.velocities;

    // Collision
    Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
    {
        _domain->resolve_collision(_opt.radius, _opt.restitution, &x[i], &v[i]);
    });
}

auto HinaPE::PCISPHAkinciSurface::_compute_delta() const -> real
{
    SpikyKernel spiky(_opt.kernel_radius);
    mVector3 sumGradW = mVector3::Zero();
    real sumGradW2 = 0.0;
    const real supportRadius = _opt.kernel_radius;
    const real particleRadius = _opt.radius;
    const real diam = static_cast<real>(2.0) * particleRadius;
    const mVector3 xi(0,0,0);
    mVector3 xj = { -supportRadius, -supportRadius, -supportRadius };
    while (xj[0] <= supportRadius)
    {
        while (xj[1] <= supportRadius)
        {
            while (xj[2] <= supportRadius)
            {
                real dist = (xi - xj).length();
                // check if xj is in the support of xi
                if ((xi - xj).length_squared() < supportRadius*supportRadius)
                {
                    mVector3 dir = (xi - xj).normalized();
                    const mVector3 gradW = spiky.gradient(dist, dir);
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
    real denom = -sumGradW.dot(sumGradW) - sumGradW2;
    real beta = 2 * Math::square(_data->Fluid.mass * _opt.current_dt / _opt.target_density);
    return (std::fabs(denom) > 0) ? -1 / (beta * denom) : 0;
}

void HinaPE::PCISPHAkinciSurface::_compute_boundary_forces() const {
    const auto &bp_f = _data->Boundary.pressure_forces;
    const auto &bf_f = _data->Boundary.friction_forces;
    const auto &bs_f = _data->Boundary.surface_tension_force;
    auto &b_f = _data->Boundary.forces;
    const auto boundary_size = _data->boundary_size();

    Util::parallelFor(Constant::ZeroSize, boundary_size, [&](size_t i) // every boundary particle
    {
        b_f[i] = bp_f[i] + bf_f[i]+ bs_f[i];
    });
}

void HinaPE::PCISPHAkinciSurface::_compute_rigid_forces_and_torque() const{
    auto &each_rigid = _data->ForceAndTorque;
    auto num_of_rigid = _data->Boundary.poses.size();
    auto &boundary = _data->Boundary.boundary_sizes;
    auto &b_f = _data->Boundary.forces;

    real diam = 2 * _opt.radius;
    real volume = static_cast<real>(0.8) * diam * diam * diam;

    std::fill(each_rigid.force.begin(), each_rigid.force.end(), mVector3::Zero());
    std::fill(each_rigid.torque.begin(), each_rigid.torque.end(), mVector3::Zero());
    Util::parallelFor(Constant::ZeroSize, num_of_rigid, [&](size_t i)
    {
        const auto start_index = boundary[i].first;
        const auto end_index = boundary[i].second;

        /*real sum_mass = 0.0;
        mVector3 center_of_mass(0.0, 0.0, 0.0);*/

        Util::parallelFor(start_index, end_index, [&](size_t j)
        {
            /*real mass = volume * _opt.target_density;
            sum_mass += mass;
            center_of_mass += _data->Boundary.positions[j] * mass;*/
            each_rigid.force[i] += b_f[j];
            each_rigid.torque[i] += (b_f[j]).cross(_data->Boundary.positions[j] - _data->Boundary.poses[i]->position);
        },Util::ExecutionPolicy::Serial);
    });
}

void HinaPE::PCISPHAkinciSurface::_update_delta_t() {
    /*real maxForce = 0.0;
    real maxVelocity = 0.0;
    auto &f = _data->Fluid.non_pressure_forces;
    auto &p_f = _data->Fluid.pressure_forces;
    auto &v = _data->Fluid.velocities;
    for (int i = 0; i < _data->fluid_size(); ++i)
    {
        maxForce = std::max(maxForce, std::abs((p_f[i] + f[i]).norm()));
        maxVelocity = std::max(maxVelocity, std::abs(v[i].norm()));
    }
    _opt.current_dt = std::min(0.2 * sqrt(_opt.kernel_radius / maxForce), 0.25 * _opt.kernel_radius / maxVelocity);*/
    _opt._maxVelocity = std::max(1e-8f, (float)_opt._maxVelocity);
    _opt._maxForce = std::max(1e-8f, (float)_opt._maxForce);

    // Adjust timeStep
    if ((0.19f * std::sqrt(_opt.kernel_radius / _opt._maxForce) > _opt.current_dt) &&
        (_opt._maxDensityVariation < 4.5f * _opt._avgDensityVariationThreshold) &&
        (_opt._avgDensityVariation < 0.9f * _opt._avgDensityVariationThreshold) &&
        (0.39f * _opt.kernel_radius / _opt._maxVelocity > _opt.current_dt)) {
        _opt.current_dt *= 1.002f;
    }
    if ((0.2f * std::sqrt(_opt.kernel_radius / _opt._maxForce) < _opt.current_dt) ||
        (_opt._maxDensityVariation > 5.5f * _opt._avgDensityVariationThreshold) ||
        (_opt._avgDensityVariation >= _opt._avgDensityVariationThreshold) ||
        (0.4f * _opt.kernel_radius / _opt._maxVelocity <= _opt.current_dt)) {
        _opt.current_dt *= 0.998f;
    }

    // Detect shock
    //if ((_maxDensityVariation - _prevMaxDensityVariation > 0.5f * (_avgDensityVariationThreshold + _maxDensityVariationThreshold)) ||
    if ((_opt._maxDensityVariation - _opt._prevMaxDensityVariation > _opt._maxDensityVariationThreshold) ||
        (_opt._maxDensityVariation > _opt._maxDensityVariationThreshold) ||
        (0.45f * _opt.kernel_radius / _opt._maxVelocity < _opt.current_dt)) {
        _opt.current_dt = std::min(0.2f * std::sqrt(_opt.kernel_radius / _opt._maxForce), 0.25f * _opt.kernel_radius / _opt._maxVelocity);
    }else{
        _opt._prevMaxDensityVariation = _opt._maxDensityVariation;
    }
}

void HinaPE::PCISPHAkinciSurface::reset() {
    _data->reset();
    init();
}

void HinaPE::PCISPHAkinciSurface::INSPECT() {
// Solver Parameters
    ImGui::Text("SOLVER INSPECTOR");
    ImGui::Text("Fluids: %zu", _data->fluid_size());
    //ImGui::Text("Boundaries: %zu", _data->boundary_size());
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
        ImGui::Text("Predict Density: %.3f", _data->Fluid.predicted_densities[inst_id]);
        ImGui::Text("Pressure: %.3f", _data->Fluid.pressures[inst_id]);
        ImGui::Text("Neighbors: %zu", _data->FluidNeighborList[inst_id].size());
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

void HinaPE::PCISPHAkinciSurface::_solve_rigid_body() const {
    auto num_of_rigid = _data->Boundary.poses.size();
    Util::parallelFor(Constant::ZeroSize, num_of_rigid, [&](size_t i)
    {
        mMatrix3x3 R = solve_constraints(i);
    });
}

void HinaPE::PCISPHAkinciSurface::_compute_rest_mass_center() const {
    auto &b_v = _data->Boundary.volume;
    auto &boundary= _data->Boundary.boundary_sizes;
    auto num_of_rigid = _data->Boundary.poses.size();

    real diam = 2 * _opt.radius;
    real volume = static_cast<real>(0.8) * diam * diam * diam;

    Util::parallelFor(Constant::ZeroSize, num_of_rigid, [&](size_t i)
    {
        real sum_mass = 0.0;
        mVector3 center_of_mass(0.0, 0.0, 0.0);
        const auto start_index = boundary[i].first;
        const auto end_index = boundary[i].second;

        for (size_t j = start_index; j < end_index; ++j)
        {
            real mass = volume * _opt.target_density;
            sum_mass += mass;
            center_of_mass += _data->Boundary.positions_origin[j] * mass;
        }
        center_of_mass /= sum_mass;
        _data->CenterOfMass[i] = center_of_mass;
    });
}

mVector3 HinaPE::PCISPHAkinciSurface::_compute_mass_center(size_t i) const {
    real sum_mass = 0.0;
    mVector3 center_of_mass(0.0, 0.0, 0.0);

    real diam = 2 * _opt.radius;
    real volume = static_cast<real>(0.8) * diam * diam * diam;

    auto &b_v = _data->Boundary.volume;
    auto &boundary= _data->Boundary.boundary_sizes;
    const auto start_index = boundary[i].first;
    const auto end_index = boundary[i].second;

    for (size_t j = start_index; j < end_index; ++j)
    {
        real mass = volume * _opt.target_density;
        sum_mass += mass;
        center_of_mass += _data->Boundary.positions[j] * mass;
    }
    center_of_mass /= sum_mass;
    return center_of_mass;
}

mMatrix3x3 HinaPE::PCISPHAkinciSurface::solve_constraints(size_t i) const {
    mVector3 center_of_mass = _compute_mass_center(i);
    mMatrix3x3 A = mMatrix3x3::Zero();

    real diam = 2 * _opt.radius;
    real volume = static_cast<real>(0.8) * diam * diam * diam;

    auto &b_v = _data->Boundary.volume;
    auto &boundary = _data->Boundary.boundary_sizes;
    auto &is_active = _data->Boundary.IsActive;

    const auto start_index = boundary[i].first;
    const auto end_index = boundary[i].second;

    for (size_t j = start_index; j < end_index; ++j)
    {
        auto x_0 = _data->Boundary.positions_origin[j];
        const auto m = _data->Boundary.poses[i]->get_model_matrix();
        auto x_0_world = (m * mVector4(x_0.x(), x_0.y(), x_0.z(), 1)).xyz();
        auto x = _data->Boundary.positions[j];
        mVector3 q = x_0_world - _data->CenterOfMass[i];
        mVector3 p = x - center_of_mass;
        A = A + volume * _opt.target_density * _compute_outer_product(p, q);
    }

    Eigen::Matrix3d eigenA;
    eigenA << A(0, 0), A(0, 1), A(0, 2),
            A(1, 0), A(1, 1), A(1, 2),
            A(2, 0), A(2, 1), A(2, 2);

    Eigen::JacobiSVD<Eigen::Matrix3d> svd(eigenA, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::Matrix3d R = svd.matrixU() * svd.matrixV().transpose();

    double tolerance = 1e-6;
    if ((R - Eigen::Matrix3d::Identity()).lpNorm<Eigen::Infinity>() < tolerance) {
        R = Eigen::Matrix3d::Identity(); // Set R to the identity matrix
    }

    mMatrix3x3 mR{R(0, 0), R(0, 1), R(0, 2),
                  R(1, 0), R(1, 1), R(1, 2),
                  R(2, 0), R(2, 1), R(2, 2)};

    for (size_t j = start_index; j < end_index; ++j)
    {
        if(is_active[j])
        {
            auto x_0 = _data->Boundary.positions_origin[j];
            auto x = _data->Boundary.positions[j];
            const auto m = _data->Boundary.poses[i]->get_model_matrix();
            auto x_0_world = (m * mVector4(x_0.x(), x_0.y(), x_0.z(), 1)).xyz();
            mVector3 goal = center_of_mass + mR * (x_0_world - _data->CenterOfMass[i]);
            mVector3 corr = (goal - x) * 1.0;
            _data->Boundary.positions[j] += corr;
        }
    }
    return mR;
}

auto HinaPE::PCISPHAkinciSurface::_compute_outer_product(mVector3 p, mVector3 q) -> mMatrix3x3 {
    mMatrix3x3 result = mMatrix3x3::Zero();
    result(0, 0) = p.x() * q.x();
    result(0, 1) = p.x() * q.y();
    result(0, 2) = p.x() * q.z();
    result(1, 0) = p.y() * q.x();
    result(1, 1) = p.y() * q.y();
    result(1, 2) = p.y() * q.z();
    result(2, 0) = p.z() * q.x();
    result(2, 1) = p.z() * q.y();
    result(2, 2) = p.z() * q.z();
    return result;
}

void HinaPE::PCISPHAkinciSurface::_compute_normal() const {
    auto &x = _data->Fluid.positions;
    auto &d = _data->Fluid.densities;
    auto &ni = _data->Fluid.normals;
    SpikyKernel spiky(_opt.kernel_radius);
    const auto fluid_size = _data->fluid_size();
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        auto &nl = _data->FluidNeighborList;
        for (size_t j: nl[i])
        {
            if(j < fluid_size)
                ni[i] += _data->Fluid.mass / d[j] * spiky.gradient(x[i] - x[j]);
        }
        ni[i] = _opt.kernel_radius * ni[i];
    });
}

// ================================================== Data ==================================================

HinaPE::PCISPHAkinciSurface::Data::Data() {
    track(&Fluid.last_positions);
    _color_map = &color_map;
}

auto HinaPE::PCISPHAkinciSurface::Data::fluid_size() const -> size_t {
    return Fluid.positions.size();
}

auto HinaPE::PCISPHAkinciSurface::Data::boundary_size() const -> size_t {
    return Boundary.positions.size();
}

void HinaPE::PCISPHAkinciSurface::Data::reset() {
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
    Fluid.mass = 1e-3;

    Fluid.normals.clear();
    Fluid.surface_tension_forces.clear();

    Boundary.positions.clear();
    Boundary.positions_origin.clear();
    Boundary.velocities.clear();
    Boundary.volume.clear();

    Boundary.forces.clear();
    Boundary.pressure_forces.clear();
    Boundary.friction_forces.clear();
    Boundary.surface_tension_force.clear();

    Boundary.poses.clear();
    Boundary.boundary_sizes.clear();

    FluidNeighborList.clear();
    BoundaryNeighborList.clear();

    CenterOfMass.clear();

    ForceAndTorque.force.clear();
    ForceAndTorque.torque.clear();

    color_map.clear();
    debug_info.clear();
}

void HinaPE::PCISPHAkinciSurface::Data::add_fluid(const std::vector<mVector3> &positions, const std::vector<mVector3> &velocities) {
    if (positions.size() != velocities.size())
        throw std::runtime_error("positions.size() != velocities.size()");

    auto size = positions.size();
    Fluid.positions.insert(Fluid.positions.end(), positions.begin(), positions.end());
    Fluid.velocities.insert(Fluid.velocities.end(), velocities.begin(), velocities.end());
    Fluid.predicted_positions.insert(Fluid.predicted_positions.end(), positions.begin(), positions.end());
    Fluid.predicted_velocities.insert(Fluid.predicted_velocities.end(), velocities.begin(), velocities.end());
    Fluid.non_pressure_forces.insert(Fluid.non_pressure_forces.end(),size,mVector3::Zero());
    Fluid.pressure_forces.insert(Fluid.pressure_forces.end(),size,mVector3::Zero());
    Fluid.densities.insert(Fluid.densities.end(), size, 0.0);
    Fluid.predicted_densities.insert(Fluid.predicted_densities.end(), size, 0.0);
    Fluid.density_errors.insert(Fluid.density_errors.end(), size, 0.0);
    Fluid.pressures.insert(Fluid.pressures.end(), size, 0.0);

    Fluid.normals.insert(Fluid.normals.end(), size, mVector3::Zero());
    Fluid.surface_tension_forces.insert(Fluid.surface_tension_forces.end(), size, mVector3::Zero());

    Fluid.last_positions.insert(Fluid.last_positions.end(), positions.begin(), positions.end());

    FluidNeighborList.insert(FluidNeighborList.end(), size, std::vector<unsigned int>());
    color_map.insert(color_map.end(), size, Color::ORANGE);
    debug_info.insert(debug_info.end(), size, std::vector<std::string>());
}

void HinaPE::PCISPHAkinciSurface::Data::add_boundary(const std::vector<mVector3> &positions, const Kasumi::Pose *pose) {
    size_t start = Boundary.positions_origin.size();
    Boundary.positions_origin.insert(Boundary.positions_origin.end(), positions.begin(), positions.end());
    size_t end = Boundary.positions_origin.size();

    Boundary.poses.push_back(pose);
    Boundary.boundary_sizes.emplace_back(start, end);
    Boundary.positions.resize(Boundary.positions_origin.size());
    Boundary.velocities.insert(Boundary.velocities.end(), positions.size(), mVector3::Zero());
    Boundary.pressure_forces.insert(Boundary.pressure_forces.end(), positions.size(), mVector3::Zero());
    Boundary.friction_forces.insert(Boundary.friction_forces.end(), positions.size(), mVector3::Zero());
    Boundary.surface_tension_force.insert(Boundary.surface_tension_force.end(), positions.size(), mVector3::Zero());
    Boundary.forces.insert(Boundary.forces.end(), positions.size(), mVector3::Zero());
    BoundaryNeighborList.insert(BoundaryNeighborList.end(), positions.size(), std::vector<unsigned int>());

    ForceAndTorque.force.insert(ForceAndTorque.force.end(), (size_t)1, mVector3::Zero());
    ForceAndTorque.torque.insert(ForceAndTorque.torque.end(), (size_t)1, mVector3::Zero());
//    CenterOfMass.insert(CenterOfMass.end(), (size_t)1, mVector3::Zero());
    update_boundary();
}

void HinaPE::PCISPHAkinciSurface::Data::update_boundary() {
    for (int i = 0; i < Boundary.poses.size(); ++i)
    {
        std::vector<mVector3> res;
        const auto [start, end] = Boundary.boundary_sizes[i];
        const auto m = Boundary.poses[i]->get_model_matrix();

        for (size_t j = start; j < end; ++j)
            Boundary.positions[j] = (m * mVector4(Boundary.positions_origin[j].x(), Boundary.positions_origin[j].y(), Boundary.positions_origin[j].z(), 1)).xyz();
    }
}