#include "viewer.h"

#include "Core/Simulation/world.h"

using namespace HinaPE;

void Viewer::render()
{
    if (!current_world_.expired())
    {
        auto &world_objects = current_world_.lock()->get_objects();
        viewer_.data().set_mesh(world_objects.back()->get_render_particle(), world_objects.back()->get_render_mesh());
//        viewer_.data().set_face_based(true);
        viewer_.launch();
    }
}

void Viewer::set_world(std::shared_ptr<World> &current_world)
{
    current_world_ = current_world;

    viewer_.callback_pre_draw = [&](igl::opengl::glfw::Viewer &viewer) -> bool
    {
        current_world_.lock()->simulate();
        return false;
    };
}
