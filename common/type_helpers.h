#ifndef HINAPE_FLUID_ENGINE_TYPE_HELPERS_H_
#define HINAPE_FLUID_ENGINE_TYPE_HELPERS_H_

namespace HinaPE
{

//! Returns the type of the value itself.
template<typename T>
struct ScalarType
{
    typedef T value;
};

}  // namespace HinaPE

#endif  // HINAPE_FLUID_ENGINE_TYPE_HELPERS_H_
