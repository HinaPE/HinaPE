#include "time_integration.h"

void TimeIntegration::semi_implicit_euler(const TimeIntegration::real h, const TimeIntegration::real mass, Vec3 &position, Vec3 &velocity, const Vec3 &acceleration)
{
    if (mass != 0.0)
    {
        velocity += acceleration * h;
        position += velocity * h;
    }
}
