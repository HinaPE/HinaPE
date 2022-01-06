#include <igl/opengl/glfw/Viewer.h>

#include "Core/Simulation/world.h"
#include "Core/Objects/cloth.h"
#include "Core/Objects/rigidbody.h"

#include "Visualization/viewer.h"

int main(int argc, char *argv[])
{
    HinaPE::Viewer viewer;

    std::shared_ptr<HinaPE::World> world(std::make_shared<HinaPE::World>());

    world->add_object(std::make_unique<HinaPE::Cloth>(2, 2, 2, 2));
    world->get_objects().back()->world_ = world;
    world->add_object(std::make_unique<HinaPE::Rigidbody>("starry_fish.obj"));
    world->get_objects().back()->world_ = world;

    viewer.set_world(world);

    viewer.render();
}
