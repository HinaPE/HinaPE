#ifndef HINAPE_CONSTRAINT_H
#define HINAPE_CONSTRAINT_H

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
public:
    virtual void solve() = 0;
};
using ConstraintPtr = std::shared_ptr<Constraint>;
class DistanceConstraint : public Constraint
{
public:
    struct Opt : public Constraint::Opt {} _opt;
    explicit DistanceConstraint(const DistanceConstraint::Opt &opt);
    void add_pair(int i, int j, float rest_length);
    void solve() override;

private:
    /// p1_id, p2_id, rest_length
    std::vector<std::tuple<int, int, double>> distance_constraints_;
};
using DistanceConstraintPtr = std::shared_ptr<DistanceConstraint>;
}

#endif //HINAPE_CONSTRAINT_H
