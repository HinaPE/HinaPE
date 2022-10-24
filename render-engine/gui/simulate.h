#ifndef HINAPE_SIMULATE_H
#define HINAPE_SIMULATE_H

#include "../rays/pathtracer.h"
#include "../scene/particles.h"
#include "../util/thread_pool.h"

#include "widgets.h"
#include <SDL2/SDL.h>

namespace Gui
{

enum class Mode;
class Manager;

enum class Solid_Type : int
{
    sphere, cube, cylinder, torus, custom, count
};
extern const char *Solid_Type_Names[(int) Solid_Type::count];

class Simulate
{
public:
    Simulate();
    ~Simulate();
    auto keydown(Widgets &widgets, Undo &undo, SDL_Keysym key) -> bool;

    void update(Scene &scene, Undo &undo);
    void update_time();

    void step(Scene &scene, float dt);

    void clear_particles(Scene &scene);
    void update_bvh(Scene &scene, Undo &undo);
    void build_scene(Scene &scene);

    void render(Scene_Maybe obj_opt, Widgets &widgets, Camera &cam);
    auto UIsidebar(Manager &manager, Scene &scene, Undo &undo, Widgets &widgets, Scene_Maybe obj) -> Mode;

    bool running = false;
    bool running_one_frame = false;

private:
    PT::Object scene_obj;
    bool use_bvh = true;

    Thread_Pool thread_pool;
    Pose old_pose;
    size_t cur_actions = 0;
    Uint64 last_update;

public: // HinaPE 1.1.0
    void register_custom_sidebar_UI(std::function<void(Manager &manager, Scene &scene, Undo &undo, Widgets &widgets, Scene_Maybe obj, int &index)> &&func);

private:
    std::vector<std::function<void(Manager &manager, Scene &scene, Undo &undo, Widgets &widgets, Scene_Maybe obj, int &index)>> custom_sidebar_UI;
};

} // namespace Gui

#endif
