#include "render-engine/platform/platform.h"
#include "render-engine/util/rand.h"
#include "render-engine/scene/particles.h"

#include "fluid-engine/geometry/bounding_box3.h"
#include "fluid-engine/geometry/plane3.h"
#include "fluid-engine/geometry/box3.h"
#include "fluid-engine/geometry/rigid_body_collider3.h"
#include "fluid-engine/geometry/sphere3.h"
#include "fluid-engine/geometry/implicit_surface_set3.h"
#include "fluid-engine/solver/particle/sph/sph_solver3.h"
#include "fluid-engine/emitter/volume_particle_emitter3.h"
#include "fluid-engine/util/logging.h"

#include "util/fluid_api.h"

using namespace HinaPE::FluidEngine;

auto main(int argc, char **argv) -> int
{
    // Init Render Engine
    RNG::seed();
    Launch_Settings set;
    Platform platform;
    App app(set, &platform);

    // Register Fluid Engine
    Logging::mute();
    int phase = 0;

    std::shared_ptr<Scene_Particles> sp = nullptr;
    std::shared_ptr<FluidAPI> fluid_api = nullptr;
    app.register_custom_simulation_sidebarUI([&](Gui::Manager &_manager, Scene &_scene, Undo &_undo, Gui::Widgets &_widgets, Scene_Maybe _obj, int &_index)
                                             {
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
                                                                 sp = std::make_shared<Scene_Particles>(_scene.reserve_id(), std::move(sphere_mesh));
                                                                 sp->opt.enabled = true;
                                                                 fluid_api = std::make_shared<FluidAPI>();
                                                                 fluid_api->assign_particles_domain(BoundingBox3D(Vector3D(-R / 2.0f, -R / 2.0f, -R / 2.0f), Vector3D(R / 2.0f, R / 2.0f, R / 2.0f)));
                                                                 sp->load_particle_system<FluidAPI>(fluid_api);
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
                                                                 auto plane = Plane3::builder().withNormal({0, 1, 0}).withPoint({0, 0.25 * fluid_api->particles_domain.height(), 0}).makeShared();
                                                                 auto sphere = Sphere3::builder().withCenter(fluid_api->particles_domain.midPoint()).withRadius(0.15 * fluid_api->particles_domain.width()).makeShared();
                                                                 auto surfaceSet = ImplicitSurfaceSet3::builder().withExplicitSurfaces({plane, sphere}).makeShared();
                                                                 auto emitter = VolumeParticleEmitter3::builder().withImplicitSurface(surfaceSet).withSpacing(fluid_api->fluid_opt.target_spacing).withMaxRegion(
                                                                         BoundingBox3D(Vector3D(-1.f, -0.5f, -1.f), Vector3D(1.f, 0.5f, 1.f))).withIsOneShot(false).makeShared();
                                                                 fluid_api->add_emitter(emitter);

                                                                 auto box = Box3::builder().withIsNormalFlipped(true).withBoundingBox(fluid_api->particles_domain).makeShared();
                                                                 auto box_collider = RigidBodyCollider3::builder().withSurface(box).makeShared();
                                                                 fluid_api->add_collider(box_collider);
                                                                 fluid_api->load_solver();
                                                                 sp->opt.scale = fluid_api->fluid_opt.target_spacing / 1.5f;
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
