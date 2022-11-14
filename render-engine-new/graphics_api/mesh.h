#ifndef HINAPE_RENDER_ENGINE_PRO_MESH_H
#define HINAPE_RENDER_ENGINE_PRO_MESH_H

#include "math_api.h"

namespace HinaPE::RenderEngine
{
class Texture
{
public:
    Texture(const std::string &path);
    Texture(const Texture &src) = delete;
    Texture(Texture &&src) noexcept = default;
    ~Texture() = default;
    void operator=(const Texture &src) = delete;
    auto operator=(Texture &&src) noexcept -> Texture & = default;

public:
    void bind(int texture_idx = 0) const;

private:
    unsigned int ID;
    int width, height, nr_channels;
};
using TexturePtr = std::shared_ptr<Texture>;

class ColoredMesh final {};

class TexturedMesh final
{
public:
    using Index = unsigned int;
    struct Vertex
    {
        mVector3 position;
        mVector3 normal;
        mVector2 tex_coord;
        unsigned int id;
    };

public:
    TexturedMesh(std::vector<Vertex> &&vertices, std::vector<Index> &&indices);
    TexturedMesh(const std::string &path);
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
    unsigned int VAO, VBO, EBO;
    size_t n_elem;

    std::vector<Vertex> _verts;
    std::vector<Index> _idxs;
    std::vector<TexturePtr> _textures;
    mBBox _bbox;
};
using TexturedMeshPtr = std::shared_ptr<TexturedMesh>;
}

#endif //HINAPE_RENDER_ENGINE_PRO_MESH_H
