#include "renderer/app.h"
#include "test_emitter.h"
#include "test_grid.h"
#include "test_pcisph.h"

auto main() -> int
{
	std::make_shared<Kasumi::Renderer>("empty.txt")
			->load_api(std::make_shared<TestPciSPH>())
			->launch();
	return 0;
}
