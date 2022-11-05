#include <filesystem>
#include "fluid_api.h"

using namespace HinaPE;
using namespace HinaPE::FluidEngine;

void saveParticleAsXyz(const ParticleSystemData3Ptr &particles, const std::string &rootDir, int frameCnt)
{
    Array1<Vector3D> positions(particles->numberOfParticles());
    copyRange1(particles->positions(), particles->numberOfParticles(), &positions);
    char basename[256];
    snprintf(basename, sizeof(basename), "frame_%06d.xyz", frameCnt);
    std::string filename = rootDir + "/" + basename;
    if (!std::filesystem::is_directory(rootDir) || !std::filesystem::exists(rootDir))
        std::filesystem::create_directory(rootDir);
    std::ofstream file(filename.c_str());
    if (file)
    {
        printf("Writing %s...\n", filename.c_str());
        for (const auto &pt: positions)
        {
            file << pt.x << ' ' << pt.y << ' ' << pt.z << std::endl;
        }
        file.close();
    }
}

void FluidAPI::step(Scene_Particles *_scene_particles, float dt)
{
    auto &_p = _scene_particles->particles;
    auto size = _solver_ptr->particleSystemData()->numberOfParticles();
    _p.resize(size);

    static HinaPE::Frame frame(0, 1.0 / 60.0);
    _solver_ptr->update(frame++);

    auto const &data = std::static_pointer_cast<HinaPE::FluidEngine::SphSolver3>(_solver_ptr)->sphSystemData();
    HinaPE::parallelFor((size_t) 0, (size_t) size, [&](size_t i)
    {
        auto &p = data->positions()[i];
        _p[i].pos = Vec3((float) p[0], (float) p[1], (float) p[2]);
    });

    saveParticleAsXyz(data,"E:\\HinaPE\\output",frame.index);
    std::cout << "Particles Size: " << _p.size() << std::endl;
}

