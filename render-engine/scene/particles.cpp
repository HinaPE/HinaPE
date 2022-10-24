#include "../geometry/util.h"
#include "../rays/pathtracer.h"

#include "particles.h"

#include "util/parallel.h"

#include <utility>
#include "renderer.h"

Scene_Particles::Scene_Particles(Scene_ID id) : arrow(Util::arrow_mesh(0.03f, 0.075f, 1.0f)), particle_instances(Util::sphere_mesh(1.0f, 1))
{

    _id = id;
    snprintf(opt.name, MAX_NAME_LEN, "Emitter %d", id);
    get_r();
}

Scene_Particles::Scene_Particles(Scene_ID id, GL::Mesh &&mesh) : arrow(Util::arrow_mesh(0.03f, 0.075f, 1.0f)), particle_instances(std::move(mesh))
{

    _id = id;
    snprintf(opt.name, MAX_NAME_LEN, "Emitter %d", id);
    get_r();
}

Scene_Particles::Scene_Particles(Scene_ID id, Pose p, std::string name) : arrow(Util::arrow_mesh(0.03f, 0.075f, 1.0f)), particle_instances(Util::sphere_mesh(1.0f, 1))
{

    _id = id;
    pose = p;
    snprintf(opt.name, MAX_NAME_LEN, "%s", name.c_str());
    get_r();
}

void Scene_Particles::get_r()
{
    BBox box = particle_instances.mesh().bbox();
    radius = (box.max - box.min).norm() / 2.0f;
}

BBox Scene_Particles::bbox() const
{
    BBox box = arrow.bbox();
    box.transform(pose.transform());
    return box;
}

void Scene_Particles::take_mesh(GL::Mesh &&mesh)
{
    particle_instances = GL::Instances(std::move(mesh));
}

const GL::Mesh &Scene_Particles::mesh() const
{
    return particle_instances.mesh();
}

void Scene_Particles::render(const Mat4 &view, bool depth_only, bool posed, bool particles_only)
{
    Renderer &renderer = Renderer::get();

    Mat4 T = posed ? Mat4::translate(pose.pos) * pose.rotation_mat() : Mat4::I;

    Renderer::MeshOpt opts;
    opts.modelview = view * T;
    opts.id = _id;
    opts.solid_color = true;
    opts.depth_only = depth_only;
    opts.color = opt.color.to_vec();

    if (!particles_only)
    {
        renderer.mesh(arrow, opts);
    }

    if (opt.enabled && !depth_only)
    {
        opts.modelview = view;
        opts.id = _id;
        opts.solid_color = false;
        renderer.instances(opts, particle_instances);
    }
}

Scene_ID Scene_Particles::id() const
{
    return _id;
}

void Scene_Particles::clear()
{
    particles.clear();
    particle_instances.clear();
}

void Scene_Particles::set_time(float time)
{
    if (panim.splines.any())
    {
        panim.at(time, opt);
        if (!opt.enabled)
            clear();
    }
    if (anim.splines.any())
    {
        pose = anim.at(time);
    }
}

const std::vector<Scene_Particles::Particle> &Scene_Particles::get_particles() const
{
    return particles;
}

void Scene_Particles::step(const PT::Object &scene, float dt)
{
    if (!opt.enabled)
    {
        clear();
        return;
    }
    if (opt.dt < EPS_F)
        return;

    last_update += dt;

//    while (last_update > opt.dt)
//    {
//        step2(scene, opt.dt);
//        last_update -= opt.dt;
//    }

    step3(scene, opt.dt);

    gen_instances();
}

void Scene_Particles::gen_instances()
{
    particle_instances.clear(particles.size());

    for (Particle &p: particles)
    {
        Mat4 T = Mat4{Vec4{opt.scale, 0.0f, 0.0f, 0.0f}, Vec4{0.0f, opt.scale, 0.0f, 0.0f}, Vec4{0.0f, 0.0f, opt.scale, 0.0f}, Vec4{p.pos, 1.0f}};
        particle_instances.add(T);
    }
}

