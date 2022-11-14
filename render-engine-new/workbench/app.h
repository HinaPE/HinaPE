#ifndef HINAPE_RENDER_ENGINE_PRO_APP_H
#define HINAPE_RENDER_ENGINE_PRO_APP_H

#include <SDL2/SDL.h>

namespace HinaPE::RenderEngine::Workbench
{
class App
{
public:
    void render();
    void event(SDL_Event e);
    auto quit() -> bool;
};
}

#endif //HINAPE_RENDER_ENGINE_PRO_APP_H
