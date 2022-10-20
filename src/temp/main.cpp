#include "math_lib/logging.h"
#include "math_lib/bounding_box3.h"
#include "math_lib/plane3.h"
#include "math_lib/box3.h"
#include "math_lib/rigid_body_collider3.h"
#include "math_lib/sphere3.h"
#include "math_lib/implicit_surface_set3.h"
#include "math_lib/array_utils.h"
#include "sph/sph_solver3.h"
#include "kernel/volume_particle_emitter3.h"

#include <iostream>
#include <filesystem>
using namespace jet;

void saveParticleAsPos(const ParticleSystemData3Ptr &particles, const std::string &rootDir, int frameCnt)
{
    Array1<Vector3D> positions(particles->numberOfParticles());
    copyRange1(particles->positions(), particles->numberOfParticles(), &positions);
    char basename[256];
    snprintf(basename, sizeof(basename), "frame_%06d.pos", frameCnt);
    std::string filename = rootDir + "/" + basename;
    if (!std::filesystem::is_directory(rootDir) || !std::filesystem::exists(rootDir))
        std::filesystem::create_directory(rootDir);
    std::ofstream file(filename.c_str(), std::ios::binary);
    if (file)
    {
        printf("Writing %s...\n", filename.c_str());
        std::vector<uint8_t> buffer;
        serialize(positions.constAccessor(), &buffer);
        file.write(reinterpret_cast<char *>(buffer.data()), buffer.size());
        file.close();
    }
}

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

auto main() -> int
{
    std::ofstream logFile("sph.log");
    if (logFile)
        Logging::setAllStream(&logFile);

    float spacing = 0.2;

    BoundingBox3D domain(Vector3D(), Vector3D(10, 20, 10));

    auto solver = SphSolver3::builder().withTargetDensity(1000.0).withTargetSpacing(spacing).makeShared();

    solver->setPseudoViscosityCoefficient(0.0);

    BoundingBox3D sourceBound(domain);
    sourceBound.expand(-0.02);

    auto plane = Plane3::builder().withNormal({0, 1, 0}).withPoint({0, 0.25 * domain.height(), 0}).makeShared();

    auto sphere = Sphere3::builder().withCenter(domain.midPoint()).withRadius(0.15 * domain.width()).makeShared();

    auto surfaceSet = ImplicitSurfaceSet3::builder().withExplicitSurfaces({plane, sphere}).makeShared();

    auto emitter = VolumeParticleEmitter3::builder().withImplicitSurface(surfaceSet).withSpacing(spacing).withMaxRegion(sourceBound).withIsOneShot(true).makeShared();

    solver->setEmitter(emitter);

    auto box = Box3::builder().withIsNormalFlipped(true).withBoundingBox(domain).makeShared();

    auto collider = RigidBodyCollider3::builder().withSurface(box).makeShared();

    solver->setCollider(collider);

    auto particles = solver->sphSystemData();

    for (Frame frame(0, 1.0 / 60.0); frame.index < 100; ++frame)
    {
        solver->update(frame);
        std::cout << "Overall particles number: " << particles->numberOfParticles() << std::endl;
        saveParticleAsXyz(particles, "F:/Projects/HinaPE/output", frame.index);
    }

    return 0;
}
