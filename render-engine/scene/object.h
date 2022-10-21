#ifndef HINAPE_SCENE_OBJECT_H
#define HINAPE_SCENE_OBJECT_H

#include "../geometry/halfedge.h"
#include "../platform/gl.h"
#include "../rays/shapes.h"

#include "material.h"
#include "pose.h"
#include "skeleton.h"

using Scene_ID = unsigned int;
constexpr int MAX_NAME_LEN = 256;

namespace PT
{
template<typename T>
class BVH;
class Object;
} // namespace PT

class Scene_Object
{
public:
    auto id() const -> Scene_ID;

    auto bbox() -> BBox;
    auto mesh() -> const GL::Mesh &;
    auto posed_mesh() -> const GL::Mesh &;
    auto get_mesh() -> Halfedge_Mesh &;
    auto get_mesh() const -> const Halfedge_Mesh &;

    auto is_shape() const -> bool;
    auto is_editable() const -> bool;
    void set_mesh_dirty();
    void set_skel_dirty();
    void set_pose_dirty();
    void set_time(float time);

    void sync_mesh();
    void sync_anim_mesh();

    void render(const Mat4 &view, bool solid = false, bool depth_only = false, bool posed = true, bool anim = true);

    void copy_mesh(Halfedge_Mesh &out);
    void take_mesh(Halfedge_Mesh &&in);
    void set_mesh(Halfedge_Mesh &in);
    auto set_mesh(Halfedge_Mesh &in, unsigned int eid) -> Halfedge_Mesh::ElementRef;

    void try_make_editable(PT::Shape_Type prev = PT::Shape_Type::none);
    void flip_normals();

    void step(const PT::Object &scene, float dt)
    {
        // TODO: temporary disable physics module
//        sync_physics_result();
    }

public:
    Scene_Object() = default;
    Scene_Object(Scene_ID id, Pose pose, GL::Mesh &&mesh, std::string n = {});
    Scene_Object(Scene_ID id, Pose pose, Halfedge_Mesh &&mesh, std::string n = {});
    ~Scene_Object() = default;
    Scene_Object(const Scene_Object &src) = delete;
    void operator=(const Scene_Object &src) = delete;
    Scene_Object(Scene_Object &&src) = default;
    auto operator=(Scene_Object &&src) -> Scene_Object & = default;

public:
    // TODO: temporary disable physics module
    /*
    void attach_physics_object(std::shared_ptr<HinaPE::PhysicsObject> o);
    void remove_physics_object();
    void sync_physics_result();
    auto get_physics_object_type() const -> HinaPE::PhysicsObjectType;
    std::shared_ptr<HinaPE::PhysicsObject> physics_object;
    */

public:

    struct Options
    {
        char name[MAX_NAME_LEN] = {};
        bool surface = true;
        bool wireframe = false;
        bool bbox = false;
        bool smooth_normals = false;
        bool render = true;
        PT::Shape_Type shape_type = PT::Shape_Type::none;
        PT::Shape shape;
    };

    Options opt;
    Pose pose;
    Anim_Pose anim;
    Skeleton armature;
    Material material;

    mutable bool editable = true;
    mutable bool mesh_dirty = false;
    mutable bool skel_dirty = false;
    mutable bool pose_dirty = false;
    mutable bool rig_dirty = false;

private:
    Scene_ID _id = 0;
    Halfedge_Mesh halfedge;

    mutable GL::Mesh _mesh, _anim_mesh;
    mutable std::vector<std::vector<Joint *>> vertex_joints;
private:
    Vec3 v;
    Vec3 a;
    Vec3 p;
};

auto operator!=(const Scene_Object::Options &l, const Scene_Object::Options &r) -> bool;

#endif
