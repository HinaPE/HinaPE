#include "api.h"
#include "imgui.h"

void SPHFluidExample::prepare()
{
	HinaPE::Logging::mute();
	auto bbox_model = std::make_shared<Kasumi::Model>("cube", Color::NO_COLORS);
	bbox_model->_opt.render_surface = false; // dont render the surface
	bbox_model->_opt.render_bbox = true; // render the bounding box instead
	bbox_model->_opt.bbox_color = Color::GRAY;
	_bbox_obj = _scene->get_object(_scene->add_object(bbox_model));
}
void SPHFluidExample::step(float dt)
{
	if (_fluid_solver == nullptr)
		return;

	static HinaPE::Frame frame(0, 1.0 / 60.0);
	_fluid_solver->update(frame);

	sync();
}

void SPHFluidExample::ui_sidebar()
{
	static int phase = 0;
	switch (phase)
	{
		case 0:
			phase += phase1();
			break;
		case 1:
			phase += phase2();
			break;
		default:
			break;
	}
}

void SPHFluidExample::sync()
{
	auto const &positions = _fluid_solver->sphSystemData()->positions();

	std::vector<Kasumi::Pose> poses;
	for (auto const &pos: positions)
	{
		Kasumi::Pose pose;
		pose.position = {pos.x, pos.y, pos.z};
		pose.scale = {0.01, 0.01, 0.01};
		poses.push_back(pose);
	}
	_fluid_obj->get_model()->clear_instances();
	if (!poses.empty())
		_fluid_obj->get_model()->add_instances(poses);
}

auto SPHFluidExample::phase1() -> bool
{
	if (ImGui::CollapsingHeader("Create Fluid Domain"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::SliderFloat("Size", &_opt.bounding_box_size, 0.01f, 10.0f, "%.2f");
		_bbox_obj->scale() = mVector3(_opt.bounding_box_size, _opt.bounding_box_size, _opt.bounding_box_size);
		if (ImGui::Button("Add"))
		{
			_bbox_obj->get_model()->_opt.bbox_color = Color::BLACK;
			return true;
		}
	}
	return false;
}
auto SPHFluidExample::phase2() -> bool
{
	if (ImGui::CollapsingHeader("Create Fluid"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::SliderFloat("Density", &_opt.target_density, 500.f, 2000.0f, "%10.f");
		ImGui::SliderFloat("Spacing", &_opt.target_spacing, 0.01f, 1.0f, "%.02f");
		ImGui::SliderFloat("Viscosity", &_opt.pseudo_viscosity_coefficient, 0.0f, 1.0f, "%.02f");
		if (ImGui::Button("Add"))
		{
			auto plane = HinaPE::Plane3::builder()
					.withNormal({0, 1, 0})
					.withPoint({0, 0.25 * _opt.bounding_box_size, 0})
					.makeShared();
			auto sphere = HinaPE::Sphere3::builder()
					.withCenter({_bbox_obj->position().x, _bbox_obj->position().y, _bbox_obj->position().z})
					.withRadius(0.15 * _opt.bounding_box_size)
					.makeShared();
			auto surfaceSet = HinaPE::ImplicitSurfaceSet3::builder()
					.withExplicitSurfaces({plane, sphere})
					.makeShared();
			_fluid_emitter = HinaPE::Fluid::VolumeParticleEmitter3::builder()
					.withImplicitSurface(surfaceSet).withSpacing(_opt.target_spacing)
					.withMaxRegion(HinaPE::BoundingBox3D({-_opt.bounding_box_size, -_opt.bounding_box_size, -_opt.bounding_box_size}, {_opt.bounding_box_size, _opt.bounding_box_size, _opt.bounding_box_size}))
					.withMaxNumberOfParticles(10000)
					.withIsOneShot(false)
					.makeShared();
			auto box = HinaPE::Box3::builder()
					.withIsNormalFlipped(true)
					.withBoundingBox(HinaPE::BoundingBox3D({-_opt.bounding_box_size, -_opt.bounding_box_size, -_opt.bounding_box_size}, {_opt.bounding_box_size, _opt.bounding_box_size, _opt.bounding_box_size}))
					.makeShared();
			_fluid_collider = HinaPE::RigidBodyCollider3::builder()
					.withSurface(box)
					.makeShared();
			_fluid_solver = HinaPE::Fluid::SphSolver3::builder()
					.withTargetDensity(_opt.target_density)
					.withTargetSpacing(_opt.target_spacing)
					.makeShared();
			_fluid_solver->setEmitter(_fluid_emitter);
			_fluid_solver->setCollider(_fluid_collider);
			_fluid_solver->setPseudoViscosityCoefficient(_opt.pseudo_viscosity_coefficient);

			auto fluid_model = std::make_shared<Kasumi::Model>("sphere", Color::BLUE);
			fluid_model->instancing();
			_fluid_obj = _scene->get_object(_scene->add_object(fluid_model));

			sync();
			return true;
		}
	}
	return false;
}
