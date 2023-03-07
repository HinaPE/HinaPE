#ifndef HINAPE_BOX_DOMAIN_H
#define HINAPE_BOX_DOMAIN_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "backends/objects/cube.h"

namespace HinaPE
{
class BoxDomain : public Kasumi::CubeObject
{
public:
	BoxDomain();
};
using BoxDomainPtr = std::shared_ptr<BoxDomain>;
} // namespace HinaPE

#endif //HINAPE_BOX_DOMAIN_H
