#ifndef HINAPE_CONSTRAINT_H
#define HINAPE_CONSTRAINT_H

#include "../../../../math_api.h"

#include <vector>
#include <tuple>
#include <memory>

namespace HinaPE::Cloth
{
class Constraint
{
public:
    struct Opt {} _opt;
    explicit Constraint(const Opt &opt);
    virtual void solve() = 0;
};
using ConstraintPtr = std::shared_ptr<Constraint>;

class DistanceConstraint : public Constraint
{
public:
    struct Opt : public Constraint::Opt
    {
        real stiffness = 1.f;
    } _opt;
    explicit DistanceConstraint(const DistanceConstraint::Opt &opt, std::vector<mVector3> &vertices, std::vector<real> &inv_masses, const std::vector<std::pair<unsigned int, unsigned int>> &edges);
    void solve() override;

private:
    /// p1_id, p2_id, rest_length
    std::vector<std::tuple<int, int, float>> _pairs;
    std::vector<mVector3> &_vertices;
    std::vector<real> &_inv_masses;
};
using DistanceConstraintPtr = std::shared_ptr<DistanceConstraint>;
}

#endif //HINAPE_CONSTRAINT_H
