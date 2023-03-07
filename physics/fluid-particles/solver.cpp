#include "solver.h"
void HinaPE::SPHSolver::INSPECT()
{
}
void HinaPE::SPHSolver::VALID_CHECK() const
{
	if (_domain == nullptr) throw std::runtime_error("SPHSolver::_domain is nullptr");
}
void HinaPE::SPHSolver::update(real dt)
{
}
