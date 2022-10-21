#include "platform/platform.h"
#include "util/rand.h"
#include <sf_libs/CLI11.hpp>

#include "geometry/bounding_box3.h"
#include "geometry/plane3.h"
#include "geometry/box3.h"
#include "geometry/rigid_body_collider3.h"
#include "geometry/sphere3.h"
#include "geometry/implicit_surface_set3.h"
#include "array/array_utils.h"
#include "solver/particle/sph//sph_solver3.h"
#include "emitter/volume_particle_emitter3.h"
#include "logging.h"

using namespace HinaPE::FluidEngine;

int main(int argc, char **argv)
{
    RNG::seed();

    Launch_Settings set;
    CLI::App args{"Scotty3D - 15-462"};

    args.add_option("-s,--scene", set.scene_file, "Scene file to load");
    args.add_option("--env_map", set.env_map_file, "Override scene environment map");
    args.add_flag("--headless", set.headless, "Path-trace scene without opening the GUI");
    args.add_option("-o,--output", set.output_file, "Image file to write (if headless)");
    args.add_flag("--animate", set.animate, "Output animation frames (if headless)");
    args.add_flag("--no_bvh", set.no_bvh, "Don't use BVH (if headless)");
    args.add_option("--width", set.w, "Output image width (if headless)");
    args.add_option("--height", set.h, "Output image height (if headless)");
    args.add_flag("--use_ar", set.w_from_ar, "Compute output image width based on camera AR (if headless)");
    args.add_option("--depth", set.d, "Maximum ray depth (if headless)");
    args.add_option("--samples", set.s, "Pixel samples (if headless)");
    args.add_option("--exposure", set.exp, "Output exposure (if headless)");

    CLI11_PARSE(args, argc, argv);

    if (!set.headless)
    {
        Platform plt;
        App app(set, &plt);

        int phase = 0;

        Logging::mute();
        float spacing = 0.02;

        BoundingBox3D domain(Vector3D(), Vector3D(1, 2, 1));
        auto solver = SphSolver3::builder().withTargetDensity(1000.0).withTargetSpacing(spacing).makeShared();
        solver->setPseudoViscosityCoefficient(0.0);
        BoundingBox3D sourceBound(domain);
        sourceBound.expand(-0.02);
        auto plane = Plane3::builder().withNormal({0, 1, 0}).withPoint({0, 0.25 * domain.height(), 0}).makeShared();
        auto sphere = Sphere3::builder().withCenter(domain.midPoint()).withRadius(0.15 * domain.width()).makeShared();
        auto surfaceSet = ImplicitSurfaceSet3::builder().withExplicitSurfaces({plane, sphere}).makeShared();
        auto emitter = VolumeParticleEmitter3::builder().withImplicitSurface(surfaceSet).withSpacing(spacing).withMaxRegion(sourceBound).withMaxNumberOfParticles(10000).withIsOneShot(true).makeShared();
        solver->setEmitter(emitter);
        auto box = Box3::builder().withIsNormalFlipped(true).withBoundingBox(domain).makeShared();
        auto collider = RigidBodyCollider3::builder().withSurface(box).makeShared();
        solver->setCollider(collider);
        auto particles = solver->sphSystemData();

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
                                     Halfedge_Mesh hm;
                                     hm.from_mesh(Util::cube_mesh(R / 2.0f));
                                     hm.flip(); // if flip
                                     Scene_Object &obj = _undo.add_obj(std::move(hm), "SPH Fluid Domain");
                                     obj.opt.wireframe = false;
                                     obj.opt.surface = false;
                                     obj.opt.bbox = true;
                                     ++phase;
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
                                     GL::Mesh mesh = Util::sphere_mesh(1.0f, 1);
                                     Scene_Particles _particles(_scene.reserve_id(), std::move(mesh));
                                     _particles.opt.enabled = true;
                                     _particles.opt.scale = 0.01f;
                                     _particles.load(solver);
                                     _undo.add(std::move(_particles));
                                 }
                                 ImGui::PopID();
                             }
                         default:
                             break;
                     }
                 });

        plt.loop(app);
    } else
    {
        App app(set);
    }
    return 0;
}
