#include "renderer.h"

enum class Opt { wireframe, offset, culling, depth_write };

void HinaPE::RenderEngine::Workbench::Renderer::setup(int width, int height) { _instance = std::make_shared<Renderer>(width, height); }
auto HinaPE::RenderEngine::Workbench::Renderer::get() -> HinaPE::RenderEngine::Workbench::Renderer & { return *_instance; }
void HinaPE::RenderEngine::Workbench::Renderer::shutdown() { _instance.reset(); }

HinaPE::RenderEngine::Workbench::Renderer::Renderer(int width, int height) : _builtin_mesh_shader(std::make_shared<Shader>("vertex_shader_texture.glsl", "fragment_shader_texture.glsl")),
                                                                             _builtin_line_shader(std::make_shared<Shader>("vertex_shader_line.glsl", "fragment_shader_line.glsl")),
                                                                             _builtin_instance_shader(std::make_shared<Shader>("vertex_shader_inst.glsl", "fragment_shader_inst.glsl")), _builtin_framebuffer(std::make_shared<Framebuffer>(width, height)),
                                                                             _builtin_sphere(std::make_shared<TexturedMesh>("sphere.obj")), _builtin_cube(std::make_shared<TexturedMesh>("cube.obj")) {}

void HinaPE::RenderEngine::Workbench::Renderer::mesh(HinaPE::RenderEngine::ColoredMesh &mesh, const HinaPE::RenderEngine::Workbench::Renderer::RenderOpt &opt)
{

}

void HinaPE::RenderEngine::Workbench::Renderer::mesh(HinaPE::RenderEngine::TexturedMesh &mesh, const HinaPE::RenderEngine::Workbench::Renderer::RenderOpt &opt)
{
    _builtin_mesh_shader->bind();

    if (opt.surface)
    {
        mesh.render();
    }
    if (opt.wireframe)
    {
        GraphicsAPI::wireframe_mode(true);
        mesh.render();
        GraphicsAPI::wireframe_mode(false);
    }
    if (opt.bbox)
    {
        // TODO: render bbox
    }
}
