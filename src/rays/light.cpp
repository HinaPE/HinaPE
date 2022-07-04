
#include "light.h"

namespace PT {

Light_Sample Directional_Light::sample(Vec3) const {
    Light_Sample ret;
    ret.direction = Vec3(0.0f, -1.0f, 0.0f);
    ret.distance = std::numeric_limits<float>::infinity();
    ret.radiance = radiance;
    return ret;
}

Light_Sample Point_Light::sample(Vec3 from) const {
    Light_Sample ret;
    ret.direction = -from.unit();
    ret.distance = from.norm();
    ret.radiance = radiance;
    return ret;
}

Light_Sample Spot_Light::sample(Vec3 from) const {
    Light_Sample ret;
    float angle = std::atan2(Vec2(from.x, from.z).norm(), from.y);
    angle = std::abs(Degrees(angle));
    ret.direction = -from.unit();
    ret.distance = from.norm();
    ret.radiance =
        (1.0f - smoothstep(angle_bounds.x / 2.0f, angle_bounds.y / 2.0f, angle)) * radiance;
    return ret;
}

} // namespace PT
