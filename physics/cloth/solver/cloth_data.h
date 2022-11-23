#ifndef HINAPE_CLOTH_CLOTH_DATA_H
#define HINAPE_CLOTH_CLOTH_DATA_H

#include "array/array1.h"
#include "math/vector3.h"
#include <memory>
#include <vector>

namespace HinaPE::Cloth
{
class ClothData
{
public:
    using ScalarData = Array1<double>;
    using VectorData = Array1<Vector3D>;
    ClothData();
    explicit ClothData(size_t number_of_particles);

public:
    auto positions() const -> ConstArrayAccessor1<Vector3D>;
    auto position() -> ArrayAccessor1<Vector3D>;
    auto velocities() const -> ConstArrayAccessor1<Vector3D>;
    auto velocity() -> ArrayAccessor1<Vector3D>;
    auto forces() const -> ConstArrayAccessor1<Vector3D>;
    auto force() -> ArrayAccessor1<Vector3D>;
    auto number_of_particles() const -> size_t;
    void resize(size_t new_number_of_particles);

protected:
    auto add_vector_data(const Vector3D &initial_value = Vector3D()) -> size_t;
    auto add_scalar_data(double initial_value = 0.0) -> size_t;
    auto vector_data_at(size_t idx) -> ArrayAccessor1<Vector3D>;
    auto vector_data_at(size_t idx) const -> ConstArrayAccessor1<Vector3D>;
    auto scalar_data_at(size_t idx) -> ArrayAccessor1<double>;
    auto scalar_data_at(size_t idx) const -> ConstArrayAccessor1<double>;

private:
    double _mass = 1e-3;
    size_t _number_of_particles = 0;
    size_t _position_idx;
    size_t _velocity_idx;
    size_t _force_idx;

    std::vector<ScalarData> _scalar_data_list;
    std::vector<VectorData> _vector_data_list;
};
using ClothDataPtr = std::shared_ptr<ClothData>;
}

#endif //HINAPE_CLOTH_CLOTH_DATA_H
