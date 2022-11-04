#include "../graphics_api.h"

#include <glad/glad.h>

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