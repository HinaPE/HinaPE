#include "api.h"
#include "imgui.h"

void HinaPE::Cloth::Api::step(Kasumi::ScenePtr &scene, float dt)
{

}
void HinaPE::Cloth::Api::ui_sidebar(Kasumi::ScenePtr &scene)
{
    ImGui::Text("Cloth");
    static std::array<int, 2> dim2 = {30, 30};
    ImGui::InputInt2("dims", dim2.data());
    if (ImGui::Button("Create"))
    {
        _cloth_data = std::make_shared<ClothGeometryData>(10.f, 10.f, (size_t) dim2[0], (size_t) dim2[1]);
        auto verts = _cloth_data->get_vertices();
        auto indices = _cloth_data->get_indices();
        std::vector<Kasumi::TexturedMesh::Vertex> res_v;
        for (auto &v: verts)
        {
            Kasumi::TexturedMesh::Vertex v_;
            v_.position = v;
            v_.tex_coord = {v.x / static_cast<float>(dim2[1]), v.y / static_cast<float>(dim2[0])};
            res_v.emplace_back(v_);
        }
        auto tex = std::make_shared<Kasumi::Texture>("/Users/xayah/Downloads/TexturesCom_FabricWool0022_2_seamless_S.jpg");
        scene->add_primitive(std::move(res_v), std::move(indices), tex);
    }
}
