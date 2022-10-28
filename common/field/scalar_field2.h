#ifndef HINAPE_FLUID_ENGINE_SCALAR_FIELD2_H_
#define HINAPE_FLUID_ENGINE_SCALAR_FIELD2_H_

#include "field2.h"

#include "math/vector2.h"

#include <functional>
#include <memory>

namespace HinaPE
{

//! Abstract base class for 2-D scalar field.
class ScalarField2 : public Field2
{
public:
    //! Default constructor.
    ScalarField2();

    //! Default destructor.
    ~ScalarField2() override;

    //! Returns sampled value at given position \p x.
    virtual auto sample(const Vector2D &x) const -> double = 0;

    //! Returns gradient vector at given position \p x.
    virtual auto gradient(const Vector2D &x) const -> Vector2D;

    //! Returns Laplacian at given position \p x.
    virtual auto laplacian(const Vector2D &x) const -> double;

    //! Returns sampler function object.
    virtual auto sampler() const -> std::function<double(const Vector2D &)>;
};

//! Shared pointer for the ScalarField2 type.
using ScalarField2Ptr = std::shared_ptr<ScalarField2>;

}  // namespace HinaPE

#endif  // HINAPE_FLUID_ENGINE_SCALAR_FIELD2_H_
