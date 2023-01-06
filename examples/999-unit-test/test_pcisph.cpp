#include "test_pcisph.h"

#include "GLFW/glfw3.h"
#include "imgui.h"

void TestPciSPH::prepare()
{
	// set up emitter
	auto bunny = HinaPE::TriangleMesh3::builder().makeShared();
	bunny->readObj(std::string(BuiltinModelDir) + "bunny.obj");
	bunny->scale(2);

	_emitter = HinaPE::Fluid::VolumeParticleEmitter3::builder()
			.withSpacing(0.05)
			.withSurface(bunny)
			.withIsOneShot(false)
			.makeShared();

	_solver = HinaPE::Fluid::PciSphSolver3::builder()
			.withTargetDensity(1000.0)
			.withTargetSpacing(0.05)
			.makeShared();

	_solver->setEmitter(_emitter);


	auto box = HinaPE::Box3::builder()
			.withIsNormalFlipped(true)
			.withBoundingBox(HinaPE::BoundingBox3D(HinaPE::Vector3D(), HinaPE::Vector3D(2, 2, 2)))
			.makeShared();

	auto collider = HinaPE::RigidBodyCollider3::builder().withSurface(box).makeShared();

	_solver->setCollider(collider);

	// scene obj
	auto model = std::make_shared<Kasumi::Model>("sphere", Color::RED);
	model->instancing();
	_fluid_obj = _scene->get_object(_scene->add_object(model));

	_physics_thread = std::thread([&]()
								  {
									  while (!_should_close)
										  if (_advance_frame)
										  {
											  _is_complete = false;
											  std::cout << ">>>>> frame: start " << _frame.index << std::endl;
											  _solver->update(_frame++);
											  sync();
											  std::cout << ">>>>> frame: end" << std::endl;

											  _advance_frame = false;
											  _is_complete = true;
										  }
								  });

	_physics_thread.detach();
}
void TestPciSPH::key(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		_advance_frame = true;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		_should_close = true;
}
void TestPciSPH::ui_sidebar()
{
	ImGui::Text("Frame: %d", _frame.index);
	ImGui::Text("Physics Running: %s", _is_complete ? "false" : "true");
}
void TestPciSPH::sync() const
{
	auto const &positions = _solver->sphSystemData()->positions();
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
