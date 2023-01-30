#ifndef HINAPE_SURFACE3_H
#define HINAPE_SURFACE3_H

#include "base/base.h"

namespace Hina
{
struct SurfaceRayIntersection3
{
	bool is_intersecting = false;
	double distance = std::numeric_limits<real>::max();
	mVector3 point;
	mVector3 normal;
};

class Surface3
{
public:
	struct Opt
	{
		bool normal_flipped = false;
	} _opt;


public:
	virtual void update_query_engine() {}
	virtual auto is_bounded() -> bool { return true; }
	virtual auto is_valid_geometry() -> bool { return true; }
	inline auto intersects(const mRay3 &ray) const -> bool { return intersects_local(_transform.to_local(ray)); }
	inline auto bounding_box() const -> mBBox3 { return _transform.to_world(bounding_box_local()); }
	inline auto closest_point(const mVector3 &other_point) const -> mVector3 { return _transform.to_world(closest_point_local(_transform.to_local(other_point))); }
	inline auto is_inside(const mVector3 &point) -> bool { return _opt.normal_flipped == is_inside_local(_transform.to_local(point)); }

protected:
	virtual auto intersects_local(const mRay3 &ray) const -> bool = 0;
	virtual auto bounding_box_local() const -> mBBox3 = 0;
	virtual auto closest_point_local(const mVector3 &other_point) const -> mVector3 = 0;
	virtual auto closest_intersection_local(const mRay3 &ray) const -> SurfaceRayIntersection3 = 0;
	virtual auto closest_distance_local(const mVector3 &other_point) const -> real = 0;
	virtual auto closest_normal_local(const mVector3 &other_point) const -> mVector3 = 0;
	inline virtual auto is_inside_local(const mVector3 &other_point) const -> bool { return (other_point - closest_point_local(other_point)).dot(closest_normal_local(other_point)) < 0; }

private:
	mTransform3 _transform;
};
using Surface3Ptr = std::shared_ptr<Surface3>;
}

#endif //HINAPE_SURFACE3_H
