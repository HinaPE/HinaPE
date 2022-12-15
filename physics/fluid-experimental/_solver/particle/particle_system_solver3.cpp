#include "particle_system_solver3.h"

#include "array/array_utils.h"
#include "parallel.h"

void HinaPE::Fluid::ParticleSystemSolver3::on_init_physics()
{
#ifdef HinaDebug
	Timer timer;
#endif

	_particle_system_data = std::make_shared<ParticleSystemData3>();

	update_collider(0.f);
	update_emitter(0.f);

#ifdef HinaDebug
	timer.duration("On Init Physics Phase");
#endif
}
void HinaPE::Fluid::ParticleSystemSolver3::on_update_physics(float time_step)
{
#ifdef HinaDebug
	Timer timer;
#endif

	begin_particle_system_update(time_step);

#ifdef HinaDebug
	timer.duration("Begin Particle System Update");
#endif

	accumulate_force(time_step);

#ifdef HinaDebug
	timer.duration("Accumulate Force");
#endif

	time_integration(time_step);

#ifdef HinaDebug
	timer.duration("Time Integration");
#endif

	resolve_collision();

#ifdef HinaDebug
	timer.duration("Resolve Collision");
#endif

	end_particle_system_update(time_step);

#ifdef HinaDebug
	timer.duration("End Particle System Update");
#endif
}
void HinaPE::Fluid::ParticleSystemSolver3::accumulate_force(float dt)
{
	size_t n = _particle_system_data->particles_num();
	auto positions = _particle_system_data->positions();
	auto velocities = _particle_system_data->velocities();
	auto forces = _particle_system_data->forces();
	const real mass = _particle_system_data->mass();

	parallelFor((size_t) 0, n, [&](size_t i)
	{
		mVector3 force = mass * _opt.gravity;
		// TODO: wind force

		forces[i] += force;
	});
}
void HinaPE::Fluid::ParticleSystemSolver3::begin_particle_system_update(real dt)
{
#ifdef HinaDebug
	Timer timer;
#endif

	// clear forces
	auto forces = _particle_system_data->forces();
	setRange1(forces.size(), mVector3(), &forces);


#ifdef HinaDebug
	timer.duration("Clear Forces");
#endif

	update_collider(dt);

#ifdef HinaDebug
	timer.duration("Update Collider");
#endif

	update_emitter(dt);

#ifdef HinaDebug
	timer.duration("Update Emitter");
#endif

	size_t n = _particle_system_data->particles_num();
	_new_positions.resize(n);
	_new_velocities.resize(n);

#ifdef HinaDebug
	timer.duration("Resize Buffers");
#endif

	on_begin_particle_system_update(dt);

#ifdef HinaDebug
	timer.duration("On Begin Particle System Update");
#endif
}
void HinaPE::Fluid::ParticleSystemSolver3::end_particle_system_update(real dt)
{
	on_end_particle_system_update(dt);
}
void HinaPE::Fluid::ParticleSystemSolver3::resolve_collision() { resolve_collision(_new_positions.accessor(), _new_velocities.accessor()); }
void HinaPE::Fluid::ParticleSystemSolver3::resolve_collision(HinaPE::ArrayAccessor1<mVector3> positions, HinaPE::ArrayAccessor1<mVector3> velocities)
{
	size_t n = _particle_system_data->particles_num();
	const auto radius = _particle_system_data->_opt.radius;

	parallelFor((size_t) 0, n, [&](size_t i)
	{
		// TODO: resolve collision
	});
}
void HinaPE::Fluid::ParticleSystemSolver3::update_collider(real dt)
{
}
void HinaPE::Fluid::ParticleSystemSolver3::update_emitter(real dt)
{
}
void HinaPE::Fluid::ParticleSystemSolver3::time_integration(real dt)
{
}
