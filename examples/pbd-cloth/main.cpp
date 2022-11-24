#include "renderer/app.h"
#include "api.h"

auto main() -> int
{
    auto platform = std::make_shared<Kasumi::Platform>(1024, 768);
    auto app = std::make_shared<Kasumi::Renderer>("empty.txt");
    auto cloth_engine = std::make_shared<HinaPE::Cloth::Api>();
    app->load_api(cloth_engine);
    platform->launch(app);
    return 0;
}