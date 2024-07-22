#include "pch.h"
#include "menu.h"
//
#include <imgui-hook.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <filesystem>
#include <fstream>
#include <chrono>
#include "state.h"
#include "explorer.h"
#include "utils.hpp"

DWORD libcocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");

bool show = false;



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
    CCDirector::sharedDirector()->m_pScheduler->setTimeScale(value);
}

void update_pitch_shifter() {
    const auto value = setting().onPitch ? setting().pitchshift : 1.f;
    if (auto fme = gd::FMODAudioEngine::sharedEngine())
        if (auto sound = fme->currentSound())
            sound->setPitch(value);
        else sound->setPitch(1.f);
}

// Pitch Shifter is currently unused.

void update_fps_bypass() {
    const auto value = setting().onFPSBypass ? setting().fps : 60.f;
    static const auto addr = cocos_symbol<&CCApplication::setAnimationInterval>("?setAnimationInterval@CCApplication@cocos2d@@UAEXN@Z");
    addr(CCApplication::sharedApplication(), 1.0 / value);
    CCDirector::sharedDirector()->setAnimationInterval(1.0 / value);
}

void save() {
    auto file = fopen("Resources/polzsave.dat", "wb");
    if (file) {
        fwrite(&setting(), sizeof(setting()), 1, file);
        fclose(file);
    }
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

bool oneX = true;

void RenderMain() {
	unsigned long long pointervalue;
    
    
    
    if (oneX) {
        auto file = fopen("Resources/polzsave.dat", "rb");
        if (file) {
            fseek(file, 0, SEEK_END);
            auto size = ftell(file);

            if (size == sizeof(setting())) {
                fseek(file, 0, SEEK_SET);
                fread(&setting(), sizeof(setting()), 1, file);
                fclose(file);
            }
        }

        //ImGuiIO& io = ImGui::GetIO();
        //io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 14);

        if (ImGui::Begin("PolzHax", nullptr)) {
            ImGui::SetWindowPos({ 5,5 });
            ImGui::EndTabItem();
        }

        if (ImGui::Begin("Bypass", nullptr)) {
            ImGui::SetWindowPos({ 160,5 });
            ImGui::EndTabItem();
        }

        if (ImGui::Begin("Cosmetic", nullptr)) {
            ImGui::SetWindowPos({ 300,5 });
            ImGui::EndTabItem();
        }

        if (ImGui::Begin("Creator", nullptr)) {
            ImGui::SetWindowPos({ 491,5 });
            ImGui::EndTabItem();
        }

        if (ImGui::Begin("Level", nullptr)) {
            ImGui::SetWindowPos({ 664,5 });
            ImGui::EndTabItem();
        }

        if (ImGui::Begin("Universal", nullptr)) {
            ImGui::SetWindowPos({ 864,5 });
            ImGui::EndTabItem();
        }

        if (ImGui::Begin("Icons", nullptr)) {
            ImGui::SetWindowPos({ 1109,5 });
            ImGui::EndTabItem();
        }
        
        if (ImGui::Begin("Speedhack", nullptr)) {
            ImGui::SetWindowPos({ 491,512 });
            ImGui::EndTabItem();
        }

        if (ImGui::Begin("Status", nullptr)) {
            ImGui::SetWindowPos({ 1000, 5 });
            ImGui::EndTabItem();
        }
        
        if (ImGui::Begin("Interface", nullptr)) {
            ImGui::SetWindowPos({ 864,512 });
            ImGui::EndTabItem();
        }

        

        // Fuck this stupid save function i have no fucking words
        // Ima need to copypaste 80+ patches for fucking saves

        //WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x518e29), "\x32", 1, NULL);

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

        if (setting().onDontEnter) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EC51C), "\x90\x90", 2, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EC51C), "\x8B\xD9", 2, NULL);
        }

        if (setting().onFObjectInvisible) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebece), "\x90\x90\x90\x90\x90\x90", 6, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebece), "\x0f\x84\xd1\x02\x00\x00", 6, NULL);
        }

        if (setting().onHidePracticeBtn) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4FEE29), "\x83\xC4\x04\x90\x90\x90", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4fee6a), "\x83\xC4\x04\x90\x90\x90", 6, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4FEE29), "\xff\x92\xdc\x00\x00\x00", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4fee6a), "\xff\x92\xdc\x00\x00\x00", 6, NULL);
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

        if (setting().onNoDeathEffect) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dde71), "\xe9\xe7\x01\x00\x00\x90", 6, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dde71), "\x0f\x84\xd5\x01\x00\x00", 6, NULL);
        }

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

        if (setting().onNoParticles) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xb77f0), "\xC3", 1, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xb77f0), "\x56", 1, NULL);
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

        if (setting().onNoWaveTrail) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xe0d54), "\xeb", 1, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xe0d54), "\x74", 1, NULL);
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
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac080), "\xC3", 1, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac080), "\x56", 1, NULL);
        }

        if (setting().onTrailAlwaysOn) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC476), "\x01", 1, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC476), "\x00", 1, NULL);
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
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D62EF), "\x75\x62", 2, NULL);
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

        if (setting().onSuicide) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ea268), "\xe9\xc9\x01\x00\x00\x90", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ea453), "\xe9\x59\x02\x00\x00\x90", 6, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ea268), "\x0f\x86\xc8\x01\x00\x00", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ea453), "\x0f\x87\x58\x02\x00\x00", 6, NULL);
        }

        // Universal

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
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x472a8e), "\x0f\x45\xdb", 3, NULL);
        }

        if (setting().onNoRotation) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60578), "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60578), "\xf3\x0f\x11\x41\x1c\xf3\x0f\x11\x41\x18", 10, NULL);
        }

        if (setting().onNoTransition) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xa49a7), "\x31\xc0\x89", 3, NULL); // 31 C0 89
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xa49a7), "\xf3\x0f\x11", 3, NULL); // 31 C0 89
        }

        if (setting().onSafeMode) {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f0624), "\xeb\x6c", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e53b6), "\xe9\x77\x01\x00\x00\x90", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e5419), "\xe9\x14\x00\x00\x00\x90", 6, NULL);
        }
        else {
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f0624), "\x75\x6c", 2, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e53b6), "\x0f\x85\x76\x01\x00\x00", 6, NULL);
            WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e5419), "\x0f\x85\x13\x01\x00\x00", 6, NULL);
        }

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

        oneX = false;
    }

    //const auto& font = ImGui::GetIO().Fonts->Fonts.back();
    //ImGui::PushFont(font);

    if (show) {
        

        

        

        if (setting().onExplorer) {
            render_explorer_window(setting().onExplorer);
        }

        if (ImGui::Begin("Bypass", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
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
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Unlocks main demons levels.");

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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Lets sliders be dragged beyond the visible limit.");

            if (ImGui::Checkbox("Text Length", &setting().onTextLength)) {
                if (setting().onTextLength) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4147DE), "\xeb\x04", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4147DE), "\x7c\x04", 2, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Allows for unlimited text length in text inputs.");
        }

        if (ImGui::Begin("Cosmetic", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::SetNextItemWidth(140 * setting().UISize);

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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Forces coins show as uncollected.");

            if (ImGui::Checkbox("Coins in Practice", &setting().onCoinsPractice)) {
                if (setting().onCoinsPractice) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4eb138), "\x90\x90", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4eb138), "\x75\x4c", 2, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Allows for coins to be picked up in practice mode.");

            if (ImGui::Checkbox("Don't Enter", &setting().onDontEnter)) {
                if (setting().onDontEnter) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EC51C), "\x90\x90", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EC51C), "\x8B\xD9", 2, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables transition trigger effects.");

            if (ImGui::Checkbox("Force Objects Invisible", &setting().onFObjectInvisible)) {
                if (setting().onFObjectInvisible) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebece), "\x90\x90\x90\x90\x90\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ebece), "\x0f\x84\xd1\x02\x00\x00", 6, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Makes all objects act as invisible ones.");

            ImGui::Checkbox("Hide Attempts", &setting().onHideAttempts);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Hides the attemps counter in-game.");

            if (ImGui::Checkbox("Hide Pause Menu", &setting().onHidePauseMenu)) {
                if (layers().PauseLayerObject)
                {
                    if (setting().onHidePauseMenu) layers().PauseLayerObject->setVisible(0);
                    else layers().PauseLayerObject->setVisible(1);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Hides the pause menu.");


            ImGui::Checkbox("Hide Player", &setting().onHidePlayer);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Hides the player.");

            if (ImGui::Checkbox("Hide Practice Buttons", &setting().onHidePracticeBtn)) {
                if (setting().onHidePracticeBtn) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4FEE29), "\x83\xC4\x04\x90\x90\x90", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4fee6a), "\x83\xC4\x04\x90\x90\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4FEE29), "\xff\x92\xdc\x00\x00\x00", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4fee6a), "\xff\x92\xdc\x00\x00\x00", 6, NULL);
                }
            }
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables the mirror portal animation.");

            if (ImGui::Checkbox("Max Particles", &setting().onMaxParticles)) {
                if (setting().onMaxParticles) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xB64D7), "\x8b\x7d\x07", 3, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xB64D7), "\x8b\x7d\x08", 3, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Increases particles to the maximum.");

            if (ImGui::Checkbox("Mini Cube Icon", &setting().onMiniCube)) {
                if (setting().onMiniCube) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d8cf4), "\x31\xff\x90", 3, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d8cf4), "\x0f\x4f\xf9", 3, NULL);
                }
            }
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Makes game animations instant (e.g. buttons).");

            /*if (ImGui::Checkbox("No Background Flash", &setting().onNoBGFlash)) {
                if (setting().onNoBGFlash) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e6cd1), "\xeb\x0e\x90\x90\x90\x90", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e6c93), "\x00", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e6cd1), "\x8b\x8f\x00\x02\x00\x00", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e6c93), "\x01", 1, NULL);
                }
            }*/

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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("No visual effects on death.");

            /*if (ImGui::Checkbox("No Force Player Glow", &setting().onNoForceGlow)) {
                if (setting().onNoForceGlow) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dfce6), "\x00", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dfce6), "\x01", 1, NULL);
                }
            }*/

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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables player ghost trail triggers.");

            if (ImGui::Checkbox("No Glow", &setting().onNoGlowObject)) {
                if (setting().onNoGlowObject) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x46d8a3), "\xe9\x8f\x01\x00\x00\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x46d8a3), "\x0f\x85\x8e\x01\x00\x00", 6, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables objects glow.");

            if (ImGui::Checkbox("No Gravity Effect", &setting().onNoGravityEffect)) {
                if (setting().onNoGravityEffect) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f4586), "\x90\x90", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f4586), "\x75\x07", 2, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables gravity effect.");

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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables mirror portals.");

            if (ImGui::Checkbox("No New Best Popup", &setting().onNoNewBest)) {
                if (setting().onNoNewBest) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e5ff0), "\xc3", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e5ff0), "\x55", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables the new best popup.");

            if (ImGui::Checkbox("No Orb Ring", &setting().onNoOrbRing)) {
                if (setting().onNoOrbRing) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F6D40), "\xc3", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F6D40), "\x55", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables orb ring effect when touching it.");

            if (ImGui::Checkbox("No Particles", &setting().onNoParticles)) {
                if (setting().onNoParticles) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xb77f0), "\xC3", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xb77f0), "\x56", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables the particle system.");

            if (ImGui::Checkbox("No Portal Lightning", &setting().onNoLightning)) {
                if (setting().onNoLightning) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xe6c19), "\xeb", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xe6c19), "\x75", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables portal lightning from size-changing portals.");

            if (ImGui::Checkbox("No Pulse", &setting().onNoPulse)) {
                if (setting().onNoPulse) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x429b6a), "\x90\x90\x90\x90\x90\x90\x90\x90", 8, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x429b6a), "\xf3\x0f\x11\x87\xac\x01\x00\x00", 8, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables pulsing on objects.");

            if (ImGui::Checkbox("No Respawn Flash", &setting().onNoRespawnFlash)) {
                if (setting().onNoRespawnFlash) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DD62F), "\xE9\x99\x00\x00\x00\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DD62F), "\x0f\x85\x98\x00\x00\x00", 6, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables the respawn flash & circle effect.");

            if (ImGui::Checkbox("No Shade Effect", &setting().onNoShadeEffect)) {
                if (setting().onNoShadeEffect) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xebfe3), "\xeb", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xebfe3), "\x77", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables the disappearing effect on invisible blocks and etc.");

            //ImGui::Checkbox("No Wave Pulse", &setting().onNoWavePulse);

            if (ImGui::Checkbox("No Wave Trail", &setting().onNoWaveTrail)) {
                if (setting().onNoWaveTrail) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xe0d54), "\xeb", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xe0d54), "\x74", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables the hard wave trail.");

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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Pulses orbs & wave in practice mode.");

            if (ImGui::Checkbox("Solid Player Trail", &setting().onSolidPlayerTrail)) {
                if (setting().onSolidPlayerTrail) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac3dc), "\x90\x90\x90", 3, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac3dc), "\x89\x41\x10", 3, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("No blending on player trail.");

            if (ImGui::Checkbox("Solid Wave Trail", &setting().onSolidWaveTrail)) {
                if (setting().onSolidWaveTrail) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d9ade), "\x90\x90", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4d9ade), "\x75\x0c", 2, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("No blending on the wave trail.");

            if (ImGui::Checkbox("Trail Always Off", &setting().onTrailAlwaysOff)) {
                if (setting().onTrailAlwaysOff) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac080), "\xC3", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xac080), "\x56", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Forces player trail to be always off");

            if (ImGui::Checkbox("Trail Always On", &setting().onTrailAlwaysOn)) {
                if (setting().onTrailAlwaysOn) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC476), "\x01", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC476), "\x00", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Forces player trail to be always on.");

            if (ImGui::Checkbox("Trail Bug Fix", &setting().onTrailBugFix)) {
                if (setting().onTrailBugFix) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC29D), "\xBB\xFF\x00\x00\x00\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xAC29D), "\xf3\x0f\x2c\xc1\x2b\xd8", 6, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Fixes trail cutting on high refresh rates.");

            if (ImGui::Checkbox("Wave Trail on Death", &setting().onWaveTrailDeath)) {
                if (setting().onWaveTrailDeath) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdddfa), "\xeb", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xdddfa), "\x74", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Keeps wave trail visible on death.");
        }

        if (ImGui::Begin("Creator", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::SetNextItemWidth(120 * setting().UISize);

            

            if (ImGui::Checkbox("Absolute Rotation", &setting().onAbsolutePosition)) {
                if (setting().onAbsolutePosition) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44B49D), "\x90\x8b\xcf\x90\x90\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44B49D), "\x51\x8b\xcf\xff\x50\x5c", 6, NULL);
                }
            }
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Allows you to build out of bounds.");

            ImGui::Checkbox("Editor Preview Mode", &setting().onEditorPreview);
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Allows you to scroll out the editor.");

            ImGui::Checkbox("Global Clipboard", &setting().onPersClip);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Allows you to copy objects between levels.");

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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Hides editor grid.");

            if (ImGui::Checkbox("Hide Trigger Lines", &setting().onHideTrigLine)) {
                if (setting().onHideTrigLine) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493e08), "\xE9\xce\x00\x00\x00\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493e08), "\x0f\x84\xcd\x00\x00\x00", 6, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Hides trigger lines.");

            if (ImGui::Checkbox("Level Edit", &setting().onLevelEdit)) {
                if (setting().onLevelEdit) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D62EF), "\x90\x90", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D62EF), "\x75\x62", 2, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Lets you edit any level through the pause menu.");

            if (ImGui::Checkbox("No (C) Mark", &setting().onNoCMark)) {
                if (setting().onNoCMark) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x454aa0), "\xb8\x00\x00\x00\x00\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x454aa0), "\x8b\x81\x04\x02\x00\x00", 6, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Removes the (C) mark when uploading copied levels.");

            if (ImGui::Checkbox("No Death X", &setting().onNoDeathX)) {
                if (setting().onNoDeathX) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48c76a), "\xc7\x04\x24\x00\x00\x00\x00", 7, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48c76a), "\xc7\x04\x24\x33\x33\x33\x3f", 7, NULL);
                }
            }
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Uncaps the object limit.");

            if (ImGui::Checkbox("Place Over", &setting().onPlaceOver)) {
                if (setting().onPlaceOver) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48D37F), "\xeb", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48D37F), "\x77", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Lets you place the same object over itself in editor.");

            if (ImGui::Checkbox("Smooth Trail", &setting().onSmoothEditTrail)) {
                if (setting().onSmoothEditTrail) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x491a34), "\x90\x90", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x491a34), "\x72\x79", 2, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Makes the editor trail smoother.");

            if (ImGui::Checkbox("Verify Hack", &setting().onVerifyHack)) {
                if (setting().onVerifyHack) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43d760), "\xeb\x2f", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43d760), "\x75\x2f", 2, NULL);
                }
            }
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Lets you zoom fully in & out.");
        }

        if (ImGui::Begin("Level", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::SetNextItemWidth(120 * setting().UISize);

            /*if (ImGui::Checkbox("Ball Rotation Fix", &setting().onBallRotationFix)) {
                if (setting().onBallRotationFix) {

                }
                else {

                }
            }*/

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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Requires confirmation when exiting a level.");

            if (ImGui::Checkbox("Corrective Music Sync", &setting().onMusicSync)) {
                if (setting().onMusicSync) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ee59e), "\xeb\x08", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4ee59e), "\x75\x08", 2, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Syncs music to checked speed-portals, instead of only ones the player hit.");

            if (ImGui::Checkbox("High FPS Rotation Fix", &setting().onHiFPSRotation)) {
                if (setting().onHiFPSRotation) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dc13b), "\x90\x90\x90\x90\x90\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4dc13b), "\x0f\x82\xd6\x00\x00\x00", 6, NULL); // 0f 82 d6 00 00 00
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Fixes vehicles rotation on high fps (affects hitboxes).");

            /*if (ImGui::Checkbox("Ignore ESC", &setting().onIgnoreEsc)) {
                if (setting().onIgnoreEsc) {

                }
                else {

                }
            }*/

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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Teleports the player to the end of a level.");

            if (ImGui::Checkbox("Instant Triggers", &setting().onInstantTriggers)) {
                if (setting().onInstantTriggers) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x89780), "\xC0", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x89780), "\xC1", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Lets color triggers to ignore their fade time.");

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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Allows you for jumping in mid-air.");

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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Makes the player invincible.");

            if (ImGui::Checkbox("Pause During Completion", &setting().onPauseDurComp)) {
                if (setting().onPauseDurComp) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E531B), "\x00", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E2906), "\x90\x90\x90\x90\x90\x90", 6, NULL);
                    cheatAdd();
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E531B), "\x01", 1, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E2906), "\x88\x81\xf9\x02\x00\x00", 6, NULL);
                    cheatDec();
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Lets you pause during the level complete animation.");

            //ImGui::Checkbox("Practice Bug Fix", &setting().onPracticeFix);

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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Plays the level's song in-sync with your position.");

            ImGui::Checkbox("StartPos Switcher", &setting().onSPSwitcher);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Lets you switch between multiple start positions in-level.");

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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Ouch.");
        }

        if (ImGui::Begin("Universal", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::SetNextItemWidth(120 * setting().UISize);
            if (ImGui::DragFloat("##fpsbypass", &setting().fps, 1.f, 1.f, 360.f))
                update_fps_bypass();
            ImGui::SameLine();
            if (ImGui::Checkbox("FPS bypass", &setting().onFPSBypass))
                update_fps_bypass();
            ImGui::EndTabItem();

            /*if (ImGui::DragFloat("##pitchshifter", &setting().pitchshift, 1.f, 1.f, 10.f))
                update_pitch_shifter();
            ImGui::SameLine();
            if (ImGui::Checkbox("Pitch Shifter", &setting().onPitch))
                update_pitch_shifter();
            ImGui::EndTabItem();*/

            /*if (ImGui::Checkbox("Allow Low Volume", &setting().onAllowLowVolume)) {
                if (setting().onAllowLowVolume) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xd0cb0), "\xeb", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xd0cb0), "\x76", 1, NULL);
                }
            }*/

            if (ImGui::Checkbox("Fast Alt-Tab", &setting().onFastAlt)) {
                if (setting().onFastAlt) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x28DFE), "\x90\x90\x90\x90\x90\x90\x90", 7, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x28DFE), "\x8b\x03\x8b\xcb\xff\x50\x18", 7, NULL);
                }
            }
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Sets all nodes to be visible.");

            if (ImGui::Checkbox("Free Window Resize", &setting().onFreeWinReSize)) {
                if (setting().onFreeWinReSize) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x10f48b), "\x90\x90\x90\x90\x90", 5, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x10f48b), "\xe8\xb0\xf3\xff\xff", 5, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Removes limits in place for window resizing.");

            ImGui::Checkbox("HUE Fix", &setting().onAlwaysHue);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Fixes that yellow and purple color bug.");

            if (ImGui::Checkbox("Ignore Slider", &setting().onIgnoreSlider)) {
                if (setting().onIgnoreSlider) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cec0), "\xc3", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x41cec0), "\x55", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Blocks sliders from moving.");

            if (ImGui::Checkbox("Immortal Icons", &setting().onImmortalIcons)) {
                if (setting().onImmortalIcons) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4aec50), "\xc3", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4aec50), "\x55", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Makes icons in menu invincible.");

            if (ImGui::Checkbox("Instant Game Work", &setting().onInstantGameWork)) {
                if (setting().onInstantGameWork) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x88170), "\xc0", 1, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x88170), "\xc1", 1, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables delays in the game.");

            if (ImGui::Checkbox("Krmal Mode", &setting().onKrmalMode)) {
                if (setting().onKrmalMode) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x472a8e), "\x8b\xd8\x90", 3, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x472a8e), "\x0f\x45\xdb", 3, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Makes level invisible.");

            if (ImGui::Checkbox("No Rotation", &setting().onNoRotation)) {
                if (setting().onNoRotation) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60578), "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0x60578), "\xf3\x0f\x11\x41\x1c\xf3\x0f\x11\x41\x18", 10, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Locks all rotation at 0 degrees.");

            if (ImGui::Checkbox("No Transition", &setting().onNoTransition)) {
                if (setting().onNoTransition) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xa49a7), "\x31\xc0\x89", 3, NULL); // 31 C0 89
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(libcocosbase + 0xa49a7), "\xf3\x0f\x11", 3, NULL); // 31 C0 89
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Shorterns scene transition time to 0s.");

            ImGui::Checkbox("Retry Keybind", &setting().onRetryBind);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Lets you to restart level by pressing R.");

            if (ImGui::Checkbox("Safe Mode", &setting().onSafeMode)) {
                if (setting().onSafeMode) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f0624), "\xeb\x6c", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e53b6), "\xe9\x77\x01\x00\x00\x90", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e5419), "\xe9\x14\x00\x00\x00\x90", 6, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4f0624), "\x75\x6c", 2, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e53b6), "\x0f\x85\x76\x01\x00\x00", 6, NULL);
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4e5419), "\x0f\x85\x13\x01\x00\x00", 6, NULL);
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disables progress and completion of levels.");

            ImGui::Checkbox("Show Percentage", &setting().onShowPercentage);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Shows levels percentage.");

            if (ImGui::Checkbox("Show Restart Button", &setting().onRestartButton)) {
                if (setting().onRestartButton) {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D64D9), "\x90\x90", 2, NULL);
                }
                else {
                    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D64D9), "\x75\x29", 2, NULL);
                }
            }
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
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
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Makes the text input areas transparent.");

            //ImGui::Checkbox("Transparent Pause", &setting().onTransparentPause);
        }

        if (ImGui::Begin("Icons", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::SetNextItemWidth(120 * setting().UISize);

            ImGui::Checkbox("Same Dual Color", &setting().onSameDualColor);
        }

        if (ImGui::Begin("Speedhack", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::SetNextItemWidth(120 * setting().UISize);

            if (ImGui::DragFloat("##speedhack", &setting().speedhack, 0.05f, 0.f, 10.f))
            {
                update_speed_hack();
                if (setting().speedhack < 0.f) setting().speedhack = 0;
            }

            ImGui::SameLine();
            if (ImGui::Checkbox("Speedhack", &setting().onSpeedhack))
            {
                update_speed_hack();
            }
            ImGui::Checkbox("Speedhack Music", &setting().onSpeedhackMusic);
        }

        /*if (ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::SetNextItemWidth(120 * setting().UISize);

            ImGui::Checkbox("Cheat Indicator", &setting().onCheatIndicator);

            if (ImGui::TreeNode("FPS Label")) {
                ImGui::Checkbox("Enable##fps", &setting().onFPSLabel);
                ImGui::Checkbox("Prefix##fps", &setting().fps_prefix);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("CPS Label")) {
                ImGui::Checkbox("Enable##cps", &setting().onCPSLabel);
                ImGui::Checkbox("Prefix##cps", &setting().cps_prefix);
                ImGui::Checkbox("Total clicks", &setting().cps_total);
            }
        }*/

        if (ImGui::Begin("Interface", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize));
        {
            ImGui::SetWindowFontScale(setting().UISize);
            if (ImGui::ColorEdit4("Color Style", setting().Overlaycolor)) {

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
            }

            if (ImGui::ColorEdit4("Background Color", setting().BGcolor)) {

                auto* colors = ImGui::GetStyle().Colors;

                color6.x = setting().BGcolor[0];
                color6.y = setting().BGcolor[1];
                color6.z = setting().BGcolor[2];
                color6.w = setting().BGcolor[3];

                colors[ImGuiCol_WindowBg] = color6;
            }


            ImGui::DragFloat("UI Size", &setting().UISize, 0.01f, 0.5f, 3.25f);
            if (setting().UISize < 0.5f) setting().UISize = 0.5f;
            if (setting().UISize > 3.25f) setting().UISize = 3.25f;

            
        }

        if (ImGui::Begin("PolzHax", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
            ImGui::SetWindowFontScale(setting().UISize);
            ImGui::SetNextItemWidth(120 * setting().UISize);

            ImGui::Text("v1.1.3-alpha.2");

            ImGui::Checkbox("Auto Save", &setting().onAutoSave);
            ImGui::SameLine();
            if (ImGui::Button("Save"))
            {
                save();
                gd::FLAlertLayer::create(nullptr, "Saved!", "Your hack state is saved!", "Ok", nullptr, 240.f, false, 0)->show();
            }
            ImGui::EndTabItem();

            //ImGui::Combo("Thread Priority", setting().priority, setting().priority, 5);

            if (ImGui::Button("Special Thanks"))
            {
                gd::FLAlertLayer::create(nullptr, "Special Thanks", "<co>Taswert</c>, <cp>Capeling</c>, <cg>Mat</c>, <cj>TheSillyDoggo</c>, Absolute, <cl>HJFod</c>.", "Ok", nullptr, 320.f, false, 0)->show();
            }
            if (ImGui::Button("Cocos Explorer")) {
                setting().onExplorer = !setting().onExplorer;
            }
        }

    }
    ImGui::End();

    if (setting().onColorPicker) {
        static bool just_opened = true;
        static Color3F color;
        if (just_opened) {
            ImGui::OpenPopup("Color picker");
            just_opened = false;
            color = Color3F::from(setting().onColorPicker->getColorValue());
        }
        bool unused_open = true;
        if (ImGui::BeginPopupModal("Color picker", &unused_open,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {

            ImGui::ColorPicker3("##color_picker.color", &color.r);

            ImGui::EndPopup();
        }
        else {
            setting().onColorPicker->setColorValue(color);
            setting().onColorPicker = nullptr;
            just_opened = true;
        }
    }

    update_fps_bypass();
    update_speed_hack();

    if (setting().onFPSBypass) {
        update_fps_bypass();
    }

    //ImGui::PopFont();
}

void imgui_init() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("Muli-SemiBold.ttf", 16.f);
    io.Fonts->Build();
}

void setup_imgui_menu() {
    ImGuiHook::setToggleCallback([]() {show = !show; });
    ImGuiHook::setRenderFunction(RenderMain);
    ImGuiHook::setInitFunction(imgui_init);
    ImGuiHook::setupHooks([](auto addr, auto hook, auto orig) {
        MH_CreateHook(addr, hook, orig);
        MH_EnableHook(addr);
        });
}
//void imgui_render() {
//	const bool force = state().just_loaded;
//	const auto& font = ImGui::GetIO().Fonts->Fonts.back();
//	ImGui::PushFont(font);
//	if (state().visible || force) {
//
//		// ImGui::ShowDemoWindow();
//		auto frame_size = CCDirector::sharedDirector()->getOpenGLView()->getFrameSize();
//
//		constexpr float border = 25;
//		ImGui::SetNextWindowPos({ border, border });
//		ImGui::SetNextWindowSizeConstraints({ 0, 0 }, { frame_size.width, frame_size.height - border * 2.f });
//
//		if (ImGui::Begin("mat's nice hacks", nullptr,
//			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar)) {
//
//		}
//	}
//		ImGui::End();