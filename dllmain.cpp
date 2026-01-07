#include "pch.h"

#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

// Hooks
#include "CustomizeObjectLayer.hpp"
#include "EditButtonBar.hpp"
#include "EditorPauseLayer.hpp"
#include "EditorUI.hpp"
#include "GameObject.hpp"
#include "LevelEditorLayer.hpp"
#include "LevelSettingsLayer.hpp"
#include "MenuLayer.hpp"
#include "PauseLayer.hpp"
#include "PlayerObject.hpp"
#include "PlayLayer.hpp"
#include "SetGroupIDLayer.hpp"

// Utils
#include "patching.hpp"
#include "CrashLogger.hpp"

// Menu
#include "Menu.hpp"

#include "Setting.hpp"

inline void(__thiscall* CCTextInputNode_updateLabel)(gd::CCTextInputNode*, std::string);
void __fastcall CCTextInputNode_updateLabelH(gd::CCTextInputNode* self, void*, std::string string) {
    if (setting().onTextLength)
        self->m_maxLabelLength = 99999;

    CCTextInputNode_updateLabel(self, string);

    if (setting().onCharacterFilter) {
        self->m_allowedChars = "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789!@#$%^&*()-=_+"
            "`~[]{}/?.>,<\\|;:'\""
            " ";
        CCTextInputNode_updateLabel(self, std::move(string));
    }
}

inline void(__thiscall* CCCircleWave_draw)(gd::CCCircleWave*);
void __fastcall CCCircleWave_drawH(gd::CCCircleWave* self) {
    if (!setting().onNoEffectCircle) CCCircleWave_draw(self);
}

inline void(__thiscall* CCParticleSystemQuad_initWithTotalParticles)(CCParticleSystemQuad*, unsigned int);
void __fastcall CCParticleSystemQuad_initWithTotalParticlesH(CCParticleSystemQuad* self, void*, unsigned int amount) {
    if (setting().onNoParticles) return CCParticleSystemQuad_initWithTotalParticles(self, 0);

    CCParticleSystemQuad_initWithTotalParticles(self, amount);
}

inline void(__thiscall* AudioEffectsLayer_updateTweenAction)(gd::AudioEffectsLayer*, float, char const*);
void __fastcall AudioEffectsLayer_updateTweenActionH(gd::AudioEffectsLayer* self, void*, float idk, char const* idk2) {
    AudioEffectsLayer_updateTweenAction(self, idk, idk2);
    if (setting().onNoPulse) self->m_baseAudioScale = .5f;
}

inline void(__thiscall* FMODAudioEngine_update)(gd::FMODAudioEngine*, float);
void __fastcall FMODAudioEngine_updateH(gd::FMODAudioEngine* self, void*, float dt) {
    FMODAudioEngine_update(self, dt);
    if (setting().onNoPulse) self->m_pulse1 = .5f;
}

inline void(__thiscall* HardStreak_updateStroke)(gd::HardStreak*, float);
void __fastcall HardStreak_updateStrokeH(gd::HardStreak* self, void*, float dt) {
    if (setting().onWavePulseSize) self->m_pulseSize = setting().wavePulseSize;
    HardStreak_updateStroke(self, dt);
}

DWORD WINAPI my_thread(void* hModule) {
    AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

    if (MH_Initialize() != MH_OK) {
        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    }

    sequence_patch(gd::base + 0x28bd5, { 0x6a, 0x00 }); // RGBA8888 format.
    sequence_patch(gd::base + 0x3a49b, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90, 0x90 }); // Play Music Button.
    sequence_patch(gd::base + 0x145128, { 0x42, 0x61, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }); // Progress Bar -> Bar

    //ccTouchFixPatch();

    auto cocos = reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll"));
    auto cocos_ext = reinterpret_cast<uintptr_t>(GetModuleHandleA("libExtensions.dll"));

    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x13e90), CCTextInputNode_updateLabelH, reinterpret_cast<void**>(&CCTextInputNode_updateLabel));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xb4b0), CCCircleWave_drawH, reinterpret_cast<void**>(&CCCircleWave_draw));
    MH_CreateHook(reinterpret_cast<void*>(cocos + 0xb7b60), CCParticleSystemQuad_initWithTotalParticlesH, reinterpret_cast<void**>(&CCParticleSystemQuad_initWithTotalParticles));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x29ac0), AudioEffectsLayer_updateTweenActionH, reinterpret_cast<void**>(&AudioEffectsLayer_updateTweenAction));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x165f0), FMODAudioEngine_updateH, reinterpret_cast<void**>(&FMODAudioEngine_update));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x833e0), HardStreak_updateStrokeH, reinterpret_cast<void**>(&HardStreak_updateStroke));

    CustomizeObjectLayer::mem_init();
    EditButtonBar::mem_init();
    EditorPauseLayer::mem_init();
    EditorUI::mem_init();
    GameObject::mem_init();
    LevelEditorLayer::mem_init();
    LevelSettingsLayer::mem_init();
    MenuLayer::mem_init();
    PauseLayer::mem_init();
    PlayerObject::mem_init();
    PlayLayer::mem_init();
    RingObject::mem_init();
    SetGroupIDLayer::mem_init();

    setupImGuiMenu();

    MH_EnableHook(MH_ALL_HOOKS);

    return true;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0x1000, my_thread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    setupExceptionHandler();
    return TRUE;
}

