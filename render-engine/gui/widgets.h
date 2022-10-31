#ifndef HINAPE_WIDGETS_H
#define HINAPE_WIDGETS_H

#include "../lib/mathlib.h"
#include "../rays/pathtracer.h"
#include "../scene/scene.h"

class Undo;
struct Launch_Settings;

namespace Gui
{

class Animate;

enum class Axis
{
    X, Y, Z
};

enum class Widget_Type
{
    move, rotate, scale, bevel, extrude, count
};
static const int n_Widget_Types = (int) Widget_Type::count;

enum class Widget_IDs : Scene_ID
{
    none, x_mov, y_mov, z_mov, xy_mov, yz_mov, xz_mov, x_rot, y_rot, z_rot, x_scl, y_scl, z_scl, xyz_scl, count
};
static const int n_Widget_IDs = (int) Widget_IDs::count;

class Widget_Camera
{
public:
    Widget_Camera(Vec2 screen_dim) : screen_dim(screen_dim), render_cam(screen_dim), saved_cam(screen_dim)
    {
        generate_cage();
    }

    auto UI(Undo &undo, Camera &user_cam) -> bool;
    void render(const Mat4 &view);

    void load(Camera c);

    auto get() const -> const Camera &
    {
        return render_cam;
    }

    void ar(Camera &user_cam, float _ar);

    auto get_ar() const -> float
    {
        return cam_ar;
    }

    auto moving() const -> bool
    {
        return moving_camera;
    }

    void dim(Vec2 d)
    {
        screen_dim = d;
    }

private:
    float cam_fov = 90.0f, cam_ar = 1.7778f, cam_ap = 0.0f, cam_dist = 1.0f;
    bool moving_camera = false;
    Vec2 screen_dim;
    Camera render_cam, saved_cam;
    GL::Lines cam_cage;

    Camera old = render_cam;
    float old_ar, old_fov, old_ap, old_dist;

    void update_cameras(Camera &user_cam);
    void generate_cage();
};

class Widget_Render
{
public:
    Widget_Render(Vec2 dim);
    void open();

    auto UI(Scene &scene, Widget_Camera &cam, Camera &user_cam, std::string &err) -> bool;

    void animate(Scene &scene, Widget_Camera &cam, Camera &user_cam, int max_frame);
    auto step(Animate &animate, Scene &scene) -> std::string;

    auto headless(Animate &animate, Scene &scene, const Camera &cam, const Launch_Settings &set) -> std::string;

    void log_ray(const Ray &ray, float t, Spectrum color = Spectrum{1.0f});
    void render_log(const Mat4 &view) const;

    auto tracer() -> PT::Pathtracer & { return pathtracer; }

    auto rendered() const -> bool { return has_rendered; }

    auto completion_time() const -> std::pair<float, float> { return pathtracer.completion_time(); }

    auto in_progress() const -> bool { return pathtracer.in_progress() || animating; }

    auto wh_ar() const -> float { return (float) out_w / (float) out_h; }

private:
    void begin(Scene &scene, Widget_Camera &cam, Camera &user_cam);

    mutable std::mutex log_mut;
    GL::Lines ray_log;

    int out_w, out_h, out_samples = 32, out_depth = 8;
    float exposure = 1.0f;
    bool use_bvh = true;

    bool has_rendered = false;
    bool render_window = false, render_window_focus = false;

    int method = 1;
    bool animating = false, init = false;
    int next_frame = 0, max_frame = 0;

    char output_path[256] = {};
    std::string folder;

    GL::MSAA msaa;
    PT::Pathtracer pathtracer;
};

class Widgets
{
public:
    Widgets();
    Widget_Type active = Widget_Type::move;

    void end_drag();
    auto apply_action(const Pose &pose) -> Pose;
    void start_drag(Vec3 pos, Vec3 cam, Vec2 spos, Vec3 dir);
    void drag_to(Vec3 pos, Vec3 cam, Vec2 spos, Vec3 dir, bool scale_invert);

    void select(Scene_ID id);
    void render(const Mat4 &view, Vec3 pos, float scl);
    auto action_button(Widget_Type act, std::string name, bool wrap = true) -> bool;

    auto want_drag() -> bool;
    auto is_dragging() -> bool;

private:
    void generate_lines(Vec3 pos);
    auto to_axis(Vec3 obj_pos, Vec3 cam_pos, Vec3 dir, Vec3 &hit) -> bool;
    auto to_plane(Vec3 obj_pos, Vec3 cam_pos, Vec3 dir, Vec3 norm, Vec3 &hit) -> bool;
    auto to_axis3(Vec3 obj_pos, Vec3 cam_pos, Vec3 dir, Vec3 &hit) -> bool;
    // interface data
    Axis axis = Axis::X;
    Vec3 drag_start, drag_end;
    Vec2 bevel_start, bevel_end;
    bool dragging = false, drag_plane = false;
    bool start_dragging = false;
    bool univ_scl = false;
    // render data
    GL::Lines lines;
    Scene_Object x_mov, y_mov, z_mov;
    Scene_Object xy_mov, yz_mov, xz_mov;
    Scene_Object x_rot, y_rot, z_rot;
    Scene_Object x_scl, z_scl, y_scl;
    Scene_Object xyz_scl;
};

} // namespace Gui

#endif
