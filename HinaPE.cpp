#include "render-engine/platform/platform.h"
#include "render-engine/util/rand.h"

#include "fluid-engine/geometry/bounding_box3.h"
#include "fluid-engine/geometry/plane3.h"
#include "fluid-engine/geometry/box3.h"
#include "fluid-engine/geometry/rigid_body_collider3.h"
#include "fluid-engine/geometry/sphere3.h"
#include "fluid-engine/geometry/implicit_surface_set3.h"
#include "fluid-engine/solver/particle/sph/sph_solver3.h"
#include "fluid-engine/emitter/volume_particle_emitter3.h"
#include "fluid-engine/util/logging.h"

using namespace HinaPE::FluidEngine;

auto main(int argc, char **argv) -> int
{
    // Init Render Engine
    RNG::seed();
    Launch_Settings set;
    Platform platform;
    App app(set, &platform);

    // Register Fluid Engine

    int phase = 0;

    Logging::mute();
//    float spacing = 0.02;
//    BoundingBox3D domain(Vector3D(), Vector3D(1, 2, 1));
//    auto solver = SphSolver3::builder().withTargetDensity(1000.0).withTargetSpacing(spacing).makeShared();
//    solver->setPseudoViscosityCoefficient(0.0);
//    BoundingBox3D sourceBound(domain);
//    sourceBound.expand(-0.02);
//    auto plane = Plane3::builder().withNormal({0, 1, 0}).withPoint({0, 0.25 * domain.height(), 0}).makeShared();
//    auto sphere = Sphere3::builder().withCenter(domain.midPoint()).withRadius(0.15 * domain.width()).makeShared();
//    auto surfaceSet = ImplicitSurfaceSet3::builder().withExplicitSurfaces({plane, sphere}).makeShared();
//    auto emitter = VolumeParticleEmitter3::builder().withImplicitSurface(surfaceSet).withSpacing(spacing).withMaxRegion(sourceBound).withMaxNumberOfParticles(10000).withIsOneShot(true).makeShared();
//    solver->setEmitter(emitter);
//    auto box = Box3::builder().withIsNormalFlipped(true).withBoundingBox(domain).makeShared();
//    auto collider = RigidBodyCollider3::builder().withSurface(box).makeShared();
//    solver->setCollider(collider);
//    auto particles = solver->sphSystemData();

    std::shared_ptr<Scene_Particles> sp = nullptr;
    app.register_custom_simulation_sidebarUI
            ([&](Gui::Manager &_manager, Scene &_scene, Undo &_undo, Gui::Widgets &_widgets, Scene_Maybe _obj, int &_index)
             {
                 switch (phase)
                 {
                     case 0:
                         if (ImGui::CollapsingHeader("Create Fluid Domain"))
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
                                 _manager.set_select(obj.id());
                                 ++phase;

                                 GL::Mesh sphere_mesh = Util::sphere_mesh(1.0f, 1);
                                 sp = std::make_shared<Scene_Particles>(_scene.reserve_id(), std::move(sphere_mesh));
                                 sp->opt.enabled = true;
                                 sp->opt.scale = 0.01f;
                                 sp->assign_particles_domain(BoundingBox3D(Vector3D(-R / 2.0f, -R / 2.0f, -R / 2.0f), Vector3D(R / 2.0f, R / 2.0f, R / 2.0f)));
                             }
                             ImGui::PopID();
                         }
                         break;
                     case 1:
                         if (ImGui::CollapsingHeader("Create Fluid"))
                         {
                             ImGui::PushID(_index++);
                             if (ImGui::Button("Add"))
                             {
                                 auto plane = Plane3::builder().withNormal({0, 1, 0}).withPoint({0, 0.25 * sp->particles_domain.height(), 0}).makeShared();
                                 auto sphere = Sphere3::builder().withCenter(sp->particles_domain.midPoint()).withRadius(0.15 * sp->particles_domain.width()).makeShared();
                                 auto surfaceSet = ImplicitSurfaceSet3::builder().withExplicitSurfaces({plane, sphere}).makeShared();
                                 auto emitter = VolumeParticleEmitter3::builder().withImplicitSurface(surfaceSet).withSpacing(sp->fluid_opt.target_spacing).withMaxRegion(sp->particles_domain).withMaxNumberOfParticles(10000).withIsOneShot(
                                         false).makeShared();
                                 sp->add_emitter(emitter);

                                 auto box = Box3::builder().withIsNormalFlipped(true).withBoundingBox(sp->particles_domain).makeShared();
                                 auto box_collider = RigidBodyCollider3::builder().withSurface(box).makeShared();
                                 sp->add_collider(box_collider);
                                 sp->load_solver();
                                 _undo.add(std::move(*sp));
                             }
                             ImGui::PopID();
                         }
                     default:
                         break;
                 }
             });

    platform.loop(app);

    return 0;
}
