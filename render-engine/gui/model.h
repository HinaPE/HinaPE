#ifndef HINAPE_MODEL_H
#define HINAPE_MODEL_H

#include "widgets.h"

#include <SDL.h>
#include <optional>
#include <unordered_map>

#include "../geometry/halfedge.h"
#include "../platform/gl.h"
#include "../scene/scene.h"
#include "../util/camera.h"

namespace Gui
{

class Model
{
public:
    Model();

    // Gui view API
    auto keydown(Widgets &widgets, SDL_Keysym key, Camera &cam) -> bool;
    void unset_mesh();

    auto UIsidebar(Undo &undo, Widgets &widgets, Scene_Maybe obj, Camera &cam) -> std::string;
    void render(Scene_Maybe obj_opt, Widgets &widgets, Camera &cam);

    auto selected_pos() -> Vec3;
    void clear_select();
    void erase_selected(Undo &undo, Scene_Maybe obj_opt);
    auto end_transform(Widgets &widgets, Undo &undo, Scene_Object &obj) -> std::string;
    void apply_transform(Widgets &widgets);
    auto select(Widgets &widgets, Scene_ID click, Vec3 cam, Vec2 spos, Vec3 dir) -> std::string;

    auto shapes() -> std::tuple<GL::Mesh &, GL::Instances &, GL::Instances &, GL::Instances &>;
    auto select_id() const -> unsigned int;
    auto hover_id() const -> unsigned int;
    void hover(unsigned int id);

private:
    template<typename T>
    auto update_mesh(Undo &undo, Scene_Object &obj, Halfedge_Mesh &&before, Halfedge_Mesh::ElementRef ref, T &&op) -> std::string;
    template<typename T>
    auto update_mesh_global(Undo &undo, Scene_Object &obj, Halfedge_Mesh &&before, T &&op) -> std::string;

    void zoom_to(Halfedge_Mesh::ElementRef ref, Camera &cam);
    void begin_transform();
    auto begin_bevel(std::string &err) -> bool;
    auto begin_extrude(std::string &err) -> bool;
    void set_selected(Halfedge_Mesh::ElementRef elem);
    auto set_my_obj(Scene_Maybe obj_opt) -> std::optional<std::reference_wrapper<Scene_Object>>;
    auto selected_element() -> std::optional<Halfedge_Mesh::ElementRef>;
    void rebuild();

    void update_vertex(Halfedge_Mesh::VertexRef vert);
    void vertex_viz(Halfedge_Mesh::VertexRef v, float &size, Mat4 &transform);
    void edge_viz(Halfedge_Mesh::EdgeRef e, Mat4 &transform);
    void halfedge_viz(Halfedge_Mesh::HalfedgeRef h, Mat4 &transform);
    void face_viz(Halfedge_Mesh::FaceRef face, std::vector<GL::Mesh::Vert> &verts, std::vector<GL::Mesh::Index> &idxs, size_t insert_at);

    auto validate() -> std::string;
    std::string warn_msg, err_msg;

    // This all needs to be updated when the mesh connectivity changes
    unsigned int warn_id = 0, err_id = 0;
    unsigned int selected_elem_id = 0, hovered_elem_id = 0;

    Halfedge_Mesh *my_mesh = nullptr;
    Halfedge_Mesh old_mesh;

    enum class Bevel
    {
        face, edge, vert
    };
    Bevel beveling;

    struct Transform_Data
    {
        std::vector<Vec3> verts;
        Vec3 center;
    };

    Transform_Data trans_begin;
    GL::Instances spheres, cylinders, arrows;
    GL::Mesh face_mesh;
    Vec3 f_col = Vec3{1.0f}, v_col = Vec3{1.0f}, e_col = Vec3{0.8f}, he_col = Vec3{0.6f}, err_col = Vec3{1.0f, 0.0f, 0.0f};

    // This is a kind of bad design and would be un-necessary if we used
    // a halfedge implementation with contiguous iterators. For now this map must
    // be updated (along with the instance data) by build_halfedge whenever
    // the mesh changes its connectivity. Note that build_halfedge also
    // re-indexes the mesh elements in the provided half-edge mesh.
    struct ElemInfo
    {
        Halfedge_Mesh::ElementRef ref;
        size_t instance = 0;
    };
    std::unordered_map<unsigned int, ElemInfo> id_to_info;
    std::unordered_map<unsigned int, float> vert_sizes;
};

} // namespace Gui

#endif
