#ifndef HINAPE_TIME_INTEGRATION_H
#define HINAPE_TIME_INTEGRATION_H

#include "lib/vec3.h"
#include "lib/quat.h"
#include "lib/mat3.h"

class TimeIntegration
{
    using real = float;
public:
    static void semi_implicit_euler(real h, real mass, Vec3 &position, Vec3 &velocity, const Vec3 &acceleration);
};


#endif //HINAPE_TIME_INTEGRATION_H
