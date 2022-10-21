#include "render-engine/platform/platform.h"
#include "render-engine/util/rand.h"

auto main(int argc, char **argv) -> int
{
    RNG::seed();
    Launch_Settings set;
    Platform plt;
    App app(set, &plt);
    plt.loop(app);
    return 0;
}