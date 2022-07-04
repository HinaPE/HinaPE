
#pragma once

#include "../lib/mathlib.h"
#include "../scene/object.h"
#include <variant>

#include "bvh.h"
#include "list.h"
#include "shapes.h"
#include "trace.h"
#include "tri_mesh.h"

namespace PT {

class Object {
public:
    Object(Shape&& shape, Scene_ID id, unsigned int m = 0, const Mat4& T = Mat4::I)
        : trans(T), itrans(T.inverse()), _id(id), material(m), underlying(std::move(shape)) {
        has_trans = trans != Mat4::I;
    }
    Object(Tri_Mesh&& tri_mesh, Scene_ID id, unsigned int m = 0, const Mat4& T = Mat4::I)
        : trans(T), itrans(T.inverse()), _id(id), material(m), underlying(std::move(tri_mesh)) {
        has_trans = trans != Mat4::I;
    }
    Object(List<Object>&& list, Scene_ID id, unsigned int m = 0, const Mat4& T = Mat4::I)
        : trans(T), itrans(T.inverse()), _id(id), material(m), underlying(std::move(list)) {
        has_trans = trans != Mat4::I;
    }
    Object(BVH<Object>&& bvh, Scene_ID id, unsigned int m = 0, const Mat4& T = Mat4::I)
        : trans(T), itrans(T.inverse()), _id(id), material(m), underlying(std::move(bvh)) {
        has_trans = trans != Mat4::I;
    }

    Object() {
    }
    Object(List<Object>&& list, const Mat4& T = Mat4::I)
        : trans(T), itrans(T.inverse()), underlying(std::move(list)) {
        has_trans = trans != Mat4::I;
    }
    Object(BVH<Object>&& bvh, const Mat4& T = Mat4::I)
        : trans(T), itrans(T.inverse()), underlying(std::move(bvh)) {
        has_trans = trans != Mat4::I;
    }

    Object(const Object& src) = delete;
    Object& operator=(const Object& src) = delete;
    Object& operator=(Object&& src) = default;
    Object(Object&& src) = default;

    BBox bbox() const {
        BBox box = std::visit([](const auto& o) { return o.bbox(); }, underlying);
        if(has_trans) box.transform(trans);
        return box;
    }

    Trace hit(Ray ray) const {
        if(has_trans) ray.transform(itrans);
        Trace ret = std::visit([&ray](const auto& o) { return o.hit(ray); }, underlying);
        if(ret.hit) {
            if(material != -1) ret.material = material;
            if(has_trans) ret.transform(trans, itrans.T());
        }
        return ret;
    }

    size_t visualize(GL::Lines& lines, GL::Lines& active, size_t level, Mat4 vtrans) const {
        if(has_trans) vtrans = vtrans * trans;
        return std::visit(
            overloaded{
                [&](const BVH<Object>& bvh) { return bvh.visualize(lines, active, level, vtrans); },
                [&](const Tri_Mesh& mesh) { return mesh.visualize(lines, active, level, vtrans); },
                [](const auto&) { return size_t(0); }},
            underlying);
    }

    Vec3 sample(Vec3 from) const {
        if(has_trans) from = itrans * from;
        Vec3 dir =
            std::visit(overloaded{[from](const List<Object>& list) { return list.sample(from); },
                                  [from](const Tri_Mesh& mesh) { return mesh.sample(from); },
                                  [](const auto&) -> Vec3 {
                                      die("Sampling implicit objects/BVHs is not yet supported.");
                                  }},
                       underlying);
        if(has_trans) dir = trans.rotate(dir).unit();
        return dir;
    }

    float pdf(Ray ray, Mat4 T = Mat4::I, Mat4 iT = Mat4::I) const {
        if(has_trans) {
            T = T * trans;
            iT = itrans * iT;
        }
        return std::visit(
            overloaded{[ray, T, iT](const List<Object>& list) { return list.pdf(ray, T, iT); },
                       [ray, T, iT](const Tri_Mesh& mesh) { return mesh.pdf(ray, T, iT); },
                       [](const auto&) -> float {
                           die("Sampling implicit objects/BVHs is not yet supported.");
                       }},
            underlying);
    }

    Scene_ID id() const {
        return _id;
    }
    void set_trans(const Mat4& T) {
        trans = T;
        itrans = T.inverse();
        has_trans = trans != Mat4::I;
    }

private:
    bool has_trans = false;
    Mat4 trans, itrans;
    int material = -1;
    Scene_ID _id = 0;
    std::variant<Tri_Mesh, Shape, BVH<Object>, List<Object>> underlying;
};

} // namespace PT
