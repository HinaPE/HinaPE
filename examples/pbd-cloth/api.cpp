#include "api.h"
#include "imgui.h"

void HinaPE::Cloth::Api::step(float dt)
{
    if (_solvers.empty())
        return;
    for (auto &pair: _solvers)
        pair.second->step(dt);
    sync();
}
void HinaPE::Cloth::Api::ui_sidebar()
{
    ImGui::Text("Cloth");
    static std::array<float, 3> position = {0, 0, 0};
    static std::array<int, 2> dim2 = {30, 30};
    ImGui::InputFloat3("position", position.data());
    ImGui::InputInt2("dims", dim2.data());
    if (ImGui::Button("Create"))
    {
        ClothSolver::Opt opt;
        opt.width = 10;
        opt.height = 10;
        auto solver = std::make_shared<ClothSolver>(opt);
        auto &verts = solver->vertices();
        auto &indices = solver->indices();
        std::vector<Kasumi::TexturedMesh::Vertex> res_v;
        for (auto &v: verts)
        {
            Kasumi::TexturedMesh::Vertex v_;
            v_.position = v;
            v_.tex_coord = {v.x / static_cast<float>(dim2[1]), v.y / static_cast<float>(dim2[0])};
            res_v.emplace_back(v_);
        }
        auto tex = std::make_shared<Kasumi::Texture>(std::string(MyAssetDir) + "TexturesCom_FabricWool0022_2_seamless_S.jpg");
        auto res = _scene->add_primitive(std::move(res_v), std::move(indices), tex);
        _solvers[res.second] = solver;
    }
}
void HinaPE::Cloth::Api::sync()
{
    for (auto &pair: _solvers)
    {
        auto &vert_mesh = pair.first->vertices();
        const auto &vert_physics = pair.second->vertices();

        for (int i = 0; i < vert_mesh.size(); ++i)
            vert_mesh[i].position = vert_physics[i];
    }
}
