#include "render-engine/platform/platform.h"
#include "render-engine/util/rand.h"
#include "render-engine/scene/particles.h"

#include "fluid_api.h"

using namespace HinaPE;

auto main(int argc, char **argv) -> int
{
    // Init Render Engine
    RNG::seed();
    Launch_Settings set;
    Platform platform;
    App app(set, &platform);

    // Register Fluid Engine
    Logging::mute();
    app.register_physics_API(std::make_shared<FluidAPI>());
    platform.loop(app);

    return 0;
}
