#include "world.h"

#include <algorithm>

using namespace HinaPE;

void World::simulate()
{
    for (auto &o: world_objects_)
    {
        o->tick(0.02);
    }
}

std::vector<std::unique_ptr<Object>> &World::get_objects()
{
    return world_objects_;
}

void World::add_object(std::unique_ptr<Object> &&o)
{
    world_objects_.emplace_back(std::forward<std::unique_ptr<Object>>(o));
}

std::unique_ptr<Object> &World::get_object_byID(unsigned int ID)
{
    static std::unique_ptr<Object> null_object = nullptr;

    /// Not efficient enough, but convenient
    for (auto &o: world_objects_)
        if (o->ID_ == ID)
            return o;

    return null_object;
}
