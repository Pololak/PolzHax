#include "pch.h"
#include "menu.h"
//
#include <imgui-hook.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <cpp/imgui_stdlib.h>
#include <filesystem>
#include <fstream>
#include <chrono>
#include "state.h"
#include "explorer.h"
#include "utils.hpp"
#include "managerViewer.h"
#include "SpeedHack.h"
#include <shellapi.h>
#include "PlayLayer.h"
#include "portable-file-dialogs.h"
#include "ImGuiUtils.h"

DWORD libcocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");

ImVec4 color1;
ImVec4 color2;
ImVec4 color3;
ImVec4 color4;
ImVec4 color5;
ImVec4 color6;

void update_speed_hack() {
	const auto value = setting().onSpeedhack ? setting().speedhack : 1.f;
	if (auto fme = gd::FMODAudioEngine::sharedEngine())
		if (auto sound = fme->currentSound())
			if (setting().onSpeedhackMusic) sound->setPitch(value);
			else sound->setPitch(1.f);
	if (setting().speedhack == 0)
		return;
	if (!setting().onClassicMode) {
		CCDirector::sharedDirector()->m_pScheduler->setTimeScale(value);
		SpeedHack::SetSpeed(1.f);
	}
	else {
		CCDirector::sharedDirector()->m_pScheduler->setTimeScale(1.f);
		SpeedHack::SetSpeed(value);
	}
}

// Pitch Shifter is currently unused.

void update_fps_bypass() {
	const auto value = setting().onFPSBypass ? setting().fps : 60.f;
	static const auto addr = cocos_symbol<&CCApplication::setAnimationInterval>("?setAnimationInterval@CCApplication@cocos2d@@UAEXN@Z");
	addr(CCApplication::sharedApplication(), 1.0 / value);
	CCDirector::sharedDirector()->setAnimationInterval(1.0 / value);
}

void cheatDec()
{
	setting().cheatsCount--;
}

void cheatAdd()
{
	setting().cheatsCount++;
	setting().beforeRestartCheatsCount++;
}

void updatePriority(int priority) {
	switch (priority)
	{
	case 4:
		SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
		break;
	case 3:
		SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
		break;
	case 2:
		SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
		break;
	case 1:
		SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
		break;
	case 0:
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
		break;
	}
}

static std::string getSongFolder() {
	return CCFileUtils::sharedFileUtils()->getWritablePath();
}

static std::string getResourcesFolder() {
	return CCFileUtils::sharedFileUtils()->getWritablePath2() + "/Resources";
}

std::vector<std::string> dllNames;

bool oneX = true;

