#ifndef HINAPE_RENDER_ENGINE_PRO_FRAMEBUFFER_H
#define HINAPE_RENDER_ENGINE_PRO_FRAMEBUFFER_H

#include <memory>

namespace HinaPE::RenderEngine
{
class Framebuffer
{
public:
    Framebuffer(int width, int height);

private:
    int _width, _height;
};
using FramebufferPtr = std::shared_ptr<Framebuffer>;
}

#endif //HINAPE_RENDER_ENGINE_PRO_FRAMEBUFFER_H
