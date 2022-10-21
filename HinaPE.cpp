#ifdef HinaRenderEngine
#include "render-engine/platform/platform.h"
#include "render-engine/util/rand.h"
#endif

#ifdef HinaFluidEngine
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
#endif

auto main(int argc, char **argv) -> int
{
#ifdef HinaFluidEngine
    // Init Fluid Engine
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
    for (Frame frame(0, 1.0 / 60.0); frame.index < 100; ++frame)
    {
        solver->update(frame);
        std::cout << "Frame Update" << std::endl;
    }
#endif

#ifdef HinaRenderEngine
    // Init Render Engine
    RNG::seed();
    Launch_Settings set;
    Platform platform;
    App app(set, &platform);
//    app.register_particle_system_data(particles->positions().data(), particles->numberOfParticles());
    platform.loop(app);
#endif
    return 0;
}
