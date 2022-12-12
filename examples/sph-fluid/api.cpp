#include "api.h"
#include "imgui.h"

void Api::step(float dt)
{
}

void Api::ui_sidebar()
{
	static int phase = 0;
	static int index = 1;
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

void Api::sync()
{
}
