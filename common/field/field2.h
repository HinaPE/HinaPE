#ifndef HINAPE_FLUID_ENGINE_FIELD2_H_
#define HINAPE_FLUID_ENGINE_FIELD2_H_

#include <memory>

namespace HinaPE
{

//! Abstract base class for 2-D fields.
class Field2
{
public:
    Field2();

    virtual ~Field2();
};

typedef std::shared_ptr<Field2> Field2Ptr;

}  // namespace HinaPE

#endif  // HINAPE_FLUID_ENGINE_FIELD2_H_