void FluidAPI::gui(Gui::Manager &_manager, Scene &_scene, Undo &_undo, Gui::Widgets &_widgets, Scene_Maybe _obj, int &_index)
{
    static int phase = 0;
    double lz = 0;
    switch (phase)
    {
        case 0:
            if (ImGui::CollapsingHeader("Create Fluid Domain"), ImGuiTreeNodeFlags_DefaultOpen)
            {
                ImGui::PushID(_index++);
                static float R = 1.0f;
                ImGui::SliderFloat("Size", &R, 0.01f, 10.0f, "%.2f");
                if (ImGui::Button("Add"))
                {
                    // Init Bounding Domain
                    Halfedge_Mesh hm;
                    hm.from_mesh(Util::cube_mesh(R / 2.0f));
                    hm.flip(); // if flip
                    Scene_Object &obj = _undo.add_obj(std::move(hm), "SPH Fluid Domain");
                    obj.opt.wireframe = false;
                    obj.opt.surface = false;
                    obj.opt.bbox = true;
                    //_manager.set_select(obj.id());
                    ++phase;

                    GL::Mesh sphere_mesh = Util::sphere_mesh(1.0f, 1);
                    _sp = std::make_shared<Scene_Particles>(_scene.reserve_id(), std::move(sphere_mesh));
                    _sp->opt.enabled = true;
                    assign_particles_domain(BoundingBox3D(Vector3D(-R / 2.0f, -R / 2.0f, -R / 2.0f), Vector3D(R / 2.0f, R / 2.0f, R / 2.0f)));
                    lz = _particles_domain.depth();
                    _sp->load_particle_system<FluidAPI>(shared_from_this());
                }
                ImGui::PopID();
            }
            break;
        case 1:
            if (ImGui::CollapsingHeader("Create Fluid"), ImGuiTreeNodeFlags_DefaultOpen)
            {
                ImGui::PushID(_index++);
                if (ImGui::Button("Add"))
                {
                    auto plane = Plane3::builder().withNormal({0, 1, 0}).withPoint({0, 0.25 * _particles_domain.height(), 0}).makeShared();
                    auto sphere = Sphere3::builder().withCenter(_particles_domain.midPoint()).withRadius(0.15 * _particles_domain.width()).makeShared();
                    auto surfaceSet = ImplicitSurfaceSet3::builder().withExplicitSurfaces({plane, sphere}).makeShared();
                    auto emitter = VolumeParticleEmitter3::builder().withImplicitSurface(surfaceSet).withSpacing(_fluid_opt.target_spacing).withMaxRegion(
                            BoundingBox3D(Vector3D(-1.f, -0.5f, -1.f), Vector3D(1.f, 0.5f, 1.f))).withIsOneShot(false).makeShared();
                    add_emitter(emitter);

                    auto box = Box3::builder().withIsNormalFlipped(true).withBoundingBox(_particles_domain).makeShared();
                    auto box_collider = RigidBodyCollider3::builder().withSurface(box).makeShared();
                    add_collider(box_collider);
                    // load_solver();
                    load_pci_solver();
                    _sp->opt.scale = _fluid_opt.target_spacing / 1.5f;
                    _undo.add(std::move(*_sp));
                }
                ImGui::PopID();
            }
            break;
//        case 2:
//            if (ImGui::CollapsingHeader("Create Fluid"), ImGuiTreeNodeFlags_DefaultOpen)
//            {
//                ImGui::PushID(_index++);
//                if (ImGui::Button("Add"))
//                {
//                    auto plane = Plane3::builder().withNormal({0, 1, 0}).withPoint({0, 0.25 * _particles_domain.height(), 0}).makeShared();
//                    auto sphere = Sphere3::builder().withCenter(_particles_domain.midPoint()).withRadius(0.15 * _particles_domain.width()).makeShared();
//                    auto surfaceSet = ImplicitSurfaceSet3::builder().withExplicitSurfaces({plane, sphere}).makeShared();
//                    auto emitter = VolumeParticleEmitter3::builder().withImplicitSurface(surfaceSet).withSpacing(_fluid_opt.target_spacing).withMaxRegion(
//                            BoundingBox3D(Vector3D(-1.f, -0.5f, -1.f), Vector3D(1.f, 0.5f, 1.f))).withIsOneShot(false).makeShared();
//                    add_emitter(emitter);
//
//                    auto box = Box3::builder().withIsNormalFlipped(true).withBoundingBox(_particles_domain).makeShared();
//                    auto box_collider = RigidBodyCollider3::builder().withSurface(box).makeShared();
//                    add_collider(box_collider);
//                    load_pci_solver();
//                    _sp->opt.scale = _fluid_opt.target_spacing / 1.5f;
//                    _undo.add(std::move(*_sp));
//                }
//                ImGui::PopID();
//            }
//            break;
//        case 1:
//            if (ImGui::CollapsingHeader("Dam-breaking"), ImGuiTreeNodeFlags_DefaultOpen)
//            {
//                ImGui::PushID(_index++);
//                if (ImGui::Button("Add"))
//                {
//                    auto box1 =Box3::builder().withLowerCorner({0, 0, 0}).withUpperCorner({0.5 + 0.001, 0.75 + 0.001, 0.75 * lz + 0.001}).makeShared();
//                    auto box2 =Box3::builder().withLowerCorner({2.5 - 0.001, 0, 0.25 * lz - 0.001}).withUpperCorner({3.5 + 0.001, 0.75 + 0.001, 1.5 * lz + 0.001}).makeShared();
//                    auto boxSet = ImplicitSurfaceSet3::builder().withExplicitSurfaces({box1, box2}).makeShared();
//                    auto emitter = VolumeParticleEmitter3::builder().withSurface(boxSet).withSpacing(_fluid_opt.target_spacing).makeShared();
//                    add_emitter(emitter);
//
//                    auto cyl1 = Cylinder3::builder().withCenter({1, 0.375, 0.375}).withRadius(0.1).withHeight(0.75).makeShared();
//                    auto cyl2 = Cylinder3::builder().withCenter({1.5, 0.375, 0.75}).withRadius(0.1).withHeight(0.75).makeShared();
//                    auto cyl3 = Cylinder3::builder().withCenter({2, 0.375, 1.125}).withRadius(0.1).withHeight(0.75).makeShared();
//                    auto box = Box3::builder().withIsNormalFlipped(true).withBoundingBox(_particles_domain).makeShared();
//                    auto surfaceSet = ImplicitSurfaceSet3::builder().withExplicitSurfaces({cyl1, cyl2, cyl3, box}).makeShared();
//                    auto collider = RigidBodyCollider3::builder().withSurface(surfaceSet).makeShared();
//                    add_collider(collider);
//
//                    load_dam_breaking_solver();
//                    _sp->opt.scale = _fluid_opt.target_spacing / 1.5f;
//                    _undo.add(std::move(*_sp));
//
//                }
//                ImGui::PopID();
//            }
//            break;
        default:
            break;
    }
}

