#include "../rays/bsdf.h"
#include "../util/rand.h"

namespace PT
{

static Vec3 reflect(Vec3 dir)
{

    // TODO (PathTracer): Task 5
    // Return reflection of dir about the surface normal (0,1,0).
    Vec3 out = dir;
    out.z = -dir.z;
    out.x = -dir.x;
    return out;
}

static Vec3 refract(Vec3 out_dir, float index_of_refraction, bool &was_internal)
{

    // TODO (PathTracer): Task 5
    // Use Snell's Law to refract out_dir through the surface.
    // Return the refracted direction. Set was_internal to true if
    // refraction does not occur due to total internal reflection,
    // and false otherwise.

    // When dot(out_dir,normal=(0,1,0)) is positive, then out_dir corresponds to a
    // ray exiting the surface into vaccum (ior = 1). However, note that
    // you should actually treat this case as _entering_ the surface, because
    // you want to compute the 'input' direction that would cause this output,
    // and to do so you can simply find the direction that out_dir would refract
    // _to_, as refraction is symmetric.
    Vec3 res;
    float costsq = 1.0f - index_of_refraction * index_of_refraction * (1.0f - out_dir.unit().y * out_dir.unit().y);
    if (costsq < 0.0f)
    {
        was_internal = true;
        return Vec3{};
    }
    float cost = sqrt(costsq);
    res.x = -out_dir.x * index_of_refraction;
    res.z = -out_dir.z * index_of_refraction;
    res.y = out_dir.y < 0.0f ? cost : -cost;
    res.normalize();
    return res;
}

Scatter BSDF_Lambertian::scatter(Vec3 out_dir) const
{

    // TODO (PathTracer): Task 4

    // Sample the BSDF distribution using the cosine-weighted hemisphere sampler.
    // You can use BSDF_Lambertian::evaluate() to compute attenuation.

    Scatter ret;
    ret.direction = sampler.sample();
    ret.attenuation = evaluate(out_dir, ret.direction);
    return ret;
}

Spectrum BSDF_Lambertian::evaluate(Vec3 out_dir, Vec3 in_dir) const
{

    // TODO (PathTracer): Task 4

    // Compute the ratio of reflected/incoming radiance when light from in_dir
    // is reflected through out_dir: albedo * cos(theta).
    float cosTheta = std::max<float>(0.0f, in_dir.unit().y);
    return albedo * cosTheta;
}

float BSDF_Lambertian::pdf(Vec3 out_dir, Vec3 in_dir) const
{

    // TODO (PathTracer): Task 4

    // Compute the PDF for sampling in_dir from the cosine-weighted hemisphere distribution.
    return std::max<float>(0.0f, in_dir.unit().y / PI_F);
}

Scatter BSDF_Mirror::scatter(Vec3 out_dir) const
{

    // TODO (PathTracer): Task 5

    Scatter ret;
    ret.direction = reflect(out_dir);
    ret.attenuation = reflectance;
    return ret;
}

Scatter BSDF_Glass::scatter(Vec3 out_dir) const
{

    // TODO (PathTracer): Task 5

    // (1) Compute Fresnel coefficient. Tip: Schlick's approximation.
    // (2) Reflect or refract probabilistically based on Fresnel coefficient. Tip: RNG::coin_flip
    // (3) Compute attenuation based on reflectance or transmittance

    // Be wary of your eta1/eta2 ratio - are you entering or leaving the surface?
    // What happens upon total internal reflection?
    float ior = out_dir.unit().y < 0.0f ? index_of_refraction : 1.0f / index_of_refraction;
    bool was_internal = false;
    Scatter ret;
    ret.direction = refract(out_dir, ior, was_internal);
    if (was_internal)
    {
        ret.direction = reflect(out_dir);
        ret.attenuation = reflectance;
        return ret;
    }
    float R0 = (1.0f - ior) * (1.0f - ior) / ((1.0f + ior) * (1.0f + ior));
    float Schlick = R0 + (1.0f - R0) * powf((1.0f - abs(out_dir.unit().y)), 5.0f);
    if (RNG::coin_flip(Schlick))
    {
        ret.direction = reflect(out_dir);
        ret.attenuation = reflectance;
        return ret;
    }
    //scale by ior^2 for transmittence for refract
    ret.attenuation = transmittance * ior * ior;
    return ret;
}

Scatter BSDF_Refract::scatter(Vec3 out_dir) const
{

    // OPTIONAL (PathTracer): Task 5

    // When debugging BSDF_Glass, it may be useful to compare to a pure-refraction BSDF

    Scatter ret;
    ret.direction = Vec3();
    ret.attenuation = Spectrum{};
    return ret;
}

Spectrum BSDF_Diffuse::emissive() const
{
    return radiance;
}

} // namespace PT
