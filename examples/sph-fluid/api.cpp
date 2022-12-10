#include "api.h"

void HinaPE::Fluid::Api::step(float dt)
{
}

void HinaPE::Fluid::Api::ui_sidebar()
{
	static int phase = 0;
	static int index = 1;
	double lz = 0;
	lz = BoundingBox3D(Vector3D(), Vector3D(3, 2, 1.5)).depth();
	switch (phase)
	{
		case 0:
			if (ImGui::CollapsingHeader("Create Fluid Domain"), ImGuiTreeNodeFlags_DefaultOpen)
			{
				static float R = 1.0f;
				ImGui::SliderFloat("Size", &R, 0.01f, 10.0f, "%.2f");
				if (ImGui::Button("Add"))
				{
					++phase;
				}
			}
			break;
		case 1:
			if (ImGui::CollapsingHeader("Create Fluid"), ImGuiTreeNodeFlags_DefaultOpen)
			{
				ImGui::PushID(index++);
				ImGui::Text("Hello");
				if (ImGui::Button("Add"))
				{
				}
			}
			ImGui::PopID();
	}
}

void HinaPE::Fluid::Api::sync()
{
}
