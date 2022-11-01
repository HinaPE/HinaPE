#ifndef HINAPE_GL_H
#define HINAPE_GL_H

#include <string>
#include <unordered_map>
#include <vector>

#include "../lib/mathlib.h"
#include <glad/glad.h>

namespace GL
{

enum class Sample_Count
{
    _1, _2, _4, _8, _16, _32, count
};
extern const char *Sample_Count_Names[(int) Sample_Count::count];

struct MSAA
{
    Sample_Count samples = Sample_Count::_4;
    auto n_options() -> int;
    auto n_samples() -> int;
};

void setup();
void shutdown();
auto version() -> std::string;
auto renderer() -> std::string;

void global_params();
void clear_screen(Vec4 col);
void viewport(Vec2 dim);
auto max_msaa() -> int;

enum class Opt
{
    wireframe, offset, culling, depth_write
};

void enable(Opt opt);
void disable(Opt opt);

void color_mask(bool enable);

using TexID = GLuint;

class Tex2D
{
public:
    Tex2D();
    Tex2D(const Tex2D &src) = delete;
    Tex2D(Tex2D &&src) noexcept;
    ~Tex2D();

    void operator=(const Tex2D &src) = delete;
    auto operator=(Tex2D &&src) noexcept -> Tex2D &;

    void image(int w, int h, unsigned char *img);
    auto get_id() const -> TexID;
    void bind(int idx = 0) const;

private:
    GLuint id;
};

class Mesh
{
public:
    using Index = GLuint;
    struct Vert
    {
        Vec3 pos;
        Vec3 norm;
        GLuint id;
    };

public:
    Mesh();
    Mesh(std::vector<Vert> &&vertices, std::vector<Index> &&indices);
    Mesh(const Mesh &src) = delete;
    Mesh(Mesh &&src) noexcept;
    ~Mesh();

    void operator=(const Mesh &src) = delete;
    auto operator=(Mesh &&src) noexcept -> Mesh &;

    /// Assumes proper shader is already bound
    void render();

    void recreate(std::vector<Vert> &&vertices, std::vector<Index> &&indices);
    auto edit_verts() -> std::vector<Vert> &;
    auto edit_indices() -> std::vector<Index> &;
    auto copy() const -> Mesh;

    auto bbox() const -> BBox;
    auto verts() const -> const std::vector<Vert> &;
    auto indices() const -> const std::vector<Index> &;
    auto tris() const -> GLuint;

private:
    void update();
    void create();
    void destroy();

    BBox _bbox;
    GLuint vao = 0, vbo = 0, ebo = 0;
    GLuint n_elem = 0;
    bool dirty = true;

    std::vector<Vert> _verts;
    std::vector<Index> _idxs;

    friend class Instances;
};

class Instances
{
public:
    Instances(GL::Mesh &&mesh);
    Instances(const Instances &src) = delete;
    Instances(Instances &&src) noexcept;
    ~Instances();

    void operator=(const Instances &src) = delete;
    auto operator=(Instances &&src) noexcept -> Instances &;

    struct Info
    {
        GLuint id;
        Mat4 transform;
    };

    void render();
    auto add(const Mat4 &transform, GLuint id = 0) -> size_t;
    auto get(size_t idx) -> Info &;
    void clear(size_t n = 0);
    auto mesh() const -> const Mesh &;

private:
    void create();
    void destroy();
    void update();

    GLuint vbo = 0;
    bool dirty = false;

    Mesh _mesh;
    std::vector<Info> data;
};

class Lines
{
public:
    struct Vert
    {
        Vec3 pos;
        Vec3 color;
    };

    Lines(float thickness = 1.0f);
    Lines(std::vector<Vert> &&verts, float thickness = 1.0f);
    Lines(const Lines &src) = delete;
    Lines(Lines &&src) noexcept;
    ~Lines();

    void operator=(const Lines &src) = delete;
    auto operator=(Lines &&src) noexcept -> Lines &;

