#include "Scene03.h"

void Scene03::init() {

}

void Scene03::load() {
    auto wall = std::make_unique<Wall>(800);
    wall->load_shader("default_shader_2D.vert", "default_shader_2D.frag");
    wall->set_position(Eigen::Vector3f(0.f, 0.f, 0.f));
}

void Scene03::update() {

}

void Scene03::render(float delta_time) {

}

void Scene03::terminate() {

}
