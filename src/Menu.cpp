#include "Menu.hpp"
#include <Geode/Geode.hpp>

#include "imgui-hook.hpp"
#include <../imgui/imgui.h>
#include <../imgui/imgui_internal.h>
#include <../imgui/misc/cpp/imgui_stdlib.h>

#include "Setting.hpp"
#include "patching.hpp"
#include "ImGuiUtils.hpp"
#include "utils.hpp"

using namespace geode::prelude;

ImVec4 color1;
ImVec4 color2;
ImVec4 color3;
ImVec4 color4;
ImVec4 color5;
ImVec4 color6;

bool oneX = true;

ImGuiTextFilter filter;

ImGuiTextFilter PolzHax::getTextFilter() {
	return filter;
}

const char* decimalPlaces[] = {
	"4 decimal places", "3 decimal places", "2 decimal places", "1 decimal place"
};

const int decimalValues[] = {
	4, 3, 2, 1
};

const char* priorities[] = {
	"Lowest", "Low", "Normal", "High", "Highest"
};

const char* statusLabelsPosition[] = {
	"Top-Left", "Top-Right", "Bottom-Right", "Bottom-Left"
};

void updatePriority() {
	switch (setting().priority) {
	case 0:
		SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS); break;
	case 1:
		SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS); break;
	case 2:
		SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS); break;
	case 3:
		SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS); break;
	case 4:
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS); break;
	}
}

void PolzHax::updateFPSBypass() {
	if (GameManager::sharedState()->getGameVariable("4080")) return; // If Zmx's FPS bypass turned on.

	auto currentFps = setting().fpsValue;

	if (currentFps == 0.f) {
		currentFps = 60.f;
	}

	if (setting().onFPSBypass) {
		CCApplication::sharedApplication()->toggleVerticalSync(false);
		CCDirector::sharedDirector()->setAnimationInterval(1 / static_cast<double>(currentFps));
	}
	else {
		CCDirector::sharedDirector()->setAnimationInterval(1 / 60.0);
		if (GameManager::sharedState()->getGameVariable("0030")) {
			CCApplication::sharedApplication()->toggleVerticalSync(true);
		}
	}
}

void updateSpeedhack() {
	if (setting().speedhackValue == 0.f) return;

	const auto value = setting().onSpeedhack ? setting().speedhackValue : 1.f;

	CCDirector::sharedDirector()->m_pScheduler->setTimeScale(setting().onClassicMode ? 1.f : value);
	//SpeedHack::SetSpeed(setting().onClassicMode ? value : 1.f);

	if (auto fme = FMODAudioEngine::sharedEngine()) {
		if (auto sound = fme->m_globalChannel) {
			if (setting().onSpeedhackAudio) sound->setPitch(value);
			else sound->setPitch(1.f);
		}
	}
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
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
}

void sortTabs() {
    float polzhax_xPos = 5.f;
	float addingInterfaceY = -1.f;
	float addingReplayY = -1.f;
	float bypass_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("PolzHax", nullptr);
		ImGui::SetWindowPos(ImVec2(5.f, 5.f));
		bypass_xPos = polzhax_xPos + ImGui::GetWindowWidth() + 5.f;
		addingInterfaceY = ImGui::GetWindowHeight() + 10.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Interface", nullptr);
		ImGui::SetWindowPos(ImVec2(5.f, addingInterfaceY));
		addingReplayY = addingInterfaceY + ImGui::GetWindowHeight() + 5.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Replay", nullptr);
		ImGui::SetWindowPos(ImVec2(5.f, addingReplayY));
	}
	float addingUtilityY = -1.f;
	float addingScreenshotY = -1.f;
	float cosmetic_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Bypass", nullptr);
		ImGui::SetWindowPos(ImVec2(bypass_xPos, 5.f));
		cosmetic_xPos = bypass_xPos + ImGui::GetWindowWidth() + 5.f;
		addingUtilityY = ImGui::GetWindowHeight() + 10.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Utility", nullptr);
		ImGui::SetWindowPos(ImVec2(bypass_xPos, addingUtilityY));
		addingScreenshotY = addingUtilityY + ImGui::GetWindowHeight() + 5.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Screenshot", nullptr);
		ImGui::SetWindowPos(ImVec2(bypass_xPos, addingScreenshotY));
	}
	float creator_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Cosmetic", nullptr);
		ImGui::SetWindowPos(ImVec2(cosmetic_xPos, 5.f));
		creator_xPos = cosmetic_xPos + ImGui::GetWindowWidth() + 5.f;
	}
	float addingVariablesY = -1.f;
	float level_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Creator", nullptr);
		ImGui::SetWindowPos(ImVec2(creator_xPos, 5.f));
		level_xPos = creator_xPos + ImGui::GetWindowWidth() + 5.f;
		addingVariablesY = ImGui::GetWindowHeight() + 10.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Variables", nullptr);
		ImGui::SetWindowPos(ImVec2(creator_xPos, addingVariablesY));
	}
	float universal_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Level", nullptr);
		ImGui::SetWindowPos(ImVec2(level_xPos, 5.f));
		universal_xPos = level_xPos + ImGui::GetWindowWidth() + 5.f;
	}
	float addingSpeedhackY = -1.f;
	float status_xPos = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Universal", nullptr);
		ImGui::SetWindowPos(ImVec2(universal_xPos, 5.f));
		status_xPos = universal_xPos + ImGui::GetWindowWidth() + 5.f;
		addingSpeedhackY = ImGui::GetWindowHeight() + 10.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Speedhack", nullptr);
		ImGui::SetWindowPos(ImVec2(universal_xPos, addingSpeedhackY));
	}
	float addingIconsY = -1.f;
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Status", nullptr);
		ImGui::SetWindowPos(ImVec2(status_xPos, 5.f));
		addingIconsY = ImGui::GetWindowHeight() + 10.f;
	}
	{
		ImGui::SetWindowSize(ImVec2(200.f, 0.f));
		ImGui::Begin("Icons", nullptr);
		ImGui::SetWindowPos(ImVec2(status_xPos, addingIconsY));
	}
}

