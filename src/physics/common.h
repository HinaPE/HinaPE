#ifndef HINAPE_COMMON_H
#define HINAPE_COMMON_H

#include "lib/vec3.h"
#include "lib/quat.h"
#include "../util/log.h"

#include "Eigen/Eigen"

using Vector3D = Eigen::Vector3d;

namespace HinaPE
{

enum PhysicsObjectType : int
{
    Rigidbody = 0, Deformable = 1, Fluid = 2,

    NOT_PHYSICS_OBJECT = -1
};

enum RigidBodyType : int
{
    DYNAMIC = 0, STATIC = 1, KINEMATIC = 2,

    NOT_RIGIDBODY = -1
};

enum DeformableType : int
{
    CLOTH = 0, MESH = 1,

    NOT_DEFORMABLE = -1
};

enum FluidType : int
{
    PARTICLE = 0, GRID = 1, HYBRID = 2,

    NOT_FLUID = -1
};

}

#endif //HINAPE_COMMON_H
