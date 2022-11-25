#include "renderer/app.h"
#include "api.h"

auto main() -> int
{
    auto platform = std::make_shared<Kasumi::Platform>(1920, 768);
    auto app = std::make_shared<Kasumi::Renderer>("empty.txt");
    app->load_api(std::make_shared<HinaPE::Rigid::Api>());
    platform->launch(app);
    return 0;
}