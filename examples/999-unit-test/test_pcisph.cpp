#include "test_pcisph.h"

#include "GLFW/glfw3.h"
#include "imgui.h"

#include <thread>

#include "logging.h"

void TestPciSPH::prepare()
{
	// set up emitter
	auto bunny = HinaPE::TriangleMesh3::builder().makeShared();
	bunny->readObj(std::string(BuiltinModelDir) + "bunny.obj");
	bunny->scale(2);

	_emitter = HinaPE::Fluid::VolumeParticleEmitter3::builder()
			.withSpacing(0.05)
			.withSurface(bunny)
			.withIsOneShot(true)
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
	collider->setFrictionCoefficient(0.5);

	_solver->setCollider(collider);

	// scene obj
	auto model = std::make_shared<Kasumi::Model>("cube", Color::RED);
	model->instancing();
	_fluid_obj = _scene->get_object(_scene->add_object(model));

	HinaPE::Logging::mute();
}
void TestPciSPH::key(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		std::thread([&]()
					{
						_physics_thread_running = true;
						_solver->update(_frame++);
						sync();
						_physics_thread_running = false;
					}).detach();
	} else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		static bool inited = false;
		if (!inited)
		{
			inited = true;
			_physics_thread_running = true;
			std::thread([&]()
						{
							while (true)
							{
								if (_physics_thread_running)
								{
									_solver->update(_frame++);
									sync();
								} else
								{
									std::cout << ""; // avoid compiler optimizing
								}
							}
						}).detach();
		} else
			_physics_thread_running = !_physics_thread_running;
	}
}
void TestPciSPH::ui_sidebar()
{
	ImGui::Text("Frame: %d", _frame.index);
	ImGui::Text("Physics Thread Running: %s", _physics_thread_running ? "true" : "false");
	ImGui::Text("Particles: %zu", _solver->sphSystemData()->positions().size());
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
