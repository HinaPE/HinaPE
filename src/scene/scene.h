#ifndef HINAPE_SCENE_H
#define HINAPE_SCENE_H

#include <functional>
#include <map>
#include <optional>

#include "../geometry/halfedge.h"
#include "../lib/mathlib.h"
#include "../platform/gl.h"
#include "../util/camera.h"

#include "light.h"
#include "object.h"
#include "particles.h"

class Undo;
class Halfedge_Editor;
namespace Gui
{
class Manager;
class Animate;
} // namespace Gui

class Scene_Item
{
public:
    Scene_Item() = default;

    template<typename T>
    Scene_Item(T &&obj) : data(std::forward<T &&>(obj)) {}

    Scene_Item(Scene_Item &&src)  noexcept : data(std::move(src.data)) {}

    Scene_Item(const Scene_Item &src) = delete;

    auto operator=(Scene_Item &&src) noexcept -> Scene_Item & ;
    auto operator=(const Scene_Item &src) -> Scene_Item & = delete;

    auto bbox() -> BBox;
    void render(const Mat4 &view, bool solid = false, bool depth_only = false, bool posed = true);
    auto id() const -> Scene_ID;

    auto pose() -> Pose &;
    auto pose() const -> const Pose &;
    auto animation() -> Anim_Pose &;
    auto animation() const -> const Anim_Pose &;
    void set_time(float time);
    void step(const PT::Object &scene, float dt);

    auto name() const -> std::string;
    auto name() -> std::pair<char *, int>;

    template<typename T>
    auto is() const -> bool { return std::holds_alternative<T>(data); }

    template<typename T>
    auto get() -> T & { return std::get<T>(data); }

    template<typename T>
    auto get() const -> const T & { return std::get<T>(data); }

private:
    std::variant<Scene_Object, Scene_Light, Scene_Particles> data;
};

using Scene_Maybe = std::optional<std::reference_wrapper<Scene_Item>>;

class Scene
{
public:
    explicit Scene(Scene_ID start);
    ~Scene() = default;

    struct Load_Opts
    {
        bool new_scene = false;
        bool drop_normals = true;
        bool join_verts = true;
        bool triangulate = false;
        bool gen_normals = false;
        bool gen_smooth_normals = false;
        bool fix_infacing_normals = false;
        bool debone = false;
    };

    auto write(std::string file, const Camera &cam, const Gui::Animate &animation) -> std::string;
    auto load(Load_Opts opt, Undo &undo, Gui::Manager &gui, std::string file) -> std::string;
    void clear(Undo &undo);

    auto empty() -> bool;
    auto size() -> size_t;

    template<typename T>
    auto add(T &&obj) -> Scene_ID
    {
        assert(objs.find(obj.id()) == objs.end());
        objs.emplace(std::make_pair(obj.id(), std::forward<T>(obj)));
        return obj.id();
    }

    auto add(Pose pose, GL::Mesh &&mesh, std::string n = {}, Scene_ID id = 0) -> Scene_ID;
    auto add(Pose pose, Halfedge_Mesh &&mesh, std::string n = {}, Scene_ID id = 0) -> Scene_ID;
    auto reserve_id() -> Scene_ID;
    auto used_ids() const -> Scene_ID;

    void erase(Scene_ID id);
    void restore(Scene_ID id);

    void for_items(const std::function<void(Scene_Item &)>& func);
    void for_items(const std::function<void(const Scene_Item &)>& func) const;

    auto get(Scene_ID id) -> Scene_Maybe;

    template<typename T>
    auto get(Scene_ID id) -> T &
    {
        auto entry = objs.find(id);
        assert(entry != objs.end());
        assert(entry->second.is<T>());
        return entry->second.get<T>();
    }

    auto set_env_map(std::string file) -> std::string;

    [[nodiscard]] auto has_env_light() const -> bool;
    [[nodiscard]] auto has_obj() const -> bool;
    [[nodiscard]] auto has_sim() const -> bool;

private:
    struct Stats
    {
        unsigned int meshes = 0;
        unsigned int lights = 0;
        unsigned int anims = 0;
        unsigned int emitters = 0;
        unsigned int armatures = 0;
        unsigned int objs = 0;
        unsigned int nodes = 0;
    };
    auto get_stats(const Gui::Animate &animation) -> Stats;

    std::map<Scene_ID, Scene_Item> objs;
    std::map<Scene_ID, Scene_Item> erased;
    Scene_ID next_id, first_id;
};

#endif