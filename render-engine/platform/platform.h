#ifndef HINAPE_PLATFORM_H
#define HINAPE_PLATFORM_H

#include <SDL2/SDL.h>

#include "../app.h"
#include "../lib/mathlib.h"

class Platform
{
public:
    Platform();
    ~Platform();

    void loop(App &app);

    auto window_draw() -> Vec2;
    auto window_size() -> Vec2;
    auto scale(Vec2 pt) -> Vec2;

    void capture_mouse();
    void release_mouse();
    void set_mouse(Vec2 pos);
    auto get_mouse() -> Vec2;
    void grab_mouse();
    void ungrab_mouse();
    auto is_down(SDL_Scancode key) -> bool;

    static void remove_console();
    static auto console_width() -> int;
    static void strcpy(char *dest, const char *src, size_t limit);

private:
    float prev_dpi = 0.0f;
    float prev_scale = 0.0f;

    void set_dpi();
    void platform_init();
    void platform_shutdown();
    void begin_frame();
    void complete_frame();

    SDL_Window *window = nullptr;
    SDL_GLContext gl_context = nullptr;
    const Uint8 *keybuf = nullptr;
};

#endif
