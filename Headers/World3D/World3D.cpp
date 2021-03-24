#include "World3D.h"

using namespace W3D;

World3D::World3D() {
    init();
}

World3D::World3D(Eigen::Vector3f gravity) : gravity(std::move(gravity)) {
    init();
}

void World3D::add(std::unique_ptr<BasicShape3D> &&obj) {
    objects.push_back(std::move(obj));
}

void World3D::solve() {
    if (on_somulation)
        for (int i = 0; i < interations_per_frame; ++i)
            for (auto &obj : objects)
                if (!obj->fixed) {
                    obj->force += gravity;
                    obj->iterate(time_step);
                }
}

void World3D::display() {
    renderer->draw(objects);
}

void World3D::init() {
    renderer = std::make_unique<Renderer3D>();
}
