#include "app.h"
#include "lagrangian/solver.h"

class FluidApp : public Kasumi::App
{
protected:
	void prepare() final
	{
	}
	void update(double dt) final
	{
	}
};

auto main() -> int
{
	std::make_shared<FluidApp>()->launch();
	return 0;
}