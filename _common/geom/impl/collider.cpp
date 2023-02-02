#include "geom/collider.h"

void HinaPE::Geom::Collider::update(real current_time, real time_interval)
{
	if (!_surface->is_valid_geometry())
		return;

	_surface->update_query_engine();

	if (_opt._on_begin_update_callback)
		_opt._on_begin_update_callback(this, current_time, time_interval);
}
void HinaPE::Geom::Collider::resolve_collision(real radius, real restitution, mVector3 &position, mVector3 &velocity) const
{
	auto result = get_closest_point(_surface, position);
	if (is_penetrating(result, position, radius))
	{
		// find the actual collision point of collision
		mVector3 target_normal = result.normal;
		mVector3 target_point = result.point + target_normal * radius;
		mVector3 collider_velocity_at_target_point = velocity_at(target_point);

		// get new candidate relative velocity from the target point
		mVector3 relative_velocity = velocity - collider_velocity_at_target_point;
		real normal_dot_relative_velocity = target_normal.dot(relative_velocity);
		mVector3 relative_velN = normal_dot_relative_velocity * target_normal;
		mVector3 relative_velT = relative_velocity - relative_velN;

		// check if the velocity is facing opposite direction of the surface
		if (normal_dot_relative_velocity < Constant::Zero)
		{
			// apply restitution coefficient
			mVector3 delta_relative_velN = (-restitution - Constant::One) * relative_velN;
			relative_velN *= -restitution;

			// Apply friction to the tangential component of the velocity
			// From Bridson et al., Robust Treatment of Collisions, Contact and
			// Friction for Cloth Animation, 2002
			// http://graphics.stanford.edu/papers/cloth-sig02/cloth.pdf
			if (relative_velT.length_squared() > Constant::Zero)
			{
				real friction_scale = std::max(
						Constant::One - _opt.friction * delta_relative_velN.length() / relative_velT.length(),
						Constant::Zero);
				relative_velT *= friction_scale;
			}

			velocity = relative_velN + relative_velT + collider_velocity_at_target_point;
		}

		// geometric fix
		position = target_point;
	}
}
auto HinaPE::Geom::Collider::get_closest_point(const Surface3Ptr &surface, const mVector3 &query_point) const -> HinaPE::Geom::Collider::ColliderQueryResult
{
	ColliderQueryResult result;
	result.distance = surface->closest_distance(query_point);
	result.point = surface->closest_point(query_point);
	result.normal = surface->closest_normal(query_point);
	result.velocity = velocity_at(result.point);
	return result;
}
auto HinaPE::Geom::Collider::is_penetrating(const HinaPE::Geom::Collider::ColliderQueryResult &result, const mVector3 &position, real radius) const -> bool
{
	return _surface->is_inside(position) || (result.distance < radius);
}
