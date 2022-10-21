#ifndef HINAPE_FLUID_ENGINE_VECTOR_FIELD3_H_
#define HINAPE_FLUID_ENGINE_VECTOR_FIELD3_H_

#include "field3.h"

#include "math/vector3.h"

#include <functional>
#include <memory>

namespace HinaPE::FluidEngine
{

//! Abstract base class for 3-D vector field.
class VectorField3 : public Field3
{
public:
    //! Default constructor.
    VectorField3();

    //! Default destructor.
    virtual ~VectorField3();

    //! Returns sampled value at given position \p x.
    virtual Vector3D sample(const Vector3D &x) const = 0;

    //! Returns divergence at given position \p x.
    virtual double divergence(const Vector3D &x) const;

    //! Returns curl at given position \p x.
    virtual Vector3D curl(const Vector3D &x) const;

    //! Returns sampler function object.
    virtual std::function<Vector3D(const Vector3D &)> sampler() const;
};

//! Shared pointer for the VectorField3 type.
typedef std::shared_ptr<VectorField3> VectorField3Ptr;

}  // namespace HinaPE::FluidEngine

#endif  // HINAPE_FLUID_ENGINE_VECTOR_FIELD3_H_
