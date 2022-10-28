#include "render-engine/platform/platform.h"
#include "render-engine/util/rand.h"
#include "render-engine/scene/particles.h"

#include "cloth_api.h"

using namespace HinaPE;
using namespace HinaPE::ClothEngine;

auto main(int argc, char **argv) -> int
{
    // Init Render Engine
    RNG::seed();
    Launch_Settings set;
    Platform platform;
    App app(set, &platform);

    // Register Cloth Engine
    Logging::mute();
    int phase = 0;

    std::shared_ptr<Scene_Object> so = nullptr;
    std::shared_ptr<ClothAPI> cloth_api = nullptr;
    app.register_custom_simulation_sidebarUI([&](Gui::Manager &_manager, Scene &_scene, Undo &_undo, Gui::Widgets &_widgets, Scene_Maybe _obj, int &_index) {});
    platform.loop(app);
    return 0;
}