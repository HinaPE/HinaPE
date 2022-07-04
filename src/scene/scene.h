
#pragma once

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
namespace Gui {
class Manager;
class Animate;
} // namespace Gui

class Scene_Item {
public:
    Scene_Item() = default;

    template<typename T> Scene_Item(T&& obj) : data(std::forward<T&&>(obj)) {
    }

    Scene_Item(Scene_Item&& src) : data(std::move(src.data)) {
    }
    Scene_Item(const Scene_Item& src) = delete;

    Scene_Item& operator=(Scene_Item&& src);
    Scene_Item& operator=(const Scene_Item& src) = delete;

    BBox bbox();
    void render(const Mat4& view, bool solid = false, bool depth_only = false, bool posed = true);
    Scene_ID id() const;

    Pose& pose();
    const Pose& pose() const;
    Anim_Pose& animation();
    const Anim_Pose& animation() const;
    void set_time(float time);
    void step(const PT::Object& scene, float dt);

    std::string name() const;
    std::pair<char*, int> name();

    template<typename T> bool is() const {
        return std::holds_alternative<T>(data);
    }
    template<typename T> T& get() {
        return std::get<T>(data);
    }
    template<typename T> const T& get() const {
        return std::get<T>(data);
    }

private:
    std::variant<Scene_Object, Scene_Light, Scene_Particles> data;
};

using Scene_Maybe = std::optional<std::reference_wrapper<Scene_Item>>;

class Scene {
public:
    Scene(Scene_ID start);
    ~Scene() = default;

    struct Load_Opts {
        bool new_scene = false;
        bool drop_normals = true;
        bool join_verts = true;
        bool triangulate = false;
        bool gen_normals = false;
        bool gen_smooth_normals = false;
        bool fix_infacing_normals = false;
        bool debone = false;
    };

    std::string write(std::string file, const Camera& cam, const Gui::Animate& animation);
    std::string load(Load_Opts opt, Undo& undo, Gui::Manager& gui, std::string file);
    void clear(Undo& undo);

    bool empty();
    size_t size();

    template<typename T> Scene_ID add(T&& obj) {
        assert(objs.find(obj.id()) == objs.end());
        objs.emplace(std::make_pair(obj.id(), std::move(obj)));
        return obj.id();
    }

    Scene_ID add(Pose pose, GL::Mesh&& mesh, std::string n = {}, Scene_ID id = 0);
    Scene_ID add(Pose pose, Halfedge_Mesh&& mesh, std::string n = {}, Scene_ID id = 0);
    Scene_ID reserve_id();
    Scene_ID used_ids();

    void erase(Scene_ID id);
    void restore(Scene_ID id);

    void for_items(std::function<void(Scene_Item&)> func);
    void for_items(std::function<void(const Scene_Item&)> func) const;

    Scene_Maybe get(Scene_ID id);

    template<typename T> T& get(Scene_ID id) {
        auto entry = objs.find(id);
        assert(entry != objs.end());
        assert(entry->second.is<T>());
        return entry->second.get<T>();
    }

    std::string set_env_map(std::string file);

    bool has_env_light() const;
    bool has_obj() const;
    bool has_sim() const;

private:
    struct Stats {
        unsigned int meshes = 0;
        unsigned int lights = 0;
        unsigned int anims = 0;
        unsigned int emitters = 0;
        unsigned int armatures = 0;
        unsigned int objs = 0;
        unsigned int nodes = 0;
    };
    Stats get_stats(const Gui::Animate& animation);

    std::map<Scene_ID, Scene_Item> objs;
    std::map<Scene_ID, Scene_Item> erased;
    Scene_ID next_id, first_id;
};
