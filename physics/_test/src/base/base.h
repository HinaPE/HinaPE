#ifndef HINAPE_BASE_H
#define HINAPE_BASE_H

#include "size.h"
#include "array.h"
#include "vector.h"
#include "quaternion.h"
#include "matrix.h"
#include "bbox.h"

#include <memory>

namespace Hina
{
using real = double;
using mVector3 = Base::Vector3<real>;
using mQuaternion = Base::Quaternion<real>;
using mMatrix3x3 = Base::Matrix3x3<real>;
using mBBox3 = Base::BoundingBox3<real>;
namespace Constant
{
static constexpr real Zero = static_cast<real>(0);
}
}

#endif //HINAPE_BASE_H
