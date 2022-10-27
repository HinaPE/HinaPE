#ifndef HINAPE_FLUID_ENGINE_VECTOR3_H_
#define HINAPE_FLUID_ENGINE_VECTOR3_H_

#include "vector2.h"

#include <algorithm>  // just make cpplint happy..
#include <limits>
#include <tuple>

namespace HinaPE::FluidEngine
{

//!
//! \brief 3-D vector class.
//!
//! This class defines simple 3-D vector data.
//!
//! \tparam T - Type of the element
//!
template<typename T>
class Vector<T, 3> final
{
public:
    static_assert(std::is_floating_point<T>::value, "Vector only can be instantiated with floating point types");

    //! X (or the first) component of the vector.
    T x;

    //! Y (or the second) component of the vector.
    T y;

    //! Z (or the third) component of the vector.
    T z;

    // MARK: Constructors

    //! Constructs default vector (0, 0, 0).
    constexpr Vector() : x(0), y(0), z(0) {}

    //! Constructs vector with given parameters \p x_, \p y_, and \p z_.
    constexpr Vector(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

    //! Constructs vector with a 2-D vector and a scalar.
    constexpr Vector(const Vector2<T> &v, T z_) : x(v.x), y(v.y), z(z_) {}

    //! Constructs vector with initializer list.
    template<typename U>
    Vector(const std::initializer_list<U> &lst);

    //! Copy constructor.
    constexpr Vector(const Vector &v) : x(v.x), y(v.y), z(v.z) {}

    // MARK: Basic setters

    //! Set all x, y, and z components to \p s.
    void set(T s);

    //! Set x, y, and z components with given parameters.
    void set(T x, T y, T z);

    //! Set x, y, and z components with given \p pt.x, \p pt.y, and \p z.
    void set(const Vector2<T> &pt, T z);

    //! Set x, y, and z components with given initializer list.
    template<typename U>
    void set(const std::initializer_list<U> &lst);

    //! Set x, y, and z with other vector \p v.
    void set(const Vector &v);

    //! Set all x, y, and z to zero.
    void setZero();

    //! Normalizes this vector.
    void normalize();

    // MARK: Binary operations: new instance = this (+) v

    //! Computes this + (v, v, v).
    auto add(T v) const -> Vector;

    //! Computes this + (v.x, v.y, v.z).
    auto add(const Vector &v) const -> Vector;

    //! Computes this - (v, v, v).
    auto sub(T v) const -> Vector;

    //! Computes this - (v.x, v.y, v.z).
    auto sub(const Vector &v) const -> Vector;

    //! Computes this * (v, v, v).
    auto mul(T v) const -> Vector;

    //! Computes this * (v.x, v.y, v.z).
    auto mul(const Vector &v) const -> Vector;

    //! Computes this / (v, v, v).
    auto div(T v) const -> Vector;

    //! Computes this / (v.x, v.y, v.z).
    auto div(const Vector &v) const -> Vector;

    //! Computes dot product.
    auto dot(const Vector &v) const -> T;

    //! Computes cross product.
    auto cross(const Vector &v) const -> Vector;

    // MARK: Binary operations: new instance = v (+) this

    //! Computes (v, v, v) - this.
    auto rsub(T v) const -> Vector;

    //! Computes (v.x, v.y, v.z) - this.
    auto rsub(const Vector &v) const -> Vector;

    //! Computes (v, v, v) / this.
    auto rdiv(T v) const -> Vector;

    //! Computes (v.x, v.y, v.z) / this.
    auto rdiv(const Vector &v) const -> Vector;

    //! Computes \p v cross this.
    auto rcross(const Vector &v) const -> Vector;

    // MARK: Augmented operations: this (+)= v

    //! Computes this += (v, v, v).
    void iadd(T v);

    //! Computes this += (v.x, v.y, v.z).
    void iadd(const Vector &v);

    //! Computes this -= (v, v, v).
    void isub(T v);

    //! Computes this -= (v.x, v.y, v.z).
    void isub(const Vector &v);

    //! Computes this *= (v, v, v).
    void imul(T v);

    //! Computes this *= (v.x, v.y, v.z).
    void imul(const Vector &v);

    //! Computes this /= (v, v, v).
    void idiv(T v);

    //! Computes this /= (v.x, v.y, v.z).
    void idiv(const Vector &v);

    // MARK: Basic getters

    //! Returns const reference to the \p i -th element of the vector.
    auto at(size_t i) const -> const T &;

    //! Returns reference to the \p i -th element of the vector.
    auto at(size_t i) -> T &;

    //! Returns the sum of all the components (i.e. x + y + z).
    auto sum() const -> T;

    //! Returns the average of all the components.
    auto avg() const -> T;

    //! Returns the minimum value among x, y, and z.
    auto min() const -> T;

    //! Returns the maximum value among x, y, and z.
    auto max() const -> T;

    //! Returns the absolute minimum value among x, y, and z.
    auto absmin() const -> T;

    //! Returns the absolute maximum value among x, y, and z.
    auto absmax() const -> T;

    //! Returns the index of the dominant axis.
    auto dominantAxis() const -> size_t;

    //! Returns the index of the subminant axis.
    auto subminantAxis() const -> size_t;

    //! Returns normalized vector.
    auto normalized() const -> Vector;

    //! Returns the length of the vector.
    auto length() const -> T;

    //! Returns the squared length of the vector.
    auto lengthSquared() const -> T;

    //! Returns the distance to the other vector.
    auto distanceTo(const Vector &other) const -> T;

    //! Returns the squared distance to the other vector.
    auto distanceSquaredTo(const Vector &other) const -> T;

    //! Returns the reflection vector to the surface with given surface normal.
    auto reflected(const Vector &normal) const -> Vector;

    //! Returns the projected vector to the surface with given surface normal.
    auto projected(const Vector &normal) const -> Vector;

    //! Returns the tangential vector for this vector.
    auto tangential() const -> std::tuple<Vector, Vector>;

    //! Returns a vector with different value type.
    template<typename U>
    auto castTo() const -> Vector<U, 3>;

    //! Returns true if \p other is the same as this vector.
    auto isEqual(const Vector &other) const -> bool;

    //! Returns true if \p other is similar to this vector.
    auto isSimilar(const Vector &other, T epsilon = std::numeric_limits<T>::epsilon()) const -> bool;

    // MARK: Operators

    //! Returns reference to the \p i -th element of the vector.
    auto operator[](size_t i) -> T &;

    //! Returns const reference to the \p i -th element of the vector.
    auto operator[](size_t i) const -> const T &;

    //! Set x and y components with given initializer list.
    template<typename U>
    auto operator=(const std::initializer_list<U> &lst) -> Vector &;

    //! Set x and y with other vector \p pt.
    auto operator=(const Vector &v) -> Vector &;

    //! Computes this += (v, v, v)
    auto operator+=(T v) -> Vector &;

    //! Computes this += (v.x, v.y, v.z)
    auto operator+=(const Vector &v) -> Vector &;

    //! Computes this -= (v, v, v)
    auto operator-=(T v) -> Vector &;

    //! Computes this -= (v.x, v.y, v.z)
    auto operator-=(const Vector &v) -> Vector &;

    //! Computes this *= (v, v, v)
    auto operator*=(T v) -> Vector &;

    //! Computes this *= (v.x, v.y, v.z)
    auto operator*=(const Vector &v) -> Vector &;

    //! Computes this /= (v, v, v)
    auto operator/=(T v) -> Vector &;

    //! Computes this /= (v.x, v.y, v.z)
    auto operator/=(const Vector &v) -> Vector &;

    //! Returns true if \p other is the same as this vector.
    auto operator==(const Vector &v) const -> bool;

    //! Returns true if \p other is the not same as this vector.
    auto operator!=(const Vector &v) const -> bool;
};

//! Type alias for three dimensional vector.
template<typename T> using Vector3 = Vector<T, 3>;

//! Positive sign operator.
template<typename T>
auto operator+(const Vector3<T> &a) -> Vector3<T>;

//! Negative sign operator.
template<typename T>
auto operator-(const Vector3<T> &a) -> Vector3<T>;

//! Computes (a, a, a) + (b.x, b.y, b.z).
template<typename T>
auto operator+(T a, const Vector3<T> &b) -> Vector3<T>;

//! Computes (a.x, a.y, a.z) + (b.x, b.y, b.z).
template<typename T>
auto operator+(const Vector3<T> &a, const Vector3<T> &b) -> Vector3<T>;

//! Computes (a.x, a.y, a.z) - (b, b, b).
template<typename T>
auto operator-(const Vector3<T> &a, T b) -> Vector3<T>;

//! Computes (a, a, a) - (b.x, b.y, b.z).
template<typename T>
auto operator-(T a, const Vector3<T> &b) -> Vector3<T>;

//! Computes (a.x, a.y, a.z) - (b.x, b.y, b.z).
template<typename T>
auto operator-(const Vector3<T> &a, const Vector3<T> &b) -> Vector3<T>;

//! Computes (a.x, a.y, a.z) * (b, b, b).
template<typename T>
auto operator*(const Vector3<T> &a, T b) -> Vector3<T>;

//! Computes (a, a, a) * (b.x, b.y, b.z).
template<typename T>
auto operator*(T a, const Vector3<T> &b) -> Vector3<T>;

//! Computes (a.x, a.y, a.z) * (b.x, b.y, b.z).
template<typename T>
auto operator*(const Vector3<T> &a, const Vector3<T> &b) -> Vector3<T>;

//! Computes (a.x, a.y, a.z) / (b, b, b).
template<typename T>
auto operator/(const Vector3<T> &a, T b) -> Vector3<T>;

//! Computes (a, a, a) / (b.x, b.y, b.z).
template<typename T>
auto operator/(T a, const Vector3<T> &b) -> Vector3<T>;

//! Computes (a.x, a.y, a.z) / (b.x, b.y, b.z).
template<typename T>
auto operator/(const Vector3<T> &a, const Vector3<T> &b) -> Vector3<T>;

//! Returns element-wise min vector.
template<typename T>
auto min(const Vector3<T> &a, const Vector3<T> &b) -> Vector3<T>;

//! Returns element-wise max vector.
template<typename T>
auto max(const Vector3<T> &a, const Vector3<T> &b) -> Vector3<T>;

//! Returns element-wise clamped vector.
template<typename T>
auto clamp(const Vector3<T> &v, const Vector3<T> &low, const Vector3<T> &high) -> Vector3<T>;

//! Returns element-wise ceiled vector.
template<typename T>
auto ceil(const Vector3<T> &a) -> Vector3<T>;

//! Returns element-wise floored vector.
template<typename T>
auto floor(const Vector3<T> &a) -> Vector3<T>;

//! Float-type 3D vector.
using Vector3F = Vector3<float>;

//! Double-type 3D vector.
using Vector3D = Vector3<double>;

// MARK: Extensions

//! Returns float-type zero vector.
template<>
constexpr auto zero<Vector3F>() -> Vector3F
{
    return Vector3F(0.f, 0.f, 0.f);
}

//! Returns double-type zero vector.
template<>
constexpr auto zero<Vector3D>() -> Vector3D
{
    return Vector3D(0.0, 0.0, 0.0);
}

//! Returns the type of the value itself.
template<typename T>
struct ScalarType<Vector3<T>>
{
    using value = T;
};

//! Computes monotonic Catmull-Rom interpolation.
template<typename T>
auto monotonicCatmullRom(const Vector3<T> &v0, const Vector3<T> &v1, const Vector3<T> &v2, const Vector3<T> &v3, T f) -> Vector3<T>;

}  // namespace HinaPE::FluidEngine

#include "vector3-inl.h"

#endif  // HINAPE_FLUID_ENGINE_VECTOR3_H_
