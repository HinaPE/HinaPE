#include <iostream>
#include "math_lib/bounding_box3.h"
using namespace jet;

int main()
{
    BoundingBox3D domain(Vector3D(), Vector3D(1, 2, 1));



    BoundingBox3D sourceBound(domain);


    return 0;
}
