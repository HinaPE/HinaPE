#include "Assignment_4.h"

void Assignment_4::init()
{
    cloth = std::make_unique<Cloth>(Eigen::Vector3f(0, 0, 0), 2, 2);
}

void Assignment_4::load()
{
    std::string SHADER_DIRECTORY = PROJECT_SHADER_DIR;
    std::string v_path = SHADER_DIRECTORY + "/" + "default_shader_3D.vert";
    std::string f_path = SHADER_DIRECTORY + "/" + "default_shader_3D.frag";

    std::shared_ptr<Shader> default_shader_3D = std::make_shared<Shader>(v_path, f_path);

    cloth->set_shader(default_shader_3D);
}

void Assignment_4::update()
{

}

void Assignment_4::render(float delta_time)
{
    cloth->draw(view, projection, delta_time);
}

void Assignment_4::terminate()
{

}
