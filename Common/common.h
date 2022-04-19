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
#else
#define REAL_MAX FLT_MAX
#define REAL_MIN FLT_MIN
using Real = float;
#endif

using Vector2r = Eigen::Matrix<Real, 2, 1, Eigen::DontAlign>;
using Vector3r = Eigen::Matrix<Real, 3, 1, Eigen::DontAlign>;
using Vector4r = Eigen::Matrix<Real, 4, 1, Eigen::DontAlign>;
using Vector5r = Eigen::Matrix<Real, 5, 1, Eigen::DontAlign>;
using Vector6r = Eigen::Matrix<Real, 6, 1, Eigen::DontAlign>;
using Matrix2r = Eigen::Matrix<Real, 2, 2, Eigen::DontAlign>;
using Matrix3r = Eigen::Matrix<Real, 3, 3, Eigen::DontAlign>;
using Matrix4r = Eigen::Matrix<Real, 4, 4, Eigen::DontAlign>;
using MatrixXr = Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic, Eigen::DontAlign>;
using Vector2i = Eigen::Matrix<int, 2, 1, Eigen::DontAlign>;
using AlignedBox2r = Eigen::AlignedBox<Real, 2>;
using AlignedBox3r = Eigen::AlignedBox<Real, 3>;
using AngleAxisr = Eigen::AngleAxis<Real>;
using Quaternionr = Eigen::Quaternion<Real, Eigen::DontAlign>;

using Vertex3Matrix = Eigen::Matrix<Real, Eigen::Dynamic, 3, Eigen::DontAlign>;
using NormalMatrix = Eigen::Matrix<Real, Eigen::Dynamic, 3, Eigen::DontAlign>;
using TriFaceMatrix = Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::DontAlign>;
using QuadFaceMatrix = Eigen::Matrix<int, Eigen::Dynamic, 4, Eigen::DontAlign>;
using EdgeMatrix = Eigen::Matrix<int, Eigen::Dynamic, 2, Eigen::DontAlign>;

#endif //HINAPE_COMMON_H
