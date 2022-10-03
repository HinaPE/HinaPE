#include "particle_emitter3.h"

auto HinaPE::ParticleEmitter3::update() -> void
{
    // TODO: bench mark

    on_update();
}
auto HinaPE::ParticleEmitter3::on_update() -> void
{
    // TODO: still some work here

    HinaPE::Array1<Vector3D> candidate_positions;
    HinaPE::Array1<Vector3D> candidate_velocities;
    HinaPE::Array1<Vector3D> new_positions;
    HinaPE::Array1<Vector3D> new_velocities;

    std::visit([&](auto &&emitter)
               {
                   emitter.generate(&candidate_positions, &candidate_velocities, 100);
               }, particle_generator);

    new_positions.append(candidate_positions);
    new_velocities.append(candidate_velocities);

    data_ptr->add_particles(new_positions.const_accessor(), new_velocities.const_accessor());
}
