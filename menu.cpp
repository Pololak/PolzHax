#include "Menu.hpp"

#include <imgui-hook.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include "shellapi.h"

#include "CocosExplorer.hpp"
#include "EditorUI.hpp"
#include "LevelEditorLayer.hpp"
#include "PauseLayer.hpp"
#include "PlayLayer.hpp"

#include "Setting.hpp"
#include "patching.hpp"
#include "ImGuiUtils.hpp"
#include "utils.hpp"
#include "SpeedHack.h"

ImVec4 color1;
ImVec4 color2;
ImVec4 color3;
ImVec4 color4;
ImVec4 color5;
ImVec4 color6;

bool oneX = true;

std::vector<std::string> dllNames;

auto libcocosbase = reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll"));

void updateFPSBypass() {
	float value = 1.f / (setting().onFPSBypass ? setting().fpsValue : 60.f);
	CCDirector::sharedDirector()->setAnimationInterval(value);
}

void updateSpeedhack() {
	if (setting().speedhackValue == 0.f) return;

	const auto value = setting().onSpeedhack ? setting().speedhackValue : 1.f;

	CCDirector::sharedDirector()->m_pScheduler->setTimeScale(setting().onClassicMode ? 1.f : value);
	SpeedHack::SetSpeed(setting().onClassicMode ? value : 1.f);

	if (auto fme = gd::FMODAudioEngine::sharedEngine()) {
		if (auto sound = fme->m_globalChannel) {
			if (setting().onSpeedhackAudio) sound->setPitch(value);
			else sound->setPitch(1.f);
		}
	}
}

void cheatAdd() {
	setting().cheatsCount++;
	setting().beforeRestartCheatsCount++;
}

void cheatDec() {
	setting().cheatsCount--;
}

void colorSet() {
	auto* colors = ImGui::GetStyle().Colors;

	color1.x = setting().overlayColor01;
	color1.y = setting().overlayColor02;
	color1.z = setting().overlayColor03;
	color1.w = setting().overlayColor04 - 0.4f;

	color2.x = setting().overlayColor01;
	color2.y = setting().overlayColor02;
	color2.z = setting().overlayColor03;
	color2.w = 1;

	color3.x = setting().overlayColor01 + 0.3f;
	color3.y = setting().overlayColor02 + 0.3f;
	color3.z = setting().overlayColor03 + 0.3f;
	color3.w = setting().overlayColor04 + 0.3f;

	color4.x = setting().overlayColor01 - 0.1f;
	color4.y = setting().overlayColor02 - 0.1f;
	color4.z = setting().overlayColor03 - 0.1f;
	color4.w = setting().overlayColor04 - 0.1f;

	color5.x = setting().overlayColor01 + 0.1f;
	color5.y = setting().overlayColor02 + 0.1f;
	color5.z = setting().overlayColor03 + 0.1f;
	color5.w = setting().overlayColor04 + 0.1f;

	color6.x = setting().bgColor01;
	color6.y = setting().bgColor02;
	color6.z = setting().bgColor03;
	color6.w = setting().bgColor04;

	//colors[ImGuiCol_Text] = ImVec4(1, 0, 0, 1);
	colors[ImGuiCol_WindowBg] = color6;
	colors[ImGuiCol_Border] = color2;
	colors[ImGuiCol_BorderShadow] = color2;
	colors[ImGuiCol_Button] = color3;
	colors[ImGuiCol_ButtonHovered] = color1;
	colors[ImGuiCol_ButtonActive] = color1;
	colors[ImGuiCol_CheckMark] = ImVec4(1, 1, 1, 1);
	colors[ImGuiCol_FrameBg] = color1;
	colors[ImGuiCol_FrameBgHovered] = color3;
	colors[ImGuiCol_FrameBgActive] = color3;
	colors[ImGuiCol_Tab] = ImVec4(color5.x, color5.y, color5.z, color5.w - 0.2f);
	colors[ImGuiCol_TabHovered] = color3;
	colors[ImGuiCol_TabActive] = color3;
	colors[ImGuiCol_TitleBg] = color3;
	colors[ImGuiCol_TitleBgActive] = color3;
	colors[ImGuiCol_TitleBgCollapsed] = color3;
	colors[ImGuiCol_SliderGrab] = color4;
	colors[ImGuiCol_SliderGrabActive] = color4;
	colors[ImGuiCol_TextSelectedBg] = color2;
	colors[ImGuiCol_Header] = color1;
	colors[ImGuiCol_HeaderHovered] = color1;
	colors[ImGuiCol_HeaderActive] = color2;
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, .33f);
	colors[ImGuiCol_ResizeGrip] = color2;
	colors[ImGuiCol_ResizeGripActive] = color5;
	colors[ImGuiCol_ResizeGripHovered] = color3;
}

void sortTabs() {
	float polzhax_xPos = 5.f;
	float addingInterfaceY = -1.f;
	float bypass_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("PolzHax", nullptr);
		ImGui::SetWindowPos(ImVec2(5.f, 5.f));
		bypass_xPos = polzhax_xPos + ImGui::GetWindowWidth() + 10.f;
		addingInterfaceY = ImGui::GetWindowHeight() + 10.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Interface", nullptr);
		ImGui::SetWindowPos(ImVec2(5.f, addingInterfaceY));
	}
	float addingUtilityY = -1.f;
	float cosmetic_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Bypass", nullptr);
		ImGui::SetWindowPos(ImVec2(bypass_xPos, 5.f));
		cosmetic_xPos = bypass_xPos + ImGui::GetWindowWidth() + 10.f;
		addingUtilityY = ImGui::GetWindowHeight() + 10.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Utility", nullptr);
		ImGui::SetWindowPos(ImVec2(bypass_xPos, addingUtilityY));
	}
	float creator_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Cosmetic", nullptr);
		ImGui::SetWindowPos(ImVec2(cosmetic_xPos, 5.f));
		creator_xPos = cosmetic_xPos + ImGui::GetWindowWidth() + 10.f;
	}
	float level_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Creator", nullptr);
		ImGui::SetWindowPos(ImVec2(creator_xPos, 5.f));
		level_xPos = creator_xPos + ImGui::GetWindowWidth() + 10.f;
	}
	float universal_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Level", nullptr);
		ImGui::SetWindowPos(ImVec2(level_xPos, 5.f));
		universal_xPos = level_xPos + ImGui::GetWindowWidth() + 10.f;
	}
	float addingSpeedhackY = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Universal", nullptr);
		ImGui::SetWindowPos(ImVec2(universal_xPos, 5.f));
		addingSpeedhackY = ImGui::GetWindowHeight() + 10.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Speedhack", nullptr);
		ImGui::SetWindowPos(ImVec2(universal_xPos, addingSpeedhackY));
	}
}