void Scene_Particles::step2(const PT::Object &scene, float dt)
{
    std::vector<Particle> next;
    next.reserve(particles.size());

    for (Particle &p: particles)
    {
        if (p.update(scene, dt, radius * opt.scale))
        {
            next.emplace_back(p);
        }
    }

    particle_cooldown -= dt;
    float cos = std::cos(Radians(opt.angle) / 2.0f);

    if (opt.pps > 0.0f)
    {
        double cooldown = 1.0 / opt.pps;
        while (particle_cooldown <= 0.0f)
        {

            float z = lerp(cos, 1.0f, RNG::unit());
            float t = 2 * PI_F * RNG::unit();
            float r = std::sqrt(1 - z * z);
            Vec3 dir = opt.velocity * Vec3(r * std::cos(t), z, r * std::sin(t));

            Particle p;
            p.pos = pose.pos;
            p.velocity = pose.rotation_mat().rotate(dir);
            p.age = opt.lifetime;
            next.push_back(p);

            particle_cooldown += cooldown;
        }
    }
    particles = std::move(next);
}

void Scene_Particles::step3(const PT::Object &scene, float dt)
{
    auto size = solver_ptr->particleSystemData()->numberOfParticles();
    particles.resize(size);

    static HinaPE::FluidEngine::Frame frame(0, 1.0 / 60.0);
    solver_ptr->update(frame++);

    auto const &data = std::static_pointer_cast<HinaPE::FluidEngine::SphSolver3>(solver_ptr)->sphSystemData();
    HinaPE::FluidEngine::parallelFor((size_t)0, (size_t)size, [&](size_t i) {
        auto &p = data->positions()[i];
        particles[i].pos = Vec3((float) p[0], (float) p[1], (float) p[2]);
    });
}

void Scene_Particles::load_solver()
{
    if (!emitter_ptr_list.empty() && !collider_ptr_list.empty() && !particles_domain.isEmpty() && !solver_prepared)
    {
        solver_ptr = HinaPE::FluidEngine::SphSolver3::builder().withTargetDensity(fluid_opt.target_density).withTargetSpacing(fluid_opt.target_spacing).makeShared();
        solver_ptr->setEmitter(emitter_ptr_list.back()); // TODO: to support more emitter
        solver_ptr->setCollider(collider_ptr_list.back()); // TODO: to support more collider
        if (fluid_opt.type == SPH){
            std::static_pointer_cast<HinaPE::FluidEngine::SphSolver3>(solver_ptr)->setPseudoViscosityCoefficient(fluid_opt.pseudo_viscosity_coefficient);
        }
        solver_prepared = true;
    }
}

void Scene_Particles::add_emitter(std::shared_ptr<HinaPE::FluidEngine::ParticleEmitter3> _emitter_ptr)
{
    emitter_ptr_list.emplace_back(_emitter_ptr); // use copy for shared ptr
}

void Scene_Particles::add_collider(std::shared_ptr<HinaPE::FluidEngine::Collider3> _collider_ptr)
{
    collider_ptr_list.emplace_back(_collider_ptr); // use copy for shared ptr
}

void Scene_Particles::assign_particles_domain(const HinaPE::FluidEngine::BoundingBox3D &_domain)
{
    particles_domain = _domain;
}

void Scene_Particles::Anim_Particles::at(float t, Scene_Particles::Options &o) const
{
    auto [c, v, a, s, l, p, e] = splines.at(t);
    o.color = c;
    o.velocity = v;
    o.angle = a;
    o.scale = s;
    o.lifetime = l;
    o.pps = p;
    o.enabled = e;
}

void Scene_Particles::Anim_Particles::set(float t, Scene_Particles::Options o)
{
    splines.set(t, o.color, o.velocity, o.angle, o.scale, o.lifetime, o.pps, o.enabled);
}

bool operator!=(const Scene_Particles::Options &l, const Scene_Particles::Options &r)
{
    return l.color != r.color || l.velocity != r.velocity || l.angle != r.angle || l.scale != r.scale || l.lifetime != r.lifetime || l.pps != r.pps ||
           l.enabled != r.enabled || l.dt != r.dt;
}
