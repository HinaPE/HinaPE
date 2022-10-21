#ifndef HINAPE_PARTICLES_H
#define HINAPE_PARTICLES_H

#include "solver/particle/sph/sph_solver3.h"

#include <vector>

#include "../lib/mathlib.h"
#include "../platform/gl.h"
#include "../util/rand.h"

#include "object.h"
#include "pose.h"

namespace PT
{
template<typename T>
class BVH;
class Object;
} // namespace PT

class Scene_Particles
{
public:
    struct Particle
    {
        Vec3 pos;
        Vec3 velocity;
        float age;
        Spectrum color = Spectrum(RNG::unit(), RNG::unit(), RNG::unit());

        static const inline Vec3 acceleration = Vec3{0.0f, -9.8f, 0.0f};

        auto update(const PT::Object &scene, float dt, float radius) -> bool;
    };

    Scene_Particles(Scene_ID id);
    Scene_Particles(Scene_ID id, Pose p, std::string name);
    Scene_Particles(Scene_ID id, GL::Mesh &&mesh);
    Scene_Particles(Scene_Particles &&src) = default;
    Scene_Particles(const Scene_Particles &src) = delete;
    ~Scene_Particles() = default;

    void operator=(const Scene_Particles &src) = delete;
    auto operator=(Scene_Particles &&src) -> Scene_Particles & = default;

    void clear();
    void step(const PT::Object &scene, float dt);
    void step2(const PT::Object &scene, float dt);
    void step3(const PT::Object &scene, float dt);
    void gen_instances();

    auto get_particles() const -> const std::vector<Particle> &;

    auto bbox() const -> BBox;
    void render(const Mat4 &view, bool depth_only = false, bool posed = true, bool particles_only = false);
    auto id() const -> Scene_ID;
    void set_time(float time);

    auto mesh() const -> const GL::Mesh &;
    void take_mesh(GL::Mesh &&mesh);

    struct Options
    {
        char name[MAX_NAME_LEN] = {};
        Spectrum color = Spectrum(1.0f);
        float velocity = 25.0f;
        float angle = 0.0f;
        float scale = 0.1f;
        float lifetime = 15.0f;
        float pps = 5.0f;
        float dt = 0.01f;
        bool enabled = false;
    };

    struct Anim_Particles
    {
        void at(float t, Options &o) const;
        void set(float t, Options o);
        Splines<Spectrum, float, float, float, float, float, bool> splines;
    };

    Options opt;
    Pose pose;
    Anim_Pose anim;
    Anim_Particles panim;

public: // HinaPE 1.1.0
    // Prepare to accept Fluid Engine Data

private:
    void get_r();
    Scene_ID _id;
    std::vector<Particle> particles;
    GL::Instances particle_instances;
    GL::Mesh arrow;

    float radius = 0.0f;
    float last_update = 0.0f;
    double particle_cooldown = 0.0f;

public: // Temp
    void load(std::shared_ptr<HinaPE::FluidEngine::SphSolver3> _solver_ptr);
    std::shared_ptr<HinaPE::FluidEngine::SphSolver3> solver_ptr;
};

auto operator!=(const Scene_Particles::Options &l, const Scene_Particles::Options &r) -> bool;

#endif
