#ifndef HINAPE_CONSTRAINTS_H
#define HINAPE_CONSTRAINTS_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "common.h"

namespace HinaPE
{
class Constraint
{
public:
	virtual void solve() = 0;
};
using ConstraintPtr = std::shared_ptr<Constraint>;

class DistanceConstraint : public Constraint
{
public:
	struct Opt
	{
		real stiffness = 0.8f;
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
