#include "pcisph_temp.h"

#define S HinaPE::Math::to_string
#define Record(x, y) _data->debug_info[i].emplace_back(x + S(y))

// ============================================================================================================
// ================================================== Solver ==================================================
void HinaPE::PCISPHSolverTEMP::init() {
// init data
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
    if (_sphere == nullptr)
    {
        auto sphere = std::make_shared<Kasumi::SphereObject>();
        sphere->POSE.position = mVector3(0.8, 0.7, 0);
        sphere->POSE.scale = 0.3 * mVector3::One();
        sphere->_update_surface();
        _sphere = sphere;
        _sphere->set_color(Color::RED);
    }
    if (_cube == nullptr)
    {
        auto cube = std::make_shared<Kasumi::CubeObject>();
        cube->POSE.position = mVector3(0, -0.8, 0);
        cube->POSE.euler = mVector3(90, 0, 90);
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
    _init_boundary_density();
}
void HinaPE::PCISPHSolverTEMP::_init_fluid_particles() const
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
void HinaPE::PCISPHSolverTEMP::_init_boundary_particles() const {
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

    _data->Boundary.IsActive.insert(_data->Boundary.IsActive.end(), target_boundary.size(), false);

    if (max_number_density > 0)
    {
        _data->Boundary.density.insert(_data->Boundary.density.end(), target_boundary.size(), HinaPE::Constant::Zero);
        _data->Boundary.volume.insert(_data->Boundary.volume.end(), target_boundary.size(), HinaPE::Constant::Zero);
        _data->Boundary.mass.insert(_data->Boundary.mass.end(), target_boundary.size(), 10 * std::max((_opt.target_density / max_number_density), HinaPE::Constant::Zero));
    }
    else
        throw std::runtime_error("max_number_density is zero");
}
void HinaPE::PCISPHSolverTEMP::_init_collider() const
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
                {
                    temp_neighbor_list[i].push_back(j);
                }
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

        _data->Boundary.IsActive.insert(_data->Boundary.IsActive.end(), target_boundary.size(), true);

        if (max_number_density > 0){
            _data->Boundary.density.insert(_data->Boundary.density.end(), target_boundary.size(), HinaPE::Constant::Zero);
            _data->Boundary.mass.insert(_data->Boundary.mass.end(), target_boundary.size(), 5 * std::max((_opt.target_density / max_number_density), HinaPE::Constant::Zero));
            _data->Boundary.volume.insert(_data->Boundary.volume.end(), target_boundary.size(), HinaPE::Constant::Zero);
        }
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

        _data->Boundary.IsActive.insert(_data->Boundary.IsActive.end(), target_boundary.size(), true);

        if (max_number_density > 0)
        {
            _data->Boundary.density.insert(_data->Boundary.density.end(), target_boundary.size(), HinaPE::Constant::Zero);
            _data->Boundary.mass.insert(_data->Boundary.mass.end(), target_boundary.size(), 5 * std::max((_opt.target_density / max_number_density), HinaPE::Constant::Zero));
            _data->Boundary.volume.insert(_data->Boundary.volume.end(), target_boundary.size(), HinaPE::Constant::Zero);
        }
        else
            throw std::runtime_error("max_number_density is zero");
    }
}
void HinaPE::PCISPHSolverTEMP::_init_boundary_density() const {
    _update_boundary_neighbor();
    auto &b_d = _data->Boundary.density;
    const auto &d_p = _data->Boundary.positions_origin;
    const auto &bnl = _data->BoundaryNeighborList;
    const auto &bm = _data->Boundary.mass;
    const auto fluid_size = _data->fluid_size();
    const auto boundary_size = _data->boundary_size();

    StdKernel poly6(_opt.kernel_radius);
    Util::parallelFor(Constant::ZeroSize, boundary_size, [&](size_t i)
    {
        real density = bm[i] * poly6(0); // self density
        for (const auto j: bnl[i])
        {
            if (j > fluid_size){
                real dist = (d_p[i] - d_p[j - fluid_size]).length();
                density += bm[i] * poly6(dist);
            }
        }
        b_d[i] = density;
    });
}
void HinaPE::PCISPHSolverTEMP::_update_density() const
{
    // Update Target: fluid densities
    auto &d = _data->Fluid.densities;
    const auto &p = _data->Fluid.positions;
    const auto &b = _data->Boundary.positions;
    const auto &m = _data->Fluid.mass;
    const auto &bV = _data->Boundary.volume;
    const auto &bd = _data->Boundary.density;
    const auto &nl = _data->FluidNeighborList;
    const auto fluid_size = _data->fluid_size();

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
                real dist = (p[i] - b[j - fluid_size]).length();
                density += bd[j - fluid_size] * bV[j - fluid_size] * poly6(dist);
            }
        }
        d[i] = density; // rho(x) = m * sum(W(x - xj))
    });
}
void HinaPE::PCISPHSolverTEMP::_update_fluid_neighbor() const
{
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

    PointHashGridSearch3 searcher(_opt.kernel_radius);
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
void HinaPE::PCISPHSolverTEMP::_update_boundary_neighbor() const {
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
void HinaPE::PCISPHSolverTEMP::_update_boundary_volume() const
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
                delta_b += poly6((p_b[i] - p_b[j-fluid_size]).length());
            }
        }
        const real volume = static_cast<real>(1.0) / delta_b;
        v_b[i] = volume;
    });
}
void HinaPE::PCISPHSolverTEMP::update(real dt)
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
    // for rigid body
    _compute_rigid_forces_and_torque();

    // algorithm line 18~20
    _correct_velocity_and_position();

    // domain collision (?
    //_resolve_collision();

    // for debug
    _data->Fluid.last_positions = _data->Fluid.positions; // show the position of last frame
}
void HinaPE::PCISPHSolverTEMP::_accumulate_non_pressure_force() const
{
    // Update Target: predicted_position, velocities, forces
/*    _data->Fluid.predicted_positions = _data->Fluid.positions;
    _data->Fluid.predicted_velocities = _data->Fluid.velocities;*/

    auto &f = _data->Fluid.non_pressure_forces;
    auto &x = _data->Fluid.positions;
    auto &v = _data->Fluid.velocities;
    auto &d = _data->Fluid.densities;
    StdKernel poly6(_opt.kernel_radius);

    const auto &b = _data->Boundary.positions;
    const auto &bV = _data->Boundary.volume;
    auto &bf_f = _data->Boundary.friction_forces;

    std::fill(bf_f.begin(), bf_f.end(), mVector3::Zero());

    const auto fluid_size = _data->fluid_size();
    //const auto boundary_size = _data->boundary_size();

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
            if (j < fluid_size){
                real dist = (x[i] - x[j]).length();
                if (d[j] > HinaPE::Constant::Epsilon)
                    f[i] += _opt.viscosity * m * m * (v[j] - v[i]) / d[j] * poly6.second_derivative(dist);
            }else
            {
                /*real dist = (x[i] - b[j - fluid_size]).length();
                mVector3 boundary_viscosity_force;
                if (d[j] > HinaPE::Constant::Epsilon)
                {
                    //f[i] += _opt.viscosity * bm[j - fluid_size] * m * (v[j] - v[i]) / d[j] * poly6.second_derivative(dist);
                    boundary_viscosity_force = _opt.viscosity * bV[j - fluid_size] * _opt.target_density * m * (v[j] - v[i]) / d[j] * poly6.second_derivative(dist);
                    f[i] += boundary_viscosity_force;
                    bf_f[j - fluid_size] += -boundary_viscosity_force;
                }*/
            }
        }
    });
}
void HinaPE::PCISPHSolverTEMP::_initialize_pressure_and_pressure_force() const {
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
void HinaPE::PCISPHSolverTEMP::_prediction_correction_step() {
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
void HinaPE::PCISPHSolverTEMP::_predict_velocity_and_position() const {
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
void HinaPE::PCISPHSolverTEMP::_predict_density() const
{
    auto &d_p = _data->Fluid.predicted_densities;
    auto &x_p = _data->Fluid.predicted_positions;

    auto &m = _data->Fluid.mass;

    const auto &b = _data->Boundary.positions;
    const auto &bV = _data->Boundary.volume;
    const auto &bd = _data->Boundary.density;

    const auto fluid_size = _data->fluid_size();
    StdKernel poly6(_opt.kernel_radius);

    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        real density = m * poly6(0); // self density
        auto &nl = _data->FluidNeighborList;
        for (size_t j: nl[i])
        {
            if (j < fluid_size)
            {
                real dist = (x_p[i] - x_p[j]).length();
                density += m * poly6(dist);
            } else
            {
                real dist = (x_p[i] - b[j - fluid_size]).length();
                density += bd[j - fluid_size] * bV[j - fluid_size] * poly6(dist);
            }
        }
        d_p[i] = density;
    });
}
void HinaPE::PCISPHSolverTEMP::_update_pressure() {
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
void HinaPE::PCISPHSolverTEMP::_accumulate_pressure_force() {
    auto &x_p = _data->Fluid.predicted_positions;
    auto &p_f = _data->Fluid.pressure_forces;
    auto &p = _data->Fluid.pressures;
    auto &d_e = _data->Fluid.density_errors;
    auto &d_p = _data->Fluid.predicted_densities;
    auto &m = _data->Fluid.mass;
    const auto &bV = _data->Boundary.volume;
    const auto &bd = _data->Boundary.density;
    const auto &b = _data->Boundary.positions;
    auto &bp_f = _data->Boundary.pressure_forces;
    auto &b_flag = _data->Boundary.IsActive;
    const auto fluid_size = _data->fluid_size();
    SpikyKernel spiky(_opt.kernel_radius);
    std::fill(p_f.begin(), p_f.end(), mVector3::Zero());
    std::fill(bp_f.begin(), bp_f.end(), mVector3::Zero());
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
    {
        auto &nl = _data->FluidNeighborList;
        for (size_t j: nl[i])
        {
            if (j < fluid_size){
                real dist = (x_p[i] - x_p[j]).length();
                if (dist > HinaPE::Constant::Epsilon && d_p[j] > HinaPE::Constant::Epsilon)
                {
                    mVector3 dir = (x_p[j] - x_p[i]) / dist;
                    p_f[i] -= m * m * (p[i] / (d_p[i] * d_p[i]) + p[j] / (d_p[j] * d_p[j])) * spiky.gradient(dist, dir);
                }
            }else
            {
                real dist = (x_p[i] - b[j - fluid_size]).length();
                if (dist > HinaPE::Constant::Epsilon && d_p[j] > HinaPE::Constant::Epsilon){
                    mVector3 dir = (b[j - fluid_size] - x_p[i]) / dist;
                    mVector3 boundary_pressure_force = -bd[j - fluid_size] * bV[j - fluid_size] * m * (p[i] / (d_p[i] * d_p[i])) * spiky.gradient(dist, dir);
                    p_f[i] += boundary_pressure_force;
                    if(b_flag[j - fluid_size])
                    {
                        bp_f[j - fluid_size] += -boundary_pressure_force;
                    }
                }
            }
        }
    });

    // Compute max density error
    real maxDensityError = 0.0;
    real DensityErrorRatio;

    for (int j = 0; j < _data->fluid_size(); ++j)
    {
        maxDensityError = std::max(maxDensityError, std::abs(d_e[j]));
    }

    DensityErrorRatio = maxDensityError / _opt.target_density;

    if(abs(DensityErrorRatio) < _opt.max_density_error_ratio)
    {
        _opt.density_error_too_large = false;
    }
}
void HinaPE::PCISPHSolverTEMP::_compute_boundary_forces() const {
    const auto &bp_f = _data->Boundary.pressure_forces;
    const auto &bf_f = _data->Boundary.friction_forces;
    auto &b_f = _data->Boundary.forces;

    const auto &bnl = _data->BoundaryNeighborList;
    const auto fluid_size = _data->fluid_size();
    const auto boundary_size = _data->boundary_size();

    Util::parallelFor(Constant::ZeroSize, boundary_size, [&](size_t i) // every boundary particle
    {
        b_f[i] = 0.4 * bp_f[i] + bf_f[i];
    });
}
void HinaPE::PCISPHSolverTEMP::_compute_rigid_forces_and_torque() const {
    std::vector<mVector3> world_rigid_center;
    std::vector<mVector3> local_rigid_center;
    auto num_of_rigid = _data->Boundary.poses.size();
    world_rigid_center.resize(num_of_rigid);
    local_rigid_center.resize(num_of_rigid);
    auto &boundary = _data->Boundary.boundary_sizes;
    const auto &b_o_p = _data->Boundary.positions_origin;
    const auto &b_f = _data->Boundary.forces;
    auto &each_rigid = _data->ForceAndTorque;
    std::fill(each_rigid.force.begin(), each_rigid.force.end(), mVector3::Zero());
    std::fill(each_rigid.torque.begin(), each_rigid.torque.end(), mVector3::Zero());
    Util::parallelFor(Constant::ZeroSize, num_of_rigid, [&](size_t i)
    {
        mMatrix4x4 modelMatrix = _data->Boundary.poses[i]->get_model_matrix();
        mMatrix3x3 rotationMatrix = _data->Boundary.poses[i]->get_rotation_matrix_3();
        world_rigid_center[i] = _data->Boundary.poses[i]->position;
        local_rigid_center[i] = rotationMatrix.inversed() * world_rigid_center[i];

        const auto start_index = boundary[i].first;
        const auto end_index = boundary[i].second;
        Util::parallelFor(start_index, end_index, [&](size_t j)
        {
            // Step 1: 计算相对位置
            mVector3 relativePosition = b_o_p[j] - local_rigid_center[i];
            // Step 2: 转换力到局部坐标系
            mVector3 transformedForce = rotationMatrix.transposed() * b_f[j];

            each_rigid.force[i] += transformedForce;
            each_rigid.torque[i] += transformedForce.cross(relativePosition);
        },Util::ExecutionPolicy::Serial);
    });
}

/*void HinaPE::PCISPHSolverTEMP::_compute_rigid_forces_and_torque() const {
    mVector3 rigid_force = mVector3::Zero();
    mVector3 rigid_torque = mVector3::Zero();

    const auto &b_p = _data->Boundary.positions;
    auto &b_f = _data->Boundary.forces;
    const auto &nl = _data->FluidNeighborList;
    auto &boundary = _data->Boundary.boundary_sizes;
    auto &each_rigid = _data->ForceAndTorque;

    auto num_of_rigid = _data->Boundary.poses.size();
    std::fill(each_rigid.force.begin(), each_rigid.force.end(), mVector3::Zero());
    std::fill(each_rigid.torque.begin(), each_rigid.torque.end(), mVector3::Zero());
    Util::parallelFor(Constant::ZeroSize, num_of_rigid, [&](size_t i)
    {
        const auto start_index = boundary[i].first;
        const auto end_index = boundary[i].second;
        Util::parallelFor(start_index, end_index, [&](size_t j)
        {
            each_rigid.force[i] += b_f[j];
            each_rigid.torque[i] += b_f[j].cross(b_p[j] - _data->Boundary.poses[i]->position);
        },Util::ExecutionPolicy::Serial);
    });
}*/
void HinaPE::PCISPHSolverTEMP::_correct_velocity_and_position() const {
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
}
auto HinaPE::PCISPHSolverTEMP::_compute_delta() const -> real
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
void HinaPE::PCISPHSolverTEMP::_resolve_collision() const {
    auto &x = _data->Fluid.positions;
    auto &v = _data->Fluid.velocities;

    // Collision
    Util::parallelFor(Constant::ZeroSize, _data->fluid_size(), [&](size_t i)
    {
        _domain->resolve_collision(_opt.radius, _opt.restitution, &x[i], &v[i]);
    });
}
void HinaPE::PCISPHSolverTEMP::reset() {
    _data->reset();
    init();
}
void HinaPE::PCISPHSolverTEMP::INSPECT()
{
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

    //ImGui::Checkbox("Akinci2012 Collision", &_opt.use_akinci2012_collision);
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

void HinaPE::PCISPHSolverTEMP::_compute_E() const {
    auto &x = _data->Fluid.positions;
    auto &v = _data->Fluid.velocities;
    auto &m = _data->Fluid.mass;
    auto &rho = _data->Fluid.densities;
    const auto fluid_size = _data->fluid_size();
    SpikyKernel spiky(_opt.kernel_radius);
    Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i){

    });
}

