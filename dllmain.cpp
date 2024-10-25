#define NOMINMAX
#include "pch.h"
#include <DbgHelp.h>
#include <sstream>

#pragma comment(lib, "dbghelp.lib")
#include "ObjectToolbox.h"
#include "MenuLayer.h"
#include <imgui-hook.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include "state.h"
#include "lvl-share.hpp"
#include "preview-mode.hpp"
#include "menu.h"
#include "utils.hpp"
#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <cocos-ext.h>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include "PlayLayer.h"
#include "CrashLogger.hpp"
#include "SimplePlayer.h"
#include "GJGarageLayer.h"
gd::ColorSelectPopup* colorSelectPopup;

void LevelInfoLayer_onClone(gd::LevelInfoLayer* self, CCObject* foo) {
    matdash::orig<&LevelInfoLayer_onClone>(self, foo);
    if (!self->shouldDownloadLevel()) {
        auto level = static_cast<gd::GJGameLevel*>(gd::LocalLevelManager::sharedState()->getLocalLevels()->objectAtIndex(0));
        level->songID() = self->level()->songID();
    }
}

void EditLevelLayer_onClone(gd::EditLevelLayer* self) {
    matdash::orig<&EditLevelLayer_onClone>(self);
    auto level = static_cast<gd::GJGameLevel*>(gd::LocalLevelManager::sharedState()->getLocalLevels()->objectAtIndex(0));
    level->songID() = self->level()->songID();
}

bool(__thiscall* CCKeyboardDispatcher_dispatchKeyboardMSG)(cocos2d::CCKeyboardDispatcher* self, int key, bool down);
void __fastcall CCKeyboardDispatcher_dispatchKeyboardMSG_H(CCKeyboardDispatcher* self, void* edx, int key, bool down) {
    if (down) {
        if ((key == 'R') && setting().onRetryBind) {
            auto pl = gd::GameManager::sharedState()->getPlayLayer();
            if (pl) {
                pl->resetLevel();
                if (layers().PauseLayerObject)
                {
                    layers().PauseLayerObject->removeMeAndCleanup();
                    layers().PauseLayerObject = nullptr;
                }
                pl->resume();
                return;
            }
        }
    }
    CCKeyboardDispatcher_dispatchKeyboardMSG(self, key, down);
}

class CustomizeObjectLayer : public gd::FLAlertLayer
{
public:
    void hightlightSelected(gd::ButtonSprite* spr) {
        reinterpret_cast<void(__thiscall*)(CustomizeObjectLayer*, gd::ButtonSprite*)>(gd::base + 0x2e730)(this, spr);
    }
};

bool(__thiscall* CustomizeObjectLayer_init)(CustomizeObjectLayer* self, gd::GameObject* obj, CCArray* objs);
bool __fastcall CustomizeObjectLayer_init_H(CustomizeObjectLayer* self, void* edx, gd::GameObject* obj, CCArray* objs) {
    if (!CustomizeObjectLayer_init(self, obj, objs)) return false;

    auto spr_3dl = gd::ButtonSprite::create("3D-Line", 250, 0, 0.4, false, "bigFont.fnt", "GJ_button_04.png", 25.0);
    auto btn_3dl = gd::CCMenuItemSpriteExtra::create(spr_3dl, nullptr, self, union_cast<SEL_MenuHandler>(gd::base + 0x2e600));
    btn_3dl ->setTag(static_cast<int>(gd::CustomColorMode::DL));
    btn_3dl->setPosition({ 80,5 });

    auto spr_wht = gd::ButtonSprite::create("White", 250, 0, 0.4, false, "bigFont.fnt", "GJ_button_04.png", 25.0);
    auto btn_wht = gd::CCMenuItemSpriteExtra::create(spr_wht, nullptr, self, union_cast<SEL_MenuHandler>(gd::base + 0x2e600));
    btn_wht->setTag(static_cast<int>(gd::CustomColorMode::White));
    btn_wht->setPosition({ -82, 5 });

    from<CCArray*>(self, 0x1c4)->addObject(spr_3dl);
    from<CCArray*>(self, 0x1c4)->addObject(spr_wht);

    self->getMenu()->addChild(btn_3dl);
    self->getMenu()->addChild(btn_wht);

    if (obj && obj->getColorMode() == gd::CustomColorMode::DL) {
        self->hightlightSelected(spr_3dl);
    }
    
    if (obj && obj->getColorMode() == gd::CustomColorMode::White) {
        self->hightlightSelected(spr_wht);
    }

    return true;
}

