#include "GDPSSwitcher.hpp"

#include "imgui-hook.hpp"
#include <../imgui/misc/cpp/imgui_stdlib.h>

#include "Setting.hpp"

const char* gdpsNames[] = {
	"1.9 GDPS", "Rewind GDPS", "Nebula PS", "Boomlings (Main GD)", "Custom"
};

void renderGDPSSwitcher(bool& open) {
	ImGui::SetNextWindowSize(ImVec2(200, 0));
	if (ImGui::Begin("GDPS Switcher", &open, ImGuiWindowFlags_NoResize)) {
		ImGui::Checkbox("Enable", &setting().onEnableSwitcher);

		ImGui::Text("Servers:");
		ImGui::SetNextItemWidth(185.f);
		ImGui::Combo("##servers", &setting().m_serverIndex, gdpsNames, IM_ARRAYSIZE(gdpsNames));

		if (setting().m_serverIndex == 4) {
			ImGui::Text("Custom URL:");
			ImGui::SetNextItemWidth(185.f);
			ImGui::InputText("##customServerURL", &setting().m_customServerURL);
		}
	}
	ImGui::End();
}