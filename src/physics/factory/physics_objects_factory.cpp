#include "physics_objects_factory.h"

HinaPE::ClothFactory::ClothPtr HinaPE::ClothFactory::create_cloth(const HinaPE::ClothFactory::ClothDesc &desc)
{
    auto cloth = std::make_shared<DeformableBase<CLOTH>>();

    return cloth;
}
