#ifndef HINAPE_FLUID_DOMAIN_H
#define HINAPE_FLUID_DOMAIN_H

#include "renderer3D/objects/cube.h"

namespace HinaPE
{
class FluidDomain3 : public Kasumi::CubeObject
{
public:

public:
	struct Opt {};
	FluidDomain3();
};
using FluidDomain3Ptr = std::shared_ptr<FluidDomain3>;
} // namespace HinaPE

#endif //HINAPE_FLUID_DOMAIN_H
