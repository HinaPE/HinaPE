#include "Scene01.h"

void Scene01::load() {
    std::string SHADER_DIRECTORY = PROJECT_SHADER_DIR;
    std::string vertex_triangles_shader_path = SHADER_DIRECTORY + "/3D/" + "default_triangles_shader.vert";
    std::string fragment_triangles_shader_path = SHADER_DIRECTORY + "/3D/" + "default_triangles_shader.frag";

    auto plane = std::make_unique<Plane>();
    plane->load_shader(vertex_triangles_shader_path, fragment_triangles_shader_path);



    auto sphere = std::make_unique<Sphere>();
    sphere->load_shader(vertex_triangles_shader_path, fragment_triangles_shader_path);



    this->objects.push_back(std::move(plane));
    this->objects.push_back(std::move(sphere));
}