void imgui_render() {
    auto playLayer = GameManager::sharedState()->getPlayLayer();
    auto editorLayer = 0;

    const float SHORT_ITEM_WIDTH = (ImGui::GetWindowWidth() / 2.f - ImGui::GetStyle().WindowPadding.x * 1.25f);
	const float LONG_ITEM_WIDTH = (ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2.f);

    if (oneX) {
		setting().load();

        sortTabs();

        colorSet();

		if (setting().onThreadPriority) {
			updatePriority();
		}

		PolzHax::updateFPSBypass();

		// Bypass

		if (setting().onIcons) {
			sequence_patch(geode::base::get() + 0x66e26, { 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0x66b4a, { 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0x66b56, { 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0x66e26, { 0x7d, 0x07 });
			sequence_patch(geode::base::get() + 0x66b4a, { 0x7d, 0x13 });
			sequence_patch(geode::base::get() + 0x66b56, { 0x7d, 0x07 });
		}

		if (setting().onMainLevels) {
			sequence_patch(geode::base::get() + 0xa81af, { 0xe9, 0xa3, 0x02, 0x00, 0x00, 0x90 });
			sequence_patch(geode::base::get() + 0xa8c22, { 0xe9, 0x98, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xa81af, { 0x0f, 0x84, 0xa2, 0x02, 0x00, 0x00 });
			sequence_patch(geode::base::get() + 0xa8c22, { 0x0f, 0x8e, 0x97, 0x00, 0x00, 0x00 });
		}

		if (setting().onSliderLimit) {
			sequence_patch(geode::base::get() + 0x1cf27, { 0xeb, 0x07 });
			sequence_patch(geode::base::get() + 0x1cf3b, { 0xeb, 0x07 });
		}
		else {
			sequence_patch(geode::base::get() + 0x1cf27, { 0x76, 0x07 });
			sequence_patch(geode::base::get() + 0x1cf3b, { 0x76, 0x07 });
		}

		// Cosmetic

		if (setting().onCoinsShowUncollected) {
			sequence_patch(geode::base::get() + 0x43368, { 0x8b, 0xc2, 0x90 });
			sequence_patch(geode::base::get() + 0x735ce, { 0x8b, 0xd9, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0x43368, { 0x0f, 0x44, 0xc2 });
			sequence_patch(geode::base::get() + 0x735ce, { 0x0f, 0x44, 0xd9 });
		}

		if (setting().onCoinsInPractice) {
			sequence_patch(geode::base::get() + 0xeb138, { 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xeb138, { 0x75, 0x4c });
		}

		if (setting().onForceDontEnter) {
			sequence_patch(geode::base::get() + 0xec51c, { 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xec51c, { 0x8b, 0xd9 });
		}

		if (setting().onForceDontFade) {
			sequence_patch(geode::base::get() + 0xebddb, { 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0xebe06, { 0xeb, 0x11 });
			sequence_patch(geode::base::get() + 0xebe20, { 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0xebdf6, { 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xebddb, { 0x74, 0x5d });
			sequence_patch(geode::base::get() + 0xebe06, { 0x74, 0x11 });
			sequence_patch(geode::base::get() + 0xebe20, { 0x75, 0x18 });
			sequence_patch(geode::base::get() + 0xebdf6, { 0x75, 0x42 });
		}

		if (setting().onForceObjectsInvisible) {
			sequence_patch(geode::base::get() + 0xebece, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xebece, { 0x0f, 0x84, 0xd1, 0x02, 0x00, 0x00 });
		}

		if (setting().onInstantMirror) {
			sequence_patch(geode::base::get() + 0xf0d36, { 0x00, 0x00, 0x00, 0x00 });
		}
		else {
			sequence_patch(geode::base::get() + 0xf0d36, { 0x00, 0x00, 0x00, 0x3f });
		}

		if (setting().onMaxParticles) {
			sequence_patch(geode::base::getCocos() + 0xb64d7, { 0x8b, 0x7d, 0x07 });
		}
		else {
			sequence_patch(geode::base::getCocos() + 0xb64d7, { 0x8b, 0x7d, 0x08 });
		}

		if (setting().onNoAnimations) {
			sequence_patch(geode::base::getCocos() + 0x886be, { 0xc0 });
			sequence_patch(geode::base::getCocos() + 0x88da0, { 0xc0 });
		}
		else {
			sequence_patch(geode::base::getCocos() + 0x886be, { 0xc1 });
			sequence_patch(geode::base::getCocos() + 0x88da0, { 0xc1 });
		}

		if (setting().onNoBackgroundFlash) {
			sequence_patch(geode::base::get() + 0xe6c92, { 0x6a, 0x00 });
			sequence_patch(geode::base::get() + 0xe6ca8, { 0xc7, 0x04, 0x24, 0x00, 0x00, 0x00, 0x00 });
		}
		else {
			sequence_patch(geode::base::get() + 0xe6c92, { 0x6a, 0x01 });
			sequence_patch(geode::base::get() + 0xe6ca8, { 0xc7, 0x04, 0x24, 0x8f, 0xc2, 0xf5, 0x3d });
		}

		if (setting().onNoCompletionEffect) {
			sequence_patch(geode::base::get() + 0xe5667, { 0xeb, 0x1a });
		}
		else {
			sequence_patch(geode::base::get() + 0xe5667, { 0x75, 0x1a });
		}

		if (setting().onNoDeathEffect) {
			sequence_patch(geode::base::get() + 0xdde71, { 0xe9, 0xe7, 0x01, 0x00, 0x00, 0x90 });
			sequence_patch(geode::base::get() + 0xf05dc, { 0xeb, 0x1b });
		}
		else {
			sequence_patch(geode::base::get() + 0xdde71, { 0x0f, 0x84, 0xd5, 0x01, 0x00, 0x00 });
			sequence_patch(geode::base::get() + 0xf05dc, { 0x74, 0x1b });
		}

		if (setting().onNoEndShake) {
			sequence_patch(geode::base::get() + 0xe5f46, { 0x00, 0x00, 0x00, 0x00 });
		}
		else {
			sequence_patch(geode::base::get() + 0xe5f46, { 0x00, 0x00, 0x40, 0x40 });
		}

		if (setting().onNoGhostTrail) {
			sequence_patch(geode::base::get() + 0xf3374, { 0x6a, 0x00, 0x90 });
			sequence_patch(geode::base::get() + 0xf338d, { 0x6a, 0x00, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xf3374, { 0xff, 0x75, 0x08 });
			sequence_patch(geode::base::get() + 0xf338d, { 0xff, 0x75, 0x08 });
		}

		if (setting().onNoObjectGlow) {
			sequence_patch(geode::base::get() + 0x6d8a3, { 0xe9, 0x8f, 0x01, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0x6d8a3, { 0x0f, 0x85, 0x8e, 0x01, 0x00, 0x00 });
		}

		if (setting().onNoGravityEffect) {
			sequence_patch(geode::base::get() + 0xf4586, { 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xf4586, { 0x75, 0x07 });
		}

		if (setting().onNoMirror) {
			sequence_patch(geode::base::get() + 0xf0bf2, { 0xe9, 0x9b, 0x01, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xf0bf2, { 0x0f, 0x84, 0x9a, 0x01, 0x00, 0x00 });
		}

		if (setting().onNoPortalLightning) {
			sequence_patch(geode::base::get() + 0xe6c19, { 0xeb, 0x71 });
		}
		else {
			sequence_patch(geode::base::get() + 0xe6c19, { 0x75, 0x71 });
		}

		if (setting().onNoRespawnFlash) {
			sequence_patch(geode::base::get() + 0xdd62f, { 0xe9, 0x99, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xdd62f, { 0x0f, 0x85, 0x98, 0x00, 0x00, 0x00 });
		}

		if (setting().onNoShadeEffect) {
			sequence_patch(geode::base::get() + 0xebfe3, { 0xeb });
		}
		else {
			sequence_patch(geode::base::get() + 0xebfe3, { 0x77 });
		}

		if (setting().onPracticePulse) {
			sequence_patch(geode::base::get() + 0x29975, { 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0xeb441, { 0xeb, 0x16 });
		}
		else {
			sequence_patch(geode::base::get() + 0x29975, { 0x75, 0x0c });
			sequence_patch(geode::base::get() + 0xeb441, { 0x74, 0x16 });
		}

		if (setting().onSolidPlayerTrail) {
			sequence_patch(geode::base::getCocos() + 0xac3dc, { 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::getCocos() + 0xac3dc, { 0x89, 0x41, 0x10 });
		}

		if (setting().onSolidWaveTrail) {
			sequence_patch(geode::base::get() + 0xd9ade, { 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xd9ade, { 0x75, 0x0c });
		}

		if (setting().onTrailAlwaysOff) {
			sequence_patch(geode::base::getCocos() + 0xac6a6, { 0xe9, 0x56, 0x02, 0x00, 0x00, 0x90 });
		}
		else if (setting().onTrailAlwaysOn) {
			sequence_patch(geode::base::getCocos() + 0xac6a6, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else if (setting().onInversedTrail) {
			sequence_patch(geode::base::getCocos() + 0xac6a6, { 0x0f, 0x85, 0x55, 0x02, 0x00, 0x00 });
		}
		else {
			sequence_patch(geode::base::getCocos() + 0xac6a6, { 0x0f, 0x84, 0x55, 0x02, 0x00, 0x00 });
		}

		if (setting().onTrailBugFix) {
			sequence_patch(geode::base::getCocos() + 0xac29d, { 0xbb, 0xff, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(geode::base::getCocos() + 0xac29d, { 0xf3, 0x0f, 0x2c, 0xc1, 0x2b, 0xd8 });
		}

		if (setting().onWaveTrailOnDeath) {
			sequence_patch(geode::base::get() + 0xdddfa, { 0xeb, 0x0f });
		}
		else {
			sequence_patch(geode::base::get() + 0xdddfa, { 0x74, 0x0f });
		}

		// Creator

		if (setting().onAbsolutePosition) {
			sequence_patch(geode::base::get() + 0x4b49d, { 0x90, 0x8b, 0xcf, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0x4b49d, { 0x51, 0x8b, 0xcf, 0xff, 0x50, 0x5c });
		}

		if (setting().onCopyHack) {
			sequence_patch(geode::base::get() + 0x9c7ed, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0x9dfe5, { 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0x9c80f, { 0x8b, 0xcf, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0x9c7ed, { 0x0f, 0x84, 0x2e, 0x01, 0x00, 0x00 });
			sequence_patch(geode::base::get() + 0x9dfe5, { 0x75, 0x0e });
			sequence_patch(geode::base::get() + 0x9c80f, { 0x0f, 0x44, 0xcf });
		}

		if (setting().onDefaultSongBypass) {
			sequence_patch(geode::base::get() + 0x9a37f, { 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0x9a391, { 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0x9a3a0, { 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0x9a3fe, { 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0x9a410, { 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0x9a41f, { 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0x9a37f, { 0x74, 0x4e });
			sequence_patch(geode::base::get() + 0x9a391, { 0x0f, 0x4f, 0xf0 });
			sequence_patch(geode::base::get() + 0x9a3a0, { 0x0f, 0x48, 0xf1 });
			sequence_patch(geode::base::get() + 0x9a3fe, { 0x74, 0x4e });
			sequence_patch(geode::base::get() + 0x9a410, { 0x0f, 0x4f, 0xf0 });
			sequence_patch(geode::base::get() + 0x9a41f, { 0x0f, 0x48, 0xf1 });
		}

		if (setting().onEditorExtension) {
			sequence_patch(geode::base::get() + 0x14a74c, { 0x00, 0x60, 0xea, 0x4a }); // 60000 to 7680000

			sequence_patch(geode::base::get() + 0x4b513, { 0xeb }); // EditorUI::getLimitedPosition
			sequence_patch(geode::base::get() + 0x4b525, { 0xeb });
			sequence_patch(geode::base::get() + 0x4b53e, { 0xeb });
			sequence_patch(geode::base::get() + 0x4b562, { 0xeb });

			sequence_patch(geode::base::get() + 0x4b445, { 0xeb }); // EditorUI::moveObject
			sequence_patch(geode::base::get() + 0x4b457, { 0xeb });
			sequence_patch(geode::base::get() + 0x4b470, { 0xeb });
			sequence_patch(geode::base::get() + 0x4b482, { 0xeb });

			sequence_patch(geode::base::get() + 0x4779c, { 0xeb }); // EditorUI::onCreateObject
			sequence_patch(geode::base::get() + 0x477b9, { 0xeb });
			sequence_patch(geode::base::get() + 0x477d2, { 0xeb });
			sequence_patch(geode::base::get() + 0x477e8, { 0xeb });

			sequence_patch(geode::base::get() + 0x93861, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x9399b, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x939d3, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x93aed, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x93c72, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x94073, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x94112, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x94667, { 0x24, 0xa3, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x946f3, { 0x24, 0xa3, 0x54, 0x00 });
		}
		else {
			sequence_patch(geode::base::get() + 0x14a74c, { 0x00, 0x60, 0x6a, 0x47 });

			sequence_patch(geode::base::get() + 0x4b513, { 0x76 });
			sequence_patch(geode::base::get() + 0x4b525, { 0x76 });
			sequence_patch(geode::base::get() + 0x4b53e, { 0x76 });
			sequence_patch(geode::base::get() + 0x4b562, { 0x76 });

			sequence_patch(geode::base::get() + 0x4b445, { 0x76 });
			sequence_patch(geode::base::get() + 0x4b457, { 0x76 });
			sequence_patch(geode::base::get() + 0x4b470, { 0x76 });
			sequence_patch(geode::base::get() + 0x4b482, { 0x76 });

			sequence_patch(geode::base::get() + 0x4779c, { 0x77 });
			sequence_patch(geode::base::get() + 0x477b9, { 0x77 });
			sequence_patch(geode::base::get() + 0x477d2, { 0x76 });
			sequence_patch(geode::base::get() + 0x477e8, { 0x76 });

			sequence_patch(geode::base::get() + 0x93861, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x9399b, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x939d3, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x93aed, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x93c72, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x94073, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x94112, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x94667, { 0xb8, 0xa6, 0x54, 0x00 });
			sequence_patch(geode::base::get() + 0x946f3, { 0xb8, 0xa6, 0x54, 0x00 });
		}

		if (setting().onFreeScroll) {
			sequence_patch(geode::base::get() + 0x4ca45, { 0xeb });
			sequence_patch(geode::base::get() + 0x4ca5c, { 0xeb });
			sequence_patch(geode::base::get() + 0x4ca75, { 0xeb });
			sequence_patch(geode::base::get() + 0x4ca8c, { 0xeb });
		}
		else {
			sequence_patch(geode::base::get() + 0x4ca45, { 0x77 });
			sequence_patch(geode::base::get() + 0x4ca5c, { 0x77 });
			sequence_patch(geode::base::get() + 0x4ca75, { 0x77 });
			sequence_patch(geode::base::get() + 0x4ca8c, { 0x77 });
		}

		if (setting().onHideGrid) {
			sequence_patch(geode::base::get() + 0x939ba, { 0x00 });
			sequence_patch(geode::base::get() + 0x93b6e, { 0x00 });
		}
		else {
			sequence_patch(geode::base::get() + 0x939ba, { 0x96 });
			sequence_patch(geode::base::get() + 0x93b6e, { 0x96 });
		}

		if (setting().onHideTriggerLines) {
			sequence_patch(geode::base::get() + 0x93e08, { 0xe9, 0xce, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0x93e08, { 0x0f, 0x84, 0xcd, 0x00, 0x00, 0x00 });
		}

		if (setting().onLevelEdit) {
			sequence_patch(geode::base::get() + 0xd62ef, { 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xd62ef, { 0x75, 0x62 });
		}

		if (setting().onNoCopyMark) {
			sequence_patch(geode::base::get() + 0x54aa0, { 0xb8, 0x00, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0x54aa0, { 0x8b, 0x81, 0x04, 0x02, 0x00, 0x00 });
		}

		if (setting().onNoDeathX) {
			sequence_patch(geode::base::get() + 0x91254, { 0x00 });
		}
		else {
			sequence_patch(geode::base::get() + 0x91254, { 0x01 });
		}

		if (setting().onNoEditorTrail) {
			sequence_patch(geode::base::get() + 0x94305, { 0x00 });
			sequence_patch(geode::base::get() + 0x9442c, { 0x00 });		
		}
		else {
			sequence_patch(geode::base::get() + 0x94305, { 0xff });
			sequence_patch(geode::base::get() + 0x9442c, { 0xff });
		}

		if (setting().onObjectBypass) {
			sequence_patch(geode::base::get() + 0x3e30f, { 0xff, 0xff, 0xff, 0x7f });
			sequence_patch(geode::base::get() + 0x476b3, { 0xff, 0xff, 0xff, 0x7f });
			sequence_patch(geode::base::get() + 0x48ed6, { 0xff, 0xff, 0xff, 0x7f });
		}
		else {
			sequence_patch(geode::base::get() + 0x3e30f, { 0x20, 0x4e, 0x00, 0x00 });
			sequence_patch(geode::base::get() + 0x476b3, { 0x20, 0x4e, 0x00, 0x00 });
			sequence_patch(geode::base::get() + 0x48ed6, { 0x20, 0x4e, 0x00, 0x00 });
		}

		if (setting().onPlaceOver) {
			sequence_patch(geode::base::get() + 0x8d37f, { 0xeb });
		}
		else {
			sequence_patch(geode::base::get() + 0x8d37f, { 0x77 });
		}

		if (setting().onRotationHack) {
			sequence_patch(geode::base::get() + 0x49a5d, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90 });
			sequence_patch(geode::base::get() + 0x49a92, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90 });
			sequence_patch(geode::base::get() + 0x74751, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90 });
			sequence_patch(geode::base::get() + 0xe84c5, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0x49a5d, { 0x8b, 0x80, 0x68, 0x02, 0x00, 0x00 });
			sequence_patch(geode::base::get() + 0x49a92, { 0x8b, 0x80, 0x68, 0x02, 0x00, 0x00 });
			sequence_patch(geode::base::get() + 0x74751, { 0x8b, 0x83, 0x68, 0x02, 0x00, 0x00 });
			sequence_patch(geode::base::get() + 0xe84c5, { 0x8b, 0x83, 0x68, 0x02, 0x00, 0x00 });
		}

		if (setting().onSmoothEditorTrail) {
			sequence_patch(geode::base::get() + 0x91a34, { 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0x91a34, { 0x72, 0x79 });
		}

		if (setting().onVerifyHack) {
			sequence_patch(geode::base::get() + 0x3d760, { 0xeb });
		}
		else {
			sequence_patch(geode::base::get() + 0x3d760, { 0x75 });
		}

		if (setting().onZoomBypass) {
			sequence_patch(geode::base::get() + 0x48bb5, { 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0x48bba, { 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0x48c15, { 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0x48c1a, { 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0x48bb5, { 0x0f, 0x2f, 0xc8 });
			sequence_patch(geode::base::get() + 0x48bba, { 0x0f, 0x28, 0xc8 });
			sequence_patch(geode::base::get() + 0x48c15, { 0x0f, 0x2f, 0xc8 });
			sequence_patch(geode::base::get() + 0x48c1a, { 0x0f, 0x28, 0xc8 });
		}

		// Level

		if (setting().onConfirmExit) {
			sequence_patch(geode::base::get() + 0xd7f80, { 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0xd7f8d, { 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0xd7f9d, { 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0xd7fa5, { 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xd7f80, { 0xe8, 0xfb, 0xbb, 0x01, 0x00 });
			sequence_patch(geode::base::get() + 0xd7f8d, { 0x6a, 0x10 });
			sequence_patch(geode::base::get() + 0xd7f9d, { 0x68, 0x7c, 0x42, 0x51, 0x00 });
			sequence_patch(geode::base::get() + 0xd7fa5, { 0xe8, 0xc6, 0xd7, 0xf2, 0xff });
		}

		if (setting().onCorrectiveMusicSync) {
			sequence_patch(geode::base::get() + 0xee59e, { 0xeb });
		}
		else {
			sequence_patch(geode::base::get() + 0xee59e, { 0x75 });
		}

		if (setting().onEverythingHurts) {
			sequence_patch(geode::base::get() + 0xeaa42, { 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xeaa42, { 0x75, 0x0b });
		}

		if (setting().onEverythingPulses) {
			sequence_patch(geode::base::get() + 0x52af0, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0x52af0, { 0x8a, 0x81, 0x95, 0x02, 0x00, 0x00 });
		}

		if (setting().onFreezePlayer) {
			sequence_patch(geode::base::get() + 0xe9dd3, { 0xe9, 0x3f, 0x01, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xe9dd3, { 0x0f, 0x85, 0x3e, 0x01, 0x00, 0x00 });
		}

		if (setting().onHighFPSRotationFix) {
			sequence_patch(geode::base::get() + 0xdc13b, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xdc13b, { 0x0f, 0x82, 0xd6, 0x00, 0x00, 0x00 });
		}

		if (setting().onInstantComplete) {
			sequence_patch(geode::base::get() + 0xe16f6, { 0xc7, 0x87, 0x74, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xe16f6, { 0xf3, 0x0f, 0x11, 0x8f, 0x74, 0x04, 0x00, 0x00, 0x9f, 0xf6, 0xc4, 0x44 });
		}

		if (setting().onJumpHack) {
			sequence_patch(geode::base::get() + 0xda510, { 0x01 });
			sequence_patch(geode::base::get() + 0xda295, { 0x01 });
		}
		else {
			sequence_patch(geode::base::get() + 0xda510, { 0x00 });
			sequence_patch(geode::base::get() + 0xda295, { 0x00 });
		}

		if (setting().onNoclip) {
			sequence_patch(geode::base::get() + 0xf04e9, { 0xe9, 0xf0, 0x02, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xf04e9, { 0x0f, 0x85, 0xef, 0x02, 0x00, 0x00 });
		}

		if (setting().onNoclipUnstuck) {
			sequence_patch(geode::base::get() + 0xdae16, { 0xe9, 0x00, 0x02, 0x00, 0x00, 0x90 });
			sequence_patch(geode::base::get() + 0xdb02d, { 0xe9, 0x10, 0x01, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xdae16, { 0x0f, 0x84, 0xff, 0x01, 0x00, 0x00 });
			sequence_patch(geode::base::get() + 0xdb02d, { 0x0f, 0x8b, 0x0f, 0x01, 0x00, 0x00 });
		}

		if (setting().onPauseDuringCompletion) {
			sequence_patch(geode::base::get() + 0xf38cf, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xf38cf, { 0x0f, 0x85, 0xc7, 0x00, 0x00, 0x00 });
		}

		if (setting().onPracticeMusic) {
			sequence_patch(geode::base::get() + 0xf284f, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0xf3663, { 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0xf0699, { 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0xf06cb, { 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0xf3691, { 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0xf3943, { 0xeb });
			sequence_patch(geode::base::get() + 0xf3a96, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xf284f, { 0x0f, 0x85, 0x4d, 0x07, 0x00, 0x00 });
			sequence_patch(geode::base::get() + 0xf3663, { 0x75, 0x41 });
			sequence_patch(geode::base::get() + 0xf0699, { 0x75, 0x3e });
			sequence_patch(geode::base::get() + 0xf06cb, { 0x75, 0x0c });
			sequence_patch(geode::base::get() + 0xf3691, { 0xe8, 0xaa, 0x42, 0xf2, 0xff });
			sequence_patch(geode::base::get() + 0xf3943, { 0x74 });
			sequence_patch(geode::base::get() + 0xf3a96, { 0x0f, 0x85, 0xb5, 0x00, 0x00, 0x00 });
		}

		if (setting().onWaveSlide) {
			sequence_patch(geode::base::get() + 0xdba98, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0xdc75a, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xdba98, { 0x0f, 0x85, 0xe0, 0x02, 0x00, 0x00 });
			sequence_patch(geode::base::get() + 0xdc75a, { 0x0f, 0x85, 0x91, 0x03, 0x00, 0x00 });
		}

		// Universal

		if (setting().onAllowLowVolume) {
			sequence_patch(geode::base::get() + 0xd772e, { 0xeb });
			sequence_patch(geode::base::get() + 0xd0e0e, { 0xeb });
			sequence_patch(geode::base::get() + 0xd0cb0, { 0xeb });
		}
		else {
			sequence_patch(geode::base::get() + 0xd772e, { 0x76 });
			sequence_patch(geode::base::get() + 0xd0e0e, { 0x76 });
			sequence_patch(geode::base::get() + 0xd0cb0, { 0x76 });
		}

		if (setting().onDisableSongAlert) {
			sequence_patch(geode::base::get() + 0x9dd2b, { 0xeb });
		}
		else {
			sequence_patch(geode::base::get() + 0x9dd2b, { 0x75 });
		}

		if (setting().onFastAltTab) {
			sequence_patch(geode::base::get() + 0x28dfe, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::get() + 0x28f2e, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0x28dfe, { 0x8b, 0x03, 0x8b, 0xcb, 0xff, 0x50, 0x18 });
			sequence_patch(geode::base::get() + 0x28f2e, { 0x8b, 0xc8, 0x8b, 0x10, 0xff, 0x52, 0x2c });
		}

		if (setting().onForceVisibility) {
			sequence_patch(geode::base::getCocos() + 0x60783, { 0xb0, 0x01, 0x90 });
			sequence_patch(geode::base::getCocos() + 0x60c9a, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::getCocos() + 0x60783, { 0x8a, 0x45, 0x08 });
			sequence_patch(geode::base::getCocos() + 0x60c9a, { 0x0f, 0x84, 0xcb, 0x00, 0x00, 0x00 });
		}

		if (setting().onFreeWindowResize) {
			sequence_patch(geode::base::getCocos() + 0x10f48b, { 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch(geode::base::getCocos() + 0x10ee81, { 0xe9, 0x2f, 0xff, 0xff, 0xff, 0x90 });
			sequence_patch(geode::base::getCocos() + 0x10e143, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::getCocos() + 0x10f48b, { 0xe8, 0xb0, 0xf3, 0xff, 0xff });
			sequence_patch(geode::base::getCocos() + 0x10ee81, { 0x0f, 0x85, 0x2e, 0xff, 0xff, 0xff });
		}

		if (setting().onIncreaseMaxLevels) {
			sequence_patch(geode::base::get() + 0x5875b, { 0x64 });
		}
		else {
			sequence_patch(geode::base::get() + 0x5875b, { 0x14 });
		}

		if (setting().onNoRotation) {
			sequence_patch(geode::base::getCocos() + 0x60578, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::getCocos() + 0x60578, { 0xf3, 0x0f, 0x11, 0x41, 0x1c, 0xf3, 0x0f, 0x11, 0x41, 0x18 });
		}

		if (setting().onQuickCheckpointMode) {
			sequence_patch(geode::base::get() + 0x14a6f4, { 0x00, 0x00, 0x70, 0x42 });
		}
		else {
			sequence_patch(geode::base::get() + 0x14a6f4, { 0x00, 0x00, 0xe1, 0x43 });
		}

		if (setting().onSafeMode) {
			setting().isSafeMode = true;
			safeModeON();
		}
		else {
			setting().isSafeMode = false;
			safeModeOFF();
		}

		if (setting().onShowRestartButton) {
			sequence_patch(geode::base::get() + 0xd64d9, { 0x90, 0x90 });
		}
		else {
			sequence_patch(geode::base::get() + 0xd64d9, { 0x75, 0x29 });
		}

		if (setting().onTransparentBG) {
			sequence_patch(geode::base::get() + 0x2cf96, { 0x90, 0xb1, 0xff }); // CreatorLayer
			sequence_patch(geode::base::get() + 0x2cf9e, { 0xff, 0xff });
			sequence_patch(geode::base::get() + 0x3b7db, { 0x90, 0xb1, 0xff }); // LevelBrowserLayer
			sequence_patch(geode::base::get() + 0x3b7e3, { 0xff, 0xff });
			sequence_patch(geode::base::get() + 0x88132, { 0x90, 0xb1, 0xff }); // LevelSearchLayer
			sequence_patch(geode::base::get() + 0x8813a, { 0xff, 0xff });
			sequence_patch(geode::base::get() + 0x8969e, { 0x90, 0xb1, 0xff }); // LevelInfoLayer
			sequence_patch(geode::base::get() + 0x896a5, { 0xff, 0xff });
			sequence_patch(geode::base::get() + 0x9bde1, { 0x90, 0xb1, 0xff }); // EditLevelLayer
			sequence_patch(geode::base::get() + 0x9bde9, { 0xff, 0xff });
			sequence_patch(geode::base::get() + 0x9f97c, { 0x90, 0xb1, 0xff }); // LeaderboardsLayer
			sequence_patch(geode::base::get() + 0x9f984, { 0xff, 0xff });
		}
		else {
			sequence_patch(geode::base::get() + 0x2cf96, { 0x80, 0xc9, 0xff }); // CreatorLayer
			sequence_patch(geode::base::get() + 0x2cf9e, { 0x00, 0x66 });
			sequence_patch(geode::base::get() + 0x3b7db, { 0x80, 0xc9, 0xff }); // LevelBrowserLayer
			sequence_patch(geode::base::get() + 0x3b7e3, { 0x00, 0x66 });
			sequence_patch(geode::base::get() + 0x88132, { 0x80, 0xc9, 0xff }); // LevelSearchLayer
			sequence_patch(geode::base::get() + 0x8813a, { 0x00, 0x66 });
			sequence_patch(geode::base::get() + 0x8969e, { 0x80, 0xc9, 0xff }); // LevelInfoLayer
			sequence_patch(geode::base::get() + 0x896a5, { 0x00, 0x66 });
			sequence_patch(geode::base::get() + 0x9bde1, { 0x80, 0xc9, 0xff }); // EditLevelLayer
			sequence_patch(geode::base::get() + 0x9bde9, { 0x00, 0x66 });
			sequence_patch(geode::base::get() + 0x9f97c, { 0x80, 0xc9, 0xff }); // LeaderboardsLayer
			sequence_patch(geode::base::get() + 0x9f984, { 0x00, 0x66 });
		}

		if (setting().onTransparentLists) {
			sequence_patch((uint32_t)geode::base::get() + 0x31c7f, { 0x00, 0x00 }); // LevelCell::updateBGColor
			sequence_patch((uint32_t)geode::base::get() + 0x31c82, { 0x00 });
			sequence_patch((uint32_t)geode::base::get() + 0x31c89, { 0x00, 0x00 });
			sequence_patch((uint32_t)geode::base::get() + 0x31c8c, { 0x00 });
			sequence_patch((uint32_t)geode::base::get() + 0x31cba, { 0x00 });

			sequence_patch((uint32_t)geode::base::get() + 0x88a4f, { 0x00, 0x00, 0x00, 0x00 }); // LeaderboardsLayer::setupLevelBrowser
			sequence_patch((uint32_t)geode::base::get() + 0x8a945, { 0x00, 0x00, 0x00, 0x00 }); // LevelBrowserLayer::setupLevelBrowser
		}
		else {
			sequence_patch((uint32_t)geode::base::get() + 0x31c7f, { 0xc2, 0x72 });
			sequence_patch((uint32_t)geode::base::get() + 0x31c82, { 0x3e });
			sequence_patch((uint32_t)geode::base::get() + 0x31c89, { 0xa1, 0x58 });
			sequence_patch((uint32_t)geode::base::get() + 0x31c8c, { 0x2c });
			sequence_patch((uint32_t)geode::base::get() + 0x31cba, { 0xff });

			sequence_patch((uint32_t)geode::base::get() + 0x88a4f, { 0xbf, 0x72, 0x3e, 0xff });
			sequence_patch((uint32_t)geode::base::get() + 0x8a945, { 0xbf, 0x72, 0x3e, 0xff });
		}

        oneX = false;
    }

    if (setting().show) {
        ImGui::SetNextWindowSize(ImVec2(200.f, 0.f));
        if (ImGui::Begin("PolzHax", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
			ImGui::Text("1.920 - v1.3.0 (Geode 060526)");

			ImGui::CheckboxF("Auto Save", &setting().onAutoSave);
			ImGui::SameLine(0.f, 0.f);
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.f + (ImGui::GetStyle().WindowPadding.x / 4.f));
			if (ImGui::Button("Save", ImVec2(SHORT_ITEM_WIDTH, 0))) {
				setting().save();
				FLAlertLayer::create("Saved", "Hack state is saved.", "OK")->show();
			}

			ImGui::HotKey("Alt Hotkey", setting().m_openMenuKey, 0.f, ImVec2(SHORT_ITEM_WIDTH, 0));

			if (ImGui::CheckboxF("Thread Priority", &setting().onThreadPriority)) {
				if (setting().onThreadPriority) {
					updatePriority();
				}
				else {
					SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
				}
			}
			ImGui::SameLine(170.f);
			if (ImGui::TreeNodeEx("##threadPrioritySettings", ImGuiTreeNodeFlags_SpanAvailWidth)) {
				ImGui::SetNextItemWidth(163.f);
				if (ImGui::Combo("##priorities", &setting().priority, priorities, IM_ARRAYSIZE(priorities))) {
					if (setting().onThreadPriority) {
						updatePriority();
					}
				}
				ImGui::TreePop();
			}

            filter.Draw("Search", 135.f);

			if (setting().onDeveloperMode) {
				if (ImGui::Button("Cocos Explorer", ImVec2(LONG_ITEM_WIDTH, 0.f))) {
					setting().onCocosExplorer = !setting().onCocosExplorer;
				}

				if (ImGui::Button("GDPS Switcher", ImVec2(LONG_ITEM_WIDTH, 0.f))) {
					setting().onGDPSSwitcher = !setting().onGDPSSwitcher;
				}
			}

			if (GetAsyncKeyState(0x31) && GetAsyncKeyState(0x37) && GetAsyncKeyState(0x30) && GetAsyncKeyState(0x33)) { // Don't say anything about this
				ImGui::CheckboxF("Developer Mode", &setting().onDeveloperMode);
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

			//ImGui::SetNextItemWidth(135.f);
			//if (ImGui::DragFloat("UI Size", &setting().UISize, .1f, .5f, 3.f, "%.1f")) {
			//	//updateUISize();
			//	//sortTabs();
			//}

			if (ImGui::Button("Sort Tabs", ImVec2(LONG_ITEM_WIDTH, 0.f))) {
				sortTabs();
			}
		}

		ImGui::SetNextWindowSize(ImVec2(200.f, 0.f));
		if (ImGui::Begin("Bypass", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
			ImGui::CheckboxF("Character Filter", &setting().onCharacterFilter);
			ImGui::Tooltip("Lets you input any character in text fields.");

			if (ImGui::CheckboxF("Icons", &setting().onIcons)) {
				if (setting().onIcons) {
					sequence_patch(geode::base::get() + 0x66e26, { 0x90, 0x90 });
					sequence_patch(geode::base::get() + 0x66b4a, { 0x90, 0x90 });
					sequence_patch(geode::base::get() + 0x66b56, { 0x90, 0x90 });
				}
				else {
					sequence_patch(geode::base::get() + 0x66e26, { 0x7d, 0x07 });
					sequence_patch(geode::base::get() + 0x66b4a, { 0x7d, 0x13 });
					sequence_patch(geode::base::get() + 0x66b56, { 0x7d, 0x07 });
				}
			}
			ImGui::Tooltip("Unlocks all icons and colors.");

			if (ImGui::CheckboxF("Main Levels", &setting().onMainLevels)) {
				if (setting().onMainLevels) {
					sequence_patch(geode::base::get() + 0xa81af, { 0xe9, 0xa3, 0x02, 0x00, 0x00, 0x90 });
					sequence_patch(geode::base::get() + 0xa8c22, { 0xe9, 0x98, 0x00, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch(geode::base::get() + 0xa81af, { 0x0f, 0x84, 0xa2, 0x02, 0x00, 0x00 });
					sequence_patch(geode::base::get() + 0xa8c22, { 0x0f, 0x8e, 0x97, 0x00, 0x00, 0x00 });
				}
			}
			ImGui::Tooltip("Unlocks main demon levels.");

			if (ImGui::CheckboxF("Slider Limit", &setting().onSliderLimit)) {
				if (setting().onSliderLimit) {
					sequence_patch(geode::base::get() + 0x1cf27, { 0xeb, 0x07 });
					sequence_patch(geode::base::get() + 0x1cf3b, { 0xeb, 0x07 });
				}
				else {
					sequence_patch(geode::base::get() + 0x1cf27, { 0x76, 0x07 });
					sequence_patch(geode::base::get() + 0x1cf3b, { 0x76, 0x07 });
				}
			}
			ImGui::Tooltip("Lets sliders be dragged beyond the visible limit.");

			ImGui::CheckboxF("Text Length", &setting().onTextLength);
			ImGui::Tooltip("Allows for unlimited text length in text inputs.");
		}

		ImGui::SetNextWindowSize(ImVec2(200.f, 0.f));
		if (ImGui::Begin("Utility", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
			ImGui::HotKey("", setting().m_p1click, 0.f, ImVec2(SHORT_ITEM_WIDTH, 0.f), "P1 Click");
			ImGui::SameLine(0.f, 0.f);
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.f + (ImGui::GetStyle().WindowPadding.x / 4.f));
			ImGui::HotKey("", setting().m_p2click, 0.f, ImVec2(SHORT_ITEM_WIDTH, 0.f), "P2 Click");

			if (ImGui::Button("Uncomplete Level", ImVec2(LONG_ITEM_WIDTH, 0.f))) {
				if (playLayer) {
					auto gsm = GameStatsManager::sharedState();
					auto glm = GameLevelManager::sharedState();

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

					FLAlertLayer::create("Success", "Save & Load your data to apply the changes.", "OK")->show();
				}
				else {
					FLAlertLayer::create("Error", "You need to enter the level first.", "OK")->show();
				}
			}

			if (ImGui::Button("Restart Level", ImVec2(LONG_ITEM_WIDTH, 0))) {
				if (playLayer) playLayer->resetLevel();
			}

			if (ImGui::Button("Practice Mode", ImVec2(LONG_ITEM_WIDTH, 0))) {
				if (playLayer) playLayer->togglePracticeMode(!playLayer->m_practiceMode);
			}

			if (ImGui::Button("Settings", ImVec2(LONG_ITEM_WIDTH, 0))) {
				//CCARRAY_FOREACH_B_TYPE(CCDirector::sharedDirector()->getRunningScene()->getChildren(), optionsLayer, gd::OptionsLayer) {
				//	if (optionsLayer->getTag() == 0x725) {
				//		
				//	}
				//	else {
						auto optionsLayer = OptionsLayer::create();
						optionsLayer->showLayer(false);
						CCDirector::sharedDirector()->getRunningScene()->addChild(optionsLayer, CCDirector::sharedDirector()->getRunningScene()->getHighestChildZ() + 1, 0x725);
				//	}
				//}
			}

			static bool showFirstBtn = true;
			static bool showSecondBtn = false;
			if (showFirstBtn) {
				if (ImGui::Button("Refresh Textures", ImVec2(LONG_ITEM_WIDTH, 0))) {
					showFirstBtn = false;
					showSecondBtn = true;
				}
			}
			if (showSecondBtn) {
				if (ImGui::Button("Cancel", ImVec2(SHORT_ITEM_WIDTH, 0))) {
					showFirstBtn = true;
					showSecondBtn = false;
				}
				ImGui::SameLine(0.f, 0.f);
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.f + (ImGui::GetStyle().WindowPadding.x / 4.f));
				if (ImGui::Button("Confirm", ImVec2(SHORT_ITEM_WIDTH, 0))) {
					showFirstBtn = true;
					showSecondBtn = false;
					GameManager::sharedState()->reloadAll(false, false, true);
				}
			}

			if (ImGui::Button("Resources", ImVec2(SHORT_ITEM_WIDTH, 0))) {
				geode::utils::file::openFolder(geode::dirs::getResourcesDir());
			}
			ImGui::SameLine(0.f, 0.f);
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.f + (ImGui::GetStyle().WindowPadding.x / 4.f));
			if (ImGui::Button("AppData", ImVec2(SHORT_ITEM_WIDTH, 0))) {
				geode::utils::file::openFolder(geode::dirs::getSaveDir());
			}
		}
    }
}

void imgui_init() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    auto font = ImGui::GetIO().Fonts->AddFontFromFileTTF((Mod::get()->getResourcesDir() / "Muli-SemiBold.ttf").string().c_str(), 16.f);
    io.Fonts->Build();
    io.FontDefault = font;

    ImGui::GetStyle().WindowTitleAlign = ImVec2(.5f, .5f);
    ImGui::GetStyle().WindowBorderSize = 0;

	colorSet();

	PolzHax::updateFPSBypass();
}

void setupImGuiMenu() {
	ImGuiHook::setToggleCallback([]() { setting().show = !setting().show; });
	ImGuiHook::setRenderFunction(imgui_render);
	ImGuiHook::setInitFunction(imgui_init);
}