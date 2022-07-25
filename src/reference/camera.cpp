#include "../util/camera.h"
#include "../rays/samplers.h"
#include "debug.h"

Ray Camera::generate_ray(Vec2 screen_coord) const
{

    // TODO (PathTracer): Task 1
    // compute position of the input sensor sample coordinate on the
    // canonical sensor plane one unit away from the pinhole.
    // Tip: compute the ray direction in view space and use
    // the camera transform to transform it back into world space.
    screen_coord = screen_coord * 2 - Vec2(1.0f, 1.0f);
    float h = 2 * std::tan(Radians(vert_fov / 2));
    float w = h * aspect_ratio;
    Vec3 cam2plane;
    cam2plane.z = -1;
    cam2plane.x = screen_coord.x * w / 2;
    cam2plane.y = screen_coord.y * h / 2;
    Vec3 homo = iview.rotate(cam2plane);
    Ray ray = Ray(pos(), homo);

    return ray;
}
