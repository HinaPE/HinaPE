#include "workbench/app.h"
#include "workbench/platform.h"

auto main(int argc, char **argv) -> int
{
    HinaPE::RenderEngine::Workbench::Platform plt(1280, 720);
    HinaPE::RenderEngine::Workbench::App app;
    plt.update(app);
    return 0;
}
