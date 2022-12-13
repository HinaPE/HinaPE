#include "api.h"
#include "imgui.h"
#include "implot.h"

void Api::step(float dt)
{
	if (_cloth_solver == nullptr || _cloth_model == nullptr)
		return;

	_cloth_solver->step(dt);
	sync();
}
void Api::ui_sidebar()
{
	static std::array<float, 2> size = {9, 6};
	static std::array<int, 2> dim2 = {30, 30};
	static float stiffness = 1;

	static int phase = 0;
	if (phase == 0)
	{
		ImGui::Text("Cloth");
		ImGui::SliderFloat2("width/height", size.data(), 1, 10);
		ImGui::SliderInt2("rows/cols", dim2.data(), 1, 100);
		ImGui::SliderFloat("stiffness", &stiffness, 0.f, 3.0f, "%.1f");

		if (ImGui::Button("Create"))
		{
			// init solver data
			HinaPE::Cloth::PBDSolver::Opt opt;
			opt.rows = dim2[0];
			opt.cols = dim2[1];
			opt.width = size[0];
			opt.height = size[1];
			_cloth_solver = std::make_shared<HinaPE::Cloth::PBDSolver>(opt);
			_cloth_solver->_opt.distance_constraint_stiffness = stiffness;

			// init corresponding rendering model
			auto &verts = _cloth_solver->vertices();
			auto &indices = _cloth_solver->indices();
			std::vector<Kasumi::Model::Vertex> res_v;
			for (auto &v: verts)
			{
				Kasumi::Model::Vertex v_;
				v_.position = v;
				v_.tex_coord = {0.5f - v.x / static_cast<float>(size[0]), 0.5f - v.y / static_cast<float>(size[1])};
				res_v.emplace_back(v_);
			}
			std::map<std::string, std::vector<Kasumi::TexturePtr>> textures;
			textures["diffuse"].push_back(std::make_shared<Kasumi::Texture>(std::string(MyAssetDir) + "miku.jpg"));
			_cloth_model = std::make_shared<Kasumi::Model>(std::move(res_v), std::move(indices), std::move(textures));
			auto id1 = _scene->add_object(_cloth_model);
			_scene->get_object(id1)->_name = "Miku Flag";

			_scene->_state.selected_object_id = id1;

			_cloth_particle_model = std::make_shared<Kasumi::Model>("sphere", Color::RED);
			_cloth_particle_model->instancing();
			auto id2 = _scene->add_object(_cloth_particle_model);
			_scene->get_object(id2)->_name = "Particles Instance";

			sync();
			++phase;
		}
	}

	ImVec2 s(200.f, 100.f);
	std::string name = "my plot";
	ImPlot::BeginPlot(name.c_str(), s, ImPlotFlags_CanvasOnly | ImPlotFlags_NoChild);
	static float x_data[1000] = {1.f, 2.f, 3.f, 4.f};
	static float y_data[1000] = {1.f, 2.f, 3.f, 4.f};
	ImPlot::PlotLine("My Line Plot", x_data, y_data, 1000);
	ImPlot::EndPlot();
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
		pose.scale = {0.005f, 0.005f, 0.005f};
		poses.emplace_back(pose);
	}
	_cloth_particle_model->clear_instances();
	_cloth_particle_model->add_instances(poses);
}
