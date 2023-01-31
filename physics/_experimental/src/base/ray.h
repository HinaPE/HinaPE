#ifndef HINAPE_RAY_H
#define HINAPE_RAY_H

#include "vector.h"

namespace Hina::Base
{
template<typename T>
class Ray3 final
{
public:
	inline auto point_at(T t) const -> Vector3<T> { return _origin + t * _direction; }

public:
	Vector3<T> _origin;
	Vector3<T> _direction;
};
}

#endif //HINAPE_RAY_H
