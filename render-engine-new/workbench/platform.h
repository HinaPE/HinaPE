#ifndef HINAPE_RENDER_ENGINE_PRO_PLATFORM_H
#define HINAPE_RENDER_ENGINE_PRO_PLATFORM_H

#include <SDL2/SDL.h>

#include "app.h"

#include <utility>

namespace HinaPE::RenderEngine::Workbench
{
class Platform final
{
public:
    Platform(int width, int height);
    ~Platform();

public:
    void update(App &app);

public:
    static void remove_console();
    static auto console_width() -> int;
    static void strcpy(char *dest, const char *src, size_t limit);

public:
    auto window_draw() -> std::pair<float, float>;
    auto window_size() -> std::pair<float, float>;
    auto scale(std::pair<float, float> pt) -> std::pair<float, float>;
    void capture_mouse();
    void release_mouse();
    void grab_mouse();
    void ungrab_mouse();
    void set_mouse(std::pair<float, float> pt);
    auto get_mouse() -> std::pair<float, float>;
    auto is_down(SDL_Scancode key) -> bool;

private:
    void set_dpi();
    void begin_frame();
    void complete_frame();

private:
    SDL_Window *window = nullptr;
    SDL_GLContext gl_context = nullptr;
    const Uint8 *keybuf = nullptr;
    float prev_dpi = 0.0f;
    float prev_scale = 0.0f;
};
}

#endif //HINAPE_RENDER_ENGINE_PRO_PLATFORM_H
