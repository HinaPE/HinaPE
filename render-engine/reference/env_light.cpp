#include "../rays/env_light.h"

#include <limits>

namespace PT
{

Vec3 Env_Map::sample() const
{

    // TODO (PathTracer): Task 7

    // First, implement Samplers::Sphere::Uniform so the following line works.
    // Second, implement Samplers::Sphere::Image and swap to image_sampler

    return image_sampler.sample();
}

float Env_Map::pdf(Vec3 dir) const
{

    // TODO (PathTracer): Task 7

    // First, return the pdf for a uniform spherical distribution.
    // Second, swap to image_sampler.pdf().
    return image_sampler.pdf(dir);
}

Spectrum Env_Map::evaluate(Vec3 dir) const
{

    // TODO (PathTracer): Task 7

    // Compute emitted radiance along a given direction by finding the corresponding
    // pixels in the enviornment image. You should bi-linearly interpolate the value
    // between the 4 nearest pixels.
    std::pair<size_t, size_t> dim = image.dimension();
    int w = (int) std::get<0>(dim);
    int h = (int) std::get<1>(dim);
    float phi = std::atan2(dir.z, dir.x);
    float theta = acos(dir.y);
    float u = phi / (2.0f * PI_F);
    if (u < 0.0f)
        u += 1.0f;
    float v = 1.0f - theta / PI_F;
    int i = (int) floor(u * (float) w - 0.5f);
    int j = (int) floor(v * (float) h - 0.5f);
    float s = u * w - ((float) i + 0.5f);
    float t = v * h - ((float) j + 0.5f);
    int f00x = std::max<int>(0, i);
    int f00y = std::max<int>(0, j);
    Spectrum f00S = image.at(f00x, f00y);
    int f10x = std::min<int>(w - 1, f00x + 1);
    Spectrum f10S = image.at(f10x, f00y);
    int f01y = std::min<int>(h - 1, f00y);
    Spectrum f01S = image.at(f00x, f01y);
    Spectrum f11S = image.at(f10x, f01y);
    Spectrum result = (1.0f - t) * ((1.0f - s) * f00S + s * f10S) + t * ((1.0f - s) * f01S + s * f11S);
    return result;
}

Vec3 Env_Hemisphere::sample() const
{
    return sampler.sample();
}

float Env_Hemisphere::pdf(Vec3 dir) const
{
    return 1.0f / (2.0f * PI_F);
}

Spectrum Env_Hemisphere::evaluate(Vec3 dir) const
{
    if (dir.y > 0.0f)
        return radiance;
    return {};
}

Vec3 Env_Sphere::sample() const
{
    return sampler.sample();
}

float Env_Sphere::pdf(Vec3 dir) const
{
    return 1.0f / (4.0f * PI_F);
}

Spectrum Env_Sphere::evaluate(Vec3) const
{
    return radiance;
}

} // namespace PT
