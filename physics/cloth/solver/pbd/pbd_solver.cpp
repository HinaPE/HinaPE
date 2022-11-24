#include "pbd_solver.h"

HinaPE::Cloth::PBDSolver::PBDSolver(const HinaPE::Cloth::PBDSolver::Opt &opt) : ClothSolver(opt), _opt(opt) { setup_pbd(); }
void HinaPE::Cloth::PBDSolver::step(real dt) { ClothSolver::step(dt); }
void HinaPE::Cloth::PBDSolver::setup_pbd()
{
    if (_opt.distance_constraint)
    {
        DistanceConstraint::Opt opt;
        auto dc = std::make_shared<DistanceConstraint>(opt);
        for (int i = 0; i < _cloth_data->_init_indices.size(); ++i)
        {

        }
        _constraints.emplace_back(dc);
    }
}
