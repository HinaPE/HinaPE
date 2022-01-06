#ifndef HINAPE_VIEWER_H
#define HINAPE_VIEWER_H

#include <igl/opengl/glfw/Viewer.h>

namespace HinaPE
{
    class World;

    class Viewer
    {
    public:

        Viewer() = default;

        ~Viewer() = default;

    public:
        void render();

        void set_world(std::shared_ptr<World> &current_world);

    private:
        igl::opengl::glfw::Viewer viewer_;

        std::weak_ptr<World> current_world_;
    };
}


#endif //HINAPE_VIEWER_H
