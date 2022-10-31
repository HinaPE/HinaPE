#ifndef HINAPE_SCENE_LIGHT_H
#define HINAPE_SCENE_LIGHT_H

#include <string>

#include "../lib/spectrum.h"
#include "../platform/gl.h"
#include "../rays/samplers.h"
#include "../util/hdr_image.h"

#include "object.h"
#include "pose.h"

enum class Light_Type : int
{
    directional, sphere, hemisphere, point, spot, count
};
extern const char *Light_Type_Names[(int) Light_Type::count];

class Scene_Light
{
public:
    Scene_Light(Light_Type type, Scene_ID id, Pose p, std::string n = {});
    Scene_Light(const Scene_Light &src) = delete;
    Scene_Light(Scene_Light &&src);
    ~Scene_Light() = default;

    void operator=(const Scene_Light &src) = delete;
    auto operator=(Scene_Light &&src) -> Scene_Light & = default;

    auto id() const -> Scene_ID;
    auto bbox() const -> BBox;

    void render(const Mat4 &view, bool depth_only = false, bool posed = true);
    void dirty();

    auto radiance() const -> Spectrum;
    void set_time(float time);

    auto emissive_load(std::string file) -> std::string;
    auto emissive_loaded() const -> std::string;
    auto emissive_copy() const -> HDR_Image;

    auto emissive_texture() const -> const GL::Tex2D &;
    void emissive_clear();
    auto is_env() const -> bool;

    struct Options
    {
        Light_Type type = Light_Type::point;
        char name[MAX_NAME_LEN] = {};
        Spectrum spectrum = Spectrum(1.0f);
        float intensity = 1.0f;
        bool has_emissive_map = false;
        Vec2 angle_bounds = Vec2(30.0f, 35.0f);
    };

    struct Anim_Light
    {
        void at(float t, Options &o) const;
        void set(float t, Options o);
        Splines<Spectrum, float, Vec2> splines;
    };

    void step(const PT::Object &scene, float dt)
    {
    }

    Options opt;
    Pose pose;
    Anim_Pose anim;
    Anim_Light lanim;

private:
    void regen_mesh();

    bool _dirty = true;
    Scene_ID _id = 0;
    GL::Mesh _mesh;
    GL::Lines _lines;
    HDR_Image _emissive;
};

auto operator!=(const Scene_Light::Options &l, const Scene_Light::Options &r) -> bool;

#endif
