#include "World.h"

World2D::World2D() {
    init();
}

World2D::World2D(Eigen::Vector2f gravity) : gravity(std::move(gravity)) {
    init();
}

void World2D::init() {

}

void World2D::solve() {
    if (on_somulation)
        for (int i = 0; i < interations_per_frame; ++i)
            for (auto &obj : objects)
                if (!obj->fixed) {
                    obj->force += gravity;
                    obj->iterate(time_step);
                }
}
