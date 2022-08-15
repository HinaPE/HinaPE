#ifndef HINAPE_POSE_H
#define HINAPE_POSE_H

#include "../geometry/spline.h"
#include "../lib/mathlib.h"
#include <set>

struct Pose
{
    Vec3 pos;
    Vec3 euler;
    Vec3 scale = Vec3{1.0f};

    auto transform() const -> Mat4;
    auto rotation_mat() const -> Mat4;
    auto rotation_quat() const -> Quat;

    void clamp_euler();
    auto valid() const -> bool;

    static auto rotated(Vec3 angles) -> Pose;
    static auto moved(Vec3 t) -> Pose;
    static auto scaled(Vec3 s) -> Pose;
    static auto id() -> Pose;
};

auto operator==(const Pose &l, const Pose &r) -> bool;
auto operator!=(const Pose &l, const Pose &r) -> bool;

struct Anim_Pose
{
    [[nodiscard]] auto at(float t) const -> Pose;
    void set(float t, Pose p);
    Splines<Vec3, Quat, Vec3> splines;
};

#endif
