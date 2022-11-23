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
        _cloth_data = std::make_shared<ClothGeometryData>(10.f, 10.f, (size_t) rows, (size_t) cols);
        auto verts = _cloth_data->get_vertices();
        auto indices = _cloth_data->get_indices();
        std::vector<Kasumi::ColoredMesh::Vertex> res_v;
        std::vector<Kasumi::ColoredMesh::Index> res_i;
        for (auto &v: verts)
        {
            Kasumi::ColoredMesh::Vertex v_;
            v_.position = v;
            res_v.emplace_back(v_);
        }
        for (auto &i: indices)
        {
            Kasumi::ColoredMesh::Index i_;
            i_ = i;
            res_i.emplace_back(i_);
        }
        scene->add_primitive(std::move(res_v), std::move(indices), "BLACK");
    }
    ImGui::End();
}
