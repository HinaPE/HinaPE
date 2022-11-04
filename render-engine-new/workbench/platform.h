#ifndef HINAPE_RENDER_ENGINE_PRO_PLATFORM_H
#define HINAPE_RENDER_ENGINE_PRO_PLATFORM_H

#include <SDL2/SDL.h>

namespace HinaPE::RenderEngine::Workbench
{
class Platform
{
public:
    Platform(int width, int height);
    ~Platform();

public:


private:
    SDL_Window *window = nullptr;
    SDL_GLContext gl_context = nullptr;
    const Uint8 *keybuf = nullptr;
};
}

#endif //HINAPE_RENDER_ENGINE_PRO_PLATFORM_H
