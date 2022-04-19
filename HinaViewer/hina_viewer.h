#ifndef HINAPE_HINA_VIEWER_H
#define HINAPE_HINA_VIEWER_H

#include "api/renderable.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>

#include <set>

namespace HinaPE::Viewer
{
    class HinaViewer
    {
    public:
        explicit HinaViewer(unsigned int width, unsigned int height, Eigen::Vector4d background_color = Eigen::Vector4d::Identity());
        virtual ~HinaViewer();

    public:


    protected:
        igl::opengl::glfw::Viewer viewer_;
        std::set<Renderable *> objects_;

    protected:
        unsigned int width_, height_;
        Eigen::Vector4d background_color_;
    };
}

#endif //HINAPE_HINA_VIEWER_H
