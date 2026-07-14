#include "TextureManager.hpp"
#include <filesystem>

#include <imgui-hook.hpp>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include "Setting.hpp"
#include "ImGuiUtils.hpp"

#include "shellapi.h"

void renderTextureManager(bool& open) {
	ImGui::SetNextWindowSize(ImVec2(200.f * setting().UISize, 0.f));
	if (ImGui::Begin("Texture Manager", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2.f);
		if (ImGui::Combo("##selectedMacro", &setting().m_texturePackIndex, texturePacks, texturePacks.size())) {
			if (texturePacks.size()) {
				std::cout << "Selected Pack: " << texturePacks[setting().m_texturePackIndex].c_str() << "Index: " << setting().m_texturePackIndex << std::endl;
				setting().m_customTexturePackPath = texturePacks[setting().m_texturePackIndex];
			}
		}

		if (ImGui::Button("Refresh List", ImVec2(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2.f, 0.f))) {
			texturePacks.clear();
			texturePacks.push_back("Base");
			auto texturePacksPath = CCFileUtils::sharedFileUtils()->getWritablePath2() + "PolzHax/texturepacks";
			for (const auto& directory : std::filesystem::directory_iterator(texturePacksPath)) {
				if (directory.is_directory()) {
					texturePacks.push_back(directory.path().filename().string());
				}
			}

			for (const auto& str : texturePacks) {
				std::cout << str << std::endl;
			}
		}

		if (ImGui::Button("Open Folder", ImVec2(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2.f, 0.f))) {
			ShellExecute(0, NULL, std::string(CCFileUtils::sharedFileUtils()->getWritablePath2() + "/PolzHax/texturepacks").c_str(), NULL, NULL, SW_SHOW);
		}

		if (ImGui::Button("Apply", ImVec2(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2.f, 0.f))) {
			gd::GameManager::sharedState()->reloadAll(false, false, true);
		}
	}
	ImGui::End();
}