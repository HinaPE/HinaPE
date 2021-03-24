#include "World2D.h"

using namespace W2D;

World2D::World2D() {
    init();
}

World2D::World2D(Eigen::Vector2f gravity) : gravity(std::move(gravity)) {
    init();
}

void World2D::add(std::unique_ptr<BasicShape2D> &&obj) {
    objects.push_back(std::move(obj));
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

void World2D::display() {
    renderer->draw(objects);
}

void World2D::init() {
    renderer = std::make_unique<Renderer2D>();
}