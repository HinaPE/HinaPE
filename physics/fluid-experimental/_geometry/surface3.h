#ifndef HINAPE_SURFACE3_H
#define HINAPE_SURFACE3_H

#include "transform3.h"

struct SurfaceRayIntersection3
{
	bool is_intersecting = false;
	double distance = std::numeric_limits<real>::max();
	mVector3 point;
	mVector3 normal;
};

namespace HinaPE
{
class Surface3
{
public:
	struct Opt
	{
		bool normal_flipped = false;
	} _opt;

protected:
	virtual auto closest_point_local(const mVector3 &other_point) const -> mVector3 = 0;
	virtual auto bounding_box_local() const -> mBBox = 0;
	virtual auto closest_intersection_local(const mRay3 &ray) const -> SurfaceRayIntersection3 = 0;
	virtual auto closest_distance_local(const mVector3 &other_point) const -> real = 0;
	virtual auto closest_normal_local(const mVector3 &other_point) const -> mVector3 = 0;
	virtual auto is_inside_local(const mVector3 &other_point) const -> bool = 0;

public:
	Transform3 _transform;
};
}

#endif //HINAPE_SURFACE3_H
