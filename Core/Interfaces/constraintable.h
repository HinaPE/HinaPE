#ifndef HINAPE_CONSTRAINTABLE_H
#define HINAPE_CONSTRAINTABLE_H

#include "Core/Solver/constraints.h"

#include <vector>

class Constraintable
{
protected:

    /// Init Constraints
    virtual void init_inner_constraints() = 0;

public:

    /// Constraints Vector
    std::vector<std::unique_ptr<HinaPE::Constraint>> constraints_;

};

#endif //HINAPE_CONSTRAINTABLE_H
