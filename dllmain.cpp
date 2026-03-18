#include "pch.h"

#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

// Hooks
#include "CCSchedulerHook.hpp"
#include "ColorSelectPopup.hpp"
#include "CustomizeObjectLayer.hpp"
#include "CustomSongWidget.hpp"
#include "DrawGridLayer.hpp"
#include "EditButtonBar.hpp"
#include "EditLevelLayer.hpp"
#include "EditorPauseLayer.hpp"
#include "EditorUI.hpp"
#include "EndLevelLayer.hpp"
#include "GameObject.hpp"
#include "GJGarageLayer.hpp"
#include "GJRotationControl.hpp"
#include "GJScoreCell.hpp"
#include "InfoLayer.hpp"
#include "LeaderboardsLayer.hpp"
#include "LevelBrowserLayer.hpp"
#include "LevelEditorLayer.hpp"
#include "LevelInfoLayer.hpp"
#include "LevelSearchLayer.hpp"
#include "LevelSettingsLayer.hpp"
#include "MenuGameLayer.hpp"
#include "MenuLayer.hpp"
#include "ObjectToolbox.hpp"
#include "PauseLayer.hpp"
#include "PlayerObject.hpp"
#include "PlayLayer.hpp"
#include "SetGroupIDLayer.hpp"
#include "UILayer.hpp"

// Utils
#include "patching.hpp"
#include "CrashLogger.hpp"
#include "SpeedHack.h"
#include "PitchShifter.hpp"

// Menu
#include "Menu.hpp"

#include "Setting.hpp"

#include <imgui-hook.hpp>

void(__thiscall* fpMainLoop)(cocos2d::CCDirector*);
void __fastcall hkMainLoop(cocos2d::CCDirector* self)
{
    ImGuiHook::poll(self->getOpenGLView());
    fpMainLoop(self);
}


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

inline void(__thiscall* FMODAudioEngine_playBackgroundMusic)(gd::FMODAudioEngine*, bool, FMOD::Channel*, std::string);
void __fastcall FMODAudioEngine_playBackgroundMusicH(gd::FMODAudioEngine* self, void*, bool fade, FMOD::Channel* channel, std::string path) {
    FMODAudioEngine_playBackgroundMusic(self, fade, channel, path);

    std::cout << "FMODAudioEngine::playBackgroundMusic()" << std::endl;

    if (setting().onPitchShifter) {
        PitchShifter::setPitch(setting().pitchValue);
    }
}

inline void(__thiscall* HardStreak_updateStroke)(gd::HardStreak*, float);
void __fastcall HardStreak_updateStrokeH(gd::HardStreak* self, void*, float dt) {
    if (setting().onWavePulseSize) self->m_pulseSize = setting().wavePulseSize;
    HardStreak_updateStroke(self, dt);
}

inline extension::RGBA(__cdecl* CCControlUtils_RGBfromHSV)(extension::HSV);
extension::RGBA __cdecl CCControlUtils_RGBfromHSVH(extension::HSV hsv) {
    if (setting().onHUEFix) {
        if (std::isnan(hsv.h)) {
            hsv.h = 0.0;
        }

        if (std::isnan(hsv.s)) {
            hsv.s = 0.0;
        }

        if (std::isnan(hsv.v)) {
            hsv.v = 0.0;
        }
    }

    return CCControlUtils_RGBfromHSV(hsv);
}

inline void(__thiscall* CCTransitionScene_initWithDuration)(CCTransitionScene*, float, CCScene*);
void __fastcall CCTransitionScene_initWithDurationH(CCTransitionScene* self, void*, float duration, CCScene* scene) {
    if (setting().onNoTransition) return CCTransitionScene_initWithDuration(self, 0.f, scene);
    else CCTransitionScene_initWithDuration(self, duration, scene);
}

inline bool(__thiscall* CCKeyboardDispatcher_dispatchKeyboardMSG)(CCKeyboardDispatcher*, enumKeyCodes, bool);
bool __fastcall CCKeyboardDispatcher_dispatchKeyboardMSGH(CCKeyboardDispatcher* self, void*, enumKeyCodes key, bool isDown) {
    bool ret = CCKeyboardDispatcher_dispatchKeyboardMSG(self, key, isDown);

    auto playLayer = gd::GameManager::sharedState()->getPlayLayer();
    if (playLayer && isDown) {
        if (!playLayer->m_endTriggered) {
            if ((key == setting().m_retryKeybind) && setting().onRetryKeybind) {
                if (PauseLayer::get()) {
                    PauseLayer::get()->onResume(nullptr);
                }
                playLayer->resetLevel();
            }

            if (setting().onStartPosSwitcher) {
                if (key == setting().m_previousStartPosKey) {
                    PlayLayer::prevStartPos();
                }
                if (key == setting().m_nextStartPosKey) {
                    PlayLayer::nextStartPos();
                }
            }
        }
    }

    return ret;
}

