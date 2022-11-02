#ifndef HINAPE_RENDERER_WORKBENCH_H
#define HINAPE_RENDERER_WORKBENCH_H

#include "../lib/bbox.h"
#include "../platform/gl.h"
#include "../platform/mesh.h"

namespace HinaPE::RenderEngine
{
class RendererWorkbench final
{
public:
    struct MeshOpt
    {

    };

public:
    void render_mesh(const TexturedMesh &mesh, const MeshOpt &opt);

private:
    GL::Shader _mesh_shader;
};
}

#endif //HINAPE_RENDERER_WORKBENCH_H
