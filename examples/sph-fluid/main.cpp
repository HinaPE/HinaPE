#include "renderer/app.h"
#include "api.h"

using namespace HinaPE;

auto main() -> int
{
	auto platform = std::make_shared<Kasumi::Platform>(1920, 768);
	auto app = std::make_shared<Kasumi::Renderer>("empty.txt");
	app->load_api(std::make_shared<HinaPE::Fluid::Api>());
	platform->launch(app);
    return 0;
}
