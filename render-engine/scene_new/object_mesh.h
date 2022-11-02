#ifndef HINAPE_OBJECT_MESH_H
#define HINAPE_OBJECT_MESH_H

#include "../platform/mesh.h"
#include "../geometry/halfedge.h"
#include "../platform/gl.h"
#include "../rays/shapes.h"
#include "../scene/pose.h"
#include "../scene/renderer.h"

#include <memory>
#include <vector>
#include <string>

namespace HinaPE::RenderEngine
{
class ObjectMesh final
{
public:
    unsigned int ID = 0;

public:
    struct Opt
    {
        std::string name = "Anonymous Mesh Object";
        bool render_surface = true;
        bool render_wireframe = false;
        bool render_normals = false;
        bool render_bbox = false;
    };

public:
    ObjectMesh() = default;
    ObjectMesh(const ObjectMesh &src) = delete;
    ObjectMesh(ObjectMesh &&src) noexcept = default;
    ~
    ObjectMesh() = default;
    auto operator=(const ObjectMesh &src) -> ObjectMesh & = delete;
    auto operator=(ObjectMesh &&src) noexcept -> ObjectMesh & = default;

public:
    void render(const Mat4 &view, bool solid = false, bool depth_only = false, bool posed = true, bool anim = true);

private:
    Pose pose;
    mutable std::shared_ptr<TexturedMesh> _mesh;
};
using ObjectMeshPtr = std::shared_ptr<ObjectMesh>;
}

#endif //HINAPE_OBJECT_MESH_H