inline void(__thiscall* AppDelegate_trySaveGame)(gd::AppDelegate*);
void __fastcall AppDelegate_trySaveGameH(gd::AppDelegate* self) {
    AppDelegate_trySaveGame(self);
    if (setting().onAutoSave) {
        setting().save();
    }
    std::cout << "Saved state..." << std::endl;
}

DWORD WINAPI my_thread(void* hModule) {
    //AllocConsole();
    //freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

    if (MH_Initialize() != MH_OK) {
        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    }

    sequence_patch(gd::base + 0x28bd5, { 0x6a, 0x00 }); // RGBA8888 format.
    sequence_patch(gd::base + 0x3a49b, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90, 0x90 }); // Play Music Button.
    sequence_patch(gd::base + 0x145128, { 0x42, 0x61, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }); // Progress Bar -> Bar
    sequence_patch(gd::base + 0x3a669, { 0x00, 0x00, 0x00, 0x43 }); // CustomSongWidget m_artistLabel->limitLabelWidth(120.f, ...)

    SpeedHack::Setup();

    auto cocos = reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll"));
    auto cocos_ext = reinterpret_cast<uintptr_t>(GetModuleHandleA("libExtensions.dll"));

    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x13e90), CCTextInputNode_updateLabelH, reinterpret_cast<void**>(&CCTextInputNode_updateLabel));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xb4b0), CCCircleWave_drawH, reinterpret_cast<void**>(&CCCircleWave_draw));
    MH_CreateHook(reinterpret_cast<void*>(cocos + 0xb7b60), CCParticleSystemQuad_initWithTotalParticlesH, reinterpret_cast<void**>(&CCParticleSystemQuad_initWithTotalParticles));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x29ac0), AudioEffectsLayer_updateTweenActionH, reinterpret_cast<void**>(&AudioEffectsLayer_updateTweenAction));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x165f0), FMODAudioEngine_updateH, reinterpret_cast<void**>(&FMODAudioEngine_update));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x16850), FMODAudioEngine_playBackgroundMusicH, reinterpret_cast<void**>(&FMODAudioEngine_playBackgroundMusic));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x833e0), HardStreak_updateStrokeH, reinterpret_cast<void**>(&HardStreak_updateStroke));
    MH_CreateHook(reinterpret_cast<void*>(cocos_ext + 0xcee0), CCControlUtils_RGBfromHSVH, reinterpret_cast<void**>(&CCControlUtils_RGBfromHSV));
    MH_CreateHook(reinterpret_cast<void*>(cocos + 0xa4990), CCTransitionScene_initWithDurationH, reinterpret_cast<void**>(&CCTransitionScene_initWithDuration));
    MH_CreateHook(reinterpret_cast<void*>(cocos + 0x97d50), CCKeyboardDispatcher_dispatchKeyboardMSGH, reinterpret_cast<void**>(&CCKeyboardDispatcher_dispatchKeyboardMSG));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x293f0), AppDelegate_trySaveGameH, reinterpret_cast<void**>(&AppDelegate_trySaveGame));

    CCSchedulerHook::mem_init();
    ColorSelectPopup::mem_init();
    CustomizeObjectLayer::mem_init();
    CustomSongWidget::mem_init();
    DrawGridLayer::mem_init();
    EditButtonBar::mem_init();
    EditLevelLayer::mem_init();
    EditorPauseLayer::mem_init();
    EditorUI::mem_init();
    EndLevelLayer::mem_init();
    GameObject::mem_init();
    GJGarageLayer::mem_init();
    //GJRotationControl::mem_init();
    GJScoreCell::mem_init();
    InfoLayer::mem_init();
    LeaderboardsLayer::mem_init();
    LevelBrowserLayer::mem_init();
    LevelEditorLayer::mem_init();
    LevelInfoLayer::mem_init();
    //LevelSearchLayer::mem_init();
    LevelSettingsLayer::mem_init();
    //MenuGameLayer::mem_init();
    MenuLayer::mem_init();
    ObjectToolbox::mem_init();
    PauseLayer::mem_init();
    PlayerObject::mem_init();
    PlayLayer::mem_init();
    RingObject::mem_init();
    SetGroupIDLayer::mem_init();
    UILayer::mem_init();

    setupImGuiMenu();

    MH_EnableHook(MH_ALL_HOOKS);

    MH_CreateHook(reinterpret_cast<LPVOID*>(reinterpret_cast<uintptr_t>(GetModuleHandleA("libcocos2d.dll")) + 0xfc240), hkMainLoop, reinterpret_cast<LPVOID*>(&fpMainLoop));

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