void RenderMain() {
	if (oneX) {
		float addingX1, addingX2, addingX3, addingX4, addingX5, addingX6, addingX7;
		float addingInterfaceY, addingSpeedhackY, addingIconsY, addingShortcutsY;
		if (ImGui::Begin("PolzHax", nullptr)) {
			ImGui::SetWindowPos({ 5 , 5 });
			addingX1 = ImGui::GetWindowWidth() + 10;
			addingInterfaceY = ImGui::GetWindowHeight() + 10;
		}

		if (ImGui::Begin("Interface", nullptr)) {
			ImGui::SetWindowPos({ 5, addingInterfaceY });
		}

		if (ImGui::Begin("Bypass", nullptr)) {
			ImGui::SetWindowPos({ addingX1 , 5 });
			addingX2 = addingX1 + ImGui::GetWindowWidth() + 5;
		}

		if (ImGui::Begin("Cosmetic", nullptr)) {
			ImGui::SetWindowPos({ addingX2, 5 });
			addingX3 = addingX2 + ImGui::GetWindowWidth() + 5;
		}

		if (ImGui::Begin("Creator", nullptr)) {
			ImGui::SetWindowPos({ addingX3, 5 });
			addingX4 = addingX3 + ImGui::GetWindowWidth() + 5;
		}

		if (ImGui::Begin("Level", nullptr)) {
			ImGui::SetWindowPos({ addingX4 , 5 });
			addingX5 = addingX4 + ImGui::GetWindowWidth() + 5;
			addingShortcutsY = ImGui::GetWindowHeight() + 10;
		}

		if (ImGui::Begin("Shortcuts", nullptr)) {
			ImGui::SetWindowPos({ addingX4, addingShortcutsY });
		}

		if (ImGui::Begin("Universal", nullptr)) {
			ImGui::SetWindowPos({ addingX5, 5 });
			addingX6 = addingX5 + ImGui::GetWindowWidth() + 5;
		}

		if (ImGui::Begin("Status", nullptr)) {
			ImGui::SetWindowPos({ addingX6, 5 });
			addingX7 = addingX6 + ImGui::GetWindowWidth() + 5;
			addingIconsY = ImGui::GetWindowHeight() + 10;
		}

		if (ImGui::Begin("Icons", nullptr)) {
			ImGui::SetWindowPos({ addingX6, addingIconsY });
		}

		if (ImGui::Begin("Display", nullptr)) {
			ImGui::SetWindowPos({ addingX7, 5 });
			addingSpeedhackY = ImGui::GetWindowHeight() + 10;
		}

		if (ImGui::Begin("Speedhack", nullptr)) {
			ImGui::SetWindowPos({ addingX7, addingSpeedhackY });
		}

		// Bypass

		if (setting().onCharFilter) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4147A7), "\x90\x90", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4147A7), "\x75\x04", 2, NULL);
		}

		if (setting().onIcons) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48000F), "\xe9\x74\x02\x00\x00\x90", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E606), "\xe9\xdc\x00\x00\x00\x90", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E098), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E128), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E1B8), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E248), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E2D8), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E9BE), "\xeb\x13", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47EAAE), "\xeb\x13", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48000F), "\x0f\x85\x73\x02\x00\x00", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E606), "\x0f\x85\xdb\x00\x00\x00", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E098), "\x74\x04", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E128), "\x74\x04", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E1B8), "\x74\x04", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E248), "\x74\x04", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E2D8), "\x74\x04", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E9BE), "\x75\x13", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47EAAE), "\x75\x13", 2, NULL);
		}

		if (setting().onMainLevels) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A81AF), "\xe9\xa3\x02\x00\x00\x90", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A8C22), "\xe9\x98\x00\x00\x00\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A81AF), "\x0f\x84\xa2\x02\x00\x00", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A8C22), "\x0f\x8e\x97\x00\x00\x00", 6, NULL);
		}

		if (setting().onSliderLimit) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cf27), "\xeb\x07", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cf3b), "\xeb\x07", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cf27), "\x76\x07", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cf3b), "\x76\x07", 2, NULL);
		}

		if (setting().onTextLength) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4147DE), "\xeb\x04", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4147DE), "\x7c\x04", 2, NULL);
		}

		// Cosmetic

		if (setting().onAlwaysNewBest) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xf077e), "\x90\x90", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xf077e), "\x74\x14", 2, NULL);
		}

		if (setting().onSCoinUncoll) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x443368), "\x8B\xC2\x90", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4735CE), "\x8B\xD9\x90", 3, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x443368), "\x0F\x44\xC2", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4735CE), "\x0F\x44\xD9", 3, NULL);
		}

		if (setting().onCoinsPractice) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4eb138), "\x90\x90", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4eb138), "\x75\x4c", 2, NULL);
		}

		if (setting().onDontFade) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebddb), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebe06), "\xeb\x11", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebddb), "\x74\x5d", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebe06), "\x74\x11", 2, NULL);
		}

		if (setting().onDontEnter) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EC51C), "\x90\x90", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EC51C), "\x8B\xD9", 2, NULL);
		}

		if (setting().onFastAnimComp) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5F99), "\x00\x00\x00\x00", 4, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5F99), "\x99\x99\xF9\x3F", 4, NULL);
		}

		if (setting().onFastShowComp) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5CA4), "\x00\x00\x00\x00", 4, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE57F6), "\x00\x00\x00\x00", 4, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE57CE), "\x90\x90\x90\x90\x90\x90\x90", 7, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE57DE), "\x90\x90\x90\x90\x90\x90\x90", 7, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5814), "\x90\x90\x90\x90\x90\x90\x90", 7, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5824), "\x90\x90\x90\x90\x90\x90\x90", 7, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5CA4), "\x00\x00\xc0\x3f", 4, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE57F6), "\xaf\x47\x61\x3f", 4, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE57CE), "\xC7\x04\x24\xC3\xF5\x28\x3F", 7, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE57DE), "\xC7\x04\x24\x9A\x99\x19\x3F", 7, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5814), "\xC7\x04\x24\xAF\x47\x61\x3E", 7, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5824), "\xC7\x04\x24\x00\x00\x00\x40", 7, NULL);
		}

		if (setting().onFObjectInvisible) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebece), "\x90\x90\x90\x90\x90\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebece), "\x0f\x84\xd1\x02\x00\x00", 6, NULL);
		}

		if (setting().onInstantMirror) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0D33), "\xc7\x04\x24\x00\x00\x00\x00", 7, NULL); // E9 9B 01 00 00 90
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0D33), "\xc7\x04\x24\x00\x00\x00\x3F", 7, NULL); // 0F 84 9A 01 00 00
		}

		if (setting().onMaxParticles) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xB64D7), "\x8b\x7d\x07", 3, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xB64D7), "\x8b\x7d\x08", 3, NULL);
		}

		if (setting().onMiniCube) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d8cf4), "\x31\xff\x90", 3, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d8cf4), "\x0f\x4f\xf9", 3, NULL);
		}

		if (setting().onNoAnimations) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x886be), "\xc0", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x88da0), "\xc0", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x886be), "\xc1", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x88da0), "\xc1", 1, NULL);
		}

		if (setting().onNoBGFlash) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e6c92), "\x6a\x00", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e6ca8), "\xc7\x04\x24\x00\x00\x00\x00", 7, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e6c92), "\x6a\x01", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e6ca8), "\xc7\x04\x24\x8f\xc2\xf5\x3d", 7, NULL);
		}

		if (setting().onNoDeathEffect) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dde71), "\xe9\xe7\x01\x00\x00\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dde71), "\x0f\x84\xd5\x01\x00\x00", 6, NULL);
		}

		//if (setting().onNoForceGlow) {
		//    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dfce5), "\x00", 1, NULL);
		//}
		//else {
		//    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dfce5), "\x01", 1, NULL);
		//}

		if (setting().onNoGhostTrail) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f3374), "\x6a\x00\x90", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f338d), "\x6a\x00\x90", 3, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f3374), "\xff\x75\x08", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f338d), "\xff\x75\x08", 3, NULL);
		}

		if (setting().onNoGlowObject) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x46d8a3), "\xe9\x8f\x01\x00\x00\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x46d8a3), "\x0f\x85\x8e\x01\x00\x00", 6, NULL);
		}

		if (setting().onNoGravityEffect) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f4586), "\x90\x90", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f4586), "\x75\x07", 2, NULL);
		}

		if (setting().onNoMirror) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0BF2), "\xe9\x9b\x01\x00\x00\x90", 6, NULL); // E9 9B 01 00 00 90
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0BF2), "\x0f\x84\x9a\x01\x00\x00", 6, NULL);
		}

		if (setting().onNoNewBest) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e5ff0), "\xc3", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e5ff0), "\x55", 1, NULL);
		}

		if (setting().onNoOrbRing) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F6D40), "\xc3", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F6D40), "\x55", 1, NULL);
		}

		if (setting().onNoLightning) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xe6c19), "\xeb", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xe6c19), "\x75", 1, NULL);
		}

		if (setting().onNoPulse) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x429b6a), "\x90\x90\x90\x90\x90\x90\x90\x90", 8, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x429b6a), "\xf3\x0f\x11\x87\xac\x01\x00\x00", 8, NULL);
		}

		if (setting().onNoRespawnFlash) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DD62F), "\xE9\x99\x00\x00\x00\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DD62F), "\x0f\x85\x98\x00\x00\x00", 6, NULL);
		}

		if (setting().onNoShadeEffect) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xebfe3), "\xeb", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xebfe3), "\x77", 1, NULL);
		}

		if (setting().onPracticePulse) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x429975), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EB441), "\xEB\x16", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x429975), "\x75\x0c", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EB441), "\x74\x16", 2, NULL);
		}

		if (setting().onSolidPlayerTrail) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac3dc), "\x90\x90\x90", 3, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac3dc), "\x89\x41\x10", 3, NULL);
		}

		if (setting().onSolidWaveTrail) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d9ade), "\x90\x90", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d9ade), "\x75\x0c", 2, NULL);
		}

		if (setting().onTrailAlwaysOff) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac6a6), "\xe9\x56\x02\x00\x00\x90", 6, NULL);
		}
		else if (setting().onTrailAlwaysOn) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac6a6), "\x90\x90\x90\x90\x90\x90", 6, NULL);
		}
		else if (setting().onInversedTrail) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac6a6), "\x0f\x85\x55\x02\x00\x00", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac6a6), "\x0f\x84\x55\x02\x00\x00", 6, NULL);
		}

		if (setting().onTrailBugFix) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC29D), "\xBB\xFF\x00\x00\x00\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC29D), "\xf3\x0f\x2c\xc1\x2b\xd8", 6, NULL);
		}

		if (setting().onWaveTrailDeath) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdddfa), "\xeb", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdddfa), "\x74", 1, NULL);
		}

		// Creator

		if (setting().onAbsolutePosition) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44B49D), "\x90\x8b\xcf\x90\x90\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44B49D), "\x51\x8b\xcf\xff\x50\x5c", 6, NULL);
		}

		if (setting().onCopyHack) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49C7ED), "\x90\x90\x90\x90\x90\x90", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49dfe5), "\x90\x90", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49C7ED), "\x0f\x84\x2e\x01\x00\x00", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49dfe5), "\x75\x0e", 2, NULL);
		}

		if (setting().onDefSongBypass) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a37f), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a3fe), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a391), "\x90\x90\x90", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a3a0), "\x90\x90\x90", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a410), "\x90\x90\x90", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a41f), "\x90\x90\x90", 3, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a37f), "\x74\x4e", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a3fe), "\x74\x4e", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a391), "\x0f\x4f\xf0", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a3a0), "\x0f\x48\xf1", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a410), "\x0f\x4f\xf0", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a41f), "\x0f\x48\xf1", 3, NULL);
		}

		if (setting().onEditorExtension) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44779c), "\xEB", 1, NULL); // Placing Objects Left
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4477d2), "\xEB", 1, NULL); // Placing Objects Right
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4477e8), "\xEB", 1, NULL); // Placing Objects Top
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4477b9), "\xEB", 1, NULL); // Placing Objects Bottom
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b445), "\xEB\x44", 2, NULL); // Moving Objects
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493861), "\x00\x20\x90", 3, NULL); // Grid V1
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49399B), "\x00\x20\x90", 3, NULL); // Grid V2
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4939D3), "\x00\x20\x90", 3, NULL); // Grid V3
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493AED), "\x00\x20\x90", 3, NULL); // Grid H
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494112), "\x00\x20\x90", 3, NULL); // Portal Lines (Top)
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494073), "\x00\x20\x90", 3, NULL); // Portal Lines (Bottom)
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493C72), "\x00\x20\x90", 3, NULL); // Music Lines
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44779c), "\x77", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4477d2), "\x76", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4477e8), "\x76", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4477b9), "\x77", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b445), "\x76\x09", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493861), "\xb8\xa6\x54", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49399B), "\xb8\xa6\x54", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4939D3), "\xb8\xa6\x54", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493AED), "\xb8\xa6\x54", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494112), "\xb8\xa6\x54", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494073), "\xb8\xa6\x54", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493C72), "\xb8\xa6\x54", 3, NULL);
		}

		if (setting().onFreeScroll) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44ca45), "\xEB", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44ca5c), "\xEB", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44Ca75), "\xEB", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44Ca8c), "\xEB", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44ca45), "\x77", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44ca5c), "\x77", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44Ca75), "\x77", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44Ca8c), "\x77", 1, NULL);
		}

		if (setting().onHideGrid) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4938a0), "\xE9\x5A\x01\x00\x00\x90", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493a4a), "\xE9\x5A\x01\x00\x00\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4938a0), "\x0f\x86\x59\x01\x00\x00", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493a4a), "\x0f\x86\x53\x01\x00\x00", 6, NULL);
		}

		if (setting().onHideTrigLine) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493e08), "\xE9\xce\x00\x00\x00\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493e08), "\x0f\x84\xcd\x00\x00\x00", 6, NULL);
		}

		if (setting().onLevelEdit) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D62EF), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x450ec2), "\x90\x90\x90\x90\x90\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D62EF), "\x75\x62", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x450ec2), "\x0f\x85\xcc\x00\x00\x00", 6, NULL);
		}

		if (setting().onNoCMark) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x454aa0), "\xb8\x00\x00\x00\x00\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x454aa0), "\x8b\x81\x04\x02\x00\x00", 6, NULL);
		}

		if (setting().onNoDeathX) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48c76a), "\xc7\x04\x24\x00\x00\x00\x00", 7, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48c76a), "\xc7\x04\x24\x33\x33\x33\x3f", 7, NULL);
		}

		if (setting().onNoEditorTrail) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494305), "\x00", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49442c), "\x00", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494305), "\xFF", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49442c), "\xFF", 1, NULL);
		}

		if (setting().onObjBypass) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43e30e), "\x68\xFF\xFF\xFF\x7F", 5, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448ed5), "\x3D\xFF\xFF\xFF\x7F", 5, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4476b2), "\x3D\xFF\xFF\xFF\x7F", 5, NULL);
			//WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4476b2), "\x3D\xFF\xFF\xFF\x7F", 5, NULL); // 25 69 20 6F 62 6A 65 63 74 73 20 20 20
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43e30e), "\x68\x20\x4e\x00\x00", 5, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448ed5), "\x3D\x20\x4e\x00\x00", 5, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4476b2), "\x3D\x20\x4e\x00\x00", 5, NULL);
		}

		if (setting().onPlaceOver) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48D37F), "\xeb", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48D37F), "\x77", 1, NULL);
		}

		if (setting().onSmoothEditTrail) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x491a34), "\x90\x90", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x491a34), "\x72\x79", 2, NULL);
		}

		if (setting().onSwitchToUpArrow) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44ee5e), "\x26", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44e5f0), "\x26", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44ee5e), "\x20", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44e5f0), "\x20", 1, NULL);
		}

		if (setting().onVerifyHack) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43d760), "\xeb\x2f", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43d760), "\x75\x2f", 2, NULL);
		}

		if (setting().onZoomBypass) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448bb5), "\x90\x90\x90", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448bba), "\x90\x90\x90", 3, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448bb5), "\x0f\x2f\xc8", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448bba), "\x0f\x28\xc8", 3, NULL);
		}

		// Level

		if (setting().onConfirmExit) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7f80), "\x90\x90\x90\x90\x90", 5, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7f8d), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7f9d), "\x90\x90\x90\x90\x90", 5, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7fa5), "\x90\x90\x90\x90\x90", 5, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7f80), "\xe8\xfb\xbb\x01\x00", 5, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7f8d), "\x6a\x10", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7f9d), "\x68\x7c\x42\x51\x00", 5, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7fa5), "\xe8\xc6\xd7\xf2\xff", 5, NULL);
		}

		if (setting().onMusicSync) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ee59e), "\xeb\x08", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ee59e), "\x75\x08", 2, NULL);
		}

		if (setting().onHiFPSRotation) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dc13b), "\x90\x90\x90\x90\x90\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dc13b), "\x0f\x82\xd6\x00\x00\x00", 6, NULL); // 0f 82 d6 00 00 00
		}

		if (setting().onInstantComplete) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e16f6), "\xC7\x87\x74\x04\x00\x00\x00\x00\x00\x70\x90\x90", 12, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e16f6), "\xF3\x0F\x11\x8F\x74\x04\x00\x00\x9F\xF6\xC4\x44", 12, NULL);
		}

		if (setting().onInstantTriggers) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x89780), "\xC0", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x89780), "\xC1", 1, NULL);
		}

		if (setting().onJumpHack) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4da510), "\x01", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4da295), "\x01", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4da510), "\x00", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4da295), "\x00", 1, NULL);
		}

		if (setting().onNoclip) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xF04E9), "\xe9\xf0\x02\x00\x00\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xF04E9), "\x0f\x85\xef\x02\x00\x00", 6, NULL);
		}

		if (setting().onNoclipUnstuck) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdae16), "\xE9\x00\x02\x00\x00\x90", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdb02d), "\xE9\x10\x01\x00\x00\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdae16), "\x0F\x84\xFF\x01\x00\x00", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdb02d), "\x0F\x8B\x0F\x01\x00\x00", 6, NULL);
		}

		if (setting().onPauseDurComp) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E531B), "\x00", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E2906), "\x90\x90\x90\x90\x90\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E531B), "\x01", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E2906), "\x88\x81\xf9\x02\x00\x00", 6, NULL);
		}

		if (setting().onPracticeMusic) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F284F), "\x90\x90\x90\x90\x90\x90", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F3663), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0699), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F06CB), "\x90\x90", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F3691), "\x90\x90\x90\x90\x90", 5, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F284F), "\x0f\x85\x4d\x07\x00\x00", 6, NULL); // 0f 85 4d 07 00 00
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F3663), "\x75\x41", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0699), "\x75\x3e", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F06CB), "\x75\x0c", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F3691), "\xe8\xaa\x42\xf2\xff", 5, NULL); // e8 aa 42 f2 ff
		}

		WriteRef(gd::base + 0xf04ff, setting().respawnTime);

		if (setting().onSuicide) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ea268), "\xe9\xc9\x01\x00\x00\x90", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ea453), "\xe9\x59\x02\x00\x00\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ea268), "\x0f\x86\xc8\x01\x00\x00", 6, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ea453), "\x0f\x87\x58\x02\x00\x00", 6, NULL);
		}

		// Universal

		if (setting().onAllowLowVolume) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xd0cb0), "\xeb", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xd772e), "\xeb", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xd0cb0), "\x76", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xd772e), "\x76", 1, NULL);
		}

		if (setting().onFastAlt) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x28DFE), "\x90\x90\x90\x90\x90\x90\x90", 7, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x28DFE), "\x8b\x03\x8b\xcb\xff\x50\x18", 7, NULL);
		}

		if (setting().onForceVis) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60783), "\xB0\x01\x90", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60c9a), "\x90\x90\x90\x90\x90\x90", 6, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60783), "\x8a\x45\x08", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60c9a), "\x0f\x84\xcb\x00\x00\x00", 6, NULL);
		}

		if (setting().onFreeWinReSize) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x10f48b), "\x90\x90\x90\x90\x90", 5, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x10f48b), "\xe8\xb0\xf3\xff\xff", 5, NULL);
		}

		if (setting().onIgnoreSlider) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cec0), "\xc3", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cec0), "\x55", 1, NULL);
		}

		if (setting().onImmortalIcons) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4aec50), "\xc3", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4aec50), "\x55", 1, NULL);
		}

		if (setting().onInstantGameWork) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x88170), "\xc0", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x88170), "\xc1", 1, NULL);
		}

		if (setting().onKrmalMode) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x472a8e), "\x8b\xd8\x90", 3, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x472a8e), "\x0f\x45\xd8", 3, NULL);
		}

		if (setting().onNoRotation) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60578), "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60578), "\xf3\x0f\x11\x41\x1c\xf3\x0f\x11\x41\x18", 10, NULL);
		}

		if (setting().onFastMenu) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x160da), "\x00\x00\x00\x00", 4, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1618b), "\x00\x00\x00\x00", 4, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x160da), "\x00\x00\x00\x3f", 4, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1618b), "\x29\x5c\x0f\x3e", 4, NULL);
		}

		if (setting().onSafeMode) safeModeON();
		else safeModeOFF();

		if (setting().onRestartButton) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D64D9), "\x90\x90", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D64D9), "\x75\x29", 2, NULL);
		}

		if (setting().onTransparentBG) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42cf96), "\x90\xb1\xff", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42cf9e), "\xff\xff", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43b7db), "\x90\xb1\xff", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43b7e3), "\xff\xff", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488132), "\x90\xb1\xff", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48813a), "\xff\xff", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48969e), "\x90\xb1\xff", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4896a5), "\xff\xff", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bde1), "\x90\xb1\xff", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bde9), "\xff\xff", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49f97c), "\x90\xb1\xff", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49f984), "\xff\xff", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42cf96), "\x80\xc9\xff", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42cf9e), "\x00\x66", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43b7db), "\x80\xc9\xff", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43b7e3), "\x00\x66", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488132), "\x80\xc9\xff", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48813a), "\x00\x66", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48969e), "\x80\xc9\xff", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4896a5), "\x00\x66", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bde1), "\x80\xc9\xff", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bde9), "\x00\x66", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49f97c), "\x80\xc9\xff", 3, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49f984), "\x00\x66", 2, NULL);
		}

		if (setting().onTransparentLists) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488a4f), "\x00\x00\x00\x40", 4, NULL); // RGBA format
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48a945), "\x00\x00\x00\x40", 4, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431cba), "\x60", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c89), "\x20\x20", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c8c), "\x20", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c7f), "\x40\x40", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c82), "\x40", 1, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488a4f), "\xbf\x72\x3e\xff", 4, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48a945), "\xbf\x72\x3e\xff", 4, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431cba), "\xff", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c89), "\xa1\x58", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c8c), "\x2c", 1, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c7f), "\xc2\x72", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c82), "\x3e", 1, NULL);
		}

		if (setting().onTransparentTextLabels) {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43b9af), "\x68\x00", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43BCA4), "\x68\x00", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49FC73), "\x68\x00", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49FD1C), "\x68\x00", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A00E3), "\x68\x00", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A04A6), "\x68\x00", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A06FC), "\x68\x00", 2, NULL);
		}
		else {
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43b9af), "\x68\xff", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43BCA4), "\x68\xff", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49FC73), "\x68\xff", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49FD1C), "\x68\xff", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A00E3), "\x68\xff", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A04A6), "\x68\xff", 2, NULL);
			WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A06FC), "\x68\xff", 2, NULL);
		}

		// fuck it.

		
	  
		auto* colors = ImGui::GetStyle().Colors;

		color1.x = setting().Overlaycolor[0];
		color1.y = setting().Overlaycolor[1];
		color1.z = setting().Overlaycolor[2];
		color1.w = setting().Overlaycolor[3] - 0.4;

		color2.x = setting().Overlaycolor[0];
		color2.y = setting().Overlaycolor[1];
		color2.z = setting().Overlaycolor[2];
		color2.w = 1;

		color3.x = setting().Overlaycolor[0] + 0.3;
		color3.y = setting().Overlaycolor[1] + 0.3;
		color3.z = setting().Overlaycolor[2] + 0.3;
		color3.w = setting().Overlaycolor[3] + 0.3;

		color4.x = setting().Overlaycolor[0] - 0.1;
		color4.y = setting().Overlaycolor[1] - 0.1;
		color4.z = setting().Overlaycolor[2] - 0.1;
		color4.w = setting().Overlaycolor[3] - 0.1;

		color5.x = setting().Overlaycolor[0] + 0.1;
		color5.y = setting().Overlaycolor[1] + 0.1;
		color5.z = setting().Overlaycolor[2] + 0.1;
		color5.w = setting().Overlaycolor[3] + 0.1;

		color6.x = setting().BGcolor[0];
		color6.y = setting().BGcolor[1];
		color6.z = setting().BGcolor[2];
		color6.w = setting().BGcolor[3];

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
		colors[ImGuiCol_Tab] = ImVec4(color5.x, color5.y, color5.z, color5.w - 0.2);
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
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0);

		oneX = false;
	}

	//const auto& font = ImGui::GetIO().Fonts->Fonts.back();
	//ImGui::PushFont(font);

	if (setting().show) {
		auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

		if (setting().onExplorer) {
			render_explorer_window(setting().onExplorer);
		}

		if (setting().managerView) {
			showManagerView(setting().managerView);
		}

		if (ImGui::Begin("Bypass", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
			ImGui::SetWindowFontScale(setting().UISize);
			ImGui::SetNextItemWidth(120 * setting().UISize);

			if (ImGui::Checkbox("Character Filter", &setting().onCharFilter)) {
				if (setting().onCharFilter) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4147A7), "\x90\x90", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4147A7), "\x75\x04", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you input any character in text fields.");

			if (ImGui::Checkbox("Icons", &setting().onIcons)) {
				if (setting().onIcons) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48000F), "\xe9\x74\x02\x00\x00\x90", 6, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E606), "\xe9\xdc\x00\x00\x00\x90", 6, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E098), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E128), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E1B8), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E248), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E2D8), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E9BE), "\xeb\x13", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47EAAE), "\xeb\x13", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48000F), "\x0f\x85\x73\x02\x00\x00", 6, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E606), "\x0f\x85\xdb\x00\x00\x00", 6, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E098), "\x74\x04", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E128), "\x74\x04", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E1B8), "\x74\x04", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E248), "\x74\x04", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E2D8), "\x74\x04", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E9BE), "\x75\x13", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47EAAE), "\x75\x13", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Unlocks all icons and colors.");

			if (ImGui::Checkbox("Main Levels", &setting().onMainLevels)) {
				if (setting().onMainLevels) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A81AF), "\xe9\xa3\x02\x00\x00\x90", 6, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A8C22), "\xe9\x98\x00\x00\x00\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A81AF), "\x0f\x84\xa2\x02\x00\x00", 6, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A8C22), "\x0f\x8e\x97\x00\x00\x00", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Unlocks main demon levels.");

			if (ImGui::Checkbox("Slider Limit", &setting().onSliderLimit)) {
				if (setting().onSliderLimit) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cf27), "\xeb\x07", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cf3b), "\xeb\x07", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cf27), "\x76\x07", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cf3b), "\x76\x07", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets sliders be dragged beyond the visible limit.");

			if (ImGui::Checkbox("Text Length", &setting().onTextLength)) {
				if (setting().onTextLength) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4147DE), "\xeb\x04", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4147DE), "\x7c\x04", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Allows for unlimited text length in text inputs.");
		}

		if (ImGui::Begin("Cosmetic", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
		{
			ImGui::SetWindowFontScale(setting().UISize);

			ImGui::Text("Accurate Percentage:");
			const char* const decimals[] = { "4 decimal places", "3 decimal places", "2 decimal places", "1 decimal place", "0 decimal place" };
			ImGui::SetNextItemWidth(160.f * setting().UISize);
			ImGui::Combo("##accuratePerc", &setting().accuratePercentage, decimals, 5);

			if (ImGui::Checkbox("Always New Best", &setting().onAlwaysNewBest)) {
				if (setting().onAlwaysNewBest) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xf077e), "\x90\x90", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xf077e), "\x74\x14", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Always shows new best on death.");

			if (ImGui::Checkbox("Coins Show Uncollected", &setting().onSCoinUncoll)) {
				if (setting().onSCoinUncoll) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x443368), "\x8B\xC2\x90", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4735CE), "\x8B\xD9\x90", 3, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x443368), "\x0F\x44\xC2", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4735CE), "\x0F\x44\xD9", 3, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Forces coins show as uncollected.");

			if (ImGui::Checkbox("Coins in Practice", &setting().onCoinsPractice)) {
				if (setting().onCoinsPractice) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4eb138), "\x90\x90", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4eb138), "\x75\x4c", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Allows for coins to be picked up in practice mode.");

			if (ImGui::Checkbox("Force Don't Enter", &setting().onDontEnter)) {
				if (setting().onDontEnter) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EC51C), "\x90\x90", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EC51C), "\x8B\xD9", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables transition trigger effects.");

			if (ImGui::Checkbox("Force Don't Fade", &setting().onDontFade)) {
				if (setting().onDontFade) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebddb), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebe06), "\xeb\x11", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebe20), "\x90\x90", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebddb), "\x74\x5d", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebe06), "\x74\x11", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebe20), "\x75\x18", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables fading when objects leave the viewable play area.");

			if (ImGui::Checkbox("Fast Animation Complete", &setting().onFastAnimComp)) {
				if (setting().onFastAnimComp) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5F99), "\x00\x00\x00\x00", 4, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5F99), "\x99\x99\xF9\x3F", 4, NULL);
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Makes the level ending animation instant.");

			if (ImGui::Checkbox("Fast Show Complete", &setting().onFastShowComp)) {
				if (setting().onFastShowComp) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5CA4), "\x00\x00\x00\x00", 4, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE57F6), "\x00\x00\x00\x00", 4, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE57CE), "\x90\x90\x90\x90\x90\x90\x90", 7, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE57DE), "\x90\x90\x90\x90\x90\x90\x90", 7, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5814), "\x90\x90\x90\x90\x90\x90\x90", 7, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5824), "\x90\x90\x90\x90\x90\x90\x90", 7, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5CA4), "\x00\x00\xc0\x3f", 4, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE57F6), "\xaf\x47\x61\x3f", 4, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE57CE), "\xC7\x04\x24\xC3\xF5\x28\x3F", 7, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE57DE), "\xC7\x04\x24\x9A\x99\x19\x3F", 7, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5814), "\xC7\x04\x24\xAF\x47\x61\x3E", 7, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xE5824), "\xC7\x04\x24\x00\x00\x00\x40", 7, NULL);
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Makes the show complete at the end of the level instant.");

			if (ImGui::Checkbox("Force Objects Invisible", &setting().onFObjectInvisible)) {
				if (setting().onFObjectInvisible) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebece), "\x90\x90\x90\x90\x90\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebece), "\x0f\x84\xd1\x02\x00\x00", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Makes all objects act as invisible ones.");

			if (ImGui::Checkbox("Hide Attempts", &setting().onHideAttempts)) {
				if (playLayer)
					playLayer->m_attemptsLabel->setVisible(!setting().onHideAttempts);
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Hides the attemps counter in-game.");

			if (ImGui::Checkbox("Hide Pause Button", &setting().onHidePauseButton)) {
				if (playLayer)
					if (gd::GameManager::sharedState()->getGameVariable("0024"))
						if (playLayer->m_uiLayer != nullptr)
							playLayer->m_uiLayer->m_pauseBtn->setVisible(!setting().onHidePauseButton);
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Hides the pause button when the in-game cursor is enabled.");

			if (ImGui::Checkbox("Hide Pause Menu", &setting().onHidePauseMenu)) {
				if (layers().PauseLayerObject)
				{
					if (setting().onHidePauseMenu) layers().PauseLayerObject->setVisible(0);
					else layers().PauseLayerObject->setVisible(1);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Hides the pause menu.");


			if (ImGui::Checkbox("Hide Player", &setting().onHidePlayer)) {
				if (playLayer)
					if (playLayer->m_player || playLayer->m_player2) {
						playLayer->m_player->setVisible(!setting().onHidePlayer);
						playLayer->m_player2->setVisible(!setting().onHidePlayer);
					}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Hides the player.");

			if (ImGui::Checkbox("Hide Practice Buttons", &setting().onHidePracticeBtn)) {
				if (playLayer && playLayer->m_practiceMode && (playLayer->m_uiLayer->m_checkpointMenu != nullptr)) {
					playLayer->m_uiLayer->m_checkpointMenu->setVisible(!setting().onHidePracticeBtn);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Hides practice buttons.");

			if (ImGui::Checkbox("Instant Mirror", &setting().onInstantMirror)) {
				if (setting().onInstantMirror) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0D33), "\xc7\x04\x24\x00\x00\x00\x00", 7, NULL); // E9 9B 01 00 00 90
					cheatAdd();
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0D33), "\xc7\x04\x24\x00\x00\x00\x3F", 7, NULL); // 0F 84 9A 01 00 00
					cheatDec();
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables the mirror portal animation.");

			if (ImGui::Checkbox("Inversed Trail", &setting().onInversedTrail)) {
				if (setting().onInversedTrail) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac6a6), "\x0f\x85\x55\x02\x00\x00", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac6a6), "\x0f\x84\x55\x02\x00\x00", 6, NULL);
				}
			}

			if (ImGui::Checkbox("Max Particles", &setting().onMaxParticles)) {
				if (setting().onMaxParticles) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xB64D7), "\x8b\x7d\x07", 3, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xB64D7), "\x8b\x7d\x08", 3, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Increases particles to the maximum.");

			if (ImGui::Checkbox("Mini Cube Icon", &setting().onMiniCube)) {
				if (setting().onMiniCube) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d8cf4), "\x31\xff\x90", 3, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d8cf4), "\x0f\x4f\xf9", 3, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Forces use of the mini cube icon.");

			if (ImGui::Checkbox("No Animations", &setting().onNoAnimations)) {
				if (setting().onNoAnimations) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x886be), "\xc0", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x88da0), "\xc0", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x886be), "\xc1", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x88da0), "\xc1", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Makes game animations instant (e.g. buttons).");

			if (ImGui::Checkbox("No Background Flash", &setting().onNoBGFlash)) {
				if (setting().onNoBGFlash) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e6c92), "\x6a\x00", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e6ca8), "\xc7\x04\x24\x00\x00\x00\x00", 7, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e6c92), "\x6a\x01", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e6ca8), "\xc7\x04\x24\x8f\xc2\xf5\x3d", 7, NULL);
				}
			}

			/*if (ImGui::Checkbox("No Cube Rotation", &setting().onNoCubeRotation)) {
				if (setting().onNoCubeRotation) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dab30), "\xc3", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dab30), "\x57", 1, NULL);
				}
			}*/

			if (ImGui::Checkbox("No Death Effect", &setting().onNoDeathEffect)) {
				if (setting().onNoDeathEffect) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dde71), "\xe9\xe7\x01\x00\x00\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dde71), "\x0f\x84\xd5\x01\x00\x00", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("No visual effects on death.");

			ImGui::Checkbox("No Effect Circle", &setting().onNoEffectCircle);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes effect circles from orb, portal & pad activations.");

			//if (ImGui::Checkbox("No Force Player Glow", &setting().onNoForceGlow)) {
			//    if (setting().onNoForceGlow) {
			//        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dfce6), "\x00", 1, NULL);
			//    }
			//    else {
			//        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dfce6), "\x01", 1, NULL);
			//    }
			//}
			//if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
			//    ImGui::SetTooltip("Doesn't force a white glow when using black secondary color.");

			if (ImGui::Checkbox("No Ghost Trail", &setting().onNoGhostTrail)) {
				if (setting().onNoGhostTrail) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f3374), "\x6a\x00\x90", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f338d), "\x6a\x00\x90", 3, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f3374), "\xff\x75\x08", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f338d), "\xff\x75\x08", 3, NULL);
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables player ghost trail triggers.");

			if (ImGui::Checkbox("No Glow", &setting().onNoGlowObject)) {
				if (setting().onNoGlowObject) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x46d8a3), "\xe9\x8f\x01\x00\x00\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x46d8a3), "\x0f\x85\x8e\x01\x00\x00", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables objects glow.");

			if (ImGui::Checkbox("No Gravity Effect", &setting().onNoGravityEffect)) {
				if (setting().onNoGravityEffect) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f4586), "\x90\x90", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f4586), "\x75\x07", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables gravity effect.");

			ImGui::Checkbox("No Mini Icon", &setting().onNoMiniIcon);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Replaces default mini icon with normal one.");

			if (ImGui::Checkbox("No Mirror", &setting().onNoMirror)) {
				if (setting().onNoMirror) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0BF2), "\xe9\x9b\x01\x00\x00\x90", 6, NULL); // E9 9B 01 00 00 90
					cheatAdd();
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0BF2), "\x0f\x84\x9a\x01\x00\x00", 6, NULL);
					cheatDec();
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables mirror portals.");

			if (ImGui::Checkbox("No New Best Popup", &setting().onNoNewBest)) {
				if (setting().onNoNewBest) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e5ff0), "\xc3", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e5ff0), "\x55", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables the new best popup.");

			if (ImGui::Checkbox("No Orb Ring", &setting().onNoOrbRing)) {
				if (setting().onNoOrbRing) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F6D40), "\xc3", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F6D40), "\x55", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables orb ring effect when touching it.");

			ImGui::Checkbox("No Particles", &setting().onNoParticles);
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables the particle system.");

			if (ImGui::Checkbox("No Portal Lightning", &setting().onNoLightning)) {
				if (setting().onNoLightning) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xe6c19), "\xeb", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xe6c19), "\x75", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables portal lightning from size-changing portals.");

			if (ImGui::Checkbox("No Pulse", &setting().onNoPulse)) {
				if (setting().onNoPulse) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x429b6a), "\x90\x90\x90\x90\x90\x90\x90\x90", 8, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x429b6a), "\xf3\x0f\x11\x87\xac\x01\x00\x00", 8, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables pulsing on objects.");

			if (ImGui::Checkbox("No Respawn Flash", &setting().onNoRespawnFlash)) {
				if (setting().onNoRespawnFlash) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DD62F), "\xE9\x99\x00\x00\x00\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DD62F), "\x0f\x85\x98\x00\x00\x00", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables the respawn flash & circle effect.");

			if (ImGui::Checkbox("No Shade Effect", &setting().onNoShadeEffect)) {
				if (setting().onNoShadeEffect) {
					cheatAdd();
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xebfe3), "\xeb", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xebfe3), "\x77", 1, NULL);
					cheatDec();
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables the disappearing effect on invisible blocks and etc.");

			ImGui::Checkbox("No Wave Pulse", &setting().onNoWavePulse);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables wave trail pulsing.");
			ImGui::SameLine();
			if (ImGui::TreeNode("##trailSize")) {
				ImGui::SetNextItemWidth(50.f);
				ImGui::DragFloat("Trail Size", &setting().wavePulseSize, 0.1f, 0.1f, 10.f);
				ImGui::TreePop();
			}

			ImGui::Checkbox("No Wave Trail", &setting().onNoWaveTrail);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables the hard wave trail.");

			ImGui::Checkbox("No Wave Trail Behind", &setting().onNoWaveTrailBehind);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables default player trail behind the wave trail.");

			if (ImGui::Checkbox("Practice Pulse", &setting().onPracticePulse)) {
				if (setting().onPracticePulse) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x429975), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EB441), "\xEB\x16", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x429975), "\x75\x0c", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EB441), "\x74\x16", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Pulses orbs & wave in practice mode.");

			if (ImGui::Checkbox("Solid Player Trail", &setting().onSolidPlayerTrail)) {
				if (setting().onSolidPlayerTrail) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac3dc), "\x90\x90\x90", 3, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac3dc), "\x89\x41\x10", 3, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("No blending on player trail.");

			if (ImGui::Checkbox("Solid Wave Trail", &setting().onSolidWaveTrail)) {
				if (setting().onSolidWaveTrail) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d9ade), "\x90\x90", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d9ade), "\x75\x0c", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("No blending on the wave trail.");

			if (ImGui::Checkbox("Trail Always Off", &setting().onTrailAlwaysOff)) {
				if (setting().onTrailAlwaysOff) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac6a6), "\xe9\x56\x02\x00\x00\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac6a6), "\x0f\x84\x55\x02\x00\x00", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Forces player trail to be always off.");

			if (ImGui::Checkbox("Trail Always On", &setting().onTrailAlwaysOn)) {
				if (setting().onTrailAlwaysOn) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac6a6), "\x90\x90\x90\x90\x90\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac6a6), "\x0f\x84\x55\x02\x00\x00", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Forces player trail to be always on.");

			if (ImGui::Checkbox("Trail Bug Fix", &setting().onTrailBugFix)) {
				if (setting().onTrailBugFix) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC29D), "\xBB\xFF\x00\x00\x00\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC29D), "\xf3\x0f\x2c\xc1\x2b\xd8", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Fixes trail cutting on high refresh rates.");

			if (ImGui::Checkbox("Wave Trail on Death", &setting().onWaveTrailDeath)) {
				if (setting().onWaveTrailDeath) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdddfa), "\xeb", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdddfa), "\x74", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Keeps wave trail visible on death.");
		}

		if (ImGui::Begin("Creator", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
		{
			ImGui::SetWindowFontScale(setting().UISize);
			ImGui::SetNextItemWidth(120 * setting().UISize);

			//ImGui::SetNextItemWidth(80.f);
			//ImGui::DragFloat("##gridsize", &setting().gridSize, 0.1f, 1.f, 120.f);
			//ImGui::SameLine();
			//ImGui::Checkbox("Grid Size", &setting().onGridSize);

			if (ImGui::Checkbox("Absolute Rotation", &setting().onAbsolutePosition)) {
				if (setting().onAbsolutePosition) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44B49D), "\x90\x8b\xcf\x90\x90\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44B49D), "\x51\x8b\xcf\xff\x50\x5c", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Locks block position for rotation.");

			if (ImGui::Checkbox("Copy Hack", &setting().onCopyHack)) {
				if (setting().onCopyHack) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49C7ED), "\x90\x90\x90\x90\x90\x90", 6, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49dfe5), "\x90\x90", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49C7ED), "\x0f\x84\x2e\x01\x00\x00", 6, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49dfe5), "\x75\x0e", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you copy any level, without a password.");

			if (ImGui::Checkbox("Default Song Bypass", &setting().onDefSongBypass)) {
				if (setting().onDefSongBypass) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a37f), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a3fe), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a391), "\x90\x90\x90", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a3a0), "\x90\x90\x90", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a410), "\x90\x90\x90", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a41f), "\x90\x90\x90", 3, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a37f), "\x74\x4e", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a3fe), "\x74\x4e", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a391), "\x0f\x4f\xf0", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a3a0), "\x0f\x48\xf1", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a410), "\x0f\x4f\xf0", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49a41f), "\x0f\x48\xf1", 3, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you use hidden default songs in the editor.");

			if (ImGui::Checkbox("Editor Extension", &setting().onEditorExtension)) {
				if (setting().onEditorExtension) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44779c), "\xEB", 1, NULL); // Placing Objects Left
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4477d2), "\xEB", 1, NULL); // Placing Objects Right
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4477e8), "\xEB", 1, NULL); // Placing Objects Top
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4477b9), "\xEB", 1, NULL); // Placing Objects Bottom
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b445), "\xEB\x44", 2, NULL); // Moving Objects
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493861), "\x00\x20\x90", 3, NULL); // Grid V1
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49399B), "\x00\x20\x90", 3, NULL); // Grid V2
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4939D3), "\x00\x20\x90", 3, NULL); // Grid V3
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493AED), "\x00\x20\x90", 3, NULL); // Grid H
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494112), "\x00\x20\x90", 3, NULL); // Portal Lines (Top)
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494073), "\x00\x20\x90", 3, NULL); // Portal Lines (Bottom)
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493C72), "\x00\x20\x90", 3, NULL); // Music Lines
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44779c), "\x77", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4477d2), "\x76", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4477e8), "\x76", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4477b9), "\x77", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44b445), "\x76\x09", 2, NULL); // Moving Objects
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493861), "\xb8\xa6\x54", 3, NULL); // Grid V1
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49399B), "\xb8\xa6\x54", 3, NULL); // Grid V2
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4939D3), "\xb8\xa6\x54", 3, NULL); // Grid V3
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493AED), "\xb8\xa6\x54", 3, NULL); // Grid H
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494112), "\xb8\xa6\x54", 3, NULL); // Portal Lines (Top)
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494073), "\xb8\xa6\x54", 3, NULL); // Portal Lines (Bottom)
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493C72), "\xb8\xa6\x54", 3, NULL); // Music Lines
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Allows you to build out of bounds.");

			ImGui::Checkbox("Editor Preview Mode", &setting().onEditorPreview);
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Allows you to see colors in editor (little bit buggy).");

			if (ImGui::Checkbox("Free Scroll", &setting().onFreeScroll)) {
				if (setting().onFreeScroll) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44ca45), "\xEB", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44ca5c), "\xEB", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44Ca75), "\xEB", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44Ca8c), "\xEB", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44ca45), "\x77", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44ca5c), "\x77", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44Ca75), "\x77", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44Ca8c), "\x77", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Allows you to scroll out the editor.");

			if (ImGui::Checkbox("Hide Grid", &setting().onHideGrid)) {
				if (setting().onHideGrid) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4938a0), "\xE9\x5A\x01\x00\x00\x90", 6, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493a4a), "\xE9\x5A\x01\x00\x00\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4938a0), "\x0f\x86\x59\x01\x00\x00", 6, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493a4a), "\x0f\x86\x53\x01\x00\x00", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Hides editor grid.");

			if (ImGui::Checkbox("Hide Trigger Lines", &setting().onHideTrigLine)) {
				if (setting().onHideTrigLine) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493e08), "\xE9\xce\x00\x00\x00\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493e08), "\x0f\x84\xcd\x00\x00\x00", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Hides trigger lines.");

			//ImGui::Checkbox("Hitbox Bug Fix", &setting().onHitboxBugFix);
			//if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
			//	ImGui::SetTooltip("Fixes oriented hitboxes in the editor.");

			if (ImGui::Checkbox("Level Edit", &setting().onLevelEdit)) {
				if (setting().onLevelEdit) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D62EF), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x450ec2), "\x90\x90\x90\x90\x90\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D62EF), "\x75\x62", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x450ec2), "\x0f\x85\xcc\x00\x00\x00", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you edit any level through the pause menu.");

			if (ImGui::Checkbox("No (C) Mark", &setting().onNoCMark)) {
				if (setting().onNoCMark) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x454aa0), "\xb8\x00\x00\x00\x00\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x454aa0), "\x8b\x81\x04\x02\x00\x00", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes the (C) mark when uploading copied levels.");

			if (ImGui::Checkbox("No Death X", &setting().onNoDeathX)) {
				if (setting().onNoDeathX) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48c76a), "\xc7\x04\x24\x00\x00\x00\x00", 7, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48c76a), "\xc7\x04\x24\x33\x33\x33\x3f", 7, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes big annoying red X when dying whilst playtesing.");

			if (ImGui::Checkbox("No Editor Trail", &setting().onNoEditorTrail)) {
				if (setting().onNoEditorTrail) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494305), "\x00", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49442c), "\x00", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494305), "\xFF", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49442c), "\xFF", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes the editor trail.");

			if (ImGui::Checkbox("Object Bypass", &setting().onObjBypass)) {
				if (setting().onObjBypass) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43e30e), "\x68\xFF\xFF\xFF\x7F", 5, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448ed5), "\x3D\xFF\xFF\xFF\x7F", 5, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4476b2), "\x3D\xFF\xFF\xFF\x7F", 5, NULL);
					//WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4476b2), "\x3D\xFF\xFF\xFF\x7F", 5, NULL); // 25 69 20 6F 62 6A 65 63 74 73 20 20 20
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43e30e), "\x68\x20\x4e\x00\x00", 5, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448ed5), "\x3D\x20\x4e\x00\x00", 5, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4476b2), "\x3D\x20\x4e\x00\x00", 5, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Uncaps the object limit.");

			if (ImGui::Checkbox("Place Over", &setting().onPlaceOver)) {
				if (setting().onPlaceOver) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48D37F), "\xeb", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48D37F), "\x77", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you place the same object over itself in editor.");

			ImGui::Checkbox("Reset Percentage", &setting().onResetEditPercentage);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Resets normal percentage while saving level.");

			if (ImGui::Checkbox("Smooth Trail", &setting().onSmoothEditTrail)) {
				if (setting().onSmoothEditTrail) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x491a34), "\x90\x90", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x491a34), "\x72\x79", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Makes the editor trail smoother.");

			if (ImGui::Checkbox("Up Arrow Jump", &setting().onSwitchToUpArrow)) {
				if (setting().onSwitchToUpArrow) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44ee5e), "\x26", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44e5f0), "\x26", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44ee5e), "\x20", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44e5f0), "\x20", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Rebinds jump to up arrow.");

			if (ImGui::Checkbox("Verify Hack", &setting().onVerifyHack)) {
				if (setting().onVerifyHack) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43d760), "\xeb\x2f", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43d760), "\x75\x2f", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you upload unverified levels.");

			if (ImGui::Checkbox("Zoom Bypass", &setting().onZoomBypass)) {
				if (setting().onZoomBypass) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448bb5), "\x90\x90\x90", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448bba), "\x90\x90\x90", 3, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448bb5), "\x0f\x2f\xc8", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x448bba), "\x0f\x28\xc8", 3, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you zoom fully in & out.");

			ImGui::SetNextWindowSize({ 200 * setting().UISize, 0});
		}

		if (ImGui::Begin("Level", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
		{
			ImGui::SetWindowFontScale(setting().UISize);
			ImGui::SetNextItemWidth(120 * setting().UISize);

			ImGui::Checkbox("0% Practice Complete", &setting().onZeroPracticeComplete);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Completes a level if you beat it in 1 practice attempt.");

			ImGui::Checkbox("Auto Deafen", &setting().onAutoDeafen);
			ImGui::SameLine();
			if (ImGui::TreeNode("##autodeafen")) {
				ImGui::SetNextItemWidth(60.f * setting().UISize);
				ImGui::DragInt("Deafen At", &setting().deafenPercent, 1, 0, 100);
				ImGui::SetNextItemWidth(60.f * setting().UISize);
				ImGui::Hotkey("##deafenKey", &setting().deafenKey);
				ImGui::SameLine();
				ImGui::Text("Deafen Key");
				ImGui::TreePop();
			}

			ImGui::Checkbox("Auto Kill", &setting().onAutoKill);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Kills the player.");
			ImGui::SameLine();
			if (ImGui::TreeNode("##autoKill")) {
				ImGui::SetNextItemWidth(60.f * setting().UISize);
				ImGui::DragInt("Kill At", &setting().killPercent, 1, 0, 100);
				ImGui::TreePop();
			}

			ImGui::Checkbox("Auto Practice Mode", &setting().onAutoPractice);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Auto-enables practice mode.");

			ImGui::Checkbox("Auto Song Download", &setting().onAutoDownloadSong);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Automatically starts downloading songs when you open the level page.");

			ImGui::Checkbox("Ball Rotation Bug Fix", &setting().onBallRotationFix);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Fixes that ball rotation bug when entering a portal mid ball animation.");

			if (ImGui::Checkbox("Confirm Exit", &setting().onConfirmExit)) {
				if (setting().onConfirmExit) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7f80), "\x90\x90\x90\x90\x90", 5, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7f8d), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7f9d), "\x90\x90\x90\x90\x90", 5, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7fa5), "\x90\x90\x90\x90\x90", 5, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7f80), "\xe8\xfb\xbb\x01\x00", 5, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7f8d), "\x6a\x10", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7f9d), "\x68\x7c\x42\x51\x00", 5, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d7fa5), "\xe8\xc6\xd7\xf2\xff", 5, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Requires confirmation when exiting a level.");

			if (ImGui::Checkbox("Corrective Music Sync", &setting().onMusicSync)) {
				if (setting().onMusicSync) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ee59e), "\xeb\x08", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ee59e), "\x75\x08", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Syncs music to checked speed-portals, instead of only ones the player hit.");

			if (ImGui::Checkbox("High FPS Rotation Fix", &setting().onHiFPSRotation)) {
				if (setting().onHiFPSRotation) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dc13b), "\x90\x90\x90\x90\x90\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dc13b), "\x0f\x82\xd6\x00\x00\x00", 6, NULL); // 0f 82 d6 00 00 00
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Fixes vehicles rotation on high fps (affects hitboxes).");

			ImGui::Checkbox("Hitboxes", &setting().onHitboxes);
			ImGui::SameLine();
			if (ImGui::TreeNode("##hitboxesSettings")) {
				ImGui::SetNextItemWidth(setting().UISize * 60);
				ImGui::DragInt("Opacity", &setting().hitboxOpacity, 1.f, 0, 255);

				ImGui::Checkbox("Player", &setting().onPlayerHitbox);

				ImGui::Checkbox("Solids", &setting().onSolidsHitbox);
				ImGui::SameLine();
				ImGui::ColorEdit3("##solidsHitboxColor", setting().solidsColor, ImGuiColorEditFlags_NoInputs);

				ImGui::Checkbox("Hazards", &setting().onHazardsHitbox);
				ImGui::SameLine();
				ImGui::ColorEdit3("##hazardsHitboxColor", setting().hazardsColor, ImGuiColorEditFlags_NoInputs);

				ImGui::Checkbox("Specials", &setting().onSpecialsHitbox);
				ImGui::SameLine();
				ImGui::ColorEdit3("##specialsHitboxColor", setting().specialsColor, ImGuiColorEditFlags_NoInputs);
				ImGui::TreePop();
			}

			ImGui::Checkbox("Hitboxes on Death", &setting().onHitboxesOnDeath);

			if (ImGui::Checkbox("Instant Complete", &setting().onInstantComplete)) {
				if (setting().onInstantComplete) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e16f6), "\xC7\x87\x74\x04\x00\x00\x00\x00\x00\x70\x90\x90", 12, NULL);
					cheatAdd();
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e16f6), "\xF3\x0F\x11\x8F\x74\x04\x00\x00\x9F\xF6\xC4\x44", 12, NULL);
					cheatDec();
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Teleports the player to the end of a level.");

			if (ImGui::Checkbox("Instant Triggers", &setting().onInstantTriggers)) {
				if (setting().onInstantTriggers) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x89780), "\xC0", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x89780), "\xC1", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets color triggers ignore their fade time.");

			if (ImGui::Checkbox("Jump Hack", &setting().onJumpHack)) {
				if (setting().onJumpHack) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4da510), "\x01", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4da295), "\x01", 1, NULL);
					cheatAdd();
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4da510), "\x00", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4da295), "\x00", 1, NULL);
					cheatDec();
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Allows you to jump in mid-air.");

			if (ImGui::Checkbox("Noclip", &setting().onNoclip)) {
				if (setting().onNoclip) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xF04E9), "\xe9\xf0\x02\x00\x00\x90", 6, NULL);
					cheatAdd();
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xF04E9), "\x0f\x85\xef\x02\x00\x00", 6, NULL);
					cheatDec();
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Makes the player invincible.");
			ImGui::SameLine();
			if (ImGui::TreeNode("##noclipsettings")) {
				if (ImGui::Checkbox("Experimental (buggy)", &setting().onNoclipUnstuck)) {
					if (setting().onNoclipUnstuck) {
						WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdae16), "\xE9\x00\x02\x00\x00\x90", 6, NULL);
						WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdb02d), "\xE9\x10\x01\x00\x00\x90", 6, NULL);
					}
					else {
						WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdae16), "\x0F\x84\xFF\x01\x00\x00", 6, NULL);
						WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdb02d), "\x0F\x8B\x0F\x01\x00\x00", 6, NULL);
					}
				}
				if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
					ImGui::SetTooltip("Prevents the player from getting stuck.");
				ImGui::TreePop();
			}

			if (ImGui::Checkbox("Pause During Completion", &setting().onPauseDurComp)) {
				if (setting().onPauseDurComp) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E531B), "\x00", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E2906), "\x90\x90\x90\x90\x90\x90", 6, NULL);
					//cheatAdd();
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E531B), "\x01", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E2906), "\x88\x81\xf9\x02\x00\x00", 6, NULL);
					//cheatDec();
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you pause during the level complete animation.");

			ImGui::Checkbox("Practice Bug Fix", &setting().onPracticeFix);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Saves & restores player velocity in practice mode.");

			if (ImGui::Checkbox("Practice Music", &setting().onPracticeMusic)) {
				if (setting().onPracticeMusic) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F284F), "\x90\x90\x90\x90\x90\x90", 6, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F3663), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0699), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F06CB), "\x90\x90", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F3691), "\x90\x90\x90\x90\x90", 5, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F284F), "\x0f\x85\x4d\x07\x00\x00", 6, NULL); // 0f 85 4d 07 00 00
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F3663), "\x75\x41", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0699), "\x75\x3e", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F06CB), "\x75\x0c", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F3691), "\xe8\xaa\x42\xf2\xff", 5, NULL); // e8 aa 42 f2 ff
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Plays the level's song in-sync with your position.");

			ImGui::Checkbox("Replay Last Checkpoint", &setting().onLastCheckpoint);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Respawn from your last practice mode checkpoint after completing a level.");

			//ImGui::Checkbox("Show Layout", &setting().onShowLayout);
			//if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
			//    ImGui::SetTooltip("Removes all decoration and colour from levels.");
			//ImGui::SameLine();
			//if (ImGui::TreeNode("")) {
			//    ImGui::ColorEdit3("Background Color", setting().BackGroundColor, ImGuiColorEditFlags_NoInputs);
			//    ImGui::ColorEdit3("Ground Color", setting().GroundColor, ImGuiColorEditFlags_NoInputs);
			//    ImGui::TreePop();
			//}

			ImGui::SetNextItemWidth(60.f);
			if (ImGui::DragFloat("Respawn Time", &setting().respawnTime, 0.1f, 0.1f, 10.f))
				WriteRef(gd::base + 0xf04ff, setting().respawnTime);
			
			//ImGui::Checkbox("Show Layout", &setting().onShowLayout);

			ImGui::Checkbox("Smart StartPos", &setting().onSmartStartPos);

			ImGui::Checkbox("StartPos Switcher", &setting().onSPSwitcher);
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you switch between multiple start positions in-level.");
			ImGui::SameLine();
			if (ImGui::TreeNode("##startPosSwitcherSettings")) {
				ImGui::SetNextItemWidth(60.f * setting().UISize);
				ImGui::Hotkey("##prevKey", &setting().spsPrevKey);
				ImGui::SameLine();
				ImGui::Text("Previous Key");
				ImGui::SetNextItemWidth(60.f * setting().UISize);
				ImGui::Hotkey("##nextKey", &setting().spsNextKey);
				ImGui::SameLine();
				ImGui::Text("Next Key");
				ImGui::TreePop();
			}

			if (ImGui::Checkbox("Suicide", &setting().onSuicide)) {
				if (setting().onSuicide) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ea268), "\xe9\xc9\x01\x00\x00\x90", 6, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ea453), "\xe9\x59\x02\x00\x00\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ea268), "\x0f\x86\xc8\x01\x00\x00", 6, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ea453), "\x0f\x87\x58\x02\x00\x00", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Ouch.");

			ImGui::SetNextWindowSize({ ImGui::GetWindowWidth() * setting().UISize, 0 });
		}

		if (ImGui::Begin("Shortcuts", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
			ImGui::SetWindowFontScale(setting().UISize);
			ImGui::SetNextItemWidth(120 * setting().UISize);

			float winWidth = ImGui::GetWindowSize().x;

			ImGui::SetCursorPosX((winWidth - 180 * setting().UISize) / 2);
			if (ImGui::Button("Options", { 180 * setting().UISize, 0 })) {
				auto optionsLayer = gd::OptionsLayer::create();
				optionsLayer->showLayer(false);
				CCDirector::sharedDirector()->getRunningScene()->addChild(optionsLayer);
			}

			ImGui::SetCursorPosX((winWidth - 180 * setting().UISize) / 2);
			if (ImGui::Button("Restart Level", { 180 * setting().UISize, 0 })) {
				if (gd::GameManager::sharedState()->getPlayLayer()) {
					PlayLayer::resetLevel_H(gd::GameManager::sharedState()->getPlayLayer());
				}
			}

			ImGui::SetCursorPosX((winWidth - 180 * setting().UISize) / 2);
			if (ImGui::Button("Practice Mode", { 180 * setting().UISize, 0 })) {
				if (gd::GameManager::sharedState()->getPlayLayer()) {
					PlayLayer::togglePracticeModeH(gd::GameManager::sharedState()->getPlayLayer(), 0, !gd::GameManager::sharedState()->getPlayLayer()->getPracticeMode());
				}
			}

			ImGui::SetCursorPosX((winWidth - 180 * setting().UISize) / 2);
			if (ImGui::Button("Uncomplete Level", { 180 * setting().UISize, 0 })) {
				if (gd::GameManager::sharedState()->getPlayLayer()) {
					auto gsm = gd::GameStatsManager::sharedState();
					auto glm = gd::GameLevelManager::sharedState();
					gd::GJGameLevel* level = nullptr;
					level = gd::GameManager::sharedState()->getPlayLayer()->getGameLevel();
					std::cout << CCString::createWithFormat("c_{}", level->m_levelID)->getCString() << std::endl;

					if (from<int>(level, 0x1d8) >= 100 && gsm->hasCompletedLevel(level)) {
						int levelID = level->m_levelID;

						gsm->setStat("4", gsm->getStat("4") - 1);
						gsm->completedLevels()->removeObjectForKey(CCString::createWithFormat("ñ_%i", levelID)->getCString());
						if (from<int>(level, 0x1f4) > 0) {
							gsm->completedLevels()->removeObjectForKey(CCString::createWithFormat("star_%i", levelID)->getCString());
							gsm->completedLevels()->removeObjectForKey(CCString::createWithFormat("demon_%i", levelID)->getCString());
							gsm->setStat("6", gsm->getStat("6") - from<int>(level, 0x1f4));
							if (from<int>(level, 0x1f4) >= 10) {
								gsm->setStat("5", gsm->getStat("5") - 1);
							}
						}
					}

					from<int>(level, 0x1d8) = 0;
					from<int>(level, 0x1dc) = 0;

					gd::FLAlertLayer::create(nullptr, "Success", "Save & Load your data to apply the changes.", "OK", nullptr, 300.f, false, 120.f)->show();
				}
			}

			ImGui::SetCursorPosX((winWidth - 180 * setting().UISize) / 2);
			if (ImGui::Button("Inject DLL", { 180 * setting().UISize, 0 })) {
				auto selection = pfd::open_file("Select a file", CCFileUtils::sharedFileUtils()->getWritablePath2(), { "DLL File", "*.dll" }, pfd::opt::multiselect).result();
				for (auto const& filename : selection) {
					LoadLibrary(filename.c_str());
					std::filesystem::path path = filename;
					dllNames.push_back(path.filename().string());
				}
			}

			ImGui::SetCursorPosX((((winWidth - 188 * setting().UISize) + (ImGui::GetStyle().ItemSpacing.x * setting().UISize)) / 2));
			if (ImGui::Button("Resources", { 86 * setting().UISize, 0 })) {
				ShellExecute(0, NULL, getResourcesFolder().c_str(), NULL, NULL, SW_SHOW);
			}
			ImGui::SameLine();
			if (ImGui::Button("AppData", { 86 * setting().UISize, 0 })) {
				ShellExecute(0, NULL, getSongFolder().c_str(), NULL, NULL, SW_SHOW);
			}
		}

		if (ImGui::Begin("Universal", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
		{
			ImGui::SetWindowFontScale(setting().UISize);

			/*if (ImGui::DragFloat("##pitchshifter", &setting().pitchshift, 1.f, 1.f, 10.f))
				update_pitch_shifter();
			ImGui::SameLine();
			if (ImGui::Checkbox("Pitch Shifter", &setting().onPitch))
				update_pitch_shifter();
			ImGui::EndTabItem();*/

			if (ImGui::Checkbox("Allow Low Volume", &setting().onAllowLowVolume)) {
				if (setting().onAllowLowVolume) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xd0cb0), "\xeb", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xd772e), "\xeb", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xd0cb0), "\x76", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xd772e), "\x76", 1, NULL);
				}
			}

			ImGui::Checkbox("Auto Safe Mode", &setting().onAutoSafe);

			if (ImGui::Checkbox("Fast Alt-Tab", &setting().onFastAlt)) {
				if (setting().onFastAlt) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x28DFE), "\x90\x90\x90\x90\x90\x90\x90", 7, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x28DFE), "\x8b\x03\x8b\xcb\xff\x50\x18", 7, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables savefile saving on minimize.");

			if (ImGui::Checkbox("Force Visibility", &setting().onForceVis)) {
				if (setting().onForceVis) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60783), "\xB0\x01\x90", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60c9a), "\x90\x90\x90\x90\x90\x90", 6, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60783), "\x8a\x45\x08", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60c9a), "\x0f\x84\xcb\x00\x00\x00", 6, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Sets all nodes to be visible.");

			if (ImGui::Checkbox("Free Window Resize", &setting().onFreeWinReSize)) {
				if (setting().onFreeWinReSize) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x10f48b), "\x90\x90\x90\x90\x90", 5, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x10f48b), "\xe8\xb0\xf3\xff\xff", 5, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes limits in place for window resizing.");

			ImGui::Checkbox("HUE Fix", &setting().onAlwaysHue);
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Fixes that yellow and purple color bug.");

			if (ImGui::Checkbox("Ignore Slider", &setting().onIgnoreSlider)) {
				if (setting().onIgnoreSlider) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cec0), "\xc3", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cec0), "\x55", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Blocks sliders from moving.");

			if (ImGui::Checkbox("Immortal Icons", &setting().onImmortalIcons)) {
				if (setting().onImmortalIcons) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4aec50), "\xc3", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4aec50), "\x55", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Makes icons in menu invincible.");

			if (ImGui::Checkbox("Instant Game Work", &setting().onInstantGameWork)) {
				if (setting().onInstantGameWork) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x88170), "\xc0", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x88170), "\xc1", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables delays in the game.");

			if (ImGui::Checkbox("Krmal Mode", &setting().onKrmalMode)) {
				if (setting().onKrmalMode) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x472a8e), "\x8b\xd8\x90", 3, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x472a8e), "\x0f\x45\xdb", 3, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Makes level invisible.");

			ImGui::Checkbox("Lock Cursor", &setting().onLockCursor);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Locks cursor position while playing.");

			if (ImGui::Checkbox("No Rotation", &setting().onNoRotation)) {
				if (setting().onNoRotation) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60578), "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60578), "\xf3\x0f\x11\x41\x1c\xf3\x0f\x11\x41\x18", 10, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Locks all rotation at 0 degrees.");

			ImGui::Checkbox("Menu Gameplay", &setting().onMenuGameplay);

			ImGui::Checkbox("No Transition", &setting().onNoTransition);
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Shorterns scene transition time to 0s.");
			ImGui::SameLine();
			if (ImGui::TreeNode("##noTransition")) {
				if (ImGui::Checkbox("Fast Menu", &setting().onFastMenu)) {
					if (setting().onFastMenu) {
						WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x160da), "\x00\x00\x00\x00", 4, NULL);
						WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1618b), "\x00\x00\x00\x00", 4, NULL);
					}
					else {
						WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x160da), "\x00\x00\x00\x3f", 4, NULL);
						WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x1618b), "\x29\x5c\x0f\x3e", 4, NULL);
					}
				}
				if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
					ImGui::SetTooltip("Same as in 2.2, affects popups and dropdowns.");
				ImGui::TreePop();
			}

			ImGui::Checkbox("Retry Keybind", &setting().onRetryBind);
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you restart level by pressing keybind.");
			ImGui::SameLine();
			if (ImGui::TreeNode("##retryKeybind")) {
				ImGui::SetNextItemWidth(60.f * setting().UISize);
				ImGui::Hotkey("##retryKey", &setting().retryKeybind);
				ImGui::SameLine();
				ImGui::Text("Retry Key");
				ImGui::TreePop();
			}

			if (ImGui::Checkbox("Safe Mode", &setting().onSafeMode)) {
				if (setting().onSafeMode) safeModeON();
				else safeModeOFF();
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables progress and completion of levels.");

			//ImGui::Checkbox("Show Percentage", &setting().onShowPercentage);
			//if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
			//    ImGui::SetTooltip("Shows levels percentage.");

			if (ImGui::Checkbox("Show Restart Button", &setting().onRestartButton)) {
				if (setting().onRestartButton) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D64D9), "\x90\x90", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D64D9), "\x75\x29", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Shows the restart button in pause menu.");

			if (ImGui::Checkbox("Transparent BG", &setting().onTransparentBG)) {
				if (setting().onTransparentBG) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42cf96), "\x90\xb1\xff", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42cf9e), "\xff\xff", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43b7db), "\x90\xb1\xff", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43b7e3), "\xff\xff", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488132), "\x90\xb1\xff", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48813a), "\xff\xff", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48969e), "\x90\xb1\xff", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4896a5), "\xff\xff", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bde1), "\x90\xb1\xff", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bde9), "\xff\xff", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49f97c), "\x90\xb1\xff", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49f984), "\xff\xff", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42cf96), "\x80\xc9\xff", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x42cf9e), "\x00\x66", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43b7db), "\x80\xc9\xff", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43b7e3), "\x00\x66", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488132), "\x80\xc9\xff", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48813a), "\x00\x66", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48969e), "\x80\xc9\xff", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4896a5), "\x00\x66", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bde1), "\x80\xc9\xff", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49bde9), "\x00\x66", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49f97c), "\x80\xc9\xff", 3, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49f984), "\x00\x66", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes the blue filter from menu's backgrounds.");

			if (ImGui::Checkbox("Transparent Lists", &setting().onTransparentLists)) {
				if (setting().onTransparentLists) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488a4f), "\x00\x00\x00\x40", 4, NULL); // RGBA format
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48a945), "\x00\x00\x00\x40", 4, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431cba), "\x60", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c89), "\x20\x20", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c8c), "\x20", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c7f), "\x40\x40", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c82), "\x40", 1, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x488a4f), "\xbf\x72\x3e\xff", 4, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48a945), "\xbf\x72\x3e\xff", 4, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431cba), "\xff", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c89), "\xa1\x58", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c8c), "\x2c", 1, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c7f), "\xc2\x72", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x431c82), "\x3e", 1, NULL);
				}
			}
			if (ImGui::IsItemHovered()  && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Makes the menu lists transparent.");

			if (ImGui::Checkbox("Transparent Labels", &setting().onTransparentTextLabels)) {
				if (setting().onTransparentTextLabels) {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43b9af), "\x68\x00", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43BCA4), "\x68\x00", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49FC73), "\x68\x00", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49FD1C), "\x68\x00", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A00E3), "\x68\x00", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A04A6), "\x68\x00", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A06FC), "\x68\x00", 2, NULL);
				}
				else {
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43b9af), "\x68\xff", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43BCA4), "\x68\xff", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49FC73), "\x68\xff", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49FD1C), "\x68\xff", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A00E3), "\x68\xff", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A04A6), "\x68\xff", 2, NULL);
					WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A06FC), "\x68\xff", 2, NULL);
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Makes the text input areas transparent.");

			if (ImGui::Checkbox("Zero Delay", &setting().onZeroDelay)) {
				auto cocos = GetModuleHandleA("libcocos2d.dll");
				if (!setting().onZeroDelay)
					MH_DisableHook(reinterpret_cast<LPVOID>(reinterpret_cast<uintptr_t>(cocos) + 0xfc240));
				else
					MH_EnableHook(reinterpret_cast<LPVOID>(reinterpret_cast<uintptr_t>(cocos) + 0xfc240));
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Reduces input delay.");
		}

		if (ImGui::Begin("Status", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
			ImGui::SetWindowFontScale(setting().UISize);
			ImGui::SetNextItemWidth(120 * setting().UISize);

			ImGui::Checkbox("Hide All", &setting().onHideLabels);
			ImGui::SetNextItemWidth(setting().UISize * 80);
			ImGui::DragFloat("Scale", &setting().labelsScale, 0.1f, 0.1f, 1.f);
			ImGui::SetNextItemWidth(setting().UISize * 80);
			ImGui::DragInt("Opacity", &setting().labelsOpacity, 1.f, 0, 255);
			ImGui::Checkbox("Cheat Indicator", &setting().onCheatIndicator);
			ImGui::Checkbox("Attempt", &setting().onAttemptsLabel);
			ImGui::Checkbox("Best Run", &setting().onBestRunLabel);
			ImGui::Checkbox("Clock", &setting().onClock);
			ImGui::Checkbox("CPS Counter", &setting().onCPSLabel);
			ImGui::Checkbox("FPS Counter", &setting().onFPSLabel);
			ImGui::Checkbox("Message", &setting().onMessageLabel);
			ImGui::SetNextItemWidth(150 * setting().UISize);
			ImGui::InputText("##message", &setting().message);
			ImGui::Checkbox("Noclip Accuracy", &setting().onNoclipAccuracy);
			ImGui::Checkbox("Noclip Deaths", &setting().onNoclipDeaths);
			ImGui::Checkbox("Session Time", &setting().onSessionTime);
			ImGui::Checkbox("Meta", &setting().onMeta);

			ImGui::SetNextWindowSize({ ImGui::GetWindowWidth(), 0 });
		}

		if (ImGui::Begin("Icons", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
		{
			ImGui::SetWindowFontScale(setting().UISize);
			ImGui::SetNextItemWidth(120 * setting().UISize);

			ImGui::Checkbox("Same Dual Color", &setting().onSameDualColor);

			//ImGui::Checkbox("Swap Wave Trail Color", &setting().onSwapWaveTrailColors);

			ImGui::Checkbox("RGB Icon", &setting().onRainbowIcon);
			ImGui::SameLine();
			if (ImGui::TreeNode("##rgbSettings")) {
				ImGui::SetNextItemWidth(50.f);
				ImGui::InputFloat("Speed", &setting().rgbSpeed);
				ImGui::TreePop();
			}

			//ImGui::Checkbox("P1 Effects", &setting().onEnableP1);
			//ImGui::SameLine();
			//if (ImGui::TreeNode("##p1effects")) {
			//	ImGui::TreePop();
			//}

			//ImGui::Checkbox("P2 Effects", &setting().onEnableP2);
			//ImGui::SameLine();
			//if (ImGui::TreeNode("##p2effects")) {
			//	ImGui::TreePop();
			//}
		}

		if (ImGui::Begin("Interface", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
		{
			ImGui::SetWindowFontScale(setting().UISize);
			if (ImGui::ColorEdit4("Color Style", setting().Overlaycolor, ImGuiColorEditFlags_NoInputs)) {

				auto* colors = ImGui::GetStyle().Colors;

				color1.x = setting().Overlaycolor[0];
				color1.y = setting().Overlaycolor[1];
				color1.z = setting().Overlaycolor[2];
				color1.w = setting().Overlaycolor[3] - 0.4;

				color2.x = setting().Overlaycolor[0];
				color2.y = setting().Overlaycolor[1];
				color2.z = setting().Overlaycolor[2];
				color2.w = 1;

				color3.x = setting().Overlaycolor[0] + 0.3;
				color3.y = setting().Overlaycolor[1] + 0.3;
				color3.z = setting().Overlaycolor[2] + 0.3;
				color3.w = setting().Overlaycolor[3] + 0.3;

				color4.x = setting().Overlaycolor[0] - 0.1;
				color4.y = setting().Overlaycolor[1] - 0.1;
				color4.z = setting().Overlaycolor[2] - 0.1;
				color4.w = setting().Overlaycolor[3] - 0.1;

				color5.x = setting().Overlaycolor[0] + 0.1;
				color5.y = setting().Overlaycolor[1] + 0.1;
				color5.z = setting().Overlaycolor[2] + 0.1;
				color5.w = setting().Overlaycolor[3] + 0.1;

				//colors[ImGuiCol_Text] = ImVec4(0.3, 0.3, 0.3, 1);
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
				colors[ImGuiCol_Tab] = ImVec4(color5.x, color5.y, color5.z, color5.w - 0.2);
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
				colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0);
			}

			if (ImGui::ColorEdit4("Background Color", setting().BGcolor, ImGuiColorEditFlags_NoInputs)) {

				auto* colors = ImGui::GetStyle().Colors;

				color6.x = setting().BGcolor[0];
				color6.y = setting().BGcolor[1];
				color6.z = setting().BGcolor[2];
				color6.w = setting().BGcolor[3];

				colors[ImGuiCol_WindowBg] = color6;
			}

			ImGui::SetNextItemWidth(setting().UISize * 80);
			ImGui::DragFloat("UI Size", &setting().UISize, 0.01f, 0.5f, 3.25f);
			if (setting().UISize < 0.5f) setting().UISize = 0.5f;
			if (setting().UISize > 3.25f) setting().UISize = 3.25f;

			if (ImGui::Button("Sort Tabs")) {
				float addingX1, addingX2, addingX3, addingX4, addingX5, addingX6, addingX7;
				float addingInterfaceY, addingSpeedhackY, addingIconsY, addingShortcutsY;
				if (ImGui::Begin("PolzHax", nullptr)) {
					ImGui::SetWindowPos({ 5 , 5 });
					addingX1 = ImGui::GetWindowWidth() + 10;
					addingInterfaceY = ImGui::GetWindowHeight() + 10;
				}

				if (ImGui::Begin("Interface", nullptr)) {
					ImGui::SetWindowPos({ 5, addingInterfaceY });
				}

				if (ImGui::Begin("Bypass", nullptr)) {
					ImGui::SetWindowPos({ addingX1 , 5 });
					addingX2 = addingX1 + ImGui::GetWindowWidth() + 5;
				}

				if (ImGui::Begin("Cosmetic", nullptr)) {
					ImGui::SetWindowPos({ addingX2, 5 });
					addingX3 = addingX2 + ImGui::GetWindowWidth() + 5;
				}

				if (ImGui::Begin("Creator", nullptr)) {
					ImGui::SetWindowPos({ addingX3, 5 });
					addingX4 = addingX3 + ImGui::GetWindowWidth() + 5;
				}

				if (ImGui::Begin("Level", nullptr)) {
					ImGui::SetWindowPos({ addingX4 , 5 });
					addingX5 = addingX4 + ImGui::GetWindowWidth() + 5;
					addingShortcutsY = ImGui::GetWindowHeight() + 10;
				}

				if (ImGui::Begin("Shortcuts", nullptr)) {
					ImGui::SetWindowPos({ addingX4, addingShortcutsY });
				}

				if (ImGui::Begin("Universal", nullptr)) {
					ImGui::SetWindowPos({ addingX5, 5 });
					addingX6 = addingX5 + ImGui::GetWindowWidth() + 5;
				}

				if (ImGui::Begin("Status", nullptr)) {
					ImGui::SetWindowPos({ addingX6, 5 });
					addingX7 = addingX6 + ImGui::GetWindowWidth() + 5;
					addingIconsY = ImGui::GetWindowHeight() + 10;
				}

				if (ImGui::Begin("Icons", nullptr)) {
					ImGui::SetWindowPos({ addingX6, addingIconsY });
				}

				if (ImGui::Begin("Display", nullptr)) {
					ImGui::SetWindowPos({ addingX7, 5 });
					addingSpeedhackY = ImGui::GetWindowHeight() + 10;
				}

				if (ImGui::Begin("Speedhack", nullptr)) {
					ImGui::SetWindowPos({ addingX7, addingSpeedhackY });
				}
			}
		}

		if (ImGui::Begin("PolzHax", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
			ImGui::SetWindowFontScale(setting().UISize);
			ImGui::SetNextItemWidth(120 * setting().UISize);

			ImGui::Text("v1.2.0-RC.1");

			ImGui::Checkbox("Auto Save", &setting().onAutoSave);
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{
				setting().saveState();
				gd::FLAlertLayer::create(nullptr, "Saved!", "Your hack state is saved!", "Ok", nullptr, 240.f, false, 0)->show();
			}
			ImGui::EndTabItem();

			const char* const priorities[] = { "Highest", "High", "Normal", "Low", "Lowest" };
			ImGui::SetNextItemWidth(22 * setting().UISize);
			ImGui::Combo("Thread Priority", &setting().priority, priorities, 5);
			if (ImGui::IsItemDeactivatedAfterEdit()) {
				updatePriority(setting().priority);
			}

			char buffer[256];
			sprintf(buffer, "Extensions: %d", dllNames.size());
			if (ImGui::TreeNode(buffer)) {
				for (const auto& name : dllNames) {
					ImGui::Text(name.c_str());
				}
				if (ImGui::Button("Extensions Folder")) {
					ShellExecute(NULL, "open", "PolzHax\\extensions", NULL, NULL, SW_SHOWNORMAL);
				}
				ImGui::TreePop();
			}

			if (ImGui::Button("Cocos Explorer")) {
				setting().onExplorer = !setting().onExplorer;
			}

			if (ImGui::Button("Manager Viewer")) {
				setting().managerView = !setting().managerView;
			}
		}

		if (ImGui::Begin("Display", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
			ImGui::SetWindowFontScale(setting().UISize);
			ImGui::SetNextItemWidth(80 * setting().UISize);
			if (ImGui::DragFloat("##fpsbypass", &setting().fps, 1.f, 1.f, 360.f))
				update_fps_bypass();
			ImGui::SameLine();
			if (ImGui::Checkbox("FPS bypass", &setting().onFPSBypass))
				update_fps_bypass();
			ImGui::EndTabItem();

			//if (ImGui::Checkbox("Vertical Sync", &setting().onVSync)) {
			//    gd::GameManager::sharedState()->setGameVariable("0030", setting().onVSync);
			//    PlayLayer::CCApplication_toggleVerticalSync(CCApplication::sharedApplication(), setting().onVSync);
			//}

			ImGui::SetNextWindowSize({ ImGui::GetWindowWidth() * setting().UISize, 0 });
		}

		if (ImGui::Begin("Speedhack", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
		{
			ImGui::SetWindowFontScale(setting().UISize);
			ImGui::SetNextItemWidth(80 * setting().UISize);

			if (ImGui::DragFloat("##speedhack", &setting().speedhack, 0.05f, 0.f, 10.f))
			{
				update_speed_hack();
				if (setting().speedhack < 0.f) setting().speedhack = 0;
			}

			ImGui::SameLine();
			if (ImGui::Checkbox("Speedhack", &setting().onSpeedhack)) {
				update_speed_hack();
			}
			if (ImGui::Checkbox("Classic Mode", &setting().onClassicMode)) {
				update_speed_hack();
			}
			ImGui::Checkbox("Speedhack Music", &setting().onSpeedhackMusic);
		}
	}
	ImGui::End();

	update_fps_bypass();
	update_speed_hack();
	updatePriority(setting().priority);

	if (setting().onFPSBypass) {
		update_fps_bypass();
	}
}

void imgui_init() {
	setting().loadState();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF("Muli-SemiBold.ttf", 16.f);
	io.Fonts->Build();

	auto* colors = ImGui::GetStyle().Colors;

	color1.x = setting().Overlaycolor[0];
	color1.y = setting().Overlaycolor[1];
	color1.z = setting().Overlaycolor[2];
	color1.w = setting().Overlaycolor[3] - 0.4;

	color2.x = setting().Overlaycolor[0];
	color2.y = setting().Overlaycolor[1];
	color2.z = setting().Overlaycolor[2];
	color2.w = 1;

	color3.x = setting().Overlaycolor[0] + 0.3;
	color3.y = setting().Overlaycolor[1] + 0.3;
	color3.z = setting().Overlaycolor[2] + 0.3;
	color3.w = setting().Overlaycolor[3] + 0.3;

	color4.x = setting().Overlaycolor[0] - 0.1;
	color4.y = setting().Overlaycolor[1] - 0.1;
	color4.z = setting().Overlaycolor[2] - 0.1;
	color4.w = setting().Overlaycolor[3] - 0.1;

	color5.x = setting().Overlaycolor[0] + 0.1;
	color5.y = setting().Overlaycolor[1] + 0.1;
	color5.z = setting().Overlaycolor[2] + 0.1;
	color5.w = setting().Overlaycolor[3] + 0.1;

	color6.x = setting().BGcolor[0];
	color6.y = setting().BGcolor[1];
	color6.z = setting().BGcolor[2];
	color6.w = setting().BGcolor[3];

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
	colors[ImGuiCol_Tab] = ImVec4(color5.x, color5.y, color5.z, color5.w - 0.2);
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
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0);
}

void setup_imgui_menu() {
	SpeedHack::Setup();
	updatePriority(setting().priority);

	if (!std::filesystem::is_directory("PolzHax") || !std::filesystem::exists("PolzHax"))
	{
		std::filesystem::create_directory("PolzHax");
	}
	if (!std::filesystem::is_directory("PolzHax/extensions") || !std::filesystem::exists("PolzHax/extensions"))
	{
		std::filesystem::create_directory("PolzHax/extensions");
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

	auto cocos = GetModuleHandleA("libcocos2d.dll");
	if (!setting().onZeroDelay)
		MH_DisableHook(reinterpret_cast<LPVOID>(reinterpret_cast<uintptr_t>(cocos) + 0xfc240));
	else
		MH_EnableHook(reinterpret_cast<LPVOID>(reinterpret_cast<uintptr_t>(cocos) + 0xfc240));

	std::cout << "Extensions Loaded: " << dllNames.size() << std::endl;
	for (const auto& name : dllNames) {
		std::cout << name << std::endl;
	}

	ImGuiHook::setToggleCallback([]() {setting().show = !setting().show; });
	ImGuiHook::setRenderFunction(RenderMain);
	ImGuiHook::setInitFunction(imgui_init);
	ImGuiHook::setupHooks([](auto addr, auto hook, auto orig) {
		MH_CreateHook(addr, hook, orig);
		MH_EnableHook(addr);
		});
}