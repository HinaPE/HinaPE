#ifndef HINAPE_RIGIDBODY_H
#define HINAPE_RIGIDBODY_H

#include "../lib/vec3.h"
#include <type_traits>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64)
#define HINA_FORCE_INLINE __forceinline
#else
#define HINA_FORCE_INLINE __attribute__((always_inline))
#endif

namespace HinaPE
{
enum RigidBodyType
{
    DYNAMIC = 1 << 0,
    STATIC = 1 << 1,
    KINEMATIC = 1 << 2,
};

template<RigidBodyType Type>
class RigidBody
{
public:
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    HINA_FORCE_INLINE RigidBody<T> *add_force(const Vec3 &f);

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    HINA_FORCE_INLINE RigidBody<T> *add_acceleration(const Vec3 &a);

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    HINA_FORCE_INLINE RigidBody<T> *set_linear_velocity(const Vec3 &v);

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    HINA_FORCE_INLINE RigidBody<T> *set_angular_velocity(const Vec3 &w);

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    HINA_FORCE_INLINE RigidBody<T> *set_linear_damping(float d);

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    HINA_FORCE_INLINE RigidBody<T> *set_angular_damping(float d);

    template<RigidBodyType T = Type, typename = typename std::enable_if<(T == DYNAMIC || T == KINEMATIC)>::type>
    HINA_FORCE_INLINE RigidBody<T> *set_mass(float m);

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] HINA_FORCE_INLINE Vec3 get_linear_velocity() const;

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] HINA_FORCE_INLINE Vec3 get_angular_velocity() const;

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] HINA_FORCE_INLINE float get_linear_damping() const;

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] HINA_FORCE_INLINE float get_angular_damping() const;

    template<RigidBodyType T = Type, typename = typename std::enable_if<(T == DYNAMIC || T == KINEMATIC)>::type>
    [[nodiscard]] HINA_FORCE_INLINE float get_mass();

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}

#include "rigidbody_impl.inl"


#endif //HINAPE_RIGIDBODY_H
