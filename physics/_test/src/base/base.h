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
static constexpr real One = static_cast<real>(1);
static constexpr real Infinity = std::numeric_limits<real>::max();
static constexpr real Epsilon = std::numeric_limits<real>::epsilon();
static constexpr real PI = static_cast<real>(3.1415926535897932384);
static constexpr real TwoPI = static_cast<real>(6.2831853071795864769);
static constexpr real HalfPI = static_cast<real>(1.5707963267948966192);
static constexpr real InvPI = static_cast<real>(0.3183098861837906715);
static constexpr real InvTwoPI = static_cast<real>(0.1591549430918953358);
static constexpr real InvHalfPI = static_cast<real>(0.6366197723675813431);
static constexpr real E = static_cast<real>(2.7182818284590452354);
static constexpr real Log2E = static_cast<real>(1.4426950408889634074);
static constexpr real Log10E = static_cast<real>(0.43429448190325182765);
static constexpr real Ln2 = static_cast<real>(0.69314718055994530942);
static constexpr real Ln10 = static_cast<real>(2.30258509299404568402);
static constexpr real Sqrt2 = static_cast<real>(1.41421356237309504880);
static constexpr real Sqrt3 = static_cast<real>(1.73205080756887729352);
static constexpr real SqrtHalf = static_cast<real>(0.70710678118654752440);
}
}

#endif //HINAPE_BASE_H
