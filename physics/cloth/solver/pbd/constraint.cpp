#include "constraint.h"
HinaPE::Cloth::Constraint::Constraint(const HinaPE::Cloth::Constraint::Opt &opt) : _opt(opt) {}
void HinaPE::Cloth::DistanceConstraint::add_pair(int i, int j, float rest_length)
{

}
void HinaPE::Cloth::DistanceConstraint::solve() {}
HinaPE::Cloth::DistanceConstraint::DistanceConstraint(const HinaPE::Cloth::DistanceConstraint::Opt &opt) : Constraint(opt), _opt(opt) {}
