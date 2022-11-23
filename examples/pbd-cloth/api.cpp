#include "api.h"
#include "imgui.h"

void HinaPE::Cloth::Api::step(Kasumi::Workbench::ScenePtr &scene, float dt)
{

}

void HinaPE::Cloth::Api::gui(Kasumi::Workbench::ScenePtr &scene)
{
    static int rows = 30, cols = 30;
    ImGui::Begin("Cloth", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SliderInt("rows", &rows, 1, 50);
    ImGui::SliderInt("cols", &cols, 1, 50);
    if (ImGui::Button("Create"))
    {
        auto data = std::make_shared<ClothGeometryData>(2.f, 2.f, (size_t)rows, (size_t)cols);
        scene->add_primitive("cube");
    }
    ImGui::End();
}
