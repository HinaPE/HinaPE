#ifndef HINAPE_CONSTRAINTS_H
#define HINAPE_CONSTRAINTS_H

namespace HinaPE
{
struct Constraint
{
    virtual void solve() = 0;
};

struct DistanceConstraint : public Constraint
{
    void solve() override;
};
}

#endif //HINAPE_CONSTRAINTS_H
