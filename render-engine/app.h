#ifndef HINAPE_APP_H
#define HINAPE_APP_H

#include <SDL2/SDL.h>
#include <map>
#include <string>

#include "gui/manager.h"
#include "lib/mathlib.h"
#include "util/camera.h"

#include "scene/scene.h"
#include "scene/undo.h"

#include "physics-system-api.h"

class Platform;

struct Launch_Settings
{

    std::string scene_file;
    std::string env_map_file;
    bool headless = false;

    // If headless is true, use all of these
    std::string output_file = "out.png";
    int w = 640;
    int h = 360;
    int s = 256;
    int d = 8;
    bool animate = false;
    float exp = 1.0f;
    bool w_from_ar = false;
    bool no_bvh = false;
};

class App
{
public:
    explicit App(const Launch_Settings &set, Platform *plt = nullptr);
    ~App();

    void render();
    auto quit() -> bool;
    void event(SDL_Event e);

    // HinaPE 1.1.2
    void register_physics_API(const std::shared_ptr<HinaPE::PhysicsSystemAPI> &);

private:
    void apply_window_dim(Vec2 new_dim);
    auto screen_to_world(Vec2 mouse) -> Vec3;

    // Camera data
    enum class Camera_Control
    {
        none, orbit, move
    };
    Vec2 window_dim, mouse_press;
    bool selection_changed = false;
    Camera_Control cam_mode = Camera_Control::none;
    Camera camera;
    Mat4 view, proj, iviewproj;

    // Systems
    Platform *plt = nullptr;
    Scene scene;
    Gui::Manager gui;
    Undo undo;
    std::shared_ptr<HinaPE::PhysicsSystemAPI> _physics_system_api = nullptr;

    bool gui_capture = false;
};

#endif
