#ifndef HINAPE_PHYSICS_OBJECTS_FACTORY_H
#define HINAPE_PHYSICS_OBJECTS_FACTORY_H

#include "../physics_object.h"

#include <memory>

namespace HinaPE
{
using PhysicsObjectPtr = std::shared_ptr<PhysicsObject>;

class RigidBodyFactory
{
public:
    static auto create_rigidbody(RigidBodyType type) -> PhysicsObjectPtr;
};

class ClothFactory
{
public:
    struct ClothDesc
    {
        Vec3 position;
        Vec3 rotation;

        float width;
        float height;
        int row;
        int col;
        float mass;
        float stiffness;
    };

public:
    static auto create_cloth(const ClothDesc &desc) -> std::shared_ptr<PhysicsObject>;
};

class SphFluidFactory
{
public:
    struct SphFluidDesc
    {
    };
};
}

#endif //HINAPE_PHYSICS_OBJECTS_FACTORY_H
