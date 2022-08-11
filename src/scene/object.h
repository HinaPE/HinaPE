#ifndef HINAPE_SCENE_OBJECT_H
#define HINAPE_SCENE_OBJECT_H

#include "../geometry/halfedge.h"
#include "../platform/gl.h"
#include "../rays/shapes.h"
#include "../physics/physics_system.h"

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
    Scene_ID id() const;

    BBox bbox();
    const GL::Mesh &mesh();
    const GL::Mesh &posed_mesh();
    Halfedge_Mesh &get_mesh();
    const Halfedge_Mesh &get_mesh() const;

    bool is_shape() const;
    bool is_editable() const;
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
    Halfedge_Mesh::ElementRef set_mesh(Halfedge_Mesh &in, unsigned int eid);

    void try_make_editable(PT::Shape_Type prev = PT::Shape_Type::none);
    void flip_normals();

    void step(const PT::Object &scene, float dt)
    {
//        for (int i = 0; i < sub_iteration; ++i)
//            HinaPE::PhysicsSystem::instance()._tick_(dt / (float) sub_iteration); // TODO: move this to separate physics thread; Hina
//        sync_physics_result();
        // step 1: 球体自由落体运动
            //1111
            Vec3 p = pose.pos;

            Vec3 newV = v + a  * dt;
            Vec3 newP = p + newV * dt;

            pose.pos = newP;
            v = newV;
            set_pose_dirty();
        // step 2: 添加隐形的地板 (0.f, -5.f, 0.f)
            //2222
            Vec3 floor = Vec3(0.f, -5.f, 0.f);
            if(pose.pos.y <= floor.y){
                v.y = -v.y;
            }
        // step 3: 添加初速度，添加四周六堵墙
            Vec3 top = Vec3(0.f, 5.f, 0.f);
            if(pose.pos.y >= top.y){
                v.y = -v.y;
            }
            Vec3 left = Vec3(-5.f, 0.f, 0.f);
            if(pose.pos.x <= left.x){
                v.x = -v.x;
            }
            Vec3 right = Vec3(5.f, -0.f, 0.f);
            if(pose.pos.x >= right.x){
                v.x = -v.x;
            }
            Vec3 front = Vec3(0.f, 0.f, -8.f);
            if(pose.pos.z <= front.z){
                v.z = -v.z;
            }
            Vec3 back = Vec3(0.f, 0.f, 8.f);
            if(pose.pos.z >= back.z){
                v.z = -v.z;
            }
            set_pose_dirty();
    }

public:
    Scene_Object() = default;
    Scene_Object(Scene_ID id, Pose pose, GL::Mesh &&mesh, std::string n = {});
    Scene_Object(Scene_ID id, Pose pose, Halfedge_Mesh &&mesh, std::string n = {});
    ~Scene_Object() = default;
    Scene_Object(const Scene_Object &src) = delete;
    void operator=(const Scene_Object &src) = delete;
    Scene_Object(Scene_Object &&src) = default;
    Scene_Object &operator=(Scene_Object &&src) = default;

public:
    void attach_physics_object(std::shared_ptr<HinaPE::PhysicsObject> o);
    void remove_physics_object();
    void sync_physics_result();
    HinaPE::PhysicsObjectType get_physics_object_type() const;
    std::shared_ptr<HinaPE::PhysicsObject> physics_object;
    int sub_iteration = 10;

public:

    struct Options
    {
        char name[MAX_NAME_LEN] = {};
        bool surface = true;
        bool wireframe = false;
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
    Vec3 v = Vec3(0.f,0.f,0.f);
    Vec3 a = Vec3(5.f,-9.8f,6.f);
};

bool operator!=(const Scene_Object::Options &l, const Scene_Object::Options &r);

#endif