void imgui_render() {
	auto playLayer = gd::GameManager::sharedState()->getPlayLayer();
	auto editorLayer = LevelEditorLayer::get();

	if (oneX) {
		setting().load();

		setting().cheatsCount = 0;
		setting().beforeRestartCheatsCount = 0;

		sortTabs();

		colorSet();

		// Bypass

		if (setting().onIcons) {
			sequence_patch(gd::base + 0x66e26, { 0x90, 0x90 });
			sequence_patch(gd::base + 0x66b4a, { 0x90, 0x90 });
			sequence_patch(gd::base + 0x66b56, { 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0x66e26, { 0x7d, 0x07 });
			sequence_patch(gd::base + 0x66b4a, { 0x7d, 0x13 });
			sequence_patch(gd::base + 0x66b56, { 0x7d, 0x07 });
		}

		if (setting().onMainLevels) {
			sequence_patch(gd::base + 0xa81af, { 0xe9, 0xa3, 0x02, 0x00, 0x00, 0x90 });
			sequence_patch(gd::base + 0xa8c22, { 0xe9, 0x98, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xa81af, { 0x0f, 0x84, 0xa2, 0x02, 0x00, 0x00 });
			sequence_patch(gd::base + 0xa8c22, { 0x0f, 0x8e, 0x97, 0x00, 0x00, 0x00 });
		}

		if (setting().onSliderLimit) {
			sequence_patch(gd::base + 0x1cf27, { 0xeb, 0x07 });
			sequence_patch(gd::base + 0x1cf3b, { 0xeb, 0x07 });
		}
		else {
			sequence_patch(gd::base + 0x1cf27, { 0x76, 0x07 });
			sequence_patch(gd::base + 0x1cf3b, { 0x76, 0x07 });
		}

		// Cosmetic

		if (setting().onCoinsShowUncollected) {
			sequence_patch(gd::base + 0x43368, { 0x8b, 0xc2, 0x90 });
			sequence_patch(gd::base + 0x735ce, { 0x8b, 0xd9, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0x43368, { 0x0f, 0x44, 0xc2 });
			sequence_patch(gd::base + 0x735ce, { 0x0f, 0x44, 0xd9 });
		}

		if (setting().onCoinsInPractice) {
			sequence_patch(gd::base + 0xeb138, { 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xeb138, { 0x75, 0x4c });
		}

		if (setting().onForceDontEnter) {
			sequence_patch(gd::base + 0xec51c, { 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xec51c, { 0x8b, 0xd9 });
		}

		if (setting().onForceDontFade) {
			sequence_patch(gd::base + 0xebddb, { 0x90, 0x90 });
			sequence_patch(gd::base + 0xebe06, { 0xeb, 0x11 });
			sequence_patch(gd::base + 0xebe20, { 0x90, 0x90 });
			sequence_patch(gd::base + 0xebdf6, { 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xebddb, { 0x74, 0x5d });
			sequence_patch(gd::base + 0xebe06, { 0x74, 0x11 });
			sequence_patch(gd::base + 0xebe20, { 0x75, 0x18 });
			sequence_patch(gd::base + 0xebdf6, { 0x75, 0x42 });
		}

		if (setting().onForceObjectsInvisible) {
			sequence_patch(gd::base + 0xebece, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xebece, { 0x0f, 0x84, 0xd1, 0x02, 0x00, 0x00 });
		}

		if (setting().onInstantMirror) {
			sequence_patch(gd::base + 0xf0d36, { 0x00, 0x00, 0x00, 0x00 });
			cheatAdd();
		}
		else {
			sequence_patch(gd::base + 0xf0d36, { 0x00, 0x00, 0x00, 0x3f });
		}

		if (setting().onMaxParticles) {
			sequence_patch(libcocosbase + 0xb64d7, { 0x8b, 0x7d, 0x07 });
		}
		else {
			sequence_patch(libcocosbase + 0xb64d7, { 0x8b, 0x7d, 0x08 });
		}

		if (setting().onNoAnimations) {
			sequence_patch(libcocosbase + 0x886be, { 0xc0 });
			sequence_patch(libcocosbase + 0x88da0, { 0xc0 });
		}
		else {
			sequence_patch(libcocosbase + 0x886be, { 0xc1 });
			sequence_patch(libcocosbase + 0x88da0, { 0xc1 });
		}

		if (setting().onNoBackgroundFlash) {
			sequence_patch(gd::base + 0xe6c92, { 0x6a, 0x00 });
			sequence_patch(gd::base + 0xe6ca8, { 0xc7, 0x04, 0x24, 0x00, 0x00, 0x00, 0x00 });
		}
		else {
			sequence_patch(gd::base + 0xe6c92, { 0x6a, 0x01 });
			sequence_patch(gd::base + 0xe6ca8, { 0xc7, 0x04, 0x24, 0x8f, 0xc2, 0xf5, 0x3d });
		}

		if (setting().onNoDeathEffect) {
			sequence_patch(gd::base + 0xdde71, { 0xe9, 0xe7, 0x01, 0x00, 0x00, 0x90 });
			sequence_patch(gd::base + 0xf05dc, { 0xeb, 0x1b });
		}
		else {
			sequence_patch(gd::base + 0xdde71, { 0x0f, 0x84, 0xd5, 0x01, 0x00, 0x00 });
			sequence_patch(gd::base + 0xf05dc, { 0x74, 0x1b });
		}

		//if (setting().onNoForcePlayerGlow) {
		//	sequence_patch(gd::base + 0xdfcd9, { 0xeb });
		//	sequence_patch(gd::base + 0xdfcef, { 0xeb });
		//}
		//else {
		//	sequence_patch(gd::base + 0xdfcd9, { 0x75 });
		//	sequence_patch(gd::base + 0xdfcef, { 0x75 });
		//}

		if (setting().onNoGhostTrail) {
			sequence_patch(gd::base + 0xf3374, { 0x6a, 0x00, 0x90 });
			sequence_patch(gd::base + 0xf338d, { 0x6a, 0x00, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xf3374, { 0xff, 0x75, 0x08 });
			sequence_patch(gd::base + 0xf338d, { 0xff, 0x75, 0x08 });
		}

		if (setting().onNoObjectGlow) {
			sequence_patch(gd::base + 0x6d8a3, { 0xe9, 0x8f, 0x01, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0x6d8a3, { 0x0f, 0x85, 0x8e, 0x01, 0x00, 0x00 });
		}

		if (setting().onNoGravityEffect) {
			sequence_patch(gd::base + 0xf4586, { 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xf4586, { 0x75, 0x07 });
		}

		if (setting().onNoMirror) {
			sequence_patch(gd::base + 0xf0bf2, { 0xe9, 0x9b, 0x01, 0x00, 0x00, 0x90 });
			cheatAdd();
		}
		else {
			sequence_patch(gd::base + 0xf0bf2, { 0x0f, 0x84, 0x9a, 0x01, 0x00, 0x00 });
		}

		if (setting().onNoPortalLightning) {
			sequence_patch(gd::base + 0xe6c19, { 0xeb, 0x71 });
		}
		else {
			sequence_patch(gd::base + 0xe6c19, { 0x75, 0x71 });
		}

		if (setting().onNoRespawnFlash) {
			sequence_patch(gd::base + 0xdd62f, { 0xe9, 0x99, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xdd62f, { 0x0f, 0x85, 0x98, 0x00, 0x00, 0x00 });
		}

		if (setting().onNoShadeEffect) {
			sequence_patch(gd::base + 0xebfe3, { 0xeb });
			cheatAdd();
		}
		else {
			sequence_patch(gd::base + 0xebfe3, { 0x77 });
		}

		if (setting().onPracticePulse) {
			sequence_patch(gd::base + 0x29975, { 0x90, 0x90 });
			sequence_patch(gd::base + 0xeb441, { 0xeb, 0x16 });
		}
		else {
			sequence_patch(gd::base + 0x29975, { 0x75, 0x0c });
			sequence_patch(gd::base + 0xeb441, { 0x74, 0x16 });
		}

		if (setting().onSolidPlayerTrail) {
			sequence_patch(libcocosbase + 0xac3dc, { 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(libcocosbase + 0xac3dc, { 0x89, 0x41, 0x10 });
		}

		if (setting().onSolidWaveTrail) {
			sequence_patch(gd::base + 0xd9ade, { 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xd9ade, { 0x75, 0x0c });
		}

		if (setting().onTrailAlwaysOff) {
			sequence_patch(libcocosbase + 0xac6a6, { 0xe9, 0x56, 0x02, 0x00, 0x00, 0x90 });
		}
		else if (setting().onTrailAlwaysOn) {
			sequence_patch(libcocosbase + 0xac6a6, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else if (setting().onInversedTrail) {
			sequence_patch(libcocosbase + 0xac6a6, { 0x0f, 0x85, 0x55, 0x02, 0x00, 0x00 });
		}
		else {
			sequence_patch(libcocosbase + 0xac6a6, { 0x0f, 0x84, 0x55, 0x02, 0x00, 0x00 });
		}

		if (setting().onTrailBugFix) {
			sequence_patch(libcocosbase + 0xac29d, { 0xbb, 0xff, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(libcocosbase + 0xac29d, { 0xf3, 0x0f, 0x2c, 0xc1, 0x2b, 0xd8 });
		}

		if (setting().onWaveTrailOnDeath) {
			sequence_patch(gd::base + 0xdddfa, { 0xeb, 0x0f });
		}
		else {
			sequence_patch(gd::base + 0xdddfa, { 0x74, 0x0f });
		}

		// Creator

		if (setting().onAbsolutePosition) {
			sequence_patch(gd::base + 0x4b49d, { 0x90, 0x8b, 0xcf, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0x4b49d, { 0x51, 0x8b, 0xcf, 0xff, 0x50, 0x5c });
		}

		if (setting().onCopyHack) {
			sequence_patch(gd::base + 0x9c7ed, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(gd::base + 0x9dfe5, { 0x90, 0x90 });
			sequence_patch(gd::base + 0x9c80f, { 0x8b, 0xcf, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0x9c7ed, { 0x0f, 0x84, 0x2e, 0x01, 0x00, 0x00 });
			sequence_patch(gd::base + 0x9dfe5, { 0x75, 0x0e });
			sequence_patch(gd::base + 0x9c80f, { 0x0f, 0x44, 0xcf });
		}

		if (setting().onDefaultSongBypass) {
			sequence_patch(gd::base + 0x9a37f, { 0x90, 0x90 });
			sequence_patch(gd::base + 0x9a391, { 0x90, 0x90, 0x90 });
			sequence_patch(gd::base + 0x9a3a0, { 0x90, 0x90, 0x90 });
			sequence_patch(gd::base + 0x9a3fe, { 0x90, 0x90 });
			sequence_patch(gd::base + 0x9a410, { 0x90, 0x90, 0x90 });
			sequence_patch(gd::base + 0x9a41f, { 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0x9a37f, { 0x74, 0x4e });
			sequence_patch(gd::base + 0x9a391, { 0x0f, 0x4f, 0xf0 });
			sequence_patch(gd::base + 0x9a3a0, { 0x0f, 0x48, 0xf1 });
			sequence_patch(gd::base + 0x9a3fe, { 0x74, 0x4e });
			sequence_patch(gd::base + 0x9a410, { 0x0f, 0x4f, 0xf0 });
			sequence_patch(gd::base + 0x9a41f, { 0x0f, 0x48, 0xf1 });
		}

		if (setting().onEditorExtension) {
			sequence_patch(gd::base + 0x14a74c, { 0x00, 0x60, 0xea, 0x4a }); // 60000 to 7680000

			sequence_patch(gd::base + 0x4b513, { 0xeb }); // EditorUI::getLimitedPosition
			sequence_patch(gd::base + 0x4b525, { 0xeb });
			sequence_patch(gd::base + 0x4b53e, { 0xeb });
			sequence_patch(gd::base + 0x4b562, { 0xeb });

			sequence_patch(gd::base + 0x4b445, { 0xeb }); // EditorUI::moveObject
			sequence_patch(gd::base + 0x4b457, { 0xeb });
			sequence_patch(gd::base + 0x4b470, { 0xeb });
			sequence_patch(gd::base + 0x4b482, { 0xeb });

			sequence_patch(gd::base + 0x4779c, { 0xeb }); // EditorUI::onCreateObject
			sequence_patch(gd::base + 0x477b9, { 0xeb });
			sequence_patch(gd::base + 0x477d2, { 0xeb });
			sequence_patch(gd::base + 0x477e8, { 0xeb });

			sequence_patch(gd::base + 0x93861, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(gd::base + 0x9399b, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(gd::base + 0x939d3, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(gd::base + 0x93aed, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(gd::base + 0x93c72, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(gd::base + 0x94073, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(gd::base + 0x94112, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(gd::base + 0x94667, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(gd::base + 0x946f3, { 0x24, 0xa3, 0x54, 0x00 });
		}
		else {
			sequence_patch(gd::base + 0x14a74c, { 0x00, 0x60, 0x6a, 0x47 });

			sequence_patch(gd::base + 0x4b513, { 0x76 });
			sequence_patch(gd::base + 0x4b525, { 0x76 });
			sequence_patch(gd::base + 0x4b53e, { 0x76 });
			sequence_patch(gd::base + 0x4b562, { 0x76 });

			sequence_patch(gd::base + 0x4b445, { 0x76 });
			sequence_patch(gd::base + 0x4b457, { 0x76 });
			sequence_patch(gd::base + 0x4b470, { 0x76 });
			sequence_patch(gd::base + 0x4b482, { 0x76 });

			sequence_patch(gd::base + 0x4779c, { 0x77 });
			sequence_patch(gd::base + 0x477b9, { 0x77 });
			sequence_patch(gd::base + 0x477d2, { 0x76 });
			sequence_patch(gd::base + 0x477e8, { 0x76 });

			sequence_patch(gd::base + 0x93861, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(gd::base + 0x9399b, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(gd::base + 0x939d3, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(gd::base + 0x93aed, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(gd::base + 0x93c72, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(gd::base + 0x94073, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(gd::base + 0x94112, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(gd::base + 0x94667, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(gd::base + 0x946f3, { 0xb8, 0xa6, 0x54, 0x00 });
		}

		if (setting().onFreeScroll) {
			sequence_patch(gd::base + 0x4ca45, { 0xeb });
			sequence_patch(gd::base + 0x4ca5c, { 0xeb });
			sequence_patch(gd::base + 0x4ca75, { 0xeb });
			sequence_patch(gd::base + 0x4ca8c, { 0xeb });
		}
		else {
			sequence_patch(gd::base + 0x4ca45, { 0x77 });
			sequence_patch(gd::base + 0x4ca5c, { 0x77 });
			sequence_patch(gd::base + 0x4ca75, { 0x77 });
			sequence_patch(gd::base + 0x4ca8c, { 0x77 });
		}

		if (setting().onHideGrid) {
			sequence_patch(gd::base + 0x939ba, { 0x00 });
			sequence_patch(gd::base + 0x93b6e, { 0x00 });
		}
		else {
			sequence_patch(gd::base + 0x939ba, { 0x96 });
			sequence_patch(gd::base + 0x93b6e, { 0x96 });
		}

		if (setting().onHideTriggerLines) {
			sequence_patch(gd::base + 0x93e08, { 0xe9, 0xce, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0x93e08, { 0x0f, 0x84, 0xcd, 0x00, 0x00, 0x00 });
		}

		if (setting().onLevelEdit) {
			sequence_patch(gd::base + 0xd62ef, { 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xd62ef, { 0x75, 0x62 });
		}

		if (setting().onNoCopyMark) {
			sequence_patch(gd::base + 0x54aa0, { 0xb8, 0x00, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0x54aa0, { 0x8b, 0x81, 0x04, 0x02, 0x00, 0x00 });
		}

		if (setting().onNoDeathX) {
			sequence_patch(gd::base + 0x91254, { 0x00 });
		}
		else {
			sequence_patch(gd::base + 0x91254, { 0x01 });
		}

		if (setting().onNoEditorTrail) {
			sequence_patch(gd::base + 0x94305, { 0x00 });
			sequence_patch(gd::base + 0x9442c, { 0x00 });		
		}
		else {
			sequence_patch(gd::base + 0x94305, { 0xff });
			sequence_patch(gd::base + 0x9442c, { 0xff });
		}

		if (setting().onObjectBypass) {
			sequence_patch(gd::base + 0x3e30f, { 0xff, 0xff, 0xff, 0x7f });
			sequence_patch(gd::base + 0x476b3, { 0xff, 0xff, 0xff, 0x7f });
			sequence_patch(gd::base + 0x48ed6, { 0xff, 0xff, 0xff, 0x7f });
		}
		else {
			sequence_patch(gd::base + 0x3e30f, { 0x20, 0x4e, 0x00, 0x00 });
			sequence_patch(gd::base + 0x476b3, { 0x20, 0x4e, 0x00, 0x00 });
			sequence_patch(gd::base + 0x48ed6, { 0x20, 0x4e, 0x00, 0x00 });
		}

		if (setting().onPlaceOver) {
			sequence_patch(gd::base + 0x8d37f, { 0xeb });
		}
		else {
			sequence_patch(gd::base + 0x8d37f, { 0x77 });
		}

		if (setting().onRotationHack) {
			sequence_patch(gd::base + 0x49a5d, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90 });
			sequence_patch(gd::base + 0x49a92, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90 });
			sequence_patch(gd::base + 0x74751, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90 });
			sequence_patch(gd::base + 0xe84c5, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0x49a5d, { 0x8b, 0x80, 0x68, 0x02, 0x00, 0x00 });
			sequence_patch(gd::base + 0x49a92, { 0x8b, 0x80, 0x68, 0x02, 0x00, 0x00 });
			sequence_patch(gd::base + 0x74751, { 0x8b, 0x83, 0x68, 0x02, 0x00, 0x00 });
			sequence_patch(gd::base + 0xe84c5, { 0x8b, 0x83, 0x68, 0x02, 0x00, 0x00 });
		}

		if (setting().onSmoothEditorTrail) {
			sequence_patch(gd::base + 0x91a34, { 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0x91a34, { 0x72, 0x79 });
		}

		if (setting().onVerifyHack) {
			sequence_patch(gd::base + 0x3d760, { 0xeb });
		}
		else {
			sequence_patch(gd::base + 0x3d760, { 0x75 });
		}

		if (setting().onZoomBypass) {
			sequence_patch(gd::base + 0x48bb5, { 0x90, 0x90, 0x90 });
			sequence_patch(gd::base + 0x48bba, { 0x90, 0x90, 0x90 });
			sequence_patch(gd::base + 0x48c15, { 0x90, 0x90, 0x90 });
			sequence_patch(gd::base + 0x48c1a, { 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0x48bb5, { 0x0f, 0x2f, 0xc8 });
			sequence_patch(gd::base + 0x48bba, { 0x0f, 0x28, 0xc8 });
			sequence_patch(gd::base + 0x48c15, { 0x0f, 0x2f, 0xc8 });
			sequence_patch(gd::base + 0x48c1a, { 0x0f, 0x28, 0xc8 });
		}

		// Level

		if (setting().onAutoPickupCoins) {
			cheatAdd();
		}

		if (setting().onConfirmExit) {
			sequence_patch(gd::base + 0xd7f80, { 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(gd::base + 0xd7f8d, { 0x90, 0x90 });
			sequence_patch(gd::base + 0xd7f9d, { 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(gd::base + 0xd7fa5, { 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xd7f80, { 0xe8, 0xfb, 0xbb, 0x01, 0x00 });
			sequence_patch(gd::base + 0xd7f8d, { 0x6a, 0x10 });
			sequence_patch(gd::base + 0xd7f9d, { 0x68, 0x7c, 0x42, 0x51, 0x00 });
			sequence_patch(gd::base + 0xd7fa5, { 0xe8, 0xc6, 0xd7, 0xf2, 0xff });
		}

		if (setting().onCorrectiveMusicSync) {
			sequence_patch(gd::base + 0xee59e, { 0xeb });
		}
		else {
			sequence_patch(gd::base + 0xee59e, { 0x75 });
		}

		if (setting().onEverythingHurts) {
			sequence_patch(gd::base + 0xeaa42, { 0x90, 0x90 });
			cheatAdd();
		}
		else {
			sequence_patch(gd::base + 0xeaa42, { 0x75, 0x0b });
		}

		if (setting().onHighFPSRotationFix) {
			sequence_patch(gd::base + 0xdc13b, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xdc13b, { 0x0f, 0x82, 0xd6, 0x00, 0x00, 0x00 });
		}

		if (setting().onHitboxes) {
			cheatAdd();
		}

		if (setting().onInstantComplete) {
			sequence_patch(gd::base + 0xe16f6, { 0xc7, 0x87, 0x74, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x90, 0x90 });
			cheatAdd();
		}
		else {
			sequence_patch(gd::base + 0xe16f6, { 0xf3, 0x0f, 0x11, 0x8f, 0x74, 0x04, 0x00, 0x00, 0x9f, 0xf6, 0xc4, 0x44 });
		}

		if (setting().onJumpHack) {
			sequence_patch(gd::base + 0xda510, { 0x01 });
			sequence_patch(gd::base + 0xda295, { 0x01 });
			cheatAdd();
		}
		else {
			sequence_patch(gd::base + 0xda510, { 0x00 });
			sequence_patch(gd::base + 0xda295, { 0x00 });
		}

		if (setting().onNoclip) {
			sequence_patch(gd::base + 0xf04e9, { 0xe9, 0xf0, 0x02, 0x00, 0x00, 0x90 });
			cheatAdd();
		}
		else {
			sequence_patch(gd::base + 0xf04e9, { 0x0f, 0x85, 0xef, 0x02, 0x00, 0x00 });
		}

		if (setting().onNoclipUnstuck) {
			sequence_patch(gd::base + 0xdae16, { 0xe9, 0x00, 0x02, 0x00, 0x00, 0x90 });
			sequence_patch(gd::base + 0xdb02d, { 0xe9, 0x10, 0x01, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xdae16, { 0x0f, 0x84, 0xff, 0x01, 0x00, 0x00 });
			sequence_patch(gd::base + 0xdb02d, { 0x0f, 0x8b, 0x0f, 0x01, 0x00, 0x00 });
		}

		if (setting().onPauseDuringCompletion) {
			sequence_patch(gd::base + 0xe531b, { 0x00 });
			sequence_patch(gd::base + 0xe2906, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xe531b, { 0x01 });
			sequence_patch(gd::base + 0xe2906, { 0x88, 0x81, 0xf9, 0x02, 0x00, 0x00 });
		}

		if (setting().onPracticeMusic) {
			sequence_patch(gd::base + 0xf284f, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(gd::base + 0xf3663, { 0x90, 0x90 });
			sequence_patch(gd::base + 0xf0699, { 0x90, 0x90 });
			sequence_patch(gd::base + 0xf06cb, { 0x90, 0x90 });
			sequence_patch(gd::base + 0xf3691, { 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(gd::base + 0xf3943, { 0xeb });
			sequence_patch(gd::base + 0xf3a96, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xf284f, { 0x0f, 0x85, 0x4d, 0x07, 0x00, 0x00 });
			sequence_patch(gd::base + 0xf3663, { 0x75, 0x41 });
			sequence_patch(gd::base + 0xf0699, { 0x75, 0x3e });
			sequence_patch(gd::base + 0xf06cb, { 0x75, 0x0c });
			sequence_patch(gd::base + 0xf3691, { 0xe8, 0xaa, 0x42, 0xf2, 0xff });
			sequence_patch(gd::base + 0xf3943, { 0x74 });
			sequence_patch(gd::base + 0xf3a96, { 0x0f, 0x85, 0xb5, 0x00, 0x00, 0x00 });
		}



		// Universal

		if (setting().onAllowLowVolume) {
			sequence_patch(gd::base + 0xd772e, { 0xeb });
			sequence_patch(gd::base + 0xd0e0e, { 0xeb });
			sequence_patch(gd::base + 0xd0cb0, { 0xeb });
		}
		else {
			sequence_patch(gd::base + 0xd772e, { 0x76 });
			sequence_patch(gd::base + 0xd0e0e, { 0x76 });
			sequence_patch(gd::base + 0xd0cb0, { 0x76 });
		}

		if (setting().onDisableSongAlert) {
			sequence_patch(gd::base + 0x9dd2b, { 0xeb });
		}
		else {
			sequence_patch(gd::base + 0x9dd2b, { 0x75 });
		}

		if (setting().onFastAltTab) {
			sequence_patch(gd::base + 0x28dfe, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0x28dfe, { 0x8b, 0x03, 0x8b, 0xcb, 0xff, 0x50, 0x18 });
		}

		if (setting().onForceVisibility) {
			sequence_patch(libcocosbase + 0x60783, { 0xb0, 0x01, 0x90 });
			sequence_patch(libcocosbase + 0x60c9a, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(libcocosbase + 0x60783, { 0x8a, 0x45, 0x08 });
			sequence_patch(libcocosbase + 0x60c9a, { 0x0f, 0x84, 0xcb, 0x00, 0x00, 0x00 });
		}

		if (setting().onFreeWindowResize) {
			sequence_patch(libcocosbase + 0x10f48b, { 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(libcocosbase + 0x10ee81, { 0xe9, 0x2f, 0xff, 0xff, 0xff, 0x90 });
			sequence_patch(libcocosbase + 0x10e143, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(libcocosbase + 0x10f48b, { 0xe8, 0xb0, 0xf3, 0xff, 0xff });
			sequence_patch(libcocosbase + 0x10ee81, { 0x0f, 0x85, 0x2e, 0xff, 0xff, 0xff });
		}

		if (setting().onIncreaseMaxLevels) {
			sequence_patch(gd::base + 0x5875b, { 0x64 });
		}
		else {
			sequence_patch(gd::base + 0x5875b, { 0x14 });
		}

		if (setting().onNoRotation) {
			sequence_patch(libcocosbase + 0x60578, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(libcocosbase + 0x60578, { 0xf3, 0x0f, 0x11, 0x41, 0x1c, 0xf3, 0x0f, 0x11, 0x41, 0x18 });
		}

		if (setting().onSafeMode) {
			safeModeON();
		}
		else {
			safeModeOFF();
		}

		if (setting().onShowRestartButton) {
			sequence_patch(gd::base + 0xd64d9, { 0x90, 0x90 });
		}
		else {
			sequence_patch(gd::base + 0xd64d9, { 0x75, 0x29 });
		}

		if (setting().onTransparentBG) {
			sequence_patch(gd::base + 0x2cf96, { 0x90, 0xb1, 0xff }); // CreatorLayer
			sequence_patch(gd::base + 0x2cf9e, { 0xff, 0xff });
			sequence_patch(gd::base + 0x3b7db, { 0x90, 0xb1, 0xff }); // LevelBrowserLayer
			sequence_patch(gd::base + 0x3b7e3, { 0xff, 0xff });
			sequence_patch(gd::base + 0x88132, { 0x90, 0xb1, 0xff }); // LevelSearchLayer
			sequence_patch(gd::base + 0x8813a, { 0xff, 0xff });
			sequence_patch(gd::base + 0x8969e, { 0x90, 0xb1, 0xff }); // LevelInfoLayer
			sequence_patch(gd::base + 0x896a5, { 0xff, 0xff });
			sequence_patch(gd::base + 0x9bde1, { 0x90, 0xb1, 0xff }); // EditLevelLayer
			sequence_patch(gd::base + 0x9bde9, { 0xff, 0xff });
			sequence_patch(gd::base + 0x9f97c, { 0x90, 0xb1, 0xff }); // LeaderboardsLayer
			sequence_patch(gd::base + 0x9f984, { 0xff, 0xff });
		}
		else {
			sequence_patch(gd::base + 0x2cf96, { 0x80, 0xc9, 0xff }); // CreatorLayer
			sequence_patch(gd::base + 0x2cf9e, { 0x00, 0x66 });
			sequence_patch(gd::base + 0x3b7db, { 0x80, 0xc9, 0xff }); // LevelBrowserLayer
			sequence_patch(gd::base + 0x3b7e3, { 0x00, 0x66 });
			sequence_patch(gd::base + 0x88132, { 0x80, 0xc9, 0xff }); // LevelSearchLayer
			sequence_patch(gd::base + 0x8813a, { 0x00, 0x66 });
			sequence_patch(gd::base + 0x8969e, { 0x80, 0xc9, 0xff }); // LevelInfoLayer
			sequence_patch(gd::base + 0x896a5, { 0x00, 0x66 });
			sequence_patch(gd::base + 0x9bde1, { 0x80, 0xc9, 0xff }); // EditLevelLayer
			sequence_patch(gd::base + 0x9bde9, { 0x00, 0x66 });
			sequence_patch(gd::base + 0x9f97c, { 0x80, 0xc9, 0xff }); // LeaderboardsLayer
			sequence_patch(gd::base + 0x9f984, { 0x00, 0x66 });
		}

		if (setting().onTransparentLists) {
			sequence_patch((uint32_t)gd::base + 0x31c7f, { 0x00, 0x00 }); // LevelCell::updateBGColor
			sequence_patch((uint32_t)gd::base + 0x31c82, { 0x00 });
			sequence_patch((uint32_t)gd::base + 0x31c89, { 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0x31c8c, { 0x00 });
			sequence_patch((uint32_t)gd::base + 0x31cba, { 0x00 });

			sequence_patch((uint32_t)gd::base + 0x88a4f, { 0x00, 0x00, 0x00, 0x00 }); // LeaderboardsLayer::setupLevelBrowser
			sequence_patch((uint32_t)gd::base + 0x8a945, { 0x00, 0x00, 0x00, 0x00 }); // LevelBrowserLayer::setupLevelBrowser
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x31c7f, { 0xc2, 0x72 });
			sequence_patch((uint32_t)gd::base + 0x31c82, { 0x3e });
			sequence_patch((uint32_t)gd::base + 0x31c89, { 0xa1, 0x58 });
			sequence_patch((uint32_t)gd::base + 0x31c8c, { 0x2c });
			sequence_patch((uint32_t)gd::base + 0x31cba, { 0xff });

			sequence_patch((uint32_t)gd::base + 0x88a4f, { 0xbf, 0x72, 0x3e, 0xff });
			sequence_patch((uint32_t)gd::base + 0x8a945, { 0xbf, 0x72, 0x3e, 0xff });
		}

		if (setting().onZeroDelay) {
			MH_DisableHook(reinterpret_cast<LPVOID>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0xfc240));
		}
		else {
			MH_DisableHook(reinterpret_cast<LPVOID*>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0xfc240));
		}

		// Speedhack

		if (setting().onSpeedhack) {
			cheatAdd();
		}

		oneX = false;
	}

	if (setting().show) {
		auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

		if (setting().onCocosExplorer) {
			renderCocosExplorer(setting().onCocosExplorer);
		}
		
		ImGui::SetNextWindowSize(ImVec2(200.f, 0.f));
		if (ImGui::Begin("PolzHax", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
			ImGui::Text("1.920 - v1.3.0 (160126)");

			ImGui::Checkbox("Auto Save", &setting().onAutoSave);
			ImGui::SameLine(0.f, 7.5f);
			if (ImGui::Button("Save", ImVec2(90, 0))) {
				setting().save();
				gd::FLAlertLayer::create("Saved!", "Your hack state is saved.", "OK")->show();
			}

			if (setting().onDeveloperMode) {
				if (ImGui::Button("Cocos Explorer", ImVec2(185.f, 0.f))) {
					setting().onCocosExplorer = !setting().onCocosExplorer;
				}
			}

			if (GetAsyncKeyState(0x31) && GetAsyncKeyState(0x37) && GetAsyncKeyState(0x30) && GetAsyncKeyState(0x33)) { // Don't say anything about this
				ImGui::Checkbox("Developer Mode", &setting().onDeveloperMode);
			}
		}

		ImGui::SetNextWindowSize(ImVec2(200.f, 0.f));
		if (ImGui::Begin("Interface", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
			static float bgColor[4] = {
				setting().bgColor01,
				setting().bgColor02,
				setting().bgColor03,
				setting().bgColor04
			};

			if (ImGui::ColorEdit4("Background Color##menu", bgColor, ImGuiColorEditFlags_NoInputs)) {
				setting().bgColor01 = bgColor[0];
				setting().bgColor02 = bgColor[1];
				setting().bgColor03 = bgColor[2];
				setting().bgColor04 = bgColor[3];

				colorSet();
			}

			static float overlayColor[4] = {
				setting().overlayColor01,
				setting().overlayColor02,
				setting().overlayColor03,
				setting().overlayColor04
			};

			if (ImGui::ColorEdit4("Overlay Color##menu", overlayColor, ImGuiColorEditFlags_NoInputs)) {
				setting().overlayColor01 = overlayColor[0];
				setting().overlayColor02 = overlayColor[1];
				setting().overlayColor03 = overlayColor[2];
				setting().overlayColor04 = overlayColor[3];

				colorSet();
			}

			if (ImGui::Button("Sort Tabs", ImVec2(185.f, 0.f))) {
				sortTabs();
			}
		}

		ImGui::SetNextWindowSize(ImVec2(200.f, 0.f));
		if (ImGui::Begin("Bypass", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
			ImGui::Checkbox("Character Filter", &setting().onCharacterFilter);
			ImGui::Tooltip("Lets you input any character in text fields.");

			if (ImGui::Checkbox("Icons", &setting().onIcons)) {
				if (setting().onIcons) {
					sequence_patch(gd::base + 0x66e26, { 0x90, 0x90 });
					sequence_patch(gd::base + 0x66b4a, { 0x90, 0x90 });
					sequence_patch(gd::base + 0x66b56, { 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0x66e26, { 0x7d, 0x07 });
					sequence_patch(gd::base + 0x66b4a, { 0x7d, 0x13 });
					sequence_patch(gd::base + 0x66b56, { 0x7d, 0x07 });
				}
			}
			ImGui::Tooltip("Unlocks all icons and colors.");

			if (ImGui::Checkbox("Main Levels", &setting().onMainLevels)) {
				if (setting().onMainLevels) {
					sequence_patch(gd::base + 0xa81af, { 0xe9, 0xa3, 0x02, 0x00, 0x00, 0x90 });
					sequence_patch(gd::base + 0xa8c22, { 0xe9, 0x98, 0x00, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xa81af, { 0x0f, 0x84, 0xa2, 0x02, 0x00, 0x00 });
					sequence_patch(gd::base + 0xa8c22, { 0x0f, 0x8e, 0x97, 0x00, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Unlocks main demon levels.");

			if (ImGui::Checkbox("Slider Limit", &setting().onSliderLimit)) {
				if (setting().onSliderLimit) {
					sequence_patch(gd::base + 0x1cf27, { 0xeb, 0x07 });
					sequence_patch(gd::base + 0x1cf3b, { 0xeb, 0x07 });
				}
				else {
					sequence_patch(gd::base + 0x1cf27, { 0x76, 0x07 });
					sequence_patch(gd::base + 0x1cf3b, { 0x76, 0x07 });
				}
			}
			ImGui::Tooltip("Lets sliders be dragged beyond the visible limit.");

			ImGui::Checkbox("Text Length", &setting().onTextLength);
			ImGui::Tooltip("Allows for unlimited text length in text inputs.");
		}

		ImGui::SetNextWindowSize(ImVec2(200.f, 0.f));
		if (ImGui::Begin("Utility", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
			ImGui::HotKey("", setting().m_p1click, 0.f, ImVec2(90.f, 0.f), "P1 Click");
			ImGui::SameLine(0.f, 0.f);
			ImGui::HotKey("", setting().m_p2click, 0.f, ImVec2(90.f, 0.f), "P2 Click");

			if (ImGui::Button("Uncomplete Level", ImVec2(185, 0))) {
				if (playLayer) {
					auto gsm = gd::GameStatsManager::sharedState();
					auto glm = gd::GameLevelManager::sharedState();

					auto level = playLayer->m_level;

					if (level->m_normalPercent >= 100 && gsm->hasCompletedLevel(level)) {
						gsm->setStat("4", gsm->getStat("4") - 1); // Completed levels

						gsm->m_completedLevels->removeObjectForKey(CCString::createWithFormat("n_%i", level->m_levelID)->getCString());
						if (level->m_stars > 0) {
							gsm->m_completedLevels->removeObjectForKey(gsm->getStarLevelKey(level->m_levelID));
							gsm->m_completedLevels->removeObjectForKey(gsm->getDemonLevelKey(level->m_levelID));
							gsm->setStat("6", gsm->getStat("6") - level->m_stars); // Stars
							
							if (level->m_stars >= 10) {
								gsm->setStat("5", gsm->getStat("5") - 1); // Demons
							}
						}
					}

					level->m_normalPercent = 0;
					level->m_practicePercent = 0;

					gd::FLAlertLayer::create("Success", "Save & Load your data to apply the changes.", "OK")->show();
				}
				else {
					gd::FLAlertLayer::create("Error", "You need to enter the level first.", "OK")->show();
				}
			}

			if (ImGui::Button("Restart Level", ImVec2(185, 0))) {
				if (playLayer) playLayer->resetLevel();
			}

			if (ImGui::Button("Practice Mode", ImVec2(185, 0))) {
				if (playLayer) playLayer->togglePracticeMode(!playLayer->m_practiceMode);
			}

			if (ImGui::Button("Settings", ImVec2(185, 0))) {
				//CCARRAY_FOREACH_B_TYPE(CCDirector::sharedDirector()->getRunningScene()->getChildren(), optionsLayer, gd::OptionsLayer) {
				//	if (optionsLayer->getTag() == 0x725) {
				//		
				//	}
				//	else {
						auto optionsLayer = gd::OptionsLayer::create();
						optionsLayer->showLayer(false);
						CCDirector::sharedDirector()->getRunningScene()->addChild(optionsLayer, CCDirector::sharedDirector()->getRunningScene()->getHighestChildZ() + 1, 0x725);
				//	}
				//}
			}

			static bool showFirstBtn = true;
			static bool showSecondBtn = false;
			if (showFirstBtn) {
				if (ImGui::Button("Refresh Textures", ImVec2(185, 0))) {
					showFirstBtn = false;
					showSecondBtn = true;
				}
			}
			if (showSecondBtn) {
				if (ImGui::Button("Cancel", ImVec2(90, 0))) {
					showFirstBtn = true;
					showSecondBtn = false;
				}
				ImGui::SameLine(0.f, 5.f);
				if (ImGui::Button("Confirm", ImVec2(90, 0))) {
					showFirstBtn = true;
					showSecondBtn = false;

					gd::GameManager::sharedState()->reloadAll(false, false, true);
				}
			}

			if (ImGui::Button("Resources", ImVec2(90, 0))) {
				ShellExecute(0, NULL, std::string(CCFileUtils::sharedFileUtils()->getWritablePath2() + "/Resources").c_str(), NULL, NULL, SW_SHOW);
			}
			ImGui::SameLine(0.f, 5.f);
			if (ImGui::Button("AppData", ImVec2(90, 0))) {
				ShellExecute(0, NULL, CCFileUtils::sharedFileUtils()->getWritablePath().c_str(), NULL, NULL, SW_SHOW);
			}
		}

		ImGui::SetNextWindowSize(ImVec2(200.f, 0.f));
		if (ImGui::Begin("Cosmetic", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
			if (ImGui::Checkbox("Coins Show Uncollected", &setting().onCoinsShowUncollected)) {
				if (setting().onCoinsShowUncollected) {
					sequence_patch(gd::base + 0x43368, { 0x8b, 0xc2, 0x90 });
					sequence_patch(gd::base + 0x735ce, { 0x8b, 0xd9, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0x43368, { 0x0f, 0x44, 0xc2 });
					sequence_patch(gd::base + 0x735ce, { 0x0f, 0x44, 0xd9 });
				}
			}
			ImGui::Tooltip("Forces coins show as uncollected.");

			if (ImGui::Checkbox("Coins in Practice", &setting().onCoinsInPractice)) {
				if (setting().onCoinsInPractice) {
					sequence_patch(gd::base + 0xeb138, { 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xeb138, { 0x75, 0x4c });
				}
			}
			ImGui::Tooltip("Allows for coins to be picked up in practice mode.");

			if (ImGui::Checkbox("Force Don't Enter", &setting().onForceDontEnter)) {
				if (setting().onForceDontEnter) {
					sequence_patch(gd::base + 0xec51c, { 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xec51c, { 0x8b, 0xd9 });
				}
			}
			ImGui::Tooltip("Disables effects when objects enter the viewable play area.");

			if (ImGui::Checkbox("Force Don't Fade", &setting().onForceDontFade)) {
				if (setting().onForceDontFade) {
					sequence_patch(gd::base + 0xebddb, { 0x90, 0x90 });
					sequence_patch(gd::base + 0xebe06, { 0xeb, 0x11 });
					sequence_patch(gd::base + 0xebe20, { 0x90, 0x90 });
					sequence_patch(gd::base + 0xebdf6, { 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xebddb, { 0x74, 0x5d });
					sequence_patch(gd::base + 0xebe06, { 0x74, 0x11 });
					sequence_patch(gd::base + 0xebe20, { 0x75, 0x18 });
					sequence_patch(gd::base + 0xebdf6, { 0x75, 0x42 });
				}
			}
			ImGui::Tooltip("Disables fading when objects leave the viewable play area.");

			if (ImGui::Checkbox("Force Objects Invisible", &setting().onForceObjectsInvisible)) {
				if (setting().onForceObjectsInvisible) {
					sequence_patch(gd::base + 0xebece, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xebece, { 0x0f, 0x84, 0xd1, 0x02, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Disables fading when objects leave the viewable play area.");

			if (ImGui::Checkbox("Hide Attempts", &setting().onHideAttempts)) {
				if (playLayer) {
					playLayer->m_attemptsLabel->setVisible(!setting().onHideAttempts);
				}
			}
			ImGui::Tooltip("Hides the attempts counter in-game.");

			if (ImGui::Checkbox("Hide Pause Button", &setting().onHidePauseButton)) {
				if (playLayer) {
					if (gd::GameManager::sharedState()->getGameVariable("0024")) {
						playLayer->m_uiLayer->m_pauseBtn->setVisible(!setting().onHidePauseButton);
					}
				}
			}
			ImGui::Tooltip("Hides the pause button when the in-game cursor is enabled");

			if (ImGui::Checkbox("Hide Pause Menu", &setting().onHidePauseMenu)) {
				if (PauseLayer::get()) {
					PauseLayer::get()->setVisible(!setting().onHidePauseMenu);
				}
			}
			ImGui::Tooltip("Hides the pause menu.");

			if (ImGui::Checkbox("Hide Player", &setting().onHidePlayer)) {
				if (playLayer) {
					playLayer->m_player->setVisible(!setting().onHidePlayer);
					playLayer->m_player2->setVisible(!setting().onHidePlayer);
				}
			}
			ImGui::Tooltip("Hides the player.");

			if (ImGui::Checkbox("Hide Practice Buttons", &setting().onHidePracticeButtons)) {
				if (playLayer && playLayer->m_practiceMode) {
					playLayer->m_uiLayer->m_checkpointMenu->setVisible(!setting().onHidePracticeButtons);
				}
			}
			ImGui::Tooltip("Hides the practice buttons.");

			if (ImGui::Checkbox("Instant Mirror", &setting().onInstantMirror)) {
				if (setting().onInstantMirror) {
					sequence_patch(gd::base + 0xf0d36, { 0x00, 0x00, 0x00, 0x00 });
					cheatAdd();
				}
				else {
					sequence_patch(gd::base + 0xf0d36, { 0x00, 0x00, 0x00, 0x3f });
					cheatDec();
				}
			}
			ImGui::Tooltip("Disables the mirror portal animation.");

			if (ImGui::Checkbox("Inversed Trail", &setting().onInversedTrail)) {
				setting().onTrailAlwaysOff = false;
				setting().onTrailAlwaysOn = false;
				if (setting().onInversedTrail) {
					sequence_patch(libcocosbase + 0xac6a6, { 0x0f, 0x85, 0x55, 0x02, 0x00, 0x00 });
				}
				else {
					sequence_patch(libcocosbase + 0xac6a6, { 0x0f, 0x84, 0x55, 0x02, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Inverses the trail state.");

			if (ImGui::Checkbox("Max Particles", &setting().onMaxParticles)) {
				if (setting().onMaxParticles) {
					sequence_patch(libcocosbase + 0xb64d7, { 0x8b, 0x7d, 0x07 });
				}
				else {
					sequence_patch(libcocosbase + 0xb64d7, { 0x8b, 0x7d, 0x08 });
				}
			}
			ImGui::Tooltip("Increases particles to the maximum.");

			if (ImGui::Checkbox("Mini Cube Icon", &setting().onMiniCubeIcon)) {
				setting().onNoMiniIcon = false;
			}
			ImGui::Tooltip("Forces use of the mini cube icon.");

			if (ImGui::Checkbox("No Animations", &setting().onNoAnimations)) {
				if (setting().onNoAnimations) {
					sequence_patch(libcocosbase + 0x886be, { 0xc0 });
					sequence_patch(libcocosbase + 0x88da0, { 0xc0 });
				}
				else {
					sequence_patch(libcocosbase + 0x886be, { 0xc1 });
					sequence_patch(libcocosbase + 0x88da0, { 0xc1 });
				}
			}
			ImGui::Tooltip("Makes game animations instant (e.g. buttons).");

			if (ImGui::Checkbox("No Background Flash", &setting().onNoBackgroundFlash)) {
				if (setting().onNoBackgroundFlash) {
					sequence_patch(gd::base + 0xe6c92, { 0x6a, 0x00 });
					sequence_patch(gd::base + 0xe6ca8, { 0xc7, 0x04, 0x24, 0x00, 0x00, 0x00, 0x00 });
				}
				else {
					sequence_patch(gd::base + 0xe6c92, { 0x6a, 0x01 });
					sequence_patch(gd::base + 0xe6ca8, { 0xc7, 0x04, 0x24, 0x8f, 0xc2, 0xf5, 0x3d });
				}
			}
			ImGui::Tooltip("Disables size portal background flash.");

			if (ImGui::Checkbox("No Death Effect", &setting().onNoDeathEffect)) {
				if (setting().onNoDeathEffect) {
					sequence_patch(gd::base + 0xdde71, { 0xe9, 0xe7, 0x01, 0x00, 0x00, 0x90 });
					sequence_patch(gd::base + 0xf05dc, { 0xeb, 0x1b });
				}
				else {
					sequence_patch(gd::base + 0xdde71, { 0x0f, 0x84, 0xd5, 0x01, 0x00, 0x00 });
					sequence_patch(gd::base + 0xf05dc, { 0x74, 0x1b });
				}
			}
			ImGui::Tooltip("No visual effects on death.");

			ImGui::Checkbox("No Effect Circle", &setting().onNoEffectCircle);
			ImGui::Tooltip("Removes effect circles from orb, portal & pad activations.");

			//if (ImGui::Checkbox("No Force Player Glow", &setting().onNoForcePlayerGlow)) {
			//	if (setting().onNoForcePlayerGlow) {
			//		sequence_patch(gd::base + 0xdfcd9, { 0xeb });
			//		sequence_patch(gd::base + 0xdfcef, { 0xeb });
			//	}
			//	else {
			//		sequence_patch(gd::base + 0xdfcd9, { 0x75 });
			//		sequence_patch(gd::base + 0xdfcef, { 0x75 });
			//	}
			//}

			if (ImGui::Checkbox("No Ghost Trail", &setting().onNoGhostTrail)) {
				if (setting().onNoGhostTrail) {
					sequence_patch(gd::base + 0xf3374, { 0x6a, 0x00, 0x90 });
					sequence_patch(gd::base + 0xf338d, { 0x6a, 0x00, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xf3374, { 0xff, 0x75, 0x08 });
					sequence_patch(gd::base + 0xf338d, { 0xff, 0x75, 0x08 });
				}
			}
			ImGui::Tooltip("Disables player ghost trail triggers.");

			if (ImGui::Checkbox("No Glow", &setting().onNoObjectGlow)) {
				if (setting().onNoObjectGlow) {
					sequence_patch(gd::base + 0x6d8a3, { 0xe9, 0x8f, 0x01, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0x6d8a3, { 0x0f, 0x85, 0x8e, 0x01, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Disables objects glow.");

			if (ImGui::Checkbox("No Gravity Effect", &setting().onNoGravityEffect)) {
				if (setting().onNoGravityEffect) {
					sequence_patch(gd::base + 0xf4586, { 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xf4586, { 0x75, 0x07 });
				}
			}
			ImGui::Tooltip("Disables gravity effect.");

			if (ImGui::Checkbox("No Mini Icon", &setting().onNoMiniIcon)) {
				setting().onMiniCubeIcon = false;
			}
			ImGui::Tooltip("Replaces default mini icon with normal one.");

			if (ImGui::Checkbox("No Mirror", &setting().onNoMirror)) {
				if (setting().onNoMirror) {
					sequence_patch(gd::base + 0xf0bf2, { 0xe9, 0x9b, 0x01, 0x00, 0x00, 0x90 });
					cheatAdd();
				}
				else {
					sequence_patch(gd::base + 0xf0bf2, { 0x0f, 0x84, 0x9a, 0x01, 0x00, 0x00 });
					cheatDec();
				}
			}
			ImGui::Tooltip("Disables mirror portals.");

			ImGui::Checkbox("No New Best Popup", &setting().onNoNewBestPopup);
			ImGui::Tooltip("Disables the new best popup.");

			ImGui::Checkbox("No Orb Ring", &setting().onNoOrbRing);
			ImGui::Tooltip("Disables orb ring effect when touching it.");

			ImGui::Checkbox("No Particles", &setting().onNoParticles);
			ImGui::Tooltip("Disables the particle system.");

			if (ImGui::Checkbox("No Portal Lightning", &setting().onNoPortalLightning)) {
				if (setting().onNoPortalLightning) {
					sequence_patch(gd::base + 0xe6c19, { 0xeb, 0x71 });
				}
				else {
					sequence_patch(gd::base + 0xe6c19, { 0x75, 0x71 });
				}
			}
			ImGui::Tooltip("Disables portal lightning from size-changing portals.");

			ImGui::Checkbox("No Portal Shine", &setting().onNoPortalShine);
			ImGui::Tooltip("Disables portal shine effect.");

			ImGui::Checkbox("No Pulse", &setting().onNoPulse);
			ImGui::Tooltip("Disables pulsing on objects.");

			if (ImGui::Checkbox("No Respawn Flash", &setting().onNoRespawnFlash)) {
				if (setting().onNoRespawnFlash) {
					sequence_patch(gd::base + 0xdd62f, { 0xe9, 0x99, 0x00, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xdd62f, { 0x0f, 0x85, 0x98, 0x00, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Disables the respawn flash & circle effect.");

			if (ImGui::Checkbox("No Shade Effect", &setting().onNoShadeEffect)) {
				if (setting().onNoShadeEffect) {
					sequence_patch(gd::base + 0xebfe3, { 0xeb });
					cheatAdd();
				}
				else {
					sequence_patch(gd::base + 0xebfe3, { 0x77 });
					cheatDec();
				}
			}
			ImGui::Tooltip("Disables the disappearing effect on invisible blocks and etc.");

			ImGui::Checkbox("No Wave Pulse", &setting().onNoWavePulse);
			ImGui::Tooltip("Disables wave trail pulsing.");

			if (ImGui::Checkbox("No Wave Trail", &setting().onNoWaveTrail)) {
				if (playLayer) {
					playLayer->m_player->m_hardStreak->setVisible(!setting().onNoWaveTrail);
					playLayer->m_player2->m_hardStreak->setVisible(!setting().onNoWaveTrail);
				}
			}
			ImGui::Tooltip("Disables the hard wave trail.");

			ImGui::Checkbox("No Wave Trail Behind", &setting().onNoWaveTrailBehind);
			ImGui::Tooltip("Disables default player trail behind the wave trail.");

			if (ImGui::Checkbox("Practice Pulse", &setting().onPracticePulse)) {
				if (setting().onPracticePulse) {
					sequence_patch(gd::base + 0x29975, { 0x90, 0x90 });
					sequence_patch(gd::base + 0xeb441, { 0xeb, 0x16 });
				}
				else {
					sequence_patch(gd::base + 0x29975, { 0x75, 0x0c });
					sequence_patch(gd::base + 0xeb441, { 0x74, 0x16 });
				}
			}
			ImGui::Tooltip("Enables pulses in practice mode.");

			ImGui::Checkbox("Show Total Attempts", &setting().onShowTotalAttempts);
			ImGui::Tooltip("Shows the total attempt count in-level instead of the session attempt count.");

			if (ImGui::Checkbox("Solid Player Trail", &setting().onSolidPlayerTrail)) {
				if (setting().onSolidPlayerTrail) {
					sequence_patch(libcocosbase + 0xac3dc, { 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(libcocosbase + 0xac3dc, { 0x89, 0x41, 0x10 });
				}
			}
			ImGui::Tooltip("No blending on player trail.");

			if (ImGui::Checkbox("Solid Wave Trail", &setting().onSolidWaveTrail)) {
				if (setting().onSolidWaveTrail) {
					sequence_patch(gd::base + 0xd9ade, { 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xd9ade, { 0x75, 0x0c });
				}
			}
			ImGui::Tooltip("No blending on the wave trail.");

			if (ImGui::Checkbox("Trail Always Off", &setting().onTrailAlwaysOff)) {
				setting().onInversedTrail = false;
				setting().onTrailAlwaysOn = false;
				if (setting().onTrailAlwaysOff) {
					sequence_patch(libcocosbase + 0xac6a6, { 0xe9, 0x56, 0x02, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch(libcocosbase + 0xac6a6, { 0x0f, 0x84, 0x55, 0x02, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Forces player trail to be always off.");

			if (ImGui::Checkbox("Trail Always On", &setting().onTrailAlwaysOn)) {
				setting().onInversedTrail = false;
				setting().onTrailAlwaysOff = false;
				if (setting().onTrailAlwaysOn) {
					sequence_patch(libcocosbase + 0xac6a6, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(libcocosbase + 0xac6a6, { 0x0f, 0x84, 0x55, 0x02, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Forces player trail to be always on.");

			if (ImGui::Checkbox("Trail Bug Fix", &setting().onTrailBugFix)) {
				if (setting().onTrailBugFix) {
					sequence_patch(libcocosbase + 0xac29d, { 0xbb, 0xff, 0x00, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch(libcocosbase + 0xac29d, { 0xf3, 0x0f, 0x2c, 0xc1, 0x2b, 0xd8 });
				}
			}
			ImGui::Tooltip("Fixes trail cutting on high refresh rates.");

			ImGui::Checkbox("Wave Pulse Size", &setting().onWavePulseSize);
			ImGui::Tooltip("Changes the wave pulse size multiplier.");
			ImGui::SameLine(170.f);
			if (ImGui::TreeNodeEx("##wavePulseSettings", ImGuiTreeNodeFlags_SpanAvailWidth)) {

				ImGui::SetNextItemWidth(80.f);
				ImGui::DragFloat("Size", &setting().wavePulseSize, .1f, .1f, 2.3f, "%.1fx");

				ImGui::TreePop();
			}

			if (ImGui::Checkbox("Wave Trail on Death", &setting().onWaveTrailOnDeath)) {
				if (setting().onWaveTrailOnDeath) {
					sequence_patch(gd::base + 0xdddfa, { 0xeb, 0x0f });
				}
				else {
					sequence_patch(gd::base + 0xdddfa, { 0x74, 0x0f });
				}
			}
			ImGui::Tooltip("Keeps wave trail visible on death.");
		}

		ImGui::SetNextWindowSize(ImVec2(200.f, 0.f));
		if (ImGui::Begin("Creator", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
			if (ImGui::Checkbox("Absolute Position", &setting().onAbsolutePosition)) {
				if (setting().onAbsolutePosition) {
					sequence_patch(gd::base + 0x4b49d, { 0x90, 0x8b, 0xcf, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0x4b49d, { 0x51, 0x8b, 0xcf, 0xff, 0x50, 0x5c });
				}
			}
			ImGui::Tooltip("Locks object position for rotation.");

			if (ImGui::Checkbox("Copy Hack", &setting().onCopyHack)) {
				if (setting().onCopyHack) {
					sequence_patch(gd::base + 0x9c7ed, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
					sequence_patch(gd::base + 0x9dfe5, { 0x90, 0x90 });
					sequence_patch(gd::base + 0x9c80f, { 0x8b, 0xcf, 0x90 }); // replaces passworded copy button sprite with normal one
				}
				else {
					sequence_patch(gd::base + 0x9c7ed, { 0x0f, 0x84, 0x2e, 0x01, 0x00, 0x00 });
					sequence_patch(gd::base + 0x9dfe5, { 0x75, 0x0e });
					sequence_patch(gd::base + 0x9c80f, { 0x0f, 0x44, 0xcf });
				}
			}
			ImGui::Tooltip("Lets you copy any level, without a password.");

			if (ImGui::Checkbox("Default Song Bypass", &setting().onDefaultSongBypass)) {
				if (setting().onDefaultSongBypass) {
					sequence_patch(gd::base + 0x9a37f, { 0x90, 0x90 });
					sequence_patch(gd::base + 0x9a391, { 0x90, 0x90, 0x90 });
					sequence_patch(gd::base + 0x9a3a0, { 0x90, 0x90, 0x90 });
					sequence_patch(gd::base + 0x9a3fe, { 0x90, 0x90 });
					sequence_patch(gd::base + 0x9a410, { 0x90, 0x90, 0x90 });
					sequence_patch(gd::base + 0x9a41f, { 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0x9a37f, { 0x74, 0x4e });
					sequence_patch(gd::base + 0x9a391, { 0x0f, 0x4f, 0xf0 });
					sequence_patch(gd::base + 0x9a3a0, { 0x0f, 0x48, 0xf1 });
					sequence_patch(gd::base + 0x9a3fe, { 0x74, 0x4e });
					sequence_patch(gd::base + 0x9a410, { 0x0f, 0x4f, 0xf0 });
					sequence_patch(gd::base + 0x9a41f, { 0x0f, 0x48, 0xf1 });
				}
			}
			ImGui::Tooltip("Lets you use hidden default songs in the editor."); // Actually the most useless hack in 1.9

			if (ImGui::Checkbox("Editor Extension", &setting().onEditorExtension)) {
				if (setting().onEditorExtension) {
					sequence_patch(gd::base + 0x14a74c, { 0x00, 0x60, 0xea, 0x4a }); // 60000 to 7680000

					sequence_patch(gd::base + 0x4b513, { 0xeb }); // EditorUI::getLimitedPosition
					sequence_patch(gd::base + 0x4b525, { 0xeb });
					sequence_patch(gd::base + 0x4b53e, { 0xeb });
					sequence_patch(gd::base + 0x4b562, { 0xeb });

					sequence_patch(gd::base + 0x4b445, { 0xeb }); // EditorUI::moveObject
					sequence_patch(gd::base + 0x4b457, { 0xeb });
					sequence_patch(gd::base + 0x4b470, { 0xeb });
					sequence_patch(gd::base + 0x4b482, { 0xeb });

					sequence_patch(gd::base + 0x4779c, { 0xeb }); // EditorUI::onCreateObject
					sequence_patch(gd::base + 0x477b9, { 0xeb });
					sequence_patch(gd::base + 0x477d2, { 0xeb });
					sequence_patch(gd::base + 0x477e8, { 0xeb });

					sequence_patch(gd::base + 0x93861, { 0x24, 0xa3, 0x54, 0x00 });
					sequence_patch(gd::base + 0x9399b, { 0x24, 0xa3, 0x54, 0x00 });
					sequence_patch(gd::base + 0x939d3, { 0x24, 0xa3, 0x54, 0x00 });
					sequence_patch(gd::base + 0x93aed, { 0x24, 0xa3, 0x54, 0x00 });
					sequence_patch(gd::base + 0x93c72, { 0x24, 0xa3, 0x54, 0x00 });
					sequence_patch(gd::base + 0x94073, { 0x24, 0xa3, 0x54, 0x00 });
					sequence_patch(gd::base + 0x94112, { 0x24, 0xa3, 0x54, 0x00 });
					sequence_patch(gd::base + 0x94667, { 0x24, 0xa3, 0x54, 0x00 });
					sequence_patch(gd::base + 0x946f3, { 0x24, 0xa3, 0x54, 0x00 });
				}
				else {
					sequence_patch(gd::base + 0x14a74c, { 0x00, 0x60, 0x6a, 0x47 });

					sequence_patch(gd::base + 0x4b513, { 0x76 });
					sequence_patch(gd::base + 0x4b525, { 0x76 });
					sequence_patch(gd::base + 0x4b53e, { 0x76 });
					sequence_patch(gd::base + 0x4b562, { 0x76 });

					sequence_patch(gd::base + 0x4b445, { 0x76 });
					sequence_patch(gd::base + 0x4b457, { 0x76 });
					sequence_patch(gd::base + 0x4b470, { 0x76 });
					sequence_patch(gd::base + 0x4b482, { 0x76 });

					sequence_patch(gd::base + 0x4779c, { 0x77 });
					sequence_patch(gd::base + 0x477b9, { 0x77 });
					sequence_patch(gd::base + 0x477d2, { 0x76 });
					sequence_patch(gd::base + 0x477e8, { 0x76 });

					sequence_patch(gd::base + 0x93861, { 0xb8, 0xa6, 0x54, 0x00 });
					sequence_patch(gd::base + 0x9399b, { 0xb8, 0xa6, 0x54, 0x00 });
					sequence_patch(gd::base + 0x939d3, { 0xb8, 0xa6, 0x54, 0x00 });
					sequence_patch(gd::base + 0x93aed, { 0xb8, 0xa6, 0x54, 0x00 });
					sequence_patch(gd::base + 0x93c72, { 0xb8, 0xa6, 0x54, 0x00 });
					sequence_patch(gd::base + 0x94073, { 0xb8, 0xa6, 0x54, 0x00 });
					sequence_patch(gd::base + 0x94112, { 0xb8, 0xa6, 0x54, 0x00 });
					sequence_patch(gd::base + 0x94667, { 0xb8, 0xa6, 0x54, 0x00 });
					sequence_patch(gd::base + 0x946f3, { 0xb8, 0xa6, 0x54, 0x00 });
				}
			}
			ImGui::Tooltip("Increases the editor length by a factor of 128.");

			if (ImGui::Checkbox("Free Scroll", &setting().onFreeScroll)) {
				if (setting().onFreeScroll) {
					sequence_patch(gd::base + 0x4ca45, { 0xeb });
					sequence_patch(gd::base + 0x4ca5c, { 0xeb });
					sequence_patch(gd::base + 0x4ca75, { 0xeb });
					sequence_patch(gd::base + 0x4ca8c, { 0xeb });
				}
				else {
					sequence_patch(gd::base + 0x4ca45, { 0x77 });
					sequence_patch(gd::base + 0x4ca5c, { 0x77 });
					sequence_patch(gd::base + 0x4ca75, { 0x77 });
					sequence_patch(gd::base + 0x4ca8c, { 0x77 });
				}
			}
			ImGui::Tooltip("Allows scrolling out of the editor.");

			if (ImGui::Checkbox("Hide Grid", &setting().onHideGrid)) {
				if (setting().onHideGrid) {
					sequence_patch(gd::base + 0x939ba, { 0x00 });
					sequence_patch(gd::base + 0x93b6e, { 0x00 });
				}
				else {
					sequence_patch(gd::base + 0x939ba, { 0x96 });
					sequence_patch(gd::base + 0x93b6e, { 0x96 });
				}
			}
			ImGui::Tooltip("Hide the editor grid.");

			if (ImGui::Checkbox("Hide Trigger Lines", &setting().onHideTriggerLines)) {
				if (setting().onHideTriggerLines) {
					sequence_patch(gd::base + 0x93e08, { 0xe9, 0xce, 0x00, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0x93e08, { 0x0f, 0x84, 0xcd, 0x00, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Hides trigger lines.");

			if (ImGui::Checkbox("Hide UI", &setting().onHideUI)) {
				if (LevelEditorLayer::get() && EditorUI::get()) {
					EditorUI::get()->setVisible(!setting().onHideUI);
				}
			}
			ImGui::Tooltip("Hide the editor UI.");

			ImGui::Checkbox("Hitbox Bug Fix", &setting().onHitboxBugFix);
			ImGui::Tooltip("Fixes oriented hitboxes in the editor.");

			if (ImGui::Checkbox("Level Edit", &setting().onLevelEdit)) {
				if (setting().onLevelEdit) {
					sequence_patch(gd::base + 0xd62ef, { 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xd62ef, { 0x75, 0x62 });
				}
			}
			ImGui::Tooltip("Lets you edit any level through the pause menu.");

			if (ImGui::Checkbox("No (C) Mark", &setting().onNoCopyMark)) {
				if (setting().onNoCopyMark) {
					sequence_patch(gd::base + 0x54aa0, { 0xb8, 0x00, 0x00, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0x54aa0, { 0x8b, 0x81, 0x04, 0x02, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Removes the (C) mark when uploading copied levels.");

			if (ImGui::Checkbox("No Death X", &setting().onNoDeathX)) {
				if (setting().onNoDeathX) {
					sequence_patch(gd::base + 0x91254, { 0x00 });
				}
				else {
					sequence_patch(gd::base + 0x91254, { 0x01 });
				}
			}
			ImGui::Tooltip("Removes big annoying red X when dying whilst playtesting.");

			if (ImGui::Checkbox("No Editor Trail", &setting().onNoEditorTrail)) {
				if (setting().onNoEditorTrail) {
					sequence_patch(gd::base + 0x94305, { 0x00 });
					sequence_patch(gd::base + 0x9442c, { 0x00 });
				}
				else {
					sequence_patch(gd::base + 0x94305, { 0xff });
					sequence_patch(gd::base + 0x9442c, { 0xff });
				}
			}
			ImGui::Tooltip("Remove the editor trail.");

			if (ImGui::Checkbox("Object Bypass", &setting().onObjectBypass)) {
				if (setting().onObjectBypass) {
					sequence_patch(gd::base + 0x3e30f, { 0xff, 0xff, 0xff, 0x7f });
					sequence_patch(gd::base + 0x476b3, { 0xff, 0xff, 0xff, 0x7f });
					sequence_patch(gd::base + 0x48ed6, { 0xff, 0xff, 0xff, 0x7f });
				}
				else {
					sequence_patch(gd::base + 0x3e30f, { 0x20, 0x4e, 0x00, 0x00 });
					sequence_patch(gd::base + 0x476b3, { 0x20, 0x4e, 0x00, 0x00 });
					sequence_patch(gd::base + 0x48ed6, { 0x20, 0x4e, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Uncaps the object limit.");

			if (ImGui::Checkbox("Place Over", &setting().onPlaceOver)) {
				if (setting().onPlaceOver) {
					sequence_patch(gd::base + 0x8d37f, { 0xeb });
				}
				else {
					sequence_patch(gd::base + 0x8d37f, { 0x77 });
				}
			}
			ImGui::Tooltip("Lets you place the same object over itself in editor.");

			if (ImGui::Checkbox("Rotation Hack (Lags)", &setting().onRotationHack)) {
				if (setting().onRotationHack) {
					sequence_patch(gd::base + 0x49a5d, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90 });
					sequence_patch(gd::base + 0x49a92, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90 });
					sequence_patch(gd::base + 0x74751, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90 });
					sequence_patch(gd::base + 0xe84c5, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0x49a5d, { 0x8b, 0x80, 0x68, 0x02, 0x00, 0x00 });
					sequence_patch(gd::base + 0x49a92, { 0x8b, 0x80, 0x68, 0x02, 0x00, 0x00 });
					sequence_patch(gd::base + 0x74751, { 0x8b, 0x83, 0x68, 0x02, 0x00, 0x00 });
					sequence_patch(gd::base + 0xe84c5, { 0x8b, 0x83, 0x68, 0x02, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Allows you to rotate any object. Only works locally.");

			if (ImGui::Checkbox("Smooth Editor Trail", &setting().onSmoothEditorTrail)) {
				if (setting().onSmoothEditorTrail) {
					sequence_patch(gd::base + 0x91a34, { 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0x91a34, { 0x72, 0x79 });
				}
			}
			ImGui::Tooltip("Makes the editor trail smoother by updating it every frame instead of at 30fps.");

			if (ImGui::Checkbox("Verify Hack", &setting().onVerifyHack)) {
				if (setting().onVerifyHack) {
					sequence_patch(gd::base + 0x3d760, { 0xeb });
				}
				else {
					sequence_patch(gd::base + 0x3d760, { 0x75 });
				}
			}
			ImGui::Tooltip("Lets you upload unverified levels.");

			if (ImGui::Checkbox("Zoom Bypass", &setting().onZoomBypass)) {
				if (setting().onZoomBypass) {
					sequence_patch(gd::base + 0x48bb5, { 0x90, 0x90, 0x90 });
					sequence_patch(gd::base + 0x48bba, { 0x90, 0x90, 0x90 });
					sequence_patch(gd::base + 0x48c15, { 0x90, 0x90, 0x90 });
					sequence_patch(gd::base + 0x48c1a, { 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0x48bb5, { 0x0f, 0x2f, 0xc8 });
					sequence_patch(gd::base + 0x48bba, { 0x0f, 0x28, 0xc8 });
					sequence_patch(gd::base + 0x48c15, { 0x0f, 0x2f, 0xc8 });
					sequence_patch(gd::base + 0x48c1a, { 0x0f, 0x28, 0xc8 });
				}
			}
			ImGui::Tooltip("Lets you zoom fully in & out. (NOTE: Can crash with an edited grid size)");
		}

		ImGui::SetNextWindowSize(ImVec2(200.f, 0.f));
		if (ImGui::Begin("Level", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
			ImGui::Checkbox("Auto Deafen", &setting().onAutoDeafen);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Deafens user in Discord after a certain %%.");
			ImGui::SameLine(170.f);
			if (ImGui::TreeNodeEx("##autodeafenSettings", ImGuiTreeNodeFlags_SpanAvailWidth)) {
				ImGui::Checkbox("Undeafen On Pause", &setting().onPauseUndeafen);

				ImGui::Checkbox("Deafen in Practice", &setting().onPracticeDeafen);

				ImGui::Checkbox("Deafen with StartPos", &setting().onTestmodeDeafen);

				ImGui::SetNextItemWidth(80.f);
				ImGui::DragFloat("Deafen at", &setting().deafenPercent, 1.f, 0.f, 100.f, "%.0f%%");
				ImGui::SetNextItemWidth(80.f);
				ImGui::DragFloat("Undeafen at", &setting().undeafenPercent, 1.f, 0.f, 100.f, "%.0f%%");

				ImGui::HotKey("Shortcut", setting().m_autoDeafenKey, 0.f, ImVec2(80.f, 0.f));

				ImGui::TreePop();
			}

			ImGui::Checkbox("Auto Kill", &setting().onAutoKill);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Kills a player at a certain percentage.");
			ImGui::SameLine(170.f);
			if (ImGui::TreeNodeEx("##autoKillSettings", ImGuiTreeNodeFlags_SpanAvailWidth)) {

				ImGui::SetNextItemWidth(80.f);
				ImGui::DragFloat("Kill at", &setting().killPercentage, 1.f, 0.f, 100.f, "%.0f%%");

				ImGui::TreePop();
			}

			if (ImGui::Checkbox("Auto Pickup Coins", &setting().onAutoPickupCoins)) {
				if (setting().onAutoPickupCoins) {
					cheatAdd();
				}
				else {
					cheatDec();
				}
			}
			ImGui::Tooltip("Automatically collects gold coins.");

			ImGui::Checkbox("Auto Practice Mode", &setting().onAutoPracticeMode);
			ImGui::Tooltip("Auto-enables practice mode.");

			ImGui::Checkbox("Auto Song Download", &setting().onAutoSongDownload);
			ImGui::Tooltip("Automatically starts downloading songs when you open the level page.");

			ImGui::Checkbox("Ball Rotation Bug Fix", &setting().onBallRotationBugFix);
			ImGui::Tooltip("Fixes that ball rotation bug when entering a portal mid ball animation.");

			ImGui::Checkbox("Checkpoint Lag Fix", &setting().onCheckpointLagFix);
			ImGui::Tooltip("Fixes lag caused by starting from a checkpoint/startpos.");

			if (ImGui::Checkbox("Confirm Exit", &setting().onConfirmExit)) {
				if (setting().onConfirmExit) {
					sequence_patch(gd::base + 0xd7f80, { 0x90, 0x90, 0x90, 0x90, 0x90 });
					sequence_patch(gd::base + 0xd7f8d, { 0x90, 0x90 });
					sequence_patch(gd::base + 0xd7f9d, { 0x90, 0x90, 0x90, 0x90, 0x90 });
					sequence_patch(gd::base + 0xd7fa5, { 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xd7f80, { 0xe8, 0xfb, 0xbb, 0x01, 0x00 });
					sequence_patch(gd::base + 0xd7f8d, { 0x6a, 0x10 });
					sequence_patch(gd::base + 0xd7f9d, { 0x68, 0x7c, 0x42, 0x51, 0x00 });
					sequence_patch(gd::base + 0xd7fa5, { 0xe8, 0xc6, 0xd7, 0xf2, 0xff });
				}
			}
			ImGui::Tooltip("Requires confirmation when exiting a level.");

			if (ImGui::Checkbox("Corrective Music Sync", &setting().onCorrectiveMusicSync)) {
				if (setting().onCorrectiveMusicSync) {
					sequence_patch(gd::base + 0xee59e, { 0xeb });
				}
				else {
					sequence_patch(gd::base + 0xee59e, { 0x75 });
				}
			}
			ImGui::Tooltip("Syncs music to checked speed-portals, instead of only ones the player hit.");

			if (ImGui::Checkbox("Everything Hurts", &setting().onEverythingHurts)) {
				if (setting().onEverythingHurts) {
					sequence_patch(gd::base + 0xeaa42, { 0x90, 0x90 });
					cheatAdd();
				}
				else {
					sequence_patch(gd::base + 0xeaa42, { 0x75, 0x0b });
					cheatDec();
				}
			}
			ImGui::Tooltip("Owie.");

			if (ImGui::Checkbox("High FPS Rotation Fix", &setting().onHighFPSRotationFix)) {
				if (setting().onHighFPSRotationFix) {
					sequence_patch(gd::base + 0xdc13b, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xdc13b, { 0x0f, 0x82, 0xd6, 0x00, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Fixed vehicles rotation on high fps (affects hitboxes).");

			if (ImGui::Checkbox("Hitboxes", &setting().onHitboxes)) {
				if (playLayer) {
					PlayLayer::updateShowHitboxes();
				}

				if (setting().onHitboxes) {
					cheatAdd();
				}
				else {
					cheatDec();
				}
			}
			ImGui::Tooltip("Visualizes level hitboxes.");
			ImGui::SameLine(170.f);
			if (ImGui::TreeNodeEx("##hitboxesSettings", ImGuiTreeNodeFlags_SpanAvailWidth)) {
				// Solids

				ImGui::Checkbox("Solids", &setting().onSolidHitboxes);

				static float solidsColor[3] = {
					setting().solidR / 255.f,
					setting().solidG / 255.f,
					setting().solidB / 255.f
				};

				ImGui::SameLine(120.f);
				if (ImGui::ColorEdit3("##solidsColor", solidsColor, ImGuiColorEditFlags_NoInputs)) {
					setting().solidR = solidsColor[0] * 255;
					setting().solidG = solidsColor[1] * 255;
					setting().solidB = solidsColor[2] * 255;
				}

				// Hazards

				ImGui::Checkbox("Hazards", &setting().onHazardHitboxes);

				static float hazardsColor[3] = {
					setting().hazardR / 255.f,
					setting().hazardG / 255.f,
					setting().hazardB / 255.f
				};

				ImGui::SameLine(120.f);
				if (ImGui::ColorEdit3("##hazardsColor", hazardsColor, ImGuiColorEditFlags_NoInputs)) {
					setting().hazardR = hazardsColor[0] * 255;
					setting().hazardG = hazardsColor[1] * 255;
					setting().hazardB = hazardsColor[2] * 255;
				}

				// Specials

				ImGui::Checkbox("Specials", &setting().onSpecialHitboxes);

				static float specialsColor[3] = {
					setting().specialR / 255.f,
					setting().specialG / 255.f,
					setting().specialB / 255.f
				};

				ImGui::SameLine(120.f);
				if (ImGui::ColorEdit3("##specialsColor", specialsColor, ImGuiColorEditFlags_NoInputs)) {
					setting().specialR = specialsColor[0] * 255;
					setting().specialG = specialsColor[1] * 255;
					setting().specialB = specialsColor[2] * 255;
				}

				ImGui::Checkbox("Player", &setting().onPlayerHitboxes);

				ImGui::SetNextItemWidth(80.f);
				ImGui::DragInt("Opacity", &setting().hitboxesOpacity, 1.f, 0, 255);

				ImGui::TreePop();
			}

			ImGui::Checkbox("Hitboxes on Death", &setting().onHitboxesOnDeath);
			ImGui::Tooltip("Visualizes level hitboxes, only on death");

			if (ImGui::Checkbox("Instant Complete", &setting().onInstantComplete)) {
				if (setting().onInstantComplete) {
					sequence_patch(gd::base + 0xe16f6, { 0xc7, 0x87, 0x74, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x90, 0x90 });
					cheatAdd();
				}
				else {
					sequence_patch(gd::base + 0xe16f6, { 0xf3, 0x0f, 0x11, 0x8f, 0x74, 0x04, 0x00, 0x00, 0x9f, 0xf6, 0xc4, 0x44 });
					cheatDec();
				}
			}
			ImGui::Tooltip("Teleports the player to the end of a level.");

			ImGui::Checkbox("Invisible Dual Fix", &setting().onInvisibleDualFix);
			ImGui::Tooltip("Fixes where your 2nd player sometimes becomes invisible.");

			if (ImGui::Checkbox("Jump Hack", &setting().onJumpHack)) {
				if (setting().onJumpHack) {
					sequence_patch(gd::base + 0xda510, { 0x01 });
					sequence_patch(gd::base + 0xda295, { 0x01 });
					cheatAdd();
				}
				else {
					sequence_patch(gd::base + 0xda510, { 0x00 });
					sequence_patch(gd::base + 0xda295, { 0x00 });
					cheatDec();
				}
			}
			ImGui::Tooltip("Allows you to jump in mid-air.");

			if (ImGui::Checkbox("Noclip", &setting().onNoclip)) {
				if (setting().onNoclip) {
					sequence_patch(gd::base + 0xf04e9, { 0xe9, 0xf0, 0x02, 0x00, 0x00, 0x90 });
					cheatAdd();
				}
				else {
					sequence_patch(gd::base + 0xf04e9, { 0x0f, 0x85, 0xef, 0x02, 0x00, 0x00 });
					cheatDec();
				}
			}
			ImGui::Tooltip("Makes the player invincible.");
			ImGui::SameLine(170.f);
			if (ImGui::TreeNodeEx("##noclipSettings", ImGuiTreeNodeFlags_SpanAvailWidth)) {
				if (ImGui::Checkbox("Experimental (buggy)", &setting().onNoclipUnstuck)) {
					if (setting().onNoclipUnstuck) {
						sequence_patch(gd::base + 0xdae16, { 0xe9, 0x00, 0x02, 0x00, 0x00, 0x90 });
						sequence_patch(gd::base + 0xdb02d, { 0xe9, 0x10, 0x01, 0x00, 0x00, 0x90 });
					}
					else {
						sequence_patch(gd::base + 0xdae16, { 0x0f, 0x84, 0xff, 0x01, 0x00, 0x00 });
						sequence_patch(gd::base + 0xdb02d, { 0x0f, 0x8b, 0x0f, 0x01, 0x00, 0x00 });
					}
				}
				ImGui::Tooltip("Prevents player from getting stuck.");

				ImGui::TreePop();
			}

			if (ImGui::Checkbox("Pause During Completion", &setting().onPauseDuringCompletion)) {
				if (setting().onPauseDuringCompletion) {
					sequence_patch(gd::base + 0xe531b, { 0x00 });
					sequence_patch(gd::base + 0xe2906, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xe531b, { 0x01 });
					sequence_patch(gd::base + 0xe2906, { 0x88, 0x81, 0xf9, 0x02, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Lets you pause during the level complete animation.");

			ImGui::Checkbox("Practice Bug Fix", &setting().onPracticeFix);
			ImGui::Tooltip("Saves & restores player velocity and object blending in practice mode.");

			if (ImGui::Checkbox("Practice Music", &setting().onPracticeMusic)) {
				if (setting().onPracticeMusic) {
					sequence_patch(gd::base + 0xf284f, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
					sequence_patch(gd::base + 0xf3663, { 0x90, 0x90 });
					sequence_patch(gd::base + 0xf0699, { 0x90, 0x90 });
					sequence_patch(gd::base + 0xf06cb, { 0x90, 0x90 });
					sequence_patch(gd::base + 0xf3691, { 0x90, 0x90, 0x90, 0x90, 0x90 });
					sequence_patch(gd::base + 0xf3943, { 0xeb });
					sequence_patch(gd::base + 0xf3a96, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xf284f, { 0x0f, 0x85, 0x4d, 0x07, 0x00, 0x00 });
					sequence_patch(gd::base + 0xf3663, { 0x75, 0x41 });
					sequence_patch(gd::base + 0xf0699, { 0x75, 0x3e });
					sequence_patch(gd::base + 0xf06cb, { 0x75, 0x0c });
					sequence_patch(gd::base + 0xf3691, { 0xe8, 0xaa, 0x42, 0xf2, 0xff });
					sequence_patch(gd::base + 0xf3943, { 0x74 });
					sequence_patch(gd::base + 0xf3a96, { 0x0f, 0x85, 0xb5, 0x00, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Plays the level's song in-sync with your position.");

			ImGui::Checkbox("Replay Last Checkpoint", &setting().onReplayLastCheckpoint);
			ImGui::Tooltip("Respawn from your last practice mode checkpoint after completing a level.");

			if (ImGui::Checkbox("Show Layout", &setting().onShowLayout)) {
				if (setting().onShowLayout) {
					cheatAdd();
				}
				else {
					cheatDec();
				}
			}
			ImGui::Tooltip("Removes all decoration and color from levels.");
			ImGui::SameLine(170.f);
			if (ImGui::TreeNodeEx("##layoutSettings", ImGuiTreeNodeFlags_SpanAvailWidth)) {
				static float layoutBg[3] = {
					setting().layoutBGR / 255.f,
					setting().layoutBGG / 255.f,
					setting().layoutBGB / 255.f
				};

				if (ImGui::ColorEdit3("Background Color##layout", layoutBg, ImGuiColorEditFlags_NoInputs)) {
					setting().layoutBGR = layoutBg[0] * 255;
					setting().layoutBGG = layoutBg[1] * 255;
					setting().layoutBGB = layoutBg[2] * 255;
				}

				static float layoutG[3] = {
					setting().layoutGR / 255.f,
					setting().layoutGG / 255.f,
					setting().layoutGB / 255.f
				};

				if (ImGui::ColorEdit3("Ground Color##layout", layoutG, ImGuiColorEditFlags_NoInputs)) {
					setting().layoutGR = layoutG[0] * 255;
					setting().layoutGG = layoutG[1] * 255;
					setting().layoutGB = layoutG[2] * 255;
				}

				ImGui::TreePop();
			}

			ImGui::Checkbox("Smart StartPos", &setting().onSmartStartPos);
			ImGui::Tooltip("Automatically sets gamemode, speed, size & border for a startpos.");

			ImGui::Checkbox("StartPos Switcher", &setting().onStartPosSwitcher);
			ImGui::Tooltip("Lets you switch between multiple start positions in-level.");
			ImGui::SameLine(170.f);
			if (ImGui::TreeNodeEx("##startPosSwitcherSettings", ImGuiTreeNodeFlags_SpanAvailWidth)) {
				ImGui::HotKey("Previous", setting().m_previousStartPosKey, 0.f, ImVec2(80.f, 0.f));
				ImGui::HotKey("Next", setting().m_nextStartPosKey, 0.f, ImVec2(80.f, 0.f));

				ImGui::TreePop();
			}
		}

		ImGui::SetNextWindowSize(ImVec2(200.f, 0.f));
		if (ImGui::Begin("Universal", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
			ImGui::SetNextItemWidth(80.f);
			if (ImGui::DragFloat("##fpsBypass", &setting().fpsValue, 1.f, 1.f, 360.f, "%.0f FPS")) {
				updateFPSBypass();
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Unlock FPS", &setting().onFPSBypass)) {
				updateFPSBypass();
			}

			if (ImGui::Checkbox("Allow Low Volume", &setting().onAllowLowVolume)) {
				if (setting().onAllowLowVolume) {
					sequence_patch(gd::base + 0xd772e, { 0xeb });
					sequence_patch(gd::base + 0xd0e0e, { 0xeb });
					sequence_patch(gd::base + 0xd0cb0, { 0xeb });
				}
				else {
					sequence_patch(gd::base + 0xd772e, { 0x76 });
					sequence_patch(gd::base + 0xd0e0e, { 0x76 });
					sequence_patch(gd::base + 0xd0cb0, { 0x76 });
				}
			}
			ImGui::Tooltip("Removes snapping to 0%% when setting volume to 3%% or below.");

			ImGui::Checkbox("Auto Safe Mode", &setting().onAutoSafeMode);
			ImGui::Tooltip("Enables Safe Mode when cheats are enabled.");

			if (ImGui::Checkbox("Disable Song Alert", &setting().onDisableSongAlert)) {
				if (setting().onDisableSongAlert) {
					sequence_patch(gd::base + 0x9dd2b, { 0xeb });
				}
				else {
					sequence_patch(gd::base + 0x9dd2b, { 0x75 });
				}
			}
			ImGui::Tooltip("Disables song alert when trying to play a level without downloaded song.");

			if (ImGui::Checkbox("Fast Alt-Tab", &setting().onFastAltTab)) {
				if (setting().onFastAltTab) {
					sequence_patch(gd::base + 0x28dfe, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0x28dfe, { 0x8b, 0x03, 0x8b, 0xcb, 0xff, 0x50, 0x18 });
				}
			}
			ImGui::Tooltip("Disables savefile saving on minimize.");

			if (ImGui::Checkbox("Force Visibility", &setting().onForceVisibility)) {
				if (setting().onForceVisibility) {
					sequence_patch(libcocosbase + 0x60783, { 0xb0, 0x01, 0x90 });
					sequence_patch(libcocosbase + 0x60c9a, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(libcocosbase + 0x60783, { 0x8a, 0x45, 0x08 });
					sequence_patch(libcocosbase + 0x60c9a, { 0x0f, 0x84, 0xcb, 0x00, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Sets all nodes to be visible.");

			if (ImGui::Checkbox("Free Window Resize", &setting().onFreeWindowResize)) {
				if (setting().onFreeWindowResize) {
					sequence_patch(libcocosbase + 0x10f48b, { 0x90, 0x90, 0x90, 0x90, 0x90 });
					sequence_patch(libcocosbase + 0x10ee81, { 0xe9, 0x2f, 0xff, 0xff, 0xff, 0x90 });
					sequence_patch(libcocosbase + 0x10e143, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(libcocosbase + 0x10f48b, { 0xe8, 0xb0, 0xf3, 0xff, 0xff });
					sequence_patch(libcocosbase + 0x10ee81, { 0x0f, 0x85, 0x2e, 0xff, 0xff, 0xff });
				}
			}
			ImGui::Tooltip("Removes limits in place for window resizing.");

			ImGui::Checkbox("HUE Fix", &setting().onHUEFix);
			ImGui::Tooltip("Fixes that yellow and purple color bug.");

			if (ImGui::Checkbox("Increase Max Levels", &setting().onIncreaseMaxLevels)) {
				if (setting().onIncreaseMaxLevels) {
					sequence_patch(gd::base + 0x5875b, { 0x64 });
				}
				else {
					sequence_patch(gd::base + 0x5875b, { 0x14 });
				}
			}
			ImGui::Tooltip("Increases the maximum saved levels from 20 to 100.");

			ImGui::Checkbox("Lock Cursor", &setting().onLockCursor);
			ImGui::Tooltip("Locks cursor position while playing."); // CURSOS

			if (ImGui::Checkbox("No Rotation", &setting().onNoRotation)) {
				if (setting().onNoRotation) {
					sequence_patch(libcocosbase + 0x60578, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch(libcocosbase + 0x60578, { 0xf3, 0x0f, 0x11, 0x41, 0x1c, 0xf3, 0x0f, 0x11, 0x41, 0x18 });
				}
			}
			ImGui::Tooltip("Locks all rotation at 0 degrees.");

			ImGui::Checkbox("No Transition", &setting().onNoTransition);
			ImGui::Tooltip("Shorterns scene transition time to 0s.");

			ImGui::Checkbox("Retry Keybind", &setting().onRetryKeybind);
			ImGui::Tooltip("Lets you restart level by pressing R.");
			ImGui::SameLine(170.f);
			if (ImGui::TreeNodeEx("##retryKeySettings", ImGuiTreeNodeFlags_SpanAvailWidth)) {
				ImGui::HotKey("Keybind", setting().m_retryKeybind, 0.f, ImVec2(80.f, 0.f));

				ImGui::TreePop();
			}

			if (ImGui::Checkbox("Safe Mode", &setting().onSafeMode)) {
				if (setting().onSafeMode) {
					safeModeON();
				}
				else {
					safeModeOFF();
				}
			}
			ImGui::Tooltip("Disables progress and completion of levels.");

			if (ImGui::Checkbox("Show Restart Button", &setting().onShowRestartButton)) {
				if (setting().onShowRestartButton) {
					sequence_patch(gd::base + 0xd64d9, { 0x90, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xd64d9, { 0x75, 0x29 });
				}
			}
			ImGui::Tooltip("Shows the restart button in pause menu.");

			if (ImGui::Checkbox("Transparent BG", &setting().onTransparentBG)) {
				if (setting().onTransparentBG) {
					sequence_patch(gd::base + 0x2cf96, { 0x90, 0xb1, 0xff }); // CreatorLayer
					sequence_patch(gd::base + 0x2cf9e, { 0xff, 0xff });
					sequence_patch(gd::base + 0x3b7db, { 0x90, 0xb1, 0xff }); // LevelBrowserLayer
					sequence_patch(gd::base + 0x3b7e3, { 0xff, 0xff });
					sequence_patch(gd::base + 0x88132, { 0x90, 0xb1, 0xff }); // LevelSearchLayer
					sequence_patch(gd::base + 0x8813a, { 0xff, 0xff });
					sequence_patch(gd::base + 0x8969e, { 0x90, 0xb1, 0xff }); // LevelInfoLayer
					sequence_patch(gd::base + 0x896a5, { 0xff, 0xff });
					sequence_patch(gd::base + 0x9bde1, { 0x90, 0xb1, 0xff }); // EditLevelLayer
					sequence_patch(gd::base + 0x9bde9, { 0xff, 0xff });
					sequence_patch(gd::base + 0x9f97c, { 0x90, 0xb1, 0xff }); // LeaderboardsLayer
					sequence_patch(gd::base + 0x9f984, { 0xff, 0xff });
				}
				else {
					sequence_patch(gd::base + 0x2cf96, { 0x80, 0xc9, 0xff }); // CreatorLayer
					sequence_patch(gd::base + 0x2cf9e, { 0x00, 0x66 });
					sequence_patch(gd::base + 0x3b7db, { 0x80, 0xc9, 0xff }); // LevelBrowserLayer
					sequence_patch(gd::base + 0x3b7e3, { 0x00, 0x66 });
					sequence_patch(gd::base + 0x88132, { 0x80, 0xc9, 0xff }); // LevelSearchLayer
					sequence_patch(gd::base + 0x8813a, { 0x00, 0x66 });
					sequence_patch(gd::base + 0x8969e, { 0x80, 0xc9, 0xff }); // LevelInfoLayer
					sequence_patch(gd::base + 0x896a5, { 0x00, 0x66 });
					sequence_patch(gd::base + 0x9bde1, { 0x80, 0xc9, 0xff }); // EditLevelLayer
					sequence_patch(gd::base + 0x9bde9, { 0x00, 0x66 });
					sequence_patch(gd::base + 0x9f97c, { 0x80, 0xc9, 0xff }); // LeaderboardsLayer
					sequence_patch(gd::base + 0x9f984, { 0x00, 0x66 });
				}
			}
			ImGui::Tooltip("Removes the blue filter from menu's backgrounds.");

			if (ImGui::Checkbox("Transparent Lists", &setting().onTransparentLists)) {
				if (setting().onTransparentLists) {
					sequence_patch((uint32_t)gd::base + 0x31c7f, { 0x00, 0x00 }); // LevelCell::updateBGColor
					sequence_patch((uint32_t)gd::base + 0x31c82, { 0x00 });
					sequence_patch((uint32_t)gd::base + 0x31c89, { 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0x31c8c, { 0x00 });
					sequence_patch((uint32_t)gd::base + 0x31cba, { 0x00 });

					sequence_patch((uint32_t)gd::base + 0x88a4f, { 0x00, 0x00, 0x00, 0x00 }); // LeaderboardsLayer::setupLevelBrowser
					sequence_patch((uint32_t)gd::base + 0x8a945, { 0x00, 0x00, 0x00, 0x00 }); // LevelBrowserLayer::setupLevelBrowser
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x31c7f, { 0xc2, 0x72 });
					sequence_patch((uint32_t)gd::base + 0x31c82, { 0x3e });
					sequence_patch((uint32_t)gd::base + 0x31c89, { 0xa1, 0x58 });
					sequence_patch((uint32_t)gd::base + 0x31c8c, { 0x2c });
					sequence_patch((uint32_t)gd::base + 0x31cba, { 0xff });

					sequence_patch((uint32_t)gd::base + 0x88a4f, { 0xbf, 0x72, 0x3e, 0xff });
					sequence_patch((uint32_t)gd::base + 0x8a945, { 0xbf, 0x72, 0x3e, 0xff });
				}
			}
			ImGui::Tooltip("Makes the menu lists transparent.");

			if (ImGui::Checkbox("Zero Delay", &setting().onZeroDelay)) {
				if (setting().onZeroDelay) {
					MH_DisableHook(reinterpret_cast<LPVOID>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0xfc240));
				}
				else {
					MH_DisableHook(reinterpret_cast<LPVOID*>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0xfc240));
				}
			}
			ImGui::Tooltip("Reduces input delay.");
		}

		ImGui::SetNextWindowSize(ImVec2(200.f, 0.f));
		if (ImGui::Begin("Speedhack", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
			ImGui::SetNextItemWidth(90.f);
			if (ImGui::DragFloat("##speedhack", &setting().speedhackValue, .05f, 0.f, 10.f)) {
				if (setting().speedhackValue < 0.f) setting().speedhackValue = 0.f;
				updateSpeedhack();
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Enabled", &setting().onSpeedhack)) {
				updateSpeedhack();
				if (setting().onSpeedhack) {
					cheatAdd();
				}
				else {
					cheatDec();
				}
			}

			if (ImGui::Checkbox("Speedhack Music", &setting().onSpeedhackAudio)) {
				updateSpeedhack();
			}

			if (ImGui::Checkbox("Classic Mode", &setting().onClassicMode)) {
				updateSpeedhack();
			}
		}
	}

	updateFPSBypass();
	updateSpeedhack();

	if (setting().onFPSBypass) {
		updateFPSBypass();
	}
	if (setting().onSpeedhack) {
		updateSpeedhack();
	}
}

void imgui_init() {
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF("Muli-SemiBold.ttf", 16.f);
	io.Fonts->Build();

	ImGui::GetStyle().WindowTitleAlign = ImVec2(.5f, .5f);
	ImGui::GetStyle().WindowBorderSize = 0;

	colorSet();
}

void setupImGuiMenu() {
	if (!std::filesystem::is_directory("PolzHax") || !std::filesystem::exists("PolzHax"))
	{
		std::filesystem::create_directory("PolzHax");
	}
	if (!std::filesystem::is_directory("PolzHax/extensions") || !std::filesystem::exists("PolzHax/extensions"))
	{
		std::filesystem::create_directory("PolzHax/extensions");
	}
	if (!std::filesystem::is_directory("PolzHax/replays") || !std::filesystem::exists("PolzHax/replays"))
	{
		std::filesystem::create_directory("PolzHax/replays");
	}

	auto path = CCFileUtils::sharedFileUtils()->getWritablePath2() + "PolzHax/extensions";

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		if (file.path().extension() == ".dll")
		{
			auto dllname = file.path().filename().string();
			dllNames.push_back(dllname);
			LoadLibrary(file.path().string().c_str());
		}
	}

	std::cout << "Extensions Loaded: " << dllNames.size() << std::endl;
	for (const auto& name : dllNames) {
		std::cout << name << std::endl;
	}

	ImGuiHook::setToggleCallback([]() { setting().show = !setting().show; });
	ImGuiHook::setRenderFunction(imgui_render);
	ImGuiHook::setInitFunction(imgui_init);
	ImGuiHook::setupHooks([](auto addr, auto hook, auto orig) {
		MH_CreateHook(addr, hook, orig);
		MH_EnableHook(addr);
		});
}