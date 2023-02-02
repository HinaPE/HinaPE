#ifndef HINAPE_MATH_EXT_H
#define HINAPE_MATH_EXT_H

#ifdef HINAPE_DOUBLE
using real = double;
#else
using real = float;
#endif

#include <memory>
#include <functional>
#include <tuple>
#include <utility>
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <limits>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

//@formatter:off
namespace HinaPE::Constant
{
static constexpr real Zero      = static_cast<real>(0);
static constexpr real One       = static_cast<real>(1);
static constexpr real Half      = static_cast<real>(0.5);
static constexpr real Infinity  = std::numeric_limits<real>::max();
static constexpr real Epsilon   = std::numeric_limits<real>::epsilon();
static constexpr real PI        = static_cast<real>(3.1415926535897932384);
static constexpr real TwoPI     = static_cast<real>(6.2831853071795864769);
static constexpr real HalfPI    = static_cast<real>(1.5707963267948966192);
static constexpr real InvPI     = static_cast<real>(0.3183098861837906715);
static constexpr real InvTwoPI  = static_cast<real>(0.1591549430918953358);
static constexpr real InvHalfPI = static_cast<real>(0.6366197723675813431);
static constexpr real E         = static_cast<real>(2.7182818284590452354);
static constexpr real Log2E     = static_cast<real>(1.4426950408889634074);
static constexpr real Log10E    = static_cast<real>(0.4342944819032518276);
static constexpr real Ln2       = static_cast<real>(0.6931471805599453094);
static constexpr real Ln10      = static_cast<real>(2.3025850929940456840);
static constexpr real Sqrt2     = static_cast<real>(1.4142135623730950488);
static constexpr real Sqrt3     = static_cast<real>(1.7320508075688772935);
static constexpr real SqrtHalf  = static_cast<real>(0.7071067811865475244);
}

namespace HinaPE::Direction
{
static constexpr int DirectionNone     = 0;
static constexpr int DirectionLeft     = 1 << 0;
static constexpr int DirectionRight    = 1 << 1;
static constexpr int DirectionUp       = 1 << 2;
static constexpr int DirectionDown     = 1 << 3;
static constexpr int DirectionForward  = 1 << 4;
static constexpr int DirectionBackward = 1 << 5;
static constexpr int DirectionAll      = DirectionLeft | DirectionRight | DirectionUp | DirectionDown | DirectionForward | DirectionBackward;
}

