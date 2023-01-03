#include "renderer/app.h"
#include "test_emitter.h"

auto main() -> int
{
	std::make_shared<Kasumi::Renderer>("empty.txt")
			->load_api(std::make_shared<TestEmitter>())
			->launch();
	return 0;
}
