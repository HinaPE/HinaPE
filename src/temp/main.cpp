#include <iostream>
#include "math_lib/bounding_box3.h"
#include "sph/sph_solver3.h"
using namespace jet;

int main()
{
    BoundingBox3D domain(Vector3D(), Vector3D(1, 2, 1));

    auto solver = SphSolver3::builder()
            .withTargetDensity(1000.0)
            .withTargetSpacing(0.02)
            .makeShared();

    BoundingBox3D sourceBound(domain);


    return 0;
}
