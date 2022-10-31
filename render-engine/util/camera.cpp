#include "camera.h"

const Vec3 UP = Vec3{0.0f, 1.0f, 0.0f};

Camera::Camera(Vec2 dim)
{
    reset();
    set_ar(dim);
}

auto Camera::get_view() const -> Mat4
{
    return view;
}

auto Camera::get_proj() const -> Mat4
{
    return Mat4::project(vert_fov, aspect_ratio, near_plane);
}

auto Camera::pos() const -> Vec3
{
    return position;
}

auto Camera::front() const -> Vec3
{
    return (looking_at - position).unit();
}

auto Camera::dist() const -> float
{
    return (position - looking_at).norm();
}

void Camera::look_at(Vec3 cent, Vec3 pos)
{
    position = pos;
    looking_at = cent;
    radius = (pos - cent).norm();
    if (dot(front(), UP) == -1.0f)
        rot = Quat::euler(Vec3{270.0f, 0.0f, 0.0f});
    else
        rot = Quat::euler(Mat4::rotate_z_to(front()).to_euler());
    update_pos();
}

void Camera::reset()
{
    vert_fov = 90.0f;
    aspect_ratio = 1.7778f;
    rot = Quat::euler(Vec3(-45.0f, 45.0f, 0.0f));
    near_plane = 0.01f;
    radius = 5.0f;
    radius_sens = 0.25f;
    move_sens = 0.015f;
    orbit_sens = 0.2f;
    aperture = 0.0f;
    focal_dist = 1.0f;
    looking_at = Vec3();
    update_pos();
}

void Camera::mouse_orbit(Vec2 off)
{
    float up_rot = -off.x * orbit_sens;
    float right_rot = -off.y * orbit_sens;

    if (orbit_flip_vertical)
    {
        right_rot = -right_rot;
    }

    Vec3 up = rot.rotate(UP);
    Vec3 f = front();
    Vec3 right = cross(f, up).unit();

    rot = Quat::axis_angle(UP, up_rot) * Quat::axis_angle(right, right_rot) * rot;
    update_pos();
}

void Camera::mouse_move(Vec2 off)
{
    Vec3 up = rot.rotate(UP);
    Vec3 f = front();
    Vec3 right = cross(f, up).unit();

    looking_at += -right * off.x * move_sens + up * off.y * move_sens;
    update_pos();
}

void Camera::mouse_radius(float off)
{
    radius -= off * radius_sens;
    radius = std::max(radius, 2.0f * near_plane);
    update_pos();
}

void Camera::set_fov(float f)
{
    vert_fov = f;
}

auto Camera::get_h_fov() const -> float
{
    float vfov = Radians(vert_fov);
    float hfov = 2.0f * std::atan(aspect_ratio * std::tan(vfov / 2.0f));
    return Degrees(hfov);
}

auto Camera::get_fov() const -> float
{
    return vert_fov;
}

auto Camera::get_ar() const -> float
{
    return aspect_ratio;
}

auto Camera::get_near() const -> float
{
    return near_plane;
}

auto Camera::center() const -> Vec3
{
    return looking_at;
}

void Camera::set_ar(float a)
{
    aspect_ratio = a;
}

void Camera::set_ar(Vec2 dim)
{
    aspect_ratio = dim.x / dim.y;
}

void Camera::set_ap(float ap)
{
    aperture = ap;
}

auto Camera::get_ap() const -> float
{
    return aperture;
}

void Camera::set_dist(float dist)
{
    focal_dist = dist;
}

auto Camera::get_dist() const -> float
{
    return focal_dist;
}

void Camera::update_pos()
{
    position = rot.rotate(Vec3{0.0f, 0.0f, 1.0f});
    position = looking_at + radius * position.unit();
    iview = Mat4::translate(position) * rot.to_mat();
    view = iview.inverse();
}
