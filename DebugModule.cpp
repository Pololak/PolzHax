#include "DebugModule.hpp"
#include "ColorChannelSprite.hpp"
#include "LevelEditorLayer.hpp"
#include "Setting.hpp"

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

		ImGui::AlignTextToFramePadding();
		ImGui::Text("ObjectToolbox: 0x%p", gd::ObjectToolbox::sharedState());
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##objectToolbox", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", gd::ObjectToolbox::sharedState())->getCString());
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

		ImGui::Checkbox("No DrawNode Clear", &setting().m_clearHitboxes);

		auto pl = gd::GameManager::sharedState()->getPlayLayer();
		if (pl) {
			ImGui::Text("PlayLayer 0x174: %i", from<CCArray*>(pl, 0x174)->count());

			static int colorID = 0;
			ImGui::InputInt("ColorID", &colorID);
			if (ImGui::Button("Update Color")) {
				pl->updateColor({0, 0, 0}, 1.f, colorID);
			}

			ImGui::Text("m_fromColor: %i %i %i", pl->m_activeBGColorAction->m_fromColor.r, pl->m_activeBGColorAction->m_fromColor.g, pl->m_activeBGColorAction->m_fromColor.b);
			ImGui::Text("m_toColor: %i %i %i", pl->m_activeBGColorAction->m_toColor.r, pl->m_activeBGColorAction->m_toColor.g, pl->m_activeBGColorAction->m_toColor.b);
		}

		ImGui::Text("VSync: %i", CCApplication::sharedApplication()->getVerticalSyncEnabled());
		if (ImGui::Button("Toggle VSync")) {
			CCApplication::sharedApplication()->toggleVerticalSync(!CCApplication::sharedApplication()->getVerticalSyncEnabled());
		}
		if (ImGui::Button("Update VSync")) {
			CCApplication::sharedApplication()->updateVerticalSync();
		}
		if (ImGui::Button("Setup VSync")) {
			CCApplication::sharedApplication()->setupVerticalSync();
		}

		auto editorLayer = LevelEditorLayer::get();
		if (editorLayer) {
			static int colorID = 0;
			static int idk = 0;
			static int idk2 = 0;

			ImGui::InputInt("ColorID", &colorID);
			ImGui::InputInt("IDK", &idk);
			ImGui::InputInt("IDK2", &idk2);

			if (ImGui::Button("Open ColorSelectPopup")) {
				gd::ColorSelectPopup::create(editorLayer->m_uiLayer->m_selectedObject)->show();
			}
		}
	}
	ImGui::End();
}