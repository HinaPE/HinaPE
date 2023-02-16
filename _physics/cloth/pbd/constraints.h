#ifndef HINAPE_CONSTRAINTS_H
#define HINAPE_CONSTRAINTS_H

#include "common.h"

namespace HinaPE
{
class Constraint
{
	virtual void solve() = 0;
};
using ConstraintPtr = std::shared_ptr<Constraint>;

class DistanceConstraint : public Constraint
{
public:
	struct Opt
	{
		real stiffness = 1.f;
	} _opt;
	explicit DistanceConstraint(std::vector<mVector3> &vertices, std::vector<real> &inv_masses, const std::vector<std::pair<unsigned int, unsigned int>> &edges);
	void solve() override;

private:
	/// p1_id, p2_id, rest_length
	std::vector<std::tuple<int, int, float>> _pairs;
	std::vector<mVector3> &_vertices;
	std::vector<real> &_inv_masses;
};
using DistanceConstraintPtr = std::shared_ptr<DistanceConstraint>;
} // namespace HinaPE

#endif //HINAPE_CONSTRAINTS_H
