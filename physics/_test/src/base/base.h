#ifndef HINAPE_BASE_H
#define HINAPE_BASE_H

#include "size.h"
#include "array.h"
#include "vector.h"
#include "bbox.h"

#include <memory>

namespace Hina::Base
{
using real = double;
using mVector3 = Vector3<real>;
using mBBox3 = BoundingBox3<real>;
}

#endif //HINAPE_BASE_H
