#ifndef HINAPE_CAMERA_H
#define HINAPE_CAMERA_H

#include "../lib/mathlib.h"

class Camera
{
public:
    Camera(Vec2 dim);

    /**
            Returns a world-space ray from the camera that corresponds to a
            ray exiting the camera that deposits light at the sensor plane
            position given by (x,y). x and y are provided in the normalized
            coordinate space of the sensor. For example (0.5, 0.5)
            corresponds to the middle of the screen.
    */
    auto generate_ray(Vec2 screen_coord) const -> Ray;

    /// View transformation matrix
    auto get_view() const -> Mat4;
    /// Perspective projection transformation matrix
    auto get_proj() const -> Mat4;

    /// Camera position
    auto pos() const -> Vec3;
    /// Camera look position
    auto center() const -> Vec3;
    /// Camera look direction
    auto front() const -> Vec3;

    /// Get distance from the current position to the viewpoint
    auto dist() const -> float;

    /// Set camera at a position and a center to look at
    void look_at(Vec3 cent, Vec3 pos);

    /// Reset to default values
    void reset();

    /// Apply movement delta to orbit position
    void mouse_orbit(Vec2 off);
    /// Apply movement delta to look point
    void mouse_move(Vec2 off);
    /// Apply movement delta to radius (distance from look point)
    void mouse_radius(float off);

    /// Unecessary helpers
    void set_ar(float ar);
    void set_ar(Vec2 dim);
    auto get_ar() const -> float;
    void set_ap(float ap);
    auto get_ap() const -> float;
    void set_dist(float dist);
    auto get_dist() const -> float;
    void set_fov(float fov);
    auto get_fov() const -> float;
    auto get_h_fov() const -> float;
    auto get_near() const -> float;

    // swap orbit behavior for vertical mouse motion
    bool orbit_flip_vertical = false;

private:
    void update_pos();

    /// Camera parameters
    Vec3 position, looking_at;
    /// FOV is in degrees
    float vert_fov, aspect_ratio;
    /// Current camera rotation
    Quat rot;

    /// For updating position & looking_at
    float radius, near_plane;
    /// For mouse control
    float orbit_sens, move_sens, radius_sens;
    /// Lens parameters
    float aperture, focal_dist;

    /// Cached view matrices
    Mat4 view, iview;
};

#endif
