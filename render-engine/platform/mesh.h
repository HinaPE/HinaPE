#ifndef HINAPE_MESH_H
#define HINAPE_MESH_H

#include <glad/glad.h>

#include "../lib/mathlib.h"

#include <vector>
#include <iostream>
#include <string>
#include <memory>

namespace HinaPE::RenderEngine
{

using Vector3 = Vec3;
using Vector2 = Vec2;

class Texture final
{
public:
    GLuint ID;

public:
    Texture(const std::string &path);
    Texture(const Texture &texture) = delete;
    Texture(Texture &&texture) noexcept = default;
    ~Texture() = default;
    auto operator=(const Texture &texture) -> Texture & = delete;
    auto operator=(Texture &&texture) noexcept -> Texture & = default;

public:
    void bind(int texture_idx = 0) const;

private:
    int width, height, nr_channels;
};
using TexturePtr = std::shared_ptr<Texture>;

class TexturedMesh final
{
public:
    using Index = GLuint;
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector2 tex_coord;
        GLuint id;
    };

public:
    TexturedMesh(std::vector<Vertex> &&vertices, std::vector<Index> &&indices);
    TexturedMesh(const TexturedMesh &src) = delete;
    TexturedMesh(TexturedMesh &&src) noexcept = default;
    ~TexturedMesh();
    void operator=(const TexturedMesh &src) = delete;
    auto operator=(TexturedMesh &&src) noexcept -> TexturedMesh & = default;

public:
    void render();
    void update();
    void attach_texture(const TexturePtr &texture);

private:
    bool dirty;
    GLuint VAO, VBO, EBO;
    GLuint n_elem;

    std::vector<Vertex> _verts;
    std::vector<Index> _idxs;
    std::vector<TexturePtr> _textures;
    BBox _bbox;
};
using TexturedMeshPtr = std::shared_ptr<TexturedMesh>;
}

#endif //HINAPE_MESH_H
