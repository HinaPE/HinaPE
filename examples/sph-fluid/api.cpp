#include "api.h"
#include "imgui.h"

void Api::step(float dt)
{
}

void Api::ui_sidebar()
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

void Api::sync()
{
}

auto Api::phase1() -> bool
{
	if (ImGui::CollapsingHeader("Create Fluid Domain"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		static float R = 1.0f;
		ImGui::SliderFloat("Size", &R, 0.01f, 10.0f, "%.2f");
		if (ImGui::Button("Add"))
		{
			_fluid_bounding_model = std::make_shared<Kasumi::Model>("cube", Color::BLACK);
			auto scene_obj = _scene->get_object(_scene->add_object(_fluid_bounding_model));
			scene_obj->scale() = mVector3(R, R, R);
			return true;
		}
	}
	return false;
}
auto Api::phase2() -> bool
{
	if (ImGui::CollapsingHeader("Create Fluid"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Text("Hello");
		if (ImGui::Button("Add"))
		{
			return true;
		}
	}
	return false;
}
