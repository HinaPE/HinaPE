#ifndef HINAPE_MESH_H
#define HINAPE_MESH_H

#include <glad/glad.h>

#include "../lib/mathlib.h"

namespace HinaPE::RenderEngine
{

using Vector3 = Vec3;
using Vector2 = Vec2;

class TexturedMesh
{
public:
    TexturedMesh() = default; // Empty Constructor;
    TexturedMesh(const TexturedMesh &src) = delete;
    TexturedMesh(TexturedMesh &&src) noexcept;
    ~TexturedMesh();
    void operator=(const TexturedMesh &src) = delete;
    auto operator=(TexturedMesh &&src) noexcept -> TexturedMesh &;

public:
    using Index = GLuint;
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector2 tex_coord;
        GLuint id;
    };
};
}

#endif //HINAPE_MESH_H
