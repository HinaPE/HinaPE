#include "../rays/samplers.h"
#include "../util/rand.h"

namespace Samplers
{

Vec2 Rect::sample() const
{

    // TODO (PathTracer): Task 1

    // Generate a uniformly random point on a rectangle of size size.x * size.y
    // Tip: RNG::unit()
    Vec2 res = RNG::unit() * size;

    return res;
}

Vec3 Sphere::Uniform::sample() const
{

    // TODO (PathTracer): Task 7

    // Generate a uniformly random point on the unit sphere.
    // Tip: start with Hemisphere::Uniform
    Vec3 sample = hemi.sample();
    if (RNG::coin_flip(0.5f))
    {
        sample.y = -sample.y;
    }
    return sample;
}

Sphere::Image::Image(const HDR_Image &image)
{

    // TODO (PathTracer): Task 7

    // Set up importance sampling data structures for a spherical environment map image.
    // You may make use of the _pdf, _cdf, and total members, or create your own.

    const auto [_w, _h] = image.dimension();
    w = _w;
    h = _h;
    total = 0.0f;
    for (size_t i = 0; i < h; i++)
    {
        float v = (float) i / ((float) h);
        float theta = PI_F - v * PI_F;
        for (size_t j = 0; j < w; j++)
        {
            Spectrum curS = image.at(j, i);
            float curLuma = curS.luma() * std::sin(theta);
            total += curLuma;
        }
    }
    _cdf.clear();
    _pdf.clear();
    float sofar = 0.0f;
    for (size_t i = 0; i < h; i++)
    {
        float v = (float) i / ((float) h);
        float theta = PI_F - v * PI_F;
        for (size_t j = 0; j < w; j++)
        {
            float curLuma = image.at(j, i).luma() * std::sin(theta);
            float curProb = curLuma / total;
            sofar += curProb;
            _cdf.push_back(sofar);
            _pdf.push_back(curProb);
        }
    }

}

Vec3 Sphere::Image::sample() const
{

    // TODO (PathTracer): Task 7

    // Use your importance sampling data structure to generate a sample direction.
    // Tip: std::upper_bound
    float rng = RNG::unit();
    auto upper = std::upper_bound(_cdf.begin(), _cdf.end(), rng);
    // Get a (x,y) coordinate based on coordinate, and convert it
    // to phi/theta and convert that to (x,y,z)
    Vec3 res;
    size_t idx = std::distance(_cdf.begin(), upper) - 1;
    size_t x = idx % w;
    size_t y = idx / w;
    float u = (float) x / ((float) w);
    float v = (float) y / ((float) h);
    float phi = u * 2.0f * PI_F;
    float theta = PI_F - v * PI_F;
    res.y = std::cos(theta);
    res.x = std::sin(theta) * std::cos(phi);
    res.z = std::sin(theta) * std::sin(phi);
    return res;
}

float Sphere::Image::pdf(Vec3 dir) const
{

    // TODO (PathTracer): Task 7

    // What is the PDF of this distribution at a particular direction?
    float phi = std::atan2(dir.z, dir.x);
    float theta = acos(dir.y);
    float u = phi / (2.0f * PI_F);
    if (u < 0.0f)
        u += 1.0f;
    float v = 1.0f - theta / PI_F;
    int i = (int) floor(u * w);
    int j = (int) floor(v * h);
    float jacob = (float) (w * h) / (2.0f * PI_F * PI_F * std::sin(theta));
    return _pdf.at(i + j * w) * jacob;
}

Vec3 Point::sample() const
{
    return point;
}

Vec3 Triangle::sample() const
{
    float u = std::sqrt(RNG::unit());
    float v = RNG::unit();
    float a = u * (1.0f - v);
    float b = u * v;
    return a * v0 + b * v1 + (1.0f - a - b) * v2;
}

Vec3 Hemisphere::Uniform::sample() const
{

    float Xi1 = RNG::unit();
    float Xi2 = RNG::unit();

    float theta = std::acos(Xi1);
    float phi = 2.0f * PI_F * Xi2;

    float xs = std::sin(theta) * std::cos(phi);
    float ys = std::cos(theta);
    float zs = std::sin(theta) * std::sin(phi);

    return Vec3(xs, ys, zs);
}

Vec3 Hemisphere::Cosine::sample() const
{

    float phi = RNG::unit() * 2.0f * PI_F;
    float cos_t = std::sqrt(RNG::unit());

    float sin_t = std::sqrt(1 - cos_t * cos_t);
    float x = std::cos(phi) * sin_t;
    float z = std::sin(phi) * sin_t;
    float y = cos_t;

    return Vec3(x, y, z);
}

} // namespace Samplers
