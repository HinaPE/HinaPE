#ifndef HINAPE_PARTICLE_SYSTEM_DATA3_H
#define HINAPE_PARTICLE_SYSTEM_DATA3_H

#include "../common.h"
#include "../util/lib/array1.h"
#include "../util/point_neighbor_searcher3.h"

#include <memory>
#include <vector>

namespace HinaPE
{
class ParticleSystemData3
{
public:
    using ScalarData = Array1<double>;
    using VectorData = Array1<Vector3D>;

private:
    double _radius;
    double _mass;
    size_t _number_of_particles;
    size_t _position_idx;
    size_t _velocity_idx;
    size_t _force_idx;

    std::vector<ScalarData> _scalar_data_list;
    std::vector<VectorData> _vector_data_list;
    PointNeighborSearcher3Ptr _neighbor_searcher;
    std::vector<std::vector<size_t>> _neighbor_lists;

public:
    [[nodiscard]] auto positions() const -> ConstArrayAccessor1<Vector3D>;
    [[nodiscard]] auto positions() -> ArrayAccessor1<Vector3D>;
    [[nodiscard]] auto velocities() const -> ConstArrayAccessor1<Vector3D>;
    [[nodiscard]] auto velocities() -> ArrayAccessor1<Vector3D>;
    [[nodiscard]] auto forces() const -> ConstArrayAccessor1<Vector3D>;
    [[nodiscard]] auto forces() -> ArrayAccessor1<Vector3D>;
    [[nodiscard]] auto scalar_data_at(size_t idx) const -> ConstArrayAccessor1<double>;
    [[nodiscard]] auto scalar_data_at(size_t idx) -> ArrayAccessor1<double>;
    [[nodiscard]] auto vector_data_at(size_t idx) const -> ConstArrayAccessor1<Vector3D>;
    [[nodiscard]] auto vector_data_at(size_t idx) -> ArrayAccessor1<Vector3D>;
    [[nodiscard]] auto number_of_particles() const -> size_t;
    [[nodiscard]] auto radius() const -> double;
    [[nodiscard]] auto mass() const -> double;
    [[nodiscard]] auto neighbor_searcher() const -> const PointNeighborSearcher3Ptr &;
    [[nodiscard]] auto neighbor_lists() const -> const std::vector<std::vector<size_t>> &;

public:
    auto set(const ParticleSystemData3 &other) -> void;
    auto resize(size_t new_number_of_particles) -> void;
    auto add_scalar_data(double initial_val = 0.0) -> size_t;
    auto add_vector_data(const Vector3D &initial_val = Vector3D()) -> size_t;
    virtual auto set_radius(double new_radius) -> void;
    virtual auto set_mass(double new_mass) -> void;
    auto set_neighbor_searcher(const PointNeighborSearcher3Ptr &new_neighbor_searcher) -> void;
    auto add_particle(const Vector3D &position, const Vector3D &velocity = Vector3D(), const Vector3D &force = Vector3D()) -> void;
    auto add_particles(const ConstArrayAccessor1<Vector3D> &positions, const ConstArrayAccessor1<Vector3D> &velocities = ConstArrayAccessor1<Vector3D>(), const ConstArrayAccessor1<Vector3D> &forces = ConstArrayAccessor1<Vector3D>()) -> void;
    auto build_neighbor_searcher(double max_search_radius) -> void;
    auto build_neighbor_lists(double max_search_radius) -> void;

public:
    ParticleSystemData3();
    explicit ParticleSystemData3(size_t number_of_particles);
    ParticleSystemData3(const ParticleSystemData3 &other);
    virtual ~ParticleSystemData3();
    auto operator=(const ParticleSystemData3 &other) -> ParticleSystemData3 &;
};
using ParticleSystemData3Ptr = std::shared_ptr<ParticleSystemData3>;
}

#endif //HINAPE_PARTICLE_SYSTEM_DATA3_H
