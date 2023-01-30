#ifndef HINAPE_BASE_H
#define HINAPE_BASE_H

#include "size.h"
#include "array.h"
#include "vector.h"
#include "quaternion.h"
#include "matrix.h"
#include "transform.h"
#include "bbox.h"
#include "ray.h"

#include <memory>
#include <functional>
#include <tuple>
#include <utility>

namespace Hina
{
using real = double;
using mVector3 = Base::Vector3<real>;
using mQuaternion = Base::Quaternion<real>;
using mMatrix3x3 = Base::Matrix3x3<real>;
using mTransform3 = Base::Transform3<real>;
using mBBox3 = Base::BoundingBox3<real>;
using mRay3 = Base::Ray3<real>;
namespace Constant
{
static constexpr real Zero = static_cast<real>(0);
}
}

#endif //HINAPE_BASE_H
