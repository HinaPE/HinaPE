#ifndef HINAPE_PBD_KERNEL_H
#define HINAPE_PBD_KERNEL_H

#include "Core/Interfaces/define.h"

namespace HinaPE
{
    class PBDKernel
    {
    public:

        /// Solve Distance Constraint
        static bool SolveDistanceConstraint(const HVector3r &p0, Real inv_mass0,
                                            const HVector3r &p1, Real inv_mass1,
                                            Real rest_length, Real compression_stiffness, Real stretch_stiffness,
                                            HVector3r &corr0, HVector3r &corr1);
    };
}


#endif //HINAPE_PBD_KERNEL_H
