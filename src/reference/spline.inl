
#include "../geometry/spline.h"
#include "debug.h"

template<typename T>
T Spline<T>::cubic_unit_spline(float time, const T &position0, const T &position1,
                               const T &tangent0, const T &tangent1)
{

    // TODO (Animation): Task 1a
    // Given time in [0,1] compute the cubic spline coefficients and use them to compute
    // the interpolated value at time 'time' based on the positions & tangents

    // Note that Spline is parameterized on type T, which allows us to create splines over
    // any type that supports the * and + operators.
    float t3 = (float) pow(time, 3);
    float t2 = (float) pow(time, 2);
    float h00 = 2 * t3 - 3 * t2 + 1;
    float h10 = t3 - 2 * t2 + time;
    float h01 = -2 * t3 + 3 * t2;
    float h11 = t3 - t2;
    T pt = h00 * position0 + h10 * tangent0 + h01 * position1 + h11 * tangent1;
    return pt;
}

template<typename T>
T Spline<T>::at(float time) const
{

    // TODO (Animation): Task 1b

    // Given a time, find the nearest positions & tangent values
    // defined by the control point map.

    // Transform them for use with cubic_unit_spline

    // Be wary of edge cases! What if time is before the first knot,
    // before the second knot, etc...
    size_t control_size = control_points.size();
    if (control_size == 0) return T();
    if (control_size == 1) return control_points.begin()->second;
    if (control_points.begin()->first >= time) return control_points.begin()->second;
    if (control_points.rbegin()->first <= time) return control_points.rbegin()->second;

    // More general case: use bound functions
    // auto upper = std::upper_bound(_cdf.begin(), _cdf.end(), rng);
    auto upper = control_points.upper_bound(time);
    auto lower = std::prev(upper);
    float t1 = lower->first;
    float t2 = upper->first;
    T knot1 = lower->second;
    T knot2 = upper->second;
    float t0, t3;
    T knot0;
    T knot3;
    if (t1 == control_points.begin()->first)
    {
        // virtual
        knot0 = knot1 - (knot2 - knot1);
        t0 = t1 - (t2 - t1);
    } else
    {
        auto prev = std::prev(upper);
        t0 = prev->first;
        knot0 = prev->second;
    }
    if (t2 == control_points.rbegin()->first)
    {
        // virtual
        knot3 = knot2 + (knot2 - knot1);
        t3 = t2 + (t2 - t1);
    } else
    {
        auto next = std::next(lower);
        t3 = next->first;
        knot3 = next->second;
    }
    T m1 = (knot2 - knot0) / (t2 - t0);
    T m2 = (knot3 - knot1) / (t3 - t1);
    float scaled_time = (time - t1) / (t2 - t1);
    return cubic_unit_spline(scaled_time, knot1, knot2, m1, m2);
}
