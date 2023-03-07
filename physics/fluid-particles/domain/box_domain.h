#ifndef HINAPE_BOX_DOMAIN_H
#define HINAPE_BOX_DOMAIN_H

#include "backends/objects/cube.h"

namespace HinaPE
{
class BoxDomain : public Kasumi::CubeObject
{
};
using BoxDomainPtr = std::shared_ptr<BoxDomain>;
} // namespace HinaPE


#endif //HINAPE_BOX_DOMAIN_H
