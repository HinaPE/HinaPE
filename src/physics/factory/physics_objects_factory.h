#ifndef HINAPE_PHYSICS_OBJECTS_FACTORY_H
#define HINAPE_PHYSICS_OBJECTS_FACTORY_H

#include "../physics_object.h"

#include <memory>

namespace HinaPE
{
class ClothFactory
{
public:
    struct ClothDesc
    {
        int width;
        int height;
        int row;
        int col;
        float mass;
        float stiffness;
    };

public:
    typedef std::shared_ptr<DeformableBase<CLOTH>> ClothPtr;
    static ClothPtr create_cloth(const ClothDesc &desc);
};
}


#endif //HINAPE_PHYSICS_OBJECTS_FACTORY_H
