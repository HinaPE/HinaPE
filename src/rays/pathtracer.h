
#pragma once

#include <atomic>
#include <mutex>
#include <unordered_map>

#include "../lib/mathlib.h"
#include "../scene/scene.h"
#include "../util/hdr_image.h"
#include "../util/thread_pool.h"

#include "bsdf.h"
#include "env_light.h"
#include "light.h"
#include "object.h"

namespace Gui {
class Widget_Render;
}

namespace PT {

class Pathtracer {
public:
    Pathtracer(Gui::Widget_Render& gui, Vec2 screen_dim);
    ~Pathtracer();

    void set_params(size_t w, size_t h, size_t pixel_samples, size_t depth, bool use_bvh);
    void set_samples(size_t samples);

    const HDR_Image& get_output();
    const GL::Tex2D& get_output_texture(float exposure);
    size_t visualize_bvh(GL::Lines& lines, GL::Lines& active, size_t level);

    void begin_render(Scene& scene, const Camera& camera, bool add_samples = false);
    void cancel();
    bool in_progress() const;
    float progress() const;
    std::pair<float, float> completion_time() const;

private:
    struct Shading_Info {
        const BSDF& bsdf;
        Mat4 world_to_object, object_to_world;
        Vec3 pos, out_dir, normal;
        size_t depth = 0;
    };

    void build_scene(Scene& scene);
    void build_lights(Scene& scene);
    void do_trace(size_t samples);
    void accumulate(const HDR_Image& sample);
    bool tonemap();

    Gui::Widget_Render& gui;
    unsigned long long render_time, build_time;
    Thread_Pool thread_pool;
    bool cancel_flag = false;

    HDR_Image accumulator;
    std::mutex accumulator_mut;
    size_t total_epochs, accumulator_samples;
    std::atomic<size_t> completed_epochs;

    Spectrum trace_pixel(size_t x, size_t y);
    Spectrum sample_direct_lighting(const Shading_Info& hit);
    Spectrum sample_indirect_lighting(const Shading_Info& hit);

    std::pair<Spectrum, Spectrum> trace(const Ray& ray);
    Spectrum point_lighting(const Shading_Info& hit);
    Vec3 sample_area_lights(Vec3 from);
    float area_lights_pdf(Vec3 from, Vec3 dir);

    void log_ray(const Ray& ray, float t, Spectrum color = Spectrum{1.0f});

    Object scene;
    List<Object> area_lights;
    bool scene_use_bvh = true;

    std::vector<BSDF> materials;
    std::vector<Delta_Light> point_lights;
    std::optional<Env_Light> env_light;

    Camera camera;
    size_t out_w, out_h, n_samples, max_depth;
};

} // namespace PT
