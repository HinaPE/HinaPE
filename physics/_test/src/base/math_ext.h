#ifndef HINAPE_MATH_EXT_H
#define HINAPE_MATH_EXT_H

#include <cmath>
#include <limits>
#include <algorithm>

namespace Hina
{
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
inline auto clamp(T val, T low, T high) -> T { if (val < low) return low; else if (val > high)return high; else return val; }

template<typename T>
inline auto degrees_to_radians(T angle_in_degrees) -> T { return angle_in_degrees * static_cast<T>(3.1415926535897932384) / 180; }

template<typename T>
inline auto radians_to_degrees(T angle_in_radians) -> T { return angle_in_radians * 180 / static_cast<T>(3.1415926535897932384); }

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
inline auto monotonicCatmullRom(const T &f0, const T &f1, const T &f2, const T &f3, T f) -> T
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
}
#endif //HINAPE_MATH_EXT_H
