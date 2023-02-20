#include "lagrangian/solver.h"

auto main() -> int
{
	std::make_shared<HinaPE::SPHSolver>()->launch();
	return 0;
}