#include "particle_system_data3.h"

#include <stdexcept>

#include "parallel.h"

HinaPE::Fluid::ParticleSystemData3::ParticleSystemData3()
{
	_vector_data_list.resize(3);
	_opt.position_index = 0;
	_opt.velocity_index = 1;
	_opt.force_index = 2;
}

void HinaPE::Fluid::ParticleSystemData3::add_particle(const mVector3 &position, const mVector3 &velocity, const mVector3 &force)
{
	Array1<mVector3> positions{position};
	Array1<mVector3> velocities{velocity};
	Array1<mVector3> forces{force};
	add_particles(positions.constAccessor(), velocities.constAccessor(), forces.constAccessor());
}
void HinaPE::Fluid::ParticleSystemData3::add_particles(const HinaPE::ConstArrayAccessor1<mVector3> &positions_, const HinaPE::ConstArrayAccessor1<mVector3> &velocities_, const HinaPE::ConstArrayAccessor1<mVector3> &forces_)
{
	if (!(positions_.size() == velocities_.size() && positions_.size() == forces_.size()))
		throw std::runtime_error("INVALID INPUT");

	size_t old_size = particles_num();
	size_t new_size = old_size + positions_.size();
	resize(new_size);

	auto pos = positions();
	auto vel = velocities();
	auto frc = forces();

	parallelFor((size_t) 0, positions_.size(), [&](size_t i) { pos[i + old_size] = positions_[i]; });
	parallelFor((size_t) 0, velocities_.size(), [&](size_t i) { vel[i + old_size] = velocities_[i]; });
	parallelFor((size_t) 0, forces_.size(), [&](size_t i) { frc[i + old_size] = forces_[i]; });
}
void HinaPE::Fluid::ParticleSystemData3::resize(size_t size)
{
	for (auto &scalars: _scalar_data_list)
		scalars.resize(size, 0.f);
	for (auto &vectors: _vector_data_list)
		vectors.resize(size, mVector3());
}