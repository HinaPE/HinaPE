
#pragma once

#include <variant>

#include "../lib/mathlib.h"
#include "../lib/spectrum.h"
#include "../util/hdr_image.h"

#include "samplers.h"

namespace PT {

struct Scatter {

    Spectrum attenuation;
    Vec3 direction;

    void transform(const Mat4& T) {
        direction = T.rotate(direction);
    }
};

struct BSDF_Lambertian {

    BSDF_Lambertian(Spectrum albedo) : albedo(albedo / PI_F) {
    }

    Scatter scatter(Vec3 out_dir) const;
    Spectrum evaluate(Vec3 out_dir, Vec3 in_dir) const;
    float pdf(Vec3 out_Dir, Vec3 in_dir) const;

    Spectrum albedo;
    Samplers::Hemisphere::Cosine sampler;
};

struct BSDF_Mirror {

    BSDF_Mirror(Spectrum reflectance) : reflectance(reflectance) {
    }

    Scatter scatter(Vec3 out_dir) const;

    Spectrum reflectance;
};

struct BSDF_Refract {

    BSDF_Refract(Spectrum transmittance, float ior)
        : transmittance(transmittance), index_of_refraction(ior) {
    }

    Scatter scatter(Vec3 out_dir) const;

    Spectrum transmittance;
    float index_of_refraction;
};

struct BSDF_Glass {

    BSDF_Glass(Spectrum transmittance, Spectrum reflectance, float ior)
        : transmittance(transmittance), reflectance(reflectance), index_of_refraction(ior) {
    }

    Scatter scatter(Vec3 out_dir) const;

    Spectrum transmittance;
    Spectrum reflectance;
    float index_of_refraction;
};

struct BSDF_Diffuse {

    BSDF_Diffuse(Spectrum radiance) : radiance(radiance) {
    }

    Spectrum emissive() const;

    Spectrum radiance;
};

class BSDF {
public:
    BSDF(BSDF_Lambertian&& b) : underlying(std::move(b)) {
    }
    BSDF(BSDF_Mirror&& b) : underlying(std::move(b)) {
    }
    BSDF(BSDF_Glass&& b) : underlying(std::move(b)) {
    }
    BSDF(BSDF_Diffuse&& b) : underlying(std::move(b)) {
    }
    BSDF(BSDF_Refract&& b) : underlying(std::move(b)) {
    }

    BSDF(const BSDF& src) = delete;
    BSDF& operator=(const BSDF& src) = delete;
    BSDF& operator=(BSDF&& src) = default;
    BSDF(BSDF&& src) = default;

    Scatter scatter(Vec3 out_dir) const {
        return std::visit(overloaded{[](const BSDF_Diffuse& d) -> Scatter {
                                         die("You scattered an emissive BSDF!");
                                     },
                                     [out_dir](const auto& b) { return b.scatter(out_dir); }},
                          underlying);
    }

    Spectrum evaluate(Vec3 out_dir, Vec3 in_dir) const {
        return std::visit(
            overloaded{
                [out_dir, in_dir](const BSDF_Lambertian& l) { return l.evaluate(out_dir, in_dir); },
                [](const auto&) -> Spectrum { die("You evaluated a delta BSDF!"); }},
            underlying);
    }

    float pdf(Vec3 out_dir, Vec3 in_dir) const {
        return std::visit(
            overloaded{
                [out_dir, in_dir](const BSDF_Lambertian& l) { return l.pdf(out_dir, in_dir); },
                [](const auto&) -> float { die("You evaluated the pdf of a delta BSDF!"); }},
            underlying);
    }

    Spectrum emissive() const {
        return std::visit(overloaded{[](const BSDF_Diffuse& d) { return d.emissive(); },
                                     [](const auto& b) { return Spectrum{}; }},
                          underlying);
    }

    bool is_discrete() const {
        return std::visit(overloaded{[](const BSDF_Lambertian&) { return false; },
                                     [](const BSDF_Diffuse&) { return false; },
                                     [](const BSDF_Mirror&) { return true; },
                                     [](const BSDF_Glass&) { return true; },
                                     [](const BSDF_Refract&) { return true; }},
                          underlying);
    }

    bool is_sided() const {
        return std::visit(overloaded{[](const BSDF_Lambertian&) { return false; },
                                     [](const BSDF_Mirror&) { return false; },
                                     [](const BSDF_Glass&) { return true; },
                                     [](const BSDF_Diffuse&) { return false; },
                                     [](const BSDF_Refract&) { return true; }},
                          underlying);
    }

private:
    std::variant<BSDF_Lambertian, BSDF_Mirror, BSDF_Glass, BSDF_Diffuse, BSDF_Refract> underlying;
};

} // namespace PT
