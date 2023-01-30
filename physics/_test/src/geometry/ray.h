#ifndef HINAPE_RAY_H
#define HINAPE_RAY_H

#include "base/base.h"

namespace Hina
{
template<typename T>
class Ray3 final
{
public:
	inline auto point_at(Base::real t) const -> Base::mVector3 { return _origin + t * _direction; }

public:
	Base::mVector3 _origin;
	Base::mVector3 _direction;
};
}

#endif //HINAPE_RAY_H
