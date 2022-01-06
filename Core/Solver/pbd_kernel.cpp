#include "pbd_kernel.h"

using namespace HinaPE;

bool PBDKernel::SolveDistanceConstraint(const HVector3r &p0, Real inv_mass0, const HVector3r &p1, Real inv_mass1, const Real rest_length, const Real compression_stiffness, const Real stretch_stiffness, HVector3r &corr0, HVector3r &corr1)
{
    Real w_sum = inv_mass0 + inv_mass1;
    if (w_sum == 0.0)
        return false;

    HVector3r n = p1 - p0;
    Real d = n.norm();
    n.normalize();

    HVector3r corr;
    if (d < rest_length)
        corr = 1 * compression_stiffness * n * (d - rest_length) / w_sum;
    else
        corr = 1 * stretch_stiffness * n * (d - rest_length) / w_sum;

    corr0 = inv_mass0 * corr;
    corr1 = -inv_mass1 * corr;
    return true;
}
