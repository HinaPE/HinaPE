#include "api.h"
#include "imgui.h"
void HinaPE::Fluid::Api::step(float dt)
{
    if (_solvers.empty())
        return;

}

void HinaPE::Fluid::Api::assign_particles_domain(const HinaPE::BoundingBox3D &_domain)
{
    _particles_domain = _domain;
}

void HinaPE::Fluid::Api::ui_sidebar()
{
    ImGui::Text("Pic-Fluid");

    static float R = 1.0f;
    ImGui::SliderFloat("Size", &R, 0.01f, 10.0f, "%.2f");
    assign_particles_domain(BoundingBox3D(Vector3D(-R / 2.0f, -R / 2.0f, -R / 2.0f), Vector3D(R / 2.0f, R / 2.0f, R / 2.0f)));

    static int resolutionX = 0;
    ImGui::SliderInt("resolutionX", &resolutionX, 0, 100,"%d");
    if(ImGui::Button("Create"))
    {
        /// Build solver
        auto solver = PicSolver3::builder()
                        .withResolution({resolutionX, 2 * resolutionX, resolutionX})
                        .withDomainSizeX(1.0)
                        .makeShared();
        auto grids = solver->gridSystemData();
        auto particles = solver->particleSystemData();

        Vector3D gridSpacing = grids->gridSpacing();
        double dx = gridSpacing.x;
        BoundingBox3D domain = grids->boundingBox();

        /// Build emitter
        auto plane = Plane3::builder()
                .withNormal({0, 1, 0})
                .withPoint({0, 0.25 * _particles_domain.height(), 0})
                .makeShared();

        auto sphere = Sphere3::builder()
                .withCenter(_particles_domain.midPoint())
                .withRadius(0.15 * _particles_domain.width())
                .makeShared();

        auto emitter1 = VolumeParticleEmitter3::builder()
                .withSurface(plane)
                .withSpacing(0.5 * dx)
                .withMaxRegion(_particles_domain)
                .withIsOneShot(true)
                .makeShared();
        emitter1->setPointGenerator(std::make_shared<GridPointGenerator3>());

        auto emitter2 = VolumeParticleEmitter3::builder()
                .withSurface(sphere)
                .withSpacing(0.5 * dx)
                .withMaxRegion(_particles_domain)
                .withIsOneShot(true)
                .makeShared();
        emitter2->setPointGenerator(std::make_shared<GridPointGenerator3>());

        auto emitterSet = ParticleEmitterSet3::builder()
                .withEmitters({emitter1, emitter2})
                .makeShared();

        solver->setParticleEmitter(emitterSet);

    }
}

void HinaPE::Fluid::Api::sync() {

}
