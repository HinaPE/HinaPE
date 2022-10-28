#include "vector_field2.h"

#include "pch.h"

using namespace HinaPE;

VectorField2::VectorField2()
{
}

VectorField2::~VectorField2()
{
}

double VectorField2::divergence(const Vector2D &) const
{
    return 0.0;
}

double VectorField2::curl(const Vector2D &) const
{
    return 0.0;
}

std::function<Vector2D(const Vector2D &)> VectorField2::sampler() const
{
    const VectorField2 *self = this;
    return [self](const Vector2D &x) -> Vector2D
    {
        return self->sample(x);
    };
}