namespace HinaPE::Math
{
constexpr float HINA_PI = 3.1415926535897932384f;
constexpr float HINA_INV_PI = 0.3183098861837906715f;

template<typename T>
inline auto similar(T x, T y, T eps) -> bool { return (std::abs(x - y) <= eps); }

template<typename T>
inline auto sign(T x) -> T { if (x >= 0) return 1; else return -1; }

template<typename T>
inline auto min3(T x, T y, T z) -> T { return std::min(std::min(x, y), z); }

template<typename T>
inline auto max3(T x, T y, T z) -> T { return std::max(std::max(x, y), z); }

template<typename T>
inline auto minn(const T *x, size_t n) -> T
{
	T m = x[0];
	for (size_t i = 1; i < n; i++)
		m = std::min(m, x[i]);
	return m;
}

template<typename T>
inline auto maxn(const T *x, size_t n) -> T
{
	T m = x[0];
	for (size_t i = 1; i < n; i++)
		m = std::max(m, x[i]);
	return m;
}

template<typename T>
inline auto absmin(T x, T y) -> T { return (x * x < y * y) ? x : y; }

template<typename T>
inline auto absmax(T x, T y) -> T { return (x * x > y * y) ? x : y; }

template<typename T>
inline auto absminn(const T *x, size_t n) -> T
{
	T m = x[0];
	for (size_t i = 1; i < n; i++)
		m = absmin(m, x[i]);
	return m;
}

template<typename T>
inline auto absmaxn(const T *x, size_t n) -> T
{
	T m = x[0];
	for (size_t i = 1; i < n; i++)
		m = absmax(m, x[i]);
	return m;
}

template<typename T>
inline auto argmin2(T x, T y) -> size_t { return (x < y) ? 0 : 1; }

template<typename T>
inline auto argmax2(T x, T y) -> size_t { return (x > y) ? 0 : 1; }

template<typename T>
inline auto argmin3(T x, T y, T z) -> size_t { if (x < y) return (x < z) ? 0 : 2; else return (y < z) ? 1 : 2; }

template<typename T>
inline auto argmax3(T x, T y, T z) -> size_t { if (x > y) return (x > z) ? 0 : 2; else return (y > z) ? 1 : 2; }

template<typename T>
inline auto square(T x) -> T { return x * x; }

template<typename T>
inline auto cubic(T x) -> T { return x * x * x; }

template<typename T>
inline auto clamp(T val, T low, T high) -> T { if (val < low) return low; else if (val > high) return high; else return val; }

template<typename T>
inline auto degrees_to_radians(T angle_in_degrees) -> T { return angle_in_degrees * static_cast<T>(HINA_PI) / 180; }

template<typename T>
inline auto radians_to_degrees(T angle_in_radians) -> T { return angle_in_radians * 180 / static_cast<T>(HINA_PI); }

template<typename T>
inline void get_barycentric(T x, long iLow, long iHigh, long *i, T *f)
{
	T s = std::floor(x);
	*i = static_cast<long>(s);

	long offset = -iLow;
	iLow += offset;
	iHigh += offset;

	if (iLow == iHigh)
	{
		*i = iLow;
		*f = 0;
	} else if (*i < iLow)
	{
		*i = iLow;
		*f = 0;
	} else if (*i > iHigh - 1)
	{
		*i = iHigh - 1;
		*f = 1;
	} else
	{
		*f = static_cast<T>(x - s);
	}

	*i -= offset;
}

template<typename S, typename T>
inline auto lerp(const S &value0, const S &value1, T f) -> S { return (1 - f) * value0 + f * value1; }

template<typename S, typename T>
inline auto bilerp(const S &f00, const S &f10, const S &f01, const S &f11, T tx, T ty) -> S { return lerp(lerp(f00, f10, tx), lerp(f01, f11, tx), ty); }

template<typename S, typename T>
inline auto trilerp(const S &f000, const S &f100, const S &f010, const S &f110, const S &f001, const S &f101, const S &f011, const S &f111, T tx, T ty, T fz) -> S { return lerp(bilerp(f000, f100, f010, f110, tx, ty), bilerp(f001, f101, f011, f111, tx, ty), fz); }

template<typename S, typename T>
inline auto catmull_rom(const S &f0, const S &f1, const S &f2, const S &f3, T f) -> S
{
	S d1 = (f2 - f0) / 2;
	S d2 = (f3 - f1) / 2;
	S D1 = f2 - f1;

	S a3 = d1 + d2 - 2 * D1;
	S a2 = 3 * D1 - 2 * d1 - d2;
	S a1 = d1;
	S a0 = f1;

	return a3 * cubic(f) + a2 * square(f) + a1 * f + a0;
}

template<typename T>
inline auto monotonic_catmull_rom(const T &f0, const T &f1, const T &f2, const T &f3, T f) -> T
{
	T d1 = (f2 - f0) / 2;
	T d2 = (f3 - f1) / 2;
	T D1 = f2 - f1;

	if (std::fabs(D1) < std::numeric_limits<T>::epsilon())
		d1 = d2 = 0;

	if (sign(D1) != sign(d1))
		d1 = 0;

	if (sign(D1) != sign(d2))
		d2 = 0;

	T a3 = d1 + d2 - 2 * D1;
	T a2 = 3 * D1 - 2 * d1 - d2;
	T a1 = d1;
	T a0 = f1;

	return a3 * cubic(f) + a2 * square(f) + a1 * f + a0;
}


template<typename T>
auto is_inside_sdf(T phi) -> bool
{
	return phi < 0;
}

template<typename T>
inline auto smeared_heaviside_sdf(T phi) -> T
{
	if (phi > 1.5)
		return 1;
	else if (phi < -1.5)
		return 0;
	else
		return 0.5f + phi / 3.0 + 0.5f * static_cast<T>(HINA_INV_PI) * std::sin(static_cast<T>(HINA_PI) * phi / 1.5);
}

template<typename T>
inline auto smeared_delta_sdf(T phi) -> T
{
	if (std::fabs(phi) > 1.5)
		return 0;
	else
		return 1.0 / 3.0 + 1.0 / 3.0 * std::cos(static_cast<T>(HINA_PI) * phi / 1.5);
}

template<typename T>
auto fraction_inside_sdf(T phi0, T phi1) -> T
{
	if (is_inside_sdf(phi0) && is_inside_sdf(phi1))
		return 1;
	else if (is_inside_sdf(phi0) && !is_inside_sdf(phi1))
		return phi0 / (phi0 - phi1);
	else if (!is_inside_sdf(phi0) && is_inside_sdf(phi1))
		return phi1 / (phi1 - phi0);
	else
		return 0;
}

template<typename T>
void cycle_array(T *arr, int size)
{
	T t = arr[0];
	for (int i = 0; i < size - 1; ++i) arr[i] = arr[i + 1];
	arr[size - 1] = t;
}

template<typename T>
auto fraction_inside(T phiBottomLeft, T phiBottomRight, T phiTopLeft,
					 T phiTopRight) -> T
{
	int inside_count = (phiBottomLeft < 0 ? 1 : 0) + (phiTopLeft < 0 ? 1 : 0) +
					   (phiBottomRight < 0 ? 1 : 0) + (phiTopRight < 0 ? 1 : 0);
	T list[] = {phiBottomLeft, phiBottomRight, phiTopRight, phiTopLeft};

	if (inside_count == 4)
	{
		return 1;
	} else if (inside_count == 3)
	{
		// rotate until the positive value is in the first position
		while (list[0] < 0)
		{
			cycle_array(list, 4);
		}

		// Work out the area of the exterior triangle
		T side0 = 1 - fraction_inside_sdf(list[0], list[3]);
		T side1 = 1 - fraction_inside_sdf(list[0], list[1]);
		return 1 - 0.5f * side0 * side1;
	} else if (inside_count == 2)
	{
		// rotate until a negative value is in the first position, and the next
		// negative is in either slot 1 or 2.
		while (list[0] >= 0 || !(list[1] < 0 || list[2] < 0))
		{
			cycle_array(list, 4);
		}

		if (list[1] < 0)
		{  // the matching signs are adjacent
			T side_left = fraction_inside_sdf(list[0], list[3]);
			T side_right = fraction_inside_sdf(list[1], list[2]);
			return 0.5f * (side_left + side_right);
		} else
		{  // matching signs are diagonally opposite
			// determine the centre point's sign to disambiguate this case
			T middle_point = 0.25f * (list[0] + list[1] + list[2] + list[3]);
			if (middle_point < 0)
			{
				T area = 0;

				// first triangle (top left)
				T side1 = 1 - fraction_inside_sdf(list[0], list[3]);
				T side3 = 1 - fraction_inside_sdf(list[2], list[3]);

				area += 0.5f * side1 * side3;

				// second triangle (top right)
				T side2 = 1 - fraction_inside_sdf(list[2], list[1]);
				T side0 = 1 - fraction_inside_sdf(list[0], list[1]);
				area += 0.5f * side0 * side2;

				return 1 - area;
			} else
			{
				T area = 0;

				// first triangle (bottom left)
				T side0 = fraction_inside_sdf(list[0], list[1]);
				T side1 = fraction_inside_sdf(list[0], list[3]);
				area += 0.5f * side0 * side1;

				// second triangle (top right)
				T side2 = fraction_inside_sdf(list[2], list[1]);
				T side3 = fraction_inside_sdf(list[2], list[3]);
				area += 0.5f * side2 * side3;
				return area;
			}
		}
	} else if (inside_count == 1)
	{
		// rotate until the negative value is in the first position
		while (list[0] >= 0)
		{
			cycle_array(list, 4);
		}

		// Work out the area of the interior triangle, and subtract from 1.
		T side0 = fraction_inside_sdf(list[0], list[3]);
		T side1 = fraction_inside_sdf(list[0], list[1]);
		return 0.5f * side0 * side1;
	} else
	{
		return 0;
	}
}

template<typename T>
auto distanceToZeroLevelSet(T phi0, T phi1) -> T
{
	if (std::fabs(phi0) + std::fabs(phi1) > std::numeric_limits<T>::epsilon())
		return std::fabs(phi0) / (std::fabs(phi0) + std::fabs(phi1));
	else
		return static_cast<T>(0.5);
}
}
#endif //HINAPE_MATH_EXT_H
