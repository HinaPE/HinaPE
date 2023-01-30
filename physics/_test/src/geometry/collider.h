#ifndef HINAPE_COLLIDER_H
#define HINAPE_COLLIDER_H

#include "surface3.h"

namespace Hina
{
class Collider
{
public:
	using OnBeginUpdateCallback = std::function<void(Collider *, real, real)>;

public:
	struct Opt
	{
		real friction = Constant::Zero;
	};
private:
	Surface3Ptr _surface;
};


using ColliderPtr = std::shared_ptr<Collider>;
}

#endif //HINAPE_COLLIDER_H
