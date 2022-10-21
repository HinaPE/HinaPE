#ifndef HINAPE_BVH_H
#define HINAPE_BVH_H

#include "../lib/mathlib.h"
#include "../platform/gl.h"

#include "trace.h"

namespace PT
{
#define PARTS 32
template<typename Primitive>
class BVH
{
    size_t compute_bucket(BBox box, Vec3 center, int axis)
    {
        float pos = (center[axis] - box.min[axis]) / (box.max[axis] - box.min[axis]);
        size_t idx = (size_t) (pos * (float) PARTS);
        return std::min<size_t>(idx, PARTS - 1);
    }

    Trace find_closest_hit(const Ray ray, size_t curIdx, Vec2 &range) const
    {
        Node curNode = nodes[curIdx];
        Trace ret;
        if (curNode.is_leaf())
        {
            for (size_t i = 0; i < curNode.size; i++)
            {
                Trace hit = primitives[curNode.start + i].hit(ray);
                ret = Trace::min(ret, hit);
            }
            return ret;
        } else
        {
            Node lchild = nodes[curNode.l];
            Node rchild = nodes[curNode.r];
            Vec2 rangeL, rangeR;
            rangeL = range;
            rangeR = range;
            bool leftHit = lchild.bbox.hit(ray, rangeL);
            bool rightHit = rchild.bbox.hit(ray, rangeR);
            if (leftHit && rightHit)
            {
                // check range
                if (rangeL.x <= rangeR.x)
                {
                    // left first
                    Trace leftRet = find_closest_hit(ray, curNode.l, rangeL);
                    if (leftRet.distance > rangeR.x || !leftRet.hit)
                    {
                        Trace rightRet = find_closest_hit(ray, curNode.r, rangeR);
                        return Trace::min(leftRet, rightRet);
                    }
                    return leftRet;
                } else
                {
                    // right first
                    Trace rightRet = find_closest_hit(ray, curNode.r, rangeR);
                    if (rightRet.distance > rangeL.x || !rightRet.hit)
                    {
                        Trace leftRet = find_closest_hit(ray, curNode.l, rangeL);
                        return Trace::min(leftRet, rightRet);
                    }
                    return rightRet;
                }
            } else if (leftHit)
            {
                return find_closest_hit(ray, curNode.l, rangeL);
            } else if (rightHit)
            {
                return find_closest_hit(ray, curNode.r, rangeR);
            }
            return ret;
        }
    }

public:
    BVH() = default;
    BVH(std::vector<Primitive> &&primitives, size_t max_leaf_size = 1);
    void build(std::vector<Primitive> &&primitives, size_t max_leaf_size = 1);

    BVH(BVH &&src) = default;
    BVH &operator=(BVH &&src) = default;

    BVH(const BVH &src) = delete;
    BVH &operator=(const BVH &src) = delete;

    BBox bbox() const;
    Trace hit(const Ray &ray) const;

    BVH copy() const;
    size_t visualize(GL::Lines &lines, GL::Lines &active, size_t level, const Mat4 &trans) const;

    std::vector<Primitive> destructure();
    void clear();

private:
    class Node
    {

        BBox bbox;
        size_t start, size, l, r;

        // A node is a leaf if l == r, since all interior nodes must have distinct children
        bool is_leaf() const;
        friend class BVH<Primitive>;
    };
    size_t new_node(BBox box = {}, size_t start = 0, size_t size = 0, size_t l = 0, size_t r = 0);

    std::vector<Node> nodes;
    std::vector<Primitive> primitives;
    size_t root_idx = 0;
};

} // namespace PT

#ifdef SCOTTY3D_BUILD_REF
#include "../reference/bvh.inl"
#else
#include "../reference/bvh.inl"
#endif

#endif