// ================================================== Data ==================================================
HinaPE::PCISPHSolverTEMP::Data::Data() {
    track(&Fluid.last_positions);
    _color_map = &color_map;
}
auto HinaPE::PCISPHSolverTEMP::Data::fluid_size() const -> size_t { return Fluid.positions.size(); }
auto HinaPE::PCISPHSolverTEMP::Data::boundary_size() const -> size_t {return Boundary.positions.size();}
void HinaPE::PCISPHSolverTEMP::Data::add_fluid(const std::vector<mVector3> &positions,
                                                       const std::vector<mVector3> &velocities){
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

    Fluid.last_positions.insert(Fluid.last_positions.end(), positions.begin(), positions.end());

    FluidNeighborList.insert(FluidNeighborList.end(), size, std::vector<unsigned int>());
    color_map.insert(color_map.end(), size, Color::ORANGE);
    debug_info.insert(debug_info.end(), size, std::vector<std::string>());
}
void HinaPE::PCISPHSolverTEMP::Data::update_boundary() {
    for (int i = 0; i < Boundary.poses.size(); ++i)
    {
        std::vector<mVector3> res;
        const auto [start, end] = Boundary.boundary_sizes[i];
        const auto m = Boundary.poses[i]->get_model_matrix();

        for (size_t j = start; j < end; ++j)
            Boundary.positions[j] = (m * mVector4(Boundary.positions_origin[j].x(), Boundary.positions_origin[j].y(), Boundary.positions_origin[j].z(), 1)).xyz();
    }
}
void HinaPE::PCISPHSolverTEMP::Data::add_boundary(const std::vector<mVector3> &positions, const Kasumi::Pose *pose) {
    size_t start = Boundary.positions_origin.size();
    Boundary.positions_origin.insert(Boundary.positions_origin.end(), positions.begin(), positions.end());
    size_t end = Boundary.positions_origin.size();

    Boundary.poses.push_back(pose);
    Boundary.boundary_sizes.emplace_back(start, end);
    Boundary.positions.resize(Boundary.positions_origin.size());
    Boundary.pressure_forces.insert(Boundary.pressure_forces.end(), positions.size(), mVector3::Zero());
    Boundary.friction_forces.insert(Boundary.friction_forces.end(), positions.size(), mVector3::Zero());
    Boundary.forces.insert(Boundary.forces.end(), positions.size(), mVector3::Zero());
    BoundaryNeighborList.insert(BoundaryNeighborList.end(), positions.size(), std::vector<unsigned int>());

    ForceAndTorque.force.insert(ForceAndTorque.force.end(), (size_t)1, mVector3::Zero());
    ForceAndTorque.torque.insert(ForceAndTorque.torque.end(), (size_t)1, mVector3::Zero());
    update_boundary();
}
void HinaPE::PCISPHSolverTEMP::Data::reset() {
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

    Boundary.positions.clear();
    Boundary.positions_origin.clear();
    Boundary.mass.clear();
    Boundary.volume.clear();
    Boundary.density.clear();

    Boundary.forces.clear();
    Boundary.pressure_forces.clear();
    Boundary.friction_forces.clear();

    Boundary.poses.clear();
    Boundary.boundary_sizes.clear();

    ForceAndTorque.force.clear();
    ForceAndTorque.torque.clear();

    FluidNeighborList.clear();
    BoundaryNeighborList.clear();

    color_map.clear();
    debug_info.clear();
}




