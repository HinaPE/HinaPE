#ifndef HINAPE_PARTICLE_SYSTEM_DATA3_H
#define HINAPE_PARTICLE_SYSTEM_DATA3_H

#include "math_api.h"

#include "array/array1.h"

#include <vector>
#include <memory>

namespace HinaPE::Fluid
{
class ParticleSystemData3
{
public:
	void add_particle(const mVector3 &position, const mVector3 &velocity, const mVector3 &force);
	void add_particles(const ConstArrayAccessor1<mVector3> &positions, const ConstArrayAccessor1<mVector3> &velocities = ConstArrayAccessor1<mVector3>(), const ConstArrayAccessor1<mVector3> &forces = ConstArrayAccessor1<mVector3>());

public:
	inline auto positions() const -> ConstArrayAccessor1<mVector3> { return _vector_data_list[_opt.position_index].constAccessor(); }
	inline auto positions() -> ArrayAccessor1<mVector3> { return _vector_data_list[_opt.position_index].accessor(); }
	inline auto velocities() const -> ConstArrayAccessor1<mVector3> { return _vector_data_list[_opt.velocity_index].constAccessor(); }
	inline auto velocities() -> ArrayAccessor1<mVector3> { return _vector_data_list[_opt.velocity_index].accessor(); }
	inline auto forces() const -> ConstArrayAccessor1<mVector3> { return _vector_data_list[_opt.force_index].constAccessor(); }
	inline auto forces() -> ArrayAccessor1<mVector3> { return _vector_data_list[_opt.force_index].accessor(); }
	inline auto mass() const -> real { return _opt.mass; }
	inline auto particles_num() -> size_t { return _vector_data_list.empty() ? 0 : _vector_data_list[_opt.position_index].size(); }

public:
	struct Opt
	{
		real radius = 1e-3;
		real mass = 1e-3;

		// data index
		size_t position_index;
		size_t velocity_index;
		size_t force_index;
	} _opt;

public:
	ParticleSystemData3();

private:
	void resize(size_t size);

private:
	std::vector<Array1<real>> _scalar_data_list; // to ensure the memory is continuous
	std::vector<Array1<mVector3>> _vector_data_list; // to ensure the memory is continuous
};
using ParticleSystemData3Ptr = std::shared_ptr<ParticleSystemData3>;
}

#endif //HINAPE_PARTICLE_SYSTEM_DATA3_H
