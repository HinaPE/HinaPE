#include "pbd_solver.h"

HinaPE::Cloth::PBDSolver::PBDSolver(const HinaPE::Cloth::PBDSolver::Opt &opt) : ClothSolver(opt), _opt(opt) { setup(); }