
#pragma once

#include "../lib/mathlib.h"
#include "../platform/gl.h"

#include "bvh.h"
#include "list.h"
#include "trace.h"

namespace PT {

struct Tri_Mesh_Vert {
    Vec3 position;
    Vec3 normal;
};

class Triangle {
public:
    BBox bbox() const;
    Trace hit(const Ray& ray) const;

    size_t visualize(GL::Lines&, GL::Lines&, size_t, const Mat4&) const {
        return size_t(0);
    }

    Vec3 sample(Vec3 from) const;
    float pdf(Ray ray, const Mat4& T, const Mat4& iT) const;

private:
    Triangle(Tri_Mesh_Vert* verts, unsigned int v0, unsigned int v1, unsigned int v2);

    unsigned int v0, v1, v2;
    Tri_Mesh_Vert* vertex_list;
    friend class Tri_Mesh;
};

class Tri_Mesh {
public:
    Tri_Mesh() = default;
    Tri_Mesh(const GL::Mesh& mesh, bool use_bvh = true);

    Tri_Mesh(Tri_Mesh&& src) = default;
    Tri_Mesh& operator=(Tri_Mesh&& src) = default;
    Tri_Mesh(const Tri_Mesh& src) = delete;
    Tri_Mesh& operator=(const Tri_Mesh& src) = delete;

    Tri_Mesh copy() const;

    BBox bbox() const;
    Trace hit(const Ray& ray) const;

    size_t visualize(GL::Lines& lines, GL::Lines& active, size_t level, const Mat4& trans) const;

    void build(const GL::Mesh& mesh, bool use_bvh = true);

    Vec3 sample(Vec3 from) const;
    float pdf(Ray ray, const Mat4& T, const Mat4& iT) const;

private:
    bool use_bvh = true;
    std::vector<Tri_Mesh_Vert> verts;
    BVH<Triangle> triangle_bvh;
    List<Triangle> triangle_list;
};

} // namespace PT
