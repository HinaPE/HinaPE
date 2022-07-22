#ifndef HINAPE_PHYSICS_OBJECTS_FACTORY_H
#define HINAPE_PHYSICS_OBJECTS_FACTORY_H

#include "../physics_object.h"

#include <memory>

namespace HinaPE
{
typedef std::shared_ptr<PhysicsObject> PhysicsObjectPtr;

class RigidBodyFactory
{
public:
    static PhysicsObjectPtr create_rigidbody(RigidBodyType type);
};

class ClothFactory
{
public:
    struct ClothDesc
    {
        Vec3 position;
        Vec3 rotation;

        int width;
        int height;
        int row;
        int col;
        float mass;
        float stiffness;
    };

public:
    static std::shared_ptr<PhysicsObject> create_cloth(const ClothDesc &desc);
};
}


#endif //HINAPE_PHYSICS_OBJECTS_FACTORY_H