void FluidAPI::load_solver()
{
    if (!_emitter_ptr_list.empty() && !_collider_ptr_list.empty() && !_particles_domain.isEmpty() && !_solver_prepared)
    {
        _solver_ptr = HinaPE::FluidEngine::SphSolver3::builder().withTargetDensity(_fluid_opt.target_density).withTargetSpacing(_fluid_opt.target_spacing).makeShared();
        _solver_ptr->setEmitter(_emitter_ptr_list.back()); // TODO: to support more emitter
        _solver_ptr->setCollider(_collider_ptr_list.back()); // TODO: to support more collider
        if (_fluid_opt.type == SPH)
        {
            std::static_pointer_cast<HinaPE::FluidEngine::SphSolver3>(_solver_ptr)->setPseudoViscosityCoefficient(_fluid_opt.pseudo_viscosity_coefficient);
        }
        _solver_prepared = true;
    }
}

void FluidAPI::load_pci_solver() {
    if (!_emitter_ptr_list.empty() && !_collider_ptr_list.empty() && !_particles_domain.isEmpty() && !_solver_prepared)
    {
        _solver_ptr = HinaPE::FluidEngine::PciSphSolver3::builder().withTargetDensity(_fluid_opt.target_density).withTargetSpacing(_fluid_opt.target_spacing).makeShared();
        _solver_ptr->setEmitter(_emitter_ptr_list.back()); // TODO: to support more emitter
        _solver_ptr->setCollider(_collider_ptr_list.back()); // TODO: to support more collider
        if (_fluid_opt.type == SPH)
        {
            std::static_pointer_cast<HinaPE::FluidEngine::SphSolver3>(_solver_ptr)->setPseudoViscosityCoefficient(_fluid_opt.pseudo_viscosity_coefficient);
        }
        _solver_prepared = true;
    }
}

void FluidAPI::load_dam_breaking_solver() {
    if (!_emitter_ptr_list.empty() && !_collider_ptr_list.empty() && !_particles_domain.isEmpty() && !_solver_prepared)
    {
        _solver_ptr = HinaPE::FluidEngine::PciSphSolver3::builder().withTargetDensity(_fluid_opt.target_density).withTargetSpacing(_fluid_opt.target_spacing).makeShared();
        _solver_ptr->setEmitter(_emitter_ptr_list.back()); // TODO: to support more emitter
        _solver_ptr->setCollider(_collider_ptr_list.back()); // TODO: to support more collider
        if (_fluid_opt.type == SPH)
        {
            std::static_pointer_cast<HinaPE::FluidEngine::SphSolver3>(_solver_ptr)->setPseudoViscosityCoefficient(_fluid_opt.pseudo_viscosity_coefficient);
        }
        _solver_prepared = true;
    }
}

void FluidAPI::add_emitter(const std::shared_ptr<HinaPE::FluidEngine::ParticleEmitter3> &_emitter_ptr)
{ _emitter_ptr_list.emplace_back(_emitter_ptr); }

void FluidAPI::add_collider(const std::shared_ptr<HinaPE::Collider3> &_collider_ptr)
{ _collider_ptr_list.emplace_back(_collider_ptr); }

void FluidAPI::assign_particles_domain(const HinaPE::BoundingBox3D &_domain)
{ _particles_domain = _domain; }



