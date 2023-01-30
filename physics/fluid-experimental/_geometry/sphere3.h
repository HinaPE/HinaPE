#ifndef HINAPE_SPHERE3_H
#define HINAPE_SPHERE3_H

#include "surface3.h"

namespace HinaPE
{
class Sphere3 : public Surface3
{
public:
	explicit Sphere3(const mVector3 &center = mVector3(0, 0, 0), real radius = 1.0) : _center(center), _radius(radius) {}

protected:
	auto intersects_local(const mRay3 &ray) const -> bool final;
	auto bounding_box_local() const -> mBBox final;
	auto closest_point_local(const mVector3 &other_point) const -> mVector3 final;
	auto closest_intersection_local(const mRay3 &ray) const -> SurfaceRayIntersection3 final;
	auto closest_distance_local(const mVector3 &other_point) const -> real final;
	auto closest_normal_local(const mVector3 &other_point) const -> mVector3 final;

private:
	mVector3 _center;
	real _radius = 1;
};
using Sphere3Ptr = std::shared_ptr<Sphere3>;
}

#endif //HINAPE_SPHERE3_H
