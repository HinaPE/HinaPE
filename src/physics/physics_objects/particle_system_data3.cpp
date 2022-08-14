#include "particle_system_data3.h"
HinaPE::ParticleSystemData3::ParticleSystemData3() = default;
HinaPE::ParticleSystemData3::ParticleSystemData3(size_t number_of_particles) : _number_of_particles(number_of_particles) {}
HinaPE::ParticleSystemData3::ParticleSystemData3(const HinaPE::ParticleSystemData3 &other) = default;
HinaPE::ParticleSystemData3::~ParticleSystemData3() = default;
auto HinaPE::ParticleSystemData3::positions() const -> HinaPE::ConstArrayAccessor1<Vector3D> { return vector_data_at(_position_idx); }
auto HinaPE::ParticleSystemData3::positions() -> HinaPE::ArrayAccessor1<Vector3D> { return vector_data_at(_position_idx); }
auto HinaPE::ParticleSystemData3::velocities() const -> HinaPE::ConstArrayAccessor1<Vector3D> { return vector_data_at(_velocity_idx); }
auto HinaPE::ParticleSystemData3::velocities() -> HinaPE::ArrayAccessor1<Vector3D> { return vector_data_at(_velocity_idx); }
auto HinaPE::ParticleSystemData3::forces() const -> HinaPE::ConstArrayAccessor1<Vector3D> { return vector_data_at(_force_idx); }
auto HinaPE::ParticleSystemData3::forces() -> HinaPE::ArrayAccessor1<Vector3D> { return vector_data_at(_force_idx); }
auto HinaPE::ParticleSystemData3::scalar_data_at(size_t idx) const -> HinaPE::ConstArrayAccessor1<double> { return _scalar_data_list[idx].const_accessor(); }
auto HinaPE::ParticleSystemData3::scalar_data_at(size_t idx) -> HinaPE::ArrayAccessor1<double> { return _scalar_data_list[idx].accessor(); }
auto HinaPE::ParticleSystemData3::vector_data_at(size_t idx) const -> HinaPE::ConstArrayAccessor1<Vector3D> { return _vector_data_list[idx].const_accessor(); }
auto HinaPE::ParticleSystemData3::vector_data_at(size_t idx) -> HinaPE::ArrayAccessor1<Vector3D> { return _vector_data_list[idx].accessor(); }
auto HinaPE::ParticleSystemData3::number_of_particles() const -> size_t { return _number_of_particles; }
auto HinaPE::ParticleSystemData3::radius() const -> double { return _radius; }
auto HinaPE::ParticleSystemData3::mass() const -> double { return _mass; }
auto HinaPE::ParticleSystemData3::neighbor_searcher() const -> const HinaPE::PointNeighborSearcher3Ptr & { return _neighbor_searcher; }
auto HinaPE::ParticleSystemData3::neighbor_lists() const -> const std::vector<std::vector<size_t>> & { return _neighbor_lists; }
auto HinaPE::ParticleSystemData3::set(const HinaPE::ParticleSystemData3 &other) -> void
{
    _radius = other._radius;
    _mass = other._mass;
    _position_idx = other._position_idx;
    _velocity_idx = other._velocity_idx;
    _force_idx = other._force_idx;
    _number_of_particles = other._number_of_particles;

    for (auto& attr : other._scalar_data_list) {
        _scalar_data_list.emplace_back(attr);
    }

    for (auto& attr : other._vector_data_list) {
        _vector_data_list.emplace_back(attr);
    }

    _neighbor_searcher = other._neighbor_searcher->clone();
    _neighbor_lists = other._neighbor_lists;
}
auto HinaPE::ParticleSystemData3::resize(size_t new_number_of_particles) -> void
{
    _number_of_particles = new_number_of_particles;
    for (auto &attr: _scalar_data_list)
        attr.resize(new_number_of_particles, 0.0);
    for (auto &attr: _vector_data_list)
        attr.resize(new_number_of_particles, Vector3D());
}
auto HinaPE::ParticleSystemData3::add_scalar_data(double initial_val) -> size_t
{
    size_t attrIdx = _scalar_data_list.size();
    _scalar_data_list.emplace_back(number_of_particles(), initial_val);
    return attrIdx;
}
auto HinaPE::ParticleSystemData3::add_vector_data(const Vector3D &initial_val) -> size_t
{
    size_t attrIdx = _vector_data_list.size();
    _vector_data_list.emplace_back(number_of_particles(), initial_val);
    return attrIdx;
}
auto HinaPE::ParticleSystemData3::set_radius(double new_radius) -> void { _radius = std::max(new_radius, 0.0); }
auto HinaPE::ParticleSystemData3::set_mass(double new_mass) -> void { _mass = std::max(new_mass, 0.0); }
auto HinaPE::ParticleSystemData3::set_neighbor_searcher(const HinaPE::PointNeighborSearcher3Ptr &newNeighborSearcher) -> void { _neighbor_searcher = newNeighborSearcher; }
auto HinaPE::ParticleSystemData3::add_particle(const Vector3D &position, const Vector3D &velocity, const Vector3D &force) -> void
{
    Array1<Vector3D> new_positions = {position};
    Array1<Vector3D> new_velocities = {velocity};
    Array1<Vector3D> new_forces = {force};

    HinaPE::ConstArrayAccessor1<Vector3D> a;
    add_particles(new_positions.const_accessor(), new_velocities.const_accessor(), new_forces.const_accessor());
}
auto HinaPE::ParticleSystemData3::add_particles(const HinaPE::ConstArrayAccessor1<Vector3D> &new_positions, const HinaPE::ConstArrayAccessor1<Vector3D> &new_velocities, const HinaPE::ConstArrayAccessor1<Vector3D> &new_forces) -> void
{
    size_t old_number_of_particles = number_of_particles();
    size_t new_number_of_particles = old_number_of_particles + new_positions.size();

    resize(new_number_of_particles);

    auto pos = positions();
    auto vel = velocities();
    auto frc = forces();

}
auto HinaPE::ParticleSystemData3::build_neighbor_searcher(double max_search_radius) -> void
{
    // TODO: implement this
}
auto HinaPE::ParticleSystemData3::build_neighbor_lists(double max_search_radius) -> void
{
    // TODO: implement this
}
auto HinaPE::ParticleSystemData3::operator=(const HinaPE::ParticleSystemData3 &other) -> HinaPE::ParticleSystemData3 &
{
    set(other);
    return *this;
}
