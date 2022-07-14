#ifndef HINAPE_COMMON_H
#define HINAPE_COMMON_H

#include "lib/vec3.h"
#include "lib/quat.h"

#if defined(WIN32) || defined(_WIN32) || defined(WIN64)
#define HINA_FORCE_INLINE __forceinline
#else
#define HINA_FORCE_INLINE __attribute__((always_inline))
#endif

namespace HinaPE
{

enum PhysicsObjectType : int
{
    Rigidbody = 0,
    Deformable = 1,
    Fluid = 2,

    NOT_PHYSICS_OBJECT = -1
};

enum RigidBodyType : int
{
    DYNAMIC = 0,
    STATIC = 1,
    KINEMATIC = 2,

    NOT_RIGIDBODY = -1
};

}

#endif //HINAPE_COMMON_H
