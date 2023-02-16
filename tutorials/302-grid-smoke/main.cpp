#include "solver_grid_fluid.h"

auto main() -> int
{
	auto solver = std::make_shared<HinaPE::GridFluidSolver>(
			mSize3{50, 50, 50},
			mVector3{1, 1, 1},
			mVector3{0, 0, 0});

	auto emitter = std::make_shared<HinaPE::VolumeGridEmitter3>();
	solver->add_emitter(emitter);

	for (HinaPE::Animation::Frame frame; frame._index < 100; frame.advance())
		solver->update(frame);

	return 0;
}