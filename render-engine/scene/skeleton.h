#ifndef HINAPE_SKELETON_H
#define HINAPE_SKELETON_H

#include <functional>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "../geometry/spline.h"
#include "../lib/mathlib.h"
#include "../platform/gl.h"

class Joint
{
public:
    Joint(unsigned int id);
    Joint(unsigned int id, Joint *parent, Vec3 extent);
    ~Joint();

    Joint(const Joint &src) = delete;
    Joint(Joint &&src) = default;

    void operator=(const Joint &src) = delete;
    auto operator=(Joint &&src) -> Joint & = default;

    auto id() const -> unsigned int;

    // Checks if this joint is a root node
    auto is_root() const -> bool;

    // Current joint rotation Euler angles in degrees
    Vec3 pose;

    // The vector representing the direction and length of the bone.
    // This is specified in Joint space, and defines the origin of child bones.
    Vec3 extent = Vec3(0.0f, 1.0f, 0.0f);

    // The distance at which the joint segment should stop effecting vertices
    float radius = 0.25f;

private:
    // Builds the transformation matrix that takes a point in joint space to the skeleton
    // space (in bind position). "Bind" position implies that the rotation of all joints
    // should be zero. Also note that this does not include the Skeleton's base_pos,
    // but it should include the transformations of the joint heirachy up to this point.
    auto joint_to_bind() const -> Mat4;

    // Similarly, builds the transformation that takes a point in the space of this joint
    // into skeleton space - taking into account the poses of the joint heirarchy. This also does
    // not include the Skeleton's base_pos.
    auto joint_to_posed() const -> Mat4;

    // Pointer to parent joint in the joint heirarchy
    Joint *parent = nullptr;

    // Set of child joints - owned by this joint (could be shared_ptr and everything else weak_ptr)
    std::unordered_set<Joint *> children;

    // Current angle gradient for IK
    Vec3 angle_gradient;

    // Call this function recursively on your parent. At each call, compute the angle gradient for
    // the joint corresponding to that call.
    void compute_gradient(Vec3 target, Vec3 current);

    void for_joints(const std::function<void(Joint *)>& func);

    unsigned int _id = 0;
    Spline<Quat> anim;

    friend class Skeleton;
    friend class Scene;
};

class Skeleton
{
public:
    struct IK_Handle
    {

        /// The position of the handle (target) in skeleton space
        Vec3 target;
        /// The joint that should reach toward this handle
        Joint *joint = nullptr;

        /// Not used in step_ik
        Splines<Vec3, bool> anim;
        bool enabled = false;
        unsigned int _id = 0;
    };

    Skeleton();
    Skeleton(unsigned int obj_id);
    ~Skeleton();

    Skeleton(const Skeleton &src) = delete;
    Skeleton(Skeleton &&src) = default;

    void operator=(const Skeleton &src) = delete;
    auto operator=(Skeleton &&src) -> Skeleton & = default;

    ////////////////////////////////////////////
    // You will implement these functions

    auto end_of(Joint *j) -> Vec3;
    auto posed_end_of(Joint *j) -> Vec3;

    void step_ik(std::vector<IK_Handle *> active_handles);

    auto joint_to_bind(const Joint *j) const -> Mat4;
    auto joint_to_posed(const Joint *j) const -> Mat4;

    void find_joints(const GL::Mesh &src, std::vector<std::vector<Joint *>> &map);
    void skin(const GL::Mesh &input, GL::Mesh &output, const std::vector<std::vector<Joint *>> &map);

    ////////////////////////////////////////////

    auto base() -> Vec3 &;
    auto has_bones() const -> bool;
    auto n_bones() -> unsigned int;
    auto n_handles() -> unsigned int;

    auto parent(Joint *j) -> Joint *;
    auto get_joint(unsigned int id) -> Joint *;
    void erase(Joint *j);
    void restore(Joint *j);
    auto base_of(Joint *j) -> Vec3;
    auto posed_base_of(Joint *j) -> Vec3;

    void for_joints(const std::function<void(Joint *)>& func);
    void for_handles(const std::function<void(IK_Handle *)>& func);

    void erase(IK_Handle *handle);
    void restore(IK_Handle *handle);
    auto get_handle(unsigned int id) -> IK_Handle *;
    auto add_handle(Vec3 pos, Joint *j) -> IK_Handle *;
    auto do_ik() -> bool;

    auto add_root(Vec3 extent) -> Joint *;
    auto add_child(Joint *j, Vec3 extent) -> Joint *;
    auto is_root_id(unsigned int id) -> bool;

    auto set_time(float time) -> bool;
    void render(const Mat4 &view, Joint *jselect, IK_Handle *hselect, bool root, bool posed, unsigned int offset = 0);
    void outline(const Mat4 &view, const Mat4 &model, bool root, bool posed, BBox &box, unsigned int offset = 0);

    void set(float t);
    void crop(float t);
    void erase(float t);
    auto has_keyframes() -> bool;
    auto has(float t) -> bool;
    auto keys() -> std::set<float>;

    using J_IK_Spline = std::variant<Spline<Quat>, Splines<Vec3, bool>>;
    using J_IK_Val = std::variant<Vec3, std::pair<Vec3, bool>>;
    using VSave = std::unordered_map<unsigned int, J_IK_Val>;
    using SSave = std::unordered_map<unsigned int, J_IK_Spline>;

    auto now() -> VSave;
    auto at(float t) -> VSave;
    void set(float t, const VSave &data);
    auto splines() -> SSave;
    void restore_splines(const SSave &data);

private:
    Vec3 base_pos;
    unsigned int root_id, next_id;
    std::unordered_set<Joint *> roots;
    std::unordered_map<Joint *, std::vector<IK_Handle *>> erased;
    std::unordered_set<IK_Handle *> handles, erased_handles;
    friend class Scene;
};

#endif