bool ColorSelectPopup_init(gd::ColorSelectPopup* self, gd::GameObject* obj, int color_id, int idk, int idk2) {

    setting().onShouldHue = true;
    if (!matdash::orig<&ColorSelectPopup_init>(self, obj, color_id, idk, idk2)) return false;

    constexpr auto handler = [](CCObject* _self, CCObject* button) {
        auto self = reinterpret_cast<gd::ColorSelectPopup*>(_self);
        auto picker = self->colorPicker();
        fuckThis().onColorPicker = picker;
        };
    auto sprite = gd::ButtonSprite::create("Picker", 0x28, 0, 0.6f, true, "goldFont.fnt", "GJ_button_04.png", 30.0);
    
    auto button = gd::CCMenuItemSpriteExtra::create(sprite, nullptr, self, to_handler<SEL_MenuHandler, handler>);
    const auto win_size = CCDirector::sharedDirector()->getWinSize();
    const auto menu = self->menu();
    button->setPosition(menu->convertToNodeSpace({ win_size.width - 50.f, win_size.height - 110 }));
    menu->addChild(button);

    return true;
}

void ColorSelectPopup_dtor(gd::ColorSelectPopup* self) {
    setting().onShouldHue = false;
    matdash::orig<&ColorSelectPopup_dtor>(self);
}

matdash::cc::c_decl<cocos2d::extension::RGBA> cocos_hsv2rgb(cocos2d::extension::HSV color) {
    if ((setting().onShouldHue || setting().onAlwaysHue) && std::isnan(color.h))
        color.h = 0.0;
    return matdash::orig<&cocos_hsv2rgb>(color);
}

void(__thiscall* CCTransitionScene_initWithDuration)(CCTransitionScene*, float, CCScene*);
void __fastcall CCTransitionScene_initWithDurationH(CCTransitionScene* self, void* edx, float duration, CCScene* scene) {
    if (setting().onNoTransition) return CCTransitionScene_initWithDuration(self, 0.f, scene);
    else CCTransitionScene_initWithDuration(self, duration, scene);
}

void(__thiscall* CCParticleSystemQuad_draw)(CCParticleSystemQuad*);
void __fastcall CCParticleSystemQuad_drawH(CCParticleSystemQuad* self) {
    if (!setting().onNoParticles) return CCParticleSystemQuad_draw(self);
}

bool(__thiscall* GameManager_isIconUnlocked)(gd::GameManager*, int, int);
bool __fastcall GameManager_isIconUnlockedH(gd::GameManager* self, void* edx, int id, int type) {
    switch (type)
    {
    case 0: if (id > 48) return true; break;
    case 1: if (id > 18) return true; break;
    case 2: if (id > 10) return true; break;
    case 3: if (id > 10) return true; break;
    case 99: if (id > 2) return true; break;
    }
    return GameManager_isIconUnlocked(self, id, type);
}

void(__thiscall* MenuGameLayer_create)(gd::MenuGameLayer*);
void __fastcall MenuGameLayer_createH(gd::MenuGameLayer* self) {
    gd::GJGameLevel* level = gd::GameLevelManager::sharedState()->getMainLevel(18, false);
    gd::PlayLayer::create(level);
}

bool(__thiscall* SupportLayer_customSetup)(gd::GJDropDownLayer*);
void __fastcall SupportLayer_customSetupH(gd::GJDropDownLayer* self) {
    SupportLayer_customSetup(self);

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto layerMain = (CCLayer*)self->getChildren()->objectAtIndex(0);

    auto menu = CCMenu::create();
    menu->setPosition(winSize.width / 2, winSize.height / 2);

    auto reqSpr = gd::ButtonSprite::create("Req", 24, 0, 0.8f, true, "bigFont.fnt", "GJ_button_04.png", 25.f);
    auto reqBtn = gd::CCMenuItemSpriteExtra::create(reqSpr, reqSpr, self, 0);
    reqBtn->setPosition(70, -90);

    layerMain->addChild(menu);
    menu->addChild(reqBtn);
}

void(__thiscall* AppDelegate_trySaveGame)(gd::AppDelegate* self);
void __fastcall AppDelegate_trySaveGame_H(gd::AppDelegate* self) {
    if (setting().onAutoSave)
    {
        auto file = fopen("Resources/polzsave.dat", "wb");
        if (file) {
            fwrite(&setting(), sizeof(setting()), 1, file);
            fclose(file);
        }
    }

    AppDelegate_trySaveGame(self);
}

