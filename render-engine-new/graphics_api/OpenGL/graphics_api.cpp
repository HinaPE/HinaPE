#include "../graphics_api.h"

#include <glad/glad.h>

#include <iostream>
#include <string>

static void debug_proc(GLenum glsource, GLenum gltype, GLuint, GLenum severity, GLsizei, const GLchar *glmessage, const void *)
{
    std::string message(glmessage);
    std::string source, type;

    switch (glsource)
    {
        case GL_DEBUG_SOURCE_API:
            source = "OpenGL API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            source = "Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            source = "Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            source = "Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            source = "Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            source = "Other";
            break;
        default:
            source = "Unknown";
            break;
    }

    switch (gltype)
    {
        case GL_DEBUG_TYPE_ERROR:
            type = "Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            type = "Deprecated";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            type = "Undefined Behavior";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            type = "Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            type = "Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            type = "Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            type = "Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            type = "Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            type = "Other";
            break;
        default:
            type = "Unknown";
            break;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::clog << "OpenGL " << type << " (" << source << "): " << message << std::endl;
            break;
        default:
            break;
    }
}
void HinaPE::RenderEngine::GraphicsAPI::setup()
{
    static bool is_gl45 = false;
    static bool is_gl41 = false;

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    is_gl45 = major == 4 && minor == 5;
    is_gl41 = major == 4 && minor == 1;

    if (!glDebugMessageCallback || !glDebugMessageControl)
        return;
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debug_proc, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // TODO: Effect init
}
void HinaPE::RenderEngine::GraphicsAPI::shutdown()
{
    // TODO: Effect shutdown
}
void HinaPE::RenderEngine::GraphicsAPI::wireframe_mode(bool enable)
{
    if (enable)
    {
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_POLYGON_OFFSET_LINE);
    }
}
void HinaPE::RenderEngine::GraphicsAPI::offset(bool enable)
{
    if (enable)
        glEnable(GL_POLYGON_OFFSET_FILL);
    else
        glDisable(GL_POLYGON_OFFSET_FILL);
}
void HinaPE::RenderEngine::GraphicsAPI::culling(bool enable)
{
    if (enable)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
}
void HinaPE::RenderEngine::GraphicsAPI::depth_write(bool enable)
{
    if (enable)
        glDepthMask(GL_TRUE);
    else
        glDepthMask(GL_FALSE);
}
