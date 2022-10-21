#include "pose.h"

auto Pose::transform() const -> Mat4
{
    return Mat4::translate(pos) * rotation_mat() * Mat4::scale(scale);
}

auto Pose::rotation_mat() const -> Mat4
{
    return Mat4::euler(euler);
}

auto Pose::rotation_quat() const -> Quat
{
    return Quat::euler(euler);
}

auto Pose::valid() const -> bool
{
    return pos.valid() && euler.valid() && scale.valid();
}

void Pose::clamp_euler()
{
    if (!valid())
        return;
    while (euler.x < 0)
        euler.x += 360.0f;
    while (euler.x >= 360.0f)
        euler.x -= 360.0f;
    while (euler.y < 0)
        euler.y += 360.0f;
    while (euler.y >= 360.0f)
        euler.y -= 360.0f;
    while (euler.z < 0)
        euler.z += 360.0f;
    while (euler.z >= 360.0f)
        euler.z -= 360.0f;
}

auto Pose::rotated(Vec3 angles) -> Pose
{
    return Pose{Vec3{}, angles, Vec3{1.0f, 1.0f, 1.0f}};
}

auto Pose::moved(Vec3 t) -> Pose
{
    return Pose{t, Vec3{}, Vec3{1.0f, 1.0f, 1.0f}};
}

auto Pose::scaled(Vec3 s) -> Pose
{
    return Pose{Vec3{}, Vec3{}, s};
}

auto Pose::id() -> Pose
{
    return Pose{Vec3{}, Vec3{}, Vec3{1.0f, 1.0f, 1.0f}};
}

auto operator==(const Pose &l, const Pose &r) -> bool
{
    return l.pos == r.pos && l.euler == r.euler && l.scale == r.scale;
}

auto operator!=(const Pose &l, const Pose &r) -> bool
{
    return l.pos != r.pos || l.euler != r.euler || l.scale != r.scale;
}

auto Anim_Pose::at(float t) const -> Pose
{
    auto [p, r, s] = splines.at(t);
    return Pose{p, r.to_euler(), s};
}

void Anim_Pose::set(float t, Pose p)
{
    splines.set(t, p.pos, Quat::euler(p.euler), p.scale);
}
