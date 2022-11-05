#include <pch.h>
#include "advection_solver3.h"
#include <limits>

using namespace HinaPE::FluidEngine;

AdvectionSolver3::AdvectionSolver3() {
}

AdvectionSolver3::~AdvectionSolver3() {
}

void AdvectionSolver3::advect(
        const CollocatedVectorGrid3& source,
        const VectorField3& flow,
        double dt,
        CollocatedVectorGrid3* target,
        const ScalarField3& boundarySdf) {
    UNUSED_VARIABLE(source);
    UNUSED_VARIABLE(flow);
    UNUSED_VARIABLE(dt);
    UNUSED_VARIABLE(target);
    UNUSED_VARIABLE(boundarySdf);
}
