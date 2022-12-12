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
		PBDSolver::Opt opt;
		opt.rows = dim2[0];
		opt.cols = dim2[1];
		opt.width = 10;
		opt.height = 10;
		auto solver = std::make_shared<PBDSolver>(opt);
		auto &verts = solver->vertices();
		auto &indices = solver->indices();
		std::vector<Kasumi::Model::Vertex> res_v;
		for (auto &v: verts)
		{
			Kasumi::Model::Vertex v_;
			v_.position = v;
			v_.tex_coord = {v.x / static_cast<float>(dim2[1]), v.y / static_cast<float>(dim2[0])};
			res_v.emplace_back(v_);
		}
		std::map<std::string, std::vector<Kasumi::TexturePtr>> textures;
		textures["diffuse"].push_back(std::make_shared<Kasumi::Texture>(std::string(MyAssetDir) + "TexturesCom_FabricWool0022_2_seamless_S.jpg"));
		auto obj_id = _scene->add_object(std::make_shared<Kasumi::Model>(std::move(res_v), std::move(indices), std::move(textures)));
		auto &model = _scene->get_object(obj_id)->get_model();
		std::vector<Kasumi::Pose> poses;
		Kasumi::Pose pose1;
		pose1.position = {0, 0, 0};
		poses.push_back(pose1);
		Kasumi::Pose pose2;
		pose2.position = {5, 5, 5};
		poses.push_back(pose2);
		model->setup_instancing(poses);
		_solvers[_scene->get_object(obj_id)->get_model()] = solver;
	}
}
void HinaPE::Cloth::Api::sync()
{
	for (auto &pair: _solvers)
	{
		auto &vert_mesh = pair.first->vertices(0);
		const auto &vert_physics = pair.second->vertices();

		for (int i = 0; i < vert_mesh.size(); ++i)
			vert_mesh[i].position = vert_physics[i];
	}
}
