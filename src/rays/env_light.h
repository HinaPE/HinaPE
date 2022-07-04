
#pragma once

#include <variant>

#include "../lib/mathlib.h"
#include "../lib/spectrum.h"
#include "../util/hdr_image.h"

#include "light.h"
#include "samplers.h"

namespace PT {

struct Env_Hemisphere {

    Env_Hemisphere(Spectrum r) : radiance(r) {
    }

    Vec3 sample() const;
    Spectrum evaluate(Vec3 dir) const;
    float pdf(Vec3 dir) const;

    Spectrum radiance;
    Samplers::Hemisphere::Uniform sampler;
};

struct Env_Sphere {

    Env_Sphere(Spectrum r) : radiance(r) {
    }

    Vec3 sample() const;
    Spectrum evaluate(Vec3 dir) const;
    float pdf(Vec3 dir) const;

    Spectrum radiance;
    Samplers::Sphere::Uniform sampler;
};

struct Env_Map {

    Env_Map(HDR_Image&& img) : image(std::move(img)), image_sampler(image) {
    }

    Vec3 sample() const;
    Spectrum evaluate(Vec3 dir) const;
    float pdf(Vec3 dir) const;

    HDR_Image image;
    Samplers::Sphere::Uniform uniform_sampler;
    Samplers::Sphere::Image image_sampler;
};

class Env_Light {
public:
    Env_Light(Env_Hemisphere&& l) : underlying(std::move(l)) {
    }
    Env_Light(Env_Sphere&& l) : underlying(std::move(l)) {
    }
    Env_Light(Env_Map&& l) : underlying(std::move(l)) {
    }

    Env_Light(const Env_Light& src) = delete;
    Env_Light& operator=(const Env_Light& src) = delete;
    Env_Light& operator=(Env_Light&& src) = default;
    Env_Light(Env_Light&& src) = default;

    Vec3 sample() const {
        return std::visit([](const auto& h) { return h.sample(); }, underlying);
    }

    float pdf(Vec3 dir) const {
        return std::visit([dir](const auto& h) { return h.pdf(dir); }, underlying);
    }

    Spectrum evaluate(Vec3 dir) const {
        return std::visit([&dir](const auto& h) { return h.evaluate(dir); }, underlying);
    }

    bool is_discrete() const {
        return false;
    }

private:
    std::variant<Env_Hemisphere, Env_Sphere, Env_Map> underlying;
};

} // namespace PT