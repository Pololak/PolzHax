#include "DebugModule.hpp"

#include <imgui-hook.hpp>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include "utils.hpp"

void renderDebugModule() {
	ImGui::SetNextWindowSize(ImVec2(300, 300));
	if (ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
		auto director = CCDirector::sharedDirector();
		auto winSize = director->getWinSize();

		ImGui::Text("Window Resolution: %.0fx%.0f", director->getOpenGLView()->getFrameSize().width, director->getOpenGLView()->getFrameSize().height);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("GameManager: 0x%p", gd::GameManager::sharedState());
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##gamemanager", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", gd::GameManager::sharedState())->getCString());
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("GameLevelManager: 0x%p", gd::GameLevelManager::sharedState());
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##gamelevelmanager", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", gd::GameLevelManager::sharedState())->getCString());
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("LocalLevelManager: 0x%p", gd::LocalLevelManager::sharedState());
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##locallevelmanager", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", gd::LocalLevelManager::sharedState())->getCString());
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("CCDirector: 0x%p", CCDirector::sharedDirector());
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##ccdirector", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", CCDirector::sharedDirector())->getCString());
		}

		auto fme = gd::FMODAudioEngine::sharedEngine();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("FMODAudioEngine: 0x%p", fme);
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##fmodaudioengine", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", fme)->getCString());
		}

		ImGui::Text("Pulse 1: %f", fme->m_pulse1);
		ImGui::Text("Pulse 2: %f", fme->m_pulse2);
		ImGui::Text("Pulse 3: %f", fme->m_pulse3);
		ImGui::Text("Pulse Counter: %i", fme->m_pulseCounter);
		ImGui::Text("Metering: %d", fme->m_metering);
		ImGui::Checkbox("m_metering", &fme->m_metering);
	}
	ImGui::End();
}