#ifndef HINAPE_PARTICLE_EMITTER3_H
#define HINAPE_PARTICLE_EMITTER3_H

#include "../common.h"

#include "particle_system_data3.h"

#include <random>
#include <variant>

namespace HinaPE
{

inline auto uniform_sample_cone(double u1, double u2, const Vector3D &axis, double angle) -> Vector3D
{
    double cos_angle_2 = std::cos(angle / 2);
    double y = 1 - (1 - cos_angle_2) * u1;
    double r = std::sqrt(std::max<double>(0, 1 - y * y));
    double phi = 2.0 * 3.14159265358 * u2;
    double x = r * std::cos(phi);
    double z = r * std::sin(phi);
    auto ts = Vector3D(-axis.y(), axis.x(), axis.z());

    Vector3D tmp = (std::fabs(ts.y()) > 0 || std::fabs(ts.z()) > 0) ? Vector3D(1, 0, 0) : Vector3D(0, 1, 0);
    Vector3D a = tmp.cross(axis).normalized();
    Vector3D b = axis.cross(a);

    return a * x + axis * y + b * z;
}

struct PointGenerator
{
    double first_frame_time_in_second = 0.0;
    size_t number_of_emitted_particles = 0;
    size_t max_number_of_emitted_particles = std::numeric_limits<size_t>::max();
    size_t max_number_of_new_particles_per_second = 1;

    Vector3D origin;
    Vector3D direction;
    double speed;
    double spread_angle_in_degree;

    auto generate(Array1<Vector3D> *new_positions, Array1<Vector3D> *new_velocities, size_t max_num) -> void
    {
        for (int i = 0; i < max_num; ++i)
        {
            Vector3D new_direction = uniform_sample_cone(random(), random(), direction, spread_angle_in_degree);
            new_positions->append(origin);
            new_velocities->append(new_direction * speed);
        }
    }

    std::mt19937 rng;
    auto random() -> double { return std::uniform_real_distribution<double>(0.0, 1.0)(rng); }
};

class ParticleEmitter3
{
public:
    auto update() -> void;

public:
    ParticleEmitter3() = default;

    struct Opt
    {
        bool is_enabled = true;
    };
    Opt opt;

private:
    auto on_update() -> void;

private:
    ParticleSystemData3Ptr data_ptr;
    std::variant<PointGenerator> particle_generator;
};

}

#endif //HINAPE_PARTICLE_EMITTER3_H
