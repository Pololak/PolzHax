#include "GDPSSwitcher.hpp"

#include <imgui-hook.hpp>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include "Setting.hpp"

const char* gdpsNames[] = {
	"1.9 GDPS", "Rewind GDPS", "Nebula PS", "Platinum GDPS", "Boomlings (Main GD)", "Custom"
};

void renderGDPSSwitcher(bool& open) {
	ImGui::SetNextWindowSize(ImVec2(200.f * setting().UISize, 0.f));
	if (ImGui::Begin("GDPS Switcher", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
		ImGui::Checkbox("Enable", &setting().onEnableSwitcher);

		ImGui::Text("Servers:");
		ImGui::SetNextItemWidth(185.f * setting().UISize);
		ImGui::Combo("##servers", &setting().m_serverIndex, gdpsNames, IM_ARRAYSIZE(gdpsNames));

		if (setting().m_serverIndex == 5) {
			ImGui::Text("Custom URL:");
			ImGui::SetNextItemWidth(185.f * setting().UISize);
			ImGui::InputText("##customServerURL", &setting().m_customServerURL);
		}

		ImGui::TextWrapped("NOTE: You can only download and play levels.");
	}
	ImGui::End();
}