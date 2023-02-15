#ifndef HINAPE_COMMON_H
#define HINAPE_COMMON_H

#include "math/array.h"
#include "math/matrix.h"
#include "math/quaternion.h"
#include "math/ray.h"
#include "math/transform.h"
#include "math/vector.h"
#include "geom/bbox.h"
#include "geom/surface3.h"
#include "util/parallel.h"
#include "util/timer.h"

namespace HinaPE
{
class CopyDisable
{
public:
	CopyDisable() = default;
	~CopyDisable() = default;
	CopyDisable(const CopyDisable &) = delete;
	auto operator=(const CopyDisable &) -> CopyDisable & = delete;
	CopyDisable(CopyDisable &&) = default;
	auto operator=(CopyDisable &&) -> CopyDisable & = default;
};
}

#endif //HINAPE_COMMON_H