DWORD WINAPI my_thread(void* hModule) {

    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43A49B), "\xB8\x01\x00\x00\x00\x90\x90", 7, NULL); // Play Music Button
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x428bd5), "\x6a\x00", 2, NULL); // rgba8888 format (better texture quality and colors)
    //WriteProcessMemory(hModule, (LPVOID)0x518e29, "\x32", 1, NULL);
    DWORD old;
    VirtualProtect(reinterpret_cast<void*>(0x518e1c), 0xF, PAGE_READWRITE, &old); // FadeTime string

    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x518e29), "\x32", 1, NULL); // FadeTime: 0.000

    ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F04E9), &setting().NoclipByte, 1, 0);
    if (MH_Initialize() != MH_OK) {
        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    }

   /* AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);*/

    auto cocos = GetModuleHandleA("libcocos2d.dll");
    auto cocos_ext = GetModuleHandleA("libExtensions.dll");

    PlayLayer::mem_init();
    EditorPauseLayer::mem_init();
    ObjectToolbox::mem_init();
    EditorUI::mem_init();
    PauseLayer::mem_init();
    EndLevelLayer::mem_init();
    Scheduler::mem_init();
    MenuLayer::mem_init();
    PlayerObject::mem_init();
    GameObject::mem_init();
    CCCircleWave::mem_init();
    MenuGameLayer::mem_init();
    SimplePlayer::mem_init();
    GJGarageLayer::mem_init();

    MH_CreateHook(
        reinterpret_cast<void*>(GetProcAddress(cocos, "?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z")),
        reinterpret_cast<void**>(&CCKeyboardDispatcher_dispatchKeyboardMSG_H),
        reinterpret_cast<void**>(&CCKeyboardDispatcher_dispatchKeyboardMSG));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x2dc70),
        reinterpret_cast<void**>(&CustomizeObjectLayer_init_H),
        reinterpret_cast<void**>(&CustomizeObjectLayer_init));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x66b10),
        reinterpret_cast<void**>(&GameManager_isIconUnlockedH),
        reinterpret_cast<void**>(&GameManager_isIconUnlocked));

    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xad430), MenuGameLayer_createH, reinterpret_cast<void**>(&MenuGameLayer_create));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xfd420), SupportLayer_customSetupH, reinterpret_cast<void**>(&SupportLayer_customSetup));

    MH_CreateHook(reinterpret_cast<void*>(GetProcAddress(cocos, "?initWithDuration@CCTransitionScene@cocos2d@@UAE_NMPAVCCScene@2@@Z")), CCTransitionScene_initWithDurationH, reinterpret_cast<void**>(&CCTransitionScene_initWithDuration));
    MH_CreateHook(reinterpret_cast<void*>(GetProcAddress(cocos, "?draw@CCParticleSystemQuad@cocos2d@@UAEXXZ")), CCParticleSystemQuad_drawH, reinterpret_cast<void**>(&CCParticleSystemQuad_draw));

    matdash::add_hook<&ColorSelectPopup_init>(gd::base + 0x29db0);
    matdash::add_hook<&ColorSelectPopup_dtor>(gd::base + 0x2b050);

    matdash::add_hook<&LevelInfoLayer_onClone>(gd::base + 0x9e2c0);
    matdash::add_hook<&EditLevelLayer_onClone>(gd::base + 0x3da30);
    
    matdash::add_hook<&cocos_hsv2rgb>(GetProcAddress(cocos_ext, "?RGBfromHSV@CCControlUtils@extension@cocos2d@@SA?AURGBA@23@UHSV@23@@Z"));

    //MH_CreateHook(
    //    reinterpret_cast<void*>(GetProcAddress(cocos, "?end@CCDirector@cocos2d@@QAEXXZ")),
    //    reinterpret_cast<void**>(&CCDirector_end_H),
    //    reinterpret_cast<void**>(&CCDirector_end));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x293f0), 
        reinterpret_cast<void**>(&AppDelegate_trySaveGame_H), 
        reinterpret_cast<void**>(&AppDelegate_trySaveGame));

    lvl_share::init();
    preview_mode::init();

    setup_imgui_menu();

    MH_EnableHook(MH_ALL_HOOKS);

    return true;
}

    BOOL APIENTRY DllMain(HMODULE hModule,
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
