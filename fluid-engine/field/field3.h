#ifndef HINAPE_FLUID_ENGINE_FIELD3_H_
#define HINAPE_FLUID_ENGINE_FIELD3_H_

#include <memory>

namespace HinaPE::FluidEngine
{

//! Abstract base class for 3-D fields.
class Field3
{
public:
    Field3();

    virtual ~Field3();
};

typedef std::shared_ptr<Field3> Field3Ptr;

}  // namespace HinaPE::FluidEngine

#endif  // HINAPE_FLUID_ENGINE_FIELD3_H_
