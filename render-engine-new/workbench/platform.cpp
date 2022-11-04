#include "platform.h"

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
