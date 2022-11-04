#ifndef HINAPE_RENDER_ENGINE_PRO_GRAPHICS_API_H
#define HINAPE_RENDER_ENGINE_PRO_GRAPHICS_API_H

namespace HinaPE::RenderEngine
{
class GraphicsAPI
{
public:
    static void setup();
    static void shutdown();

public:
    static void wireframe_mode(bool enable);
    static void offset(bool enable);
    static void culling(bool enable);
    static void depth_write(bool enable);
};
}

#endif //HINAPE_RENDER_ENGINE_PRO_GRAPHICS_API_H
