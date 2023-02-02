#ifndef HINAPE_COLLIDER_H
#define HINAPE_COLLIDER_H

#include "surface3.h"

namespace HinaPE::Geom
{
class Collider
{
public:
	using OnBeginUpdateCallback = std::function<void(Collider *, real, real)>;
	void update(real current_time, real time_interval);
	void resolve_collision(real radius, real restitution, mVector3 &position, mVector3 &velocity) const;

	virtual auto velocity_at(const mVector3 &point) const -> mVector3 = 0;

public:
	struct Opt
	{
		real friction = Constant::Zero;
		OnBeginUpdateCallback _on_begin_update_callback;
	} _opt;
	struct ColliderQueryResult final
	{
		real distance;
		mVector3 point;
		mVector3 normal;
		mVector3 velocity;
	};

protected:
	auto get_closest_point(const Surface3Ptr &surface, const mVector3 &query_point) const -> ColliderQueryResult;
	auto is_penetrating(const ColliderQueryResult &result, const mVector3 &position, real radius) const -> bool;

private:
	Surface3Ptr _surface;
};


using ColliderPtr = std::shared_ptr<Collider>;
}

#endif //HINAPE_COLLIDER_H