    /// Assumes proper shader is already bound
    void render(bool smooth) const;
    void add(Vec3 start, Vec3 end, Vec3 color);
    void pop();
    void clear();

private:
    void create();
    void destroy();
    void update() const;

    mutable bool dirty = false;
    float thickness = 0.0f;
    GLuint vao = 0, vbo = 0;

    std::vector<Vert> vertices;
};

class Shader
{
public:
    Shader();
    Shader(std::string vertex_file, std::string fragment_file);
    Shader(const Shader &src) = delete;
    Shader(Shader &&src) noexcept;
    ~Shader();

    void operator=(const Shader &src) = delete;
    auto operator=(Shader &&src) noexcept -> Shader &;

public:
    void bind() const;
    void load(std::string vertex, std::string fragment);

public:
    void uniform(std::string name, const Mat4 &mat) const;
    void uniform(std::string name, Vec3 vec3) const;
    void uniform(std::string name, Vec2 vec2) const;
    void uniform(std::string name, GLint i) const;
    void uniform(std::string name, GLuint i) const;
    void uniform(std::string name, GLfloat f) const;
    void uniform(std::string name, bool b) const;
    void uniform(std::string name, int count, const Vec2 items[]) const;
    void uniform_block(std::string name, GLuint i) const;

private:
    auto loc(std::string name) const -> GLuint;
    static auto validate(GLuint program) -> bool;

    GLuint program = 0;
    GLuint v = 0, f = 0;

    void destroy();
};

/// this is very restrictive; it assumes a set number of gl_rgb8 output
/// textures and a floating point depth render buffer.
class Framebuffer
{
public:
    Framebuffer();
    Framebuffer(int outputs, Vec2 dim, int samples = 1, bool depth = true);
    Framebuffer(const Framebuffer &src) = delete;
    Framebuffer(Framebuffer &&src) noexcept;
    ~Framebuffer();

    void operator=(const Framebuffer &src) = delete;
    auto operator=(Framebuffer &&src) noexcept -> Framebuffer &;

    static void bind_screen();

    void setup(int outputs, Vec2 dim, int samples, bool d);
    void resize(Vec2 dim, int samples = 1);
    void bind() const;
    auto is_multisampled() const -> bool;

    auto get_output(int buf) const -> GLuint;
    auto get_depth() const -> GLuint;
    void flush() const;
    auto samples() const -> int;
    auto bytes() const -> int;

    auto can_read_at() const -> bool;
    void read_at(int buf, int x, int y, GLubyte *data) const;
    void read(int buf, GLubyte *data) const;

    void blit_to_screen(int buf, Vec2 dim) const;
    void blit_to(int buf, const Framebuffer &fb, bool avg = true) const;

    void clear(int buf, Vec4 col) const;
    void clear_d() const;

private:
    void create();
    void destroy();

    std::vector<GLuint> output_textures;
    GLuint depth_tex = 0;
    GLuint framebuffer = 0;

    int w = 0, h = 0, s = 0;
    bool depth = true;

    friend class Effects;
};

class Effects
{
public:
    static void resolve_to_screen(int buf, const Framebuffer &framebuffer);
    static void resolve_to(int buf, const Framebuffer &from, const Framebuffer &to, bool avg = true);

    static void outline(const Framebuffer &from, const Framebuffer &to, Vec3 color, Vec2 min, Vec2 max);

private:
    static void init();
    static void destroy();

    static inline Shader resolve_shader, outline_shader, outline_shader_ms;
    static inline GLuint vao;
    static inline const Vec2 screen_quad[] = {Vec2{-1.0f, 1.0f}, Vec2{-1.0f, -1.0f}, Vec2{1.0f, 1.0f}, Vec2{1.0f, -1.0f}};

    friend void setup();
    friend void shutdown();

    static const std::string effects_v;
    static const std::string outline_f, outline_ms_f_33, outline_ms_f_4;
    static const std::string resolve_f;
};

namespace Shaders
{
extern const std::string line_v, line_f;
extern const std::string mesh_v, mesh_f;
extern const std::string inst_v;
extern const std::string dome_v, dome_f;

} // namespace Shaders
} // namespace GL

#endif