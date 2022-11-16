#ifndef HINAPE_LAYOUT_H
#define HINAPE_LAYOUT_H

#include <SDL.h>

#include "../scene/scene.h"
#include "../util/camera.h"

#include "widgets.h"

namespace Gui
{

enum class Mode;
class Manager;

class Layout
{
public:
    auto keydown(Widgets &widgets, SDL_Keysym key) -> bool;
    void render(Scene_Maybe obj_opt, Widgets &widgets, Camera &cam);
    void select(Scene &scene, Widgets &widgets, Scene_ID id, Vec3 cam, Vec2 spos, Vec3 dir);
    auto selected_pos(Scene &scene) -> Vec3;

    void apply_transform(Scene_Item &obj, Widgets &widgets);
    void end_transform(Undo &undo, Scene_Item &obj);
    auto UIsidebar(Manager &manager, Undo &undo, Widgets &widgets, Scene_Maybe obj) -> Mode;

    auto selected() const -> Scene_ID;
    void clear_select();
    void set_selected(Scene_ID id);

private:
    Pose old_pose;
    Scene_ID selected_mesh = (Scene_ID) Widget_IDs::none;
};

} // namespace Gui
#endif
