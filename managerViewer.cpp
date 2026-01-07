#include "managerViewer.h"
#include <gd.h>
#include <cocos2d.h>
#include <imgui.h>
#include "utils.hpp"

using namespace cocos2d;

void showManagerView(bool& open) {
	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Manager Viewer", &open)) {
		ImGui::Text("FMODAudioEngine: 0x%p", gd::FMODAudioEngine::sharedEngine());
		ImGui::SameLine();
		if (ImGui::Button("Copy##fmodAudioEngine")) {
			clipboard::write(CCString::createWithFormat("%p", gd::FMODAudioEngine::sharedEngine())->getCString());
		}
		ImGui::Text("GameLevelManager: 0x%p", gd::GameLevelManager::sharedState());
		ImGui::SameLine();
		if (ImGui::Button("Copy##gameLevelManager")) {
			clipboard::write(CCString::createWithFormat("%p", gd::GameLevelManager::sharedState())->getCString());
		}
		ImGui::Text("GameManager: 0x%p", gd::GameManager::sharedState());
		ImGui::SameLine();
		if (ImGui::Button("Copy##gameManager")) {
			clipboard::write(CCString::createWithFormat("%p", gd::GameManager::sharedState())->getCString());
		}
		ImGui::Text("GameSoundManager: 0x%p", gd::GameSoundManager::sharedState());
		ImGui::SameLine();
		if (ImGui::Button("Copy##gameSoundManager")) {
			clipboard::write(CCString::createWithFormat("%p", gd::GameSoundManager::sharedState())->getCString());
		}
		ImGui::Text("GameStatsManager: 0x%p", gd::GameStatsManager::sharedState());
		ImGui::SameLine();
		if (ImGui::Button("Copy##gameStatsManager")) {
			clipboard::write(CCString::createWithFormat("%p", gd::GameStatsManager::sharedState())->getCString());
		}
		ImGui::Text("LocalLevelManager: 0x%p", gd::LocalLevelManager::sharedState());
		ImGui::SameLine();
		if (ImGui::Button("Copy##localLevelManager")) {
			clipboard::write(CCString::createWithFormat("%p", gd::LocalLevelManager::sharedState())->getCString());
		}
	}
	ImGui::End();
}