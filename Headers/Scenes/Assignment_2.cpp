#include "Assignment_2.h"

void Assignment_2::init() {

}

void Assignment_2::load() {
    std::string SHADER_DIRECTORY = PROJECT_SHADER_DIR;
    std::string v_path = SHADER_DIRECTORY + "/" + "default_shader_3D.vert";
    std::string f_path = SHADER_DIRECTORY + "/" + "default_shader_3D.frag";

    std::shared_ptr<Shader> default_shader_3D = std::make_shared<Shader>(v_path, f_path);

    auto ball = std::make_unique<Ball>(1);
    ball->shader = default_shader_3D;
    ball->set_position(Eigen::Vector3f(0,0,0));

    objects.push_back(std::move(ball));
}

void Assignment_2::update() {

}

void Assignment_2::render(float delta_time) {
    for (auto &obj : objects) {
        if (scene_state != STATE_PAUSE) {
//            EulerSolver::ForwardEuler(obj, objects, delta_time);
        }
        obj->set_vp(view, projection);
        obj->set_ortho(ortho);
        obj->draw();
    }
}

void Assignment_2::terminate() {

}
