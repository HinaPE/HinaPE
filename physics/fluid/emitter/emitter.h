#ifndef HINAPE_EMITTER_H
#define HINAPE_EMITTER_H

#include "geom/grid.h"
#include "geom/surface3.h"

namespace HinaPE
{
class GridEmitter3
{
public:
	using OnBeginUpdateCallback = std::function<void(real current_time, real time_interval)>;
	void update(real current_time, real time_interval);

public:
	struct Opt
	{
		bool is_enabled = false;
	} _opt;
	virtual void VALID_CHECK() {}

protected:
	virtual void on_update(real current_time, real time_interval) = 0;

private:
	OnBeginUpdateCallback _on_begin_update_callback = nullptr;
};

class VolumeGridEmitter3 final : public GridEmitter3
{
public:
	using ScalarMapper = std::function<real(real, const mVector3 &, real)>;
	using VectorMapper = std::function<mVector3(const mVector3 &, const mVector3 &, const mVector3 &)>;
	void add_signed_distance_target(const Geom::ScalarGrid3Ptr &scalar_grid_target);
	void add_step_function_target(const Geom::ScalarGrid3Ptr &scalar_grid_target, real min_value, real max_value);
	void add_target(const Geom::ScalarGrid3Ptr &scalar_grid_target, ScalarMapper &mapper);
	void add_target(const Geom::VectorGrid3Ptr &vector_grid_target, ScalarMapper &mapper);

public:
	struct Opt : GridEmitter3::Opt
	{
		// one shot
		bool is_one_shot = true;
		bool has_emitted = false;
	} _opt;
	void VALID_CHECK() final;

protected:
	void on_update(real current_time, real time_interval) final;

private:
	void _emit();

private:
	using ScalarTarget = std::tuple<Geom::ScalarGrid3Ptr, ScalarMapper>;
	using VectorTarget = std::tuple<Geom::VectorGrid3Ptr, ScalarMapper>;
	std::vector<ScalarTarget> _scalar_targets;
	std::vector<VectorTarget> _vector_targets;
	Geom::ImplicitSurface3Ptr _source_region;
};


using GridEmitter3Ptr = std::shared_ptr<GridEmitter3>;
using VolumeGridEmitter3Ptr = std::shared_ptr<VolumeGridEmitter3>;
}

#endif //HINAPE_EMITTER_H
