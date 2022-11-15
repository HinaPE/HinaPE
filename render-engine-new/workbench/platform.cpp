#include "platform.h"

#include "../../render-engine/platform/font.dat"
#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

#ifdef _WIN32
#include <ConsoleApi.h>
#include <ShellScalingApi.h>
extern "C" {
__declspec(dllexport) bool NvOptimusEnablement = true;
__declspec(dllexport) bool AmdPowerXpressRequestHighPerformance = true;
}
#else
#include <sys/ioctl.h>
#endif

#include "graphics_api/graphics_api.h"
#include <iostream>

HinaPE::RenderEngine::Workbench::Platform::Platform(int width, int height)
{
#ifdef _WIN32
    if (SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE) != S_OK)
        std::cerr << "Failed to set process DPI aware." << std::endl;
#endif

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow("HinaPE is not a physics Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window)
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;

    auto context = [&](int major, int minor)
    {
        if (gl_context)
            return;
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
        gl_context = SDL_GL_CreateContext(window);
    };

#ifndef __APPLE__ // >:|
    context(4, 5);
    if (!gl_context)
        std::clog << "Failed to create OpenGL 4.5 context, trying 4.1 (" << SDL_GetError() << ")" << std::endl;
#endif
    context(4, 1);
    if (!gl_context)
        std::clog << "Failed to create OpenGL 4.1 context, trying 3.3 (" << SDL_GetError() << ")" << std::endl;
    context(3, 3);
    if (!gl_context)
        std::cerr << "Failed to create OpenGL 3.3 context, shutting down (" << SDL_GetError() << ")" << std::endl;

    SDL_GL_MakeCurrent(window, gl_context);
    if (SDL_GL_SetSwapInterval(-1))
    {
        std::cerr << "Failed to enable VSync (" << SDL_GetError() << ")" << std::endl;
        SDL_GL_SetSwapInterval(1);
    }

    if (!gladLoadGL())
        std::cerr << "Failed to initialize OpenGL" << std::endl;

    keybuf = SDL_GetKeyboardState(nullptr);

    GraphicsAPI::setup();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init();
}
HinaPE::RenderEngine::Workbench::Platform::~Platform()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    GraphicsAPI::shutdown();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    window = nullptr;
    gl_context = nullptr;
    SDL_Quit();
}
void HinaPE::RenderEngine::Workbench::Platform::update(App &app)
{
    bool running = true;
    while (running)
    {
        set_dpi();
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            ImGui_ImplSDL2_ProcessEvent(&e);

            switch (e.type)
            {
                case SDL_QUIT:
                {
                    if (app.quit())
                        running = false;
                }
                    break;
            }

            app.event(e);
        }

        begin_frame();
        app.render();
        complete_frame();
    }
}
void HinaPE::RenderEngine::Workbench::Platform::set_dpi()
{
    float dpi;
    int index = SDL_GetWindowDisplayIndex(window);
    if (index < 0)
    {
        return;
    }
    if (SDL_GetDisplayDPI(index, nullptr, &dpi, nullptr))
    {
        return;
    }
    float scale = window_draw().first / window_size().first;
    if (prev_dpi == dpi && prev_scale == scale)
        return;

    ImGuiStyle style;
    ImGui::StyleColorsDark(&style);
    style.WindowRounding = 0.0f;
#ifndef __APPLE__
    style.ScaleAllSizes(0.8f * dpi / 96.0f);
#else
    style.ScaleAllSizes(0.8f);
#endif
    ImGui::GetStyle() = style;

    ImGuiIO &IO = ImGui::GetIO();
    ImFontConfig config;
    config.FontDataOwnedByAtlas = false;
    IO.IniFilename = nullptr;
    IO.Fonts->Clear();
#ifdef __APPLE__
    IO.Fonts->AddFontFromMemoryTTF(font_ttf, font_ttf_len, 14.0f * scale, &config);
    IO.FontGlobalScale = 1.0f / scale;
#else
    IO.Fonts->AddFontFromMemoryTTF(font_ttf, font_ttf_len, 14.0f / 96.0f * dpi, &config);
#endif
    IO.Fonts->Build();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();

    prev_dpi = dpi;
    prev_scale = scale;
}
void HinaPE::RenderEngine::Workbench::Platform::remove_console()
{
#ifdef _WIN32
    FreeConsole();
#endif
}
auto HinaPE::RenderEngine::Workbench::Platform::console_width() -> int
{
    int cols = 0;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    cols = w.ws_col;
#endif
    return cols;
}
void HinaPE::RenderEngine::Workbench::Platform::strcpy(char *dest, const char *src, size_t limit)
{
#ifdef _WIN32
    strncpy_s(dest, limit, src, limit - 1);
#else
    strncpy(dest, src, limit - 1);
    dest[limit - 1] = '\0';
#endif
}
auto HinaPE::RenderEngine::Workbench::Platform::window_draw() -> std::pair<float, float>
{
    int w, h;
    SDL_GL_GetDrawableSize(window, &w, &h);
    return {static_cast<float>(w), static_cast<float>(h)};
}
auto HinaPE::RenderEngine::Workbench::Platform::window_size() -> std::pair<float, float>
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return {static_cast<float>(w), static_cast<float>(h)};
}
auto HinaPE::RenderEngine::Workbench::Platform::scale(std::pair<float, float> pt) -> std::pair<float, float>
{
    auto draw = window_draw();
    auto size = window_size();
    return {pt.first * draw.first / size.first, pt.second * draw.second / size.second};
}
void HinaPE::RenderEngine::Workbench::Platform::capture_mouse()
{
    SDL_CaptureMouse(SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
}
void HinaPE::RenderEngine::Workbench::Platform::release_mouse()
{
    SDL_CaptureMouse(SDL_FALSE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
}
void HinaPE::RenderEngine::Workbench::Platform::grab_mouse() { SDL_SetWindowGrab(window, SDL_TRUE); }
void HinaPE::RenderEngine::Workbench::Platform::ungrab_mouse() { SDL_SetWindowGrab(window, SDL_FALSE); }
void HinaPE::RenderEngine::Workbench::Platform::set_mouse(std::pair<float, float> pt) { SDL_WarpMouseInWindow(window, (int) pt.first, (int) pt.second); }
auto HinaPE::RenderEngine::Workbench::Platform::get_mouse() -> std::pair<float, float>
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return {static_cast<float>(x), static_cast<float>(y)};
}
auto HinaPE::RenderEngine::Workbench::Platform::is_down(SDL_Scancode key) -> bool { return keybuf[key]; }
void HinaPE::RenderEngine::Workbench::Platform::begin_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();
}
void HinaPE::RenderEngine::Workbench::Platform::complete_frame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}
