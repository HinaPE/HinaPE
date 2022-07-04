
#include "platform/platform.h"
#include "util/rand.h"
#include <sf_libs/CLI11.hpp>

int main(int argc, char** argv) {

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
    args.add_flag("--use_ar", set.w_from_ar,
                  "Compute output image width based on camera AR (if headless)");
    args.add_option("--depth", set.d, "Maximum ray depth (if headless)");
    args.add_option("--samples", set.s, "Pixel samples (if headless)");
    args.add_option("--exposure", set.exp, "Output exposure (if headless)");

    CLI11_PARSE(args, argc, argv);

    if(!set.headless) {
        Platform plt;
        App app(set, &plt);
        plt.loop(app);
    } else {
        App app(set);
    }
    return 0;
}
