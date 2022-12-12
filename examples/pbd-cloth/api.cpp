#include "api.h"
#include "imgui.h"

void Api::step(float dt)
{
	if (_cloth_solver == nullptr || _cloth_model == nullptr)
		return;

	_cloth_solver->step(dt);
	sync();
}
void Api::ui_sidebar()
{
	static std::array<float, 3> position = {0, 0, 0};
	static std::array<int, 2> dim2 = {30, 30};

	ImGui::Text("Cloth");
	ImGui::InputFloat3("position", position.data());
	ImGui::InputInt2("dims", dim2.data());

	if (ImGui::Button("Create"))
	{
		// init solver data
		HinaPE::Cloth::PBDSolver::Opt opt;
		opt.rows = dim2[0];
		opt.cols = dim2[1];
		opt.width = 10;
		opt.height = 10;
		_cloth_solver = std::make_shared<HinaPE::Cloth::PBDSolver>(opt);

		// init corresponding rendering model
		auto &verts = _cloth_solver->vertices();
		auto &indices = _cloth_solver->indices();
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
		_cloth_model = std::make_shared<Kasumi::Model>(std::move(res_v), std::move(indices), std::move(textures));
		_scene->add_object(_cloth_model);

		_cloth_particle_model = std::make_shared<Kasumi::Model>("sphere", Color::RED);
		_cloth_particle_model->instancing();
		_scene->add_object(_cloth_particle_model);

		sync();
	}
}
void Api::sync() const
{
	auto &vert_mesh = _cloth_model->vertices(0);
	const auto &vert_physics = _cloth_solver->vertices();

	// sync physics res to mesh
	for (int i = 0; i < vert_mesh.size(); ++i)
		vert_mesh[i].position = vert_physics[i];

	// sync physics res to instancing spheres
	std::vector<Kasumi::Pose> poses;
	for (int i = 0; i < vert_mesh.size(); ++i)
	{
		Kasumi::Pose pose;
		pose.position = vert_physics[i];
		pose.scale = {0.05f, 0.05f, 0.05f};
		poses.emplace_back(pose);
	}
	_cloth_particle_model->clear_instances();
	_cloth_particle_model->add_instances(poses);
}
