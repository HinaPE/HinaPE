#ifndef HINAPE_COMMON_H
#define HINAPE_COMMON_H

#if defined(WIN32) || defined(_WIN32) || defined(WIN64)
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE __attribute__((always_inline))
#endif

#include <Eigen/Eigen>

#ifdef HINAPE_DOUBLE
#define REAL_MAX DBL_MAX
#define REAL_MIN DBL_MIN
using Real=double;
#define PMP_SCALAR_TYPE_64
#else
#define REAL_MAX FLT_MAX
#define REAL_MIN FLT_MIN
using Real = float;
#endif

using Vector2r = Eigen::Matrix<Real, 2, 1>;
using Vector3r = Eigen::Matrix<Real, 3, 1>;
using Vector4r = Eigen::Matrix<Real, 4, 1>;
using Vector5r = Eigen::Matrix<Real, 5, 1>;
using Vector6r = Eigen::Matrix<Real, 6, 1>;
using Matrix2r = Eigen::Matrix<Real, 2, 2>;
using Matrix3r = Eigen::Matrix<Real, 3, 3>;
using Matrix4r = Eigen::Matrix<Real, 4, 4>;
using MatrixXr = Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>;
using Vector2i = Eigen::Matrix<int, 2, 1>;
using Vector3i = Eigen::Matrix<int, 3, 1>;
using Vector4i = Eigen::Matrix<int, 4, 1>;
using AlignedBox2r = Eigen::AlignedBox<Real, 2>;
using AlignedBox3r = Eigen::AlignedBox<Real, 3>;
using AngleAxisr = Eigen::AngleAxis<Real>;
using Quaternionr = Eigen::Quaternion<Real>;

using Vertex3Matrix = Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>;
using NormalMatrix = Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>;
using TriFaceMatrix = Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>;
using QuadFaceMatrix = Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>;
using EdgeMatrix = Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>;


#ifdef _DEBUG
#include <iostream>
#include <fstream>
#endif

namespace HINA_CHECK
{
    struct No
    {
    };
    template<typename T, typename Arg>
    No operator==(const T &, const Arg &);

    template<typename T>
    No operator<<(std::ostream &os, const T &);

    template<typename T, typename Arg = T>
    struct EqualExists
    {
        enum
        {
            value = !std::is_same<decltype(*(T *) (0) == *(Arg *) (0)), No>::value
        };
    };
// TODO: Type Check
    template<typename T, typename Arg = T>
    struct CoutExists
    {
        enum
        {
            value = !std::is_same<decltype(*(T *) (0) == *(Arg *) (0)), No>::value
        };
    };
}

template<typename T>
FORCE_INLINE void log(T &msg)
{
#ifdef _DEBUG
        std::cout << msg << std::endl;
#endif
}

#endif //HINAPE_COMMON_H
