#include "api.h"

#include "geometry/sphere3.h"
#include "geometry/plane3.h"
#include "geometry/box3.h"
#include "geometry/rigid_body_collider3.h"

#include "imgui.h"

void ParticleSystem::prepare()
{
	// init render object
	{
		auto fluid_model = std::make_shared<Kasumi::Model>("sphere", Color::RED);
		fluid_model->instancing();
		auto id = _scene->add_object(fluid_model);
		_fluid_obj = _scene->get_object(id);
	}
	{
		auto bbox_model = std::make_shared<Kasumi::Model>("cube", Color::NO_COLORS);
		bbox_model->_opt.render_surface = false;
		bbox_model->_opt.render_bbox = true;
		bbox_model->_opt.bbox_color = Color::GRAY;
		auto id = _scene->add_object(bbox_model);
		_bbox_obj = _scene->get_object(id);
	}
}
void ParticleSystem::step(float dt)
{
	static HinaPE::Frame frame;
	if (!_solver)
		return;

	frame.advance();
	_solver->update(frame);
	sync();
}
void ParticleSystem::ui_sidebar()
{
	static bool disable = false;
	ImGui::BeginDisabled(disable);
	if (ImGui::CollapsingHeader("Create Fluid Domain"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		static float bounding_box_size = 1.f;
		ImGui::SliderFloat("Size", &bounding_box_size, 0.01f, 10.0f, "%.2f");
		domain.lowerCorner = mVector3(-bounding_box_size, -bounding_box_size, -bounding_box_size);
		domain.upperCorner = mVector3(bounding_box_size, bounding_box_size, bounding_box_size);
		_bbox_obj->scale() = mVector3(bounding_box_size, bounding_box_size, bounding_box_size);
	}
	if (ImGui::CollapsingHeader("Create Fluid"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		static float target_density = 1000.f;
		static float target_spacing = 0.05f;
		static float pseudo_viscosity_coefficient = 0.f;
		ImGui::SliderFloat("Density", &target_density, 500.f, 2000.0f, "%10.f");
		ImGui::SliderFloat("Spacing", &target_spacing, 0.01f, 1.0f, "%.02f");
		ImGui::SliderFloat("Viscosity", &pseudo_viscosity_coefficient, 0.0f, 1.0f, "%.02f");
		if (ImGui::Button("Add"))
		{
			// init physics object
			auto sphere = HinaPE::Sphere3::builder()
					.withCenter({_bbox_obj->position().x, _bbox_obj->position().y, _bbox_obj->position().z})
					.withRadius(0.15)
					.makeShared();
			HinaPE::ImplicitSurface3Ptr surface_set = HinaPE::ImplicitSurfaceSet3::builder()
					.withExplicitSurfaces({sphere})
					.makeShared();
			auto box = HinaPE::Box3::builder()
					.withIsNormalFlipped(true)
					.withBoundingBox(HinaPE::BoundingBox3D({domain.lowerCorner.x, domain.lowerCorner.y, domain.lowerCorner.z}, {domain.upperCorner.x, domain.upperCorner.y, domain.upperCorner.z}))
					.makeShared();
			auto collider = HinaPE::RigidBodyCollider3::builder()
					.withSurface(box)
					.makeShared();
			_solver = std::make_shared<HinaPE::Fluid::ParticleSystemSolver3>(surface_set, domain, collider);

			sync();

			disable = true;
		}
	}
	ImGui::EndDisabled();
}
void ParticleSystem::key(int key, int scancode, int action, int mods) {}
void ParticleSystem::sync()
{
	auto const &positions = _solver->particle_positions();

	auto model = _fluid_obj->get_model();
	std::vector<Kasumi::Pose> poses;
	for (auto const &pos: positions)
	{
		Kasumi::Pose pose;
		pose.position = {pos.x, pos.y, pos.z};
		pose.scale = {0.01, 0.01, 0.01};
		poses.push_back(pose);
	}
	model->clear_instances();
	if (!poses.empty())
		model->add_instances(poses);
}
