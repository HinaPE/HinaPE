#include "Assignment_3.h"

void Assignment_3::init() {

}

void Assignment_3::load() {

    /* Step 1: load shaders */
    std::string SHADER_DIRECTORY = PROJECT_SHADER_DIR;
    std::string v_path = SHADER_DIRECTORY + "/" + "default_shader_2D.vert";
    std::string f_path = SHADER_DIRECTORY + "/" + "default_shader_2D.frag";
    std::shared_ptr<Shader> default_shader_2D = std::make_shared<Shader>(v_path, f_path);

    mass_spring_system = std::make_unique<MassSpringTEMPLATE>();
    mass_spring_system->init(default_shader_2D, default_shader_2D);
}

void Assignment_3::update() {

}

void Assignment_3::render(float delta_time) {
    EulerSolver::ForwardEulerMassSpring(mass_spring_system, delta_time);
    mass_spring_system->draw(view, projection, ortho, delta_time);
}

void Assignment_3::terminate() {

}
