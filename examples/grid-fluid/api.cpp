#include "api.h"
#include "imgui.h"
void HinaPE::Fluid::Api::step(float dt)
{
    if (_solvers.empty())
        return;

    static HinaPE::Frame frame(0, 1.0 / 60.0);

}

void HinaPE::Fluid::Api::assign_particles_domain(const HinaPE::BoundingBox3D &_domain)
{
    _particles_domain = _domain;
}

void HinaPE::Fluid::Api::ui_sidebar()
{
    ImGui::Text("Grid-Fluid");

    static float R = 1.0f;
    ImGui::SliderFloat("Size", &R, 0.01f, 10.0f, "%.2f");
    assign_particles_domain(BoundingBox3D(Vector3D(-R / 2.0f, -R / 2.0f, -R / 2.0f), Vector3D(R / 2.0f, R / 2.0f, R / 2.0f)));

    static int resX = 0;
    ImGui::SliderInt("resolutionX", &resX, 0, 100,"%d");
    if(ImGui::Button("Create")) {
        auto solver = GridFluidSolver3::builder()
                .withResolution({resX, 2 * resX, resX})
                .withDomainSizeX(1.0)
                .makeShared();

        auto grids = solver->gridSystemData();

        auto plane = Plane3::builder()
                .withNormal({0, 1, 0})
                .withPoint({0, 0.25 * _particles_domain.height(), 0})
                .makeShared();

        auto sphere = Sphere3::builder()
                .withCenter(_particles_domain.midPoint())
                .withRadius(0.15 * _particles_domain.width())
                .makeShared();

        auto surfaceSet = ImplicitSurfaceSet3::builder()
                .withExplicitSurfaces({plane, sphere})
                .makeShared();

        auto emitter =
                VolumeGridEmitter3::builder().withSourceRegion(surfaceSet).makeShared();

        solver->setEmitter(emitter);

    }
}

void HinaPE::Fluid::Api::sync() {

}
