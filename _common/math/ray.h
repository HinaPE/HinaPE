#ifndef HINAPE_RAY_H
#define HINAPE_RAY_H

#include "math/vector.h"
#include "math/math_ext.h"

namespace HinaPE::Math
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

#ifdef HINAPE_DOUBLE
using mRay3 = HinaPE::Math::Ray3<double>;
#else
using mRay3 = HinaPE::Math::Ray3<float>;
#endif
#endif //HINAPE_RAY_H
