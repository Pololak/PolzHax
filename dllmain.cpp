#define NOMINMAX
#include "pch.h"
#include <DbgHelp.h>
#include <sstream>

#pragma comment(lib, "dbghelp.lib")
//#include "EditorPauseLayer.h"
#include "ObjectToolbox.h"
//#include "EditorUI.h"
#include "PauseLayer.h"
#include "EndLevelLayer.h"
//#include "Scheduler.h"
//#include "CCSchedulerHook.h"
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
//#include "HitboxNode.h"
//#include "ObjectsIds.h"
gd::ColorSelectPopup* colorSelectPopup;

// Hitboxes thing by Taswert

std::ostringstream crashInfo;

void printRegisters(CONTEXT* context) {
    crashInfo << "=== Registers ===\n";
    crashInfo << "EIP: " << std::hex << context->Eip << "\n";
    crashInfo << "ESP: " << std::hex << context->Esp << "\n";
    crashInfo << "EBP: " << std::hex << context->Ebp << "\n";
    crashInfo << "EAX: " << std::hex << context->Eax << "\n";
    crashInfo << "EBX: " << std::hex << context->Ebx << "\n";
    crashInfo << "ECX: " << std::hex << context->Ecx << "\n";
    crashInfo << "EDX: " << std::hex << context->Edx << "\n";
    crashInfo << "ESI: " << std::hex << context->Esi << "\n";
    crashInfo << "EDI: " << std::hex << context->Edi << "\n\n";
}

void printStackTrace(CONTEXT* context) {
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    SymInitialize(process, NULL, TRUE);

    STACKFRAME64 stackFrame;
    memset(&stackFrame, 0, sizeof(STACKFRAME64));

    DWORD machineType;
#ifdef _M_X64
    machineType = IMAGE_FILE_MACHINE_AMD64;
    stackFrame.AddrPC.Offset = context->Rip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context->Rsp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context->Rsp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#elif _M_IX86
    machineType = IMAGE_FILE_MACHINE_I386;
    stackFrame.AddrPC.Offset = context->Eip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context->Ebp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context->Esp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#endif

    crashInfo << "=== Stack trace ===\n";
    while (StackWalk64(machineType, process, thread, &stackFrame, context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL)) {
        DWORD64 address = stackFrame.AddrPC.Offset;
        DWORD64 moduleBase = SymGetModuleBase64(process, address);

        crashInfo << "Address: " << std::hex << address;

        // Получение информации о символе
        char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
        SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(symbolBuffer);
        symbol->MaxNameLen = MAX_SYM_NAME;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        DWORD64 displacement = 0;
        if (SymFromAddr(process, address, &displacement, symbol)) {
            crashInfo << " (" << symbol->Name << "+" << std::hex << displacement << ")";
        }
        else {
            crashInfo << " (No symbol)";
        }

        // Получение информации о модуле
        if (moduleBase) {
            IMAGEHLP_MODULE64 moduleInfo;
            memset(&moduleInfo, 0, sizeof(IMAGEHLP_MODULE64));
            moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
            if (SymGetModuleInfo64(process, moduleBase, &moduleInfo)) {
                crashInfo << " [" << moduleInfo.ModuleName << "+" << std::hex << (address - moduleBase) << "]";
            }
        }

        crashInfo << "\n";
    }

    SymCleanup(process);
}

LONG WINAPI exceptionHandler(EXCEPTION_POINTERS* exceptionInfo) {
    crashInfo << "Exception code: " << std::hex << exceptionInfo->ExceptionRecord->ExceptionCode << "\n";
    crashInfo << "Exception address: " << exceptionInfo->ExceptionRecord->ExceptionAddress << "\n\n";

    printRegisters(exceptionInfo->ContextRecord);
    printStackTrace(exceptionInfo->ContextRecord);

    MessageBoxA(NULL, crashInfo.str().c_str(), "Crash Info", MB_OK | MB_ICONERROR);

    return EXCEPTION_EXECUTE_HANDLER;
}

void setupExceptionHandler() {
    SetUnhandledExceptionFilter(exceptionHandler);
}

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

bool(__thiscall* UILayer_keyDown)(gd::UILayer* self, enumKeyCodes key);
void __fastcall UILayer_keyDown_H(gd::UILayer* self, void*, enumKeyCodes key) {
    if ((key == KEY_R) && setting().onRetryBind) {
        auto pl = gd::GameManager::sharedState()->getPlayLayer();
        if (pl) {
            pl->resetLevel();
            pl->resume();
            return;
        }
    }
    UILayer_keyDown(self, key);
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

    //gd::ColorSelectPopup* fadeTimeOk = from<gd::ColorSelectPopup*>(self, 0x1ec);

    constexpr auto handler = [](CCObject* _self, CCObject* button) {
        auto self = reinterpret_cast<gd::ColorSelectPopup*>(_self);
        auto picker = self->colorPicker();
        setting().onColorPicker = picker;
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

bool(__thiscall* ObjectToolbox_gridNodeSizeForKey)(int id);
void __stdcall ObjectToolbox_gridNodeSizeForKey_H(int id) {

    ObjectToolbox_gridNodeSizeForKey(id);
}

bool(__thiscall* CCDirector_end)(CCDirector* self);
void __fastcall CCDirector_end_H(CCDirector* self, void* edx) {
    if (setting().onAutoSave)
    {
        auto file = fopen("Resources/polzsave.dat", "wb");
        if (file) {
            fwrite(&setting(), sizeof(setting()), 1, file);
            fclose(file);
        }
    }
    CCDirector_end(self);
}

DWORD WINAPI my_thread(void* hModule) {

    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43A49B), "\xB8\x01\x00\x00\x00\x90\x90", 7, NULL); //B8 01 00 00 00 90 90
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x428bd5), "\x6a\x00", 2, NULL); // rgba8888 format (better texture quality and colors)
    //WriteProcessMemory(hModule, (LPVOID)0x518e29, "\x32", 1, NULL);

    ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F04E9), &setting().NoclipByte, 1, 0);
    if (MH_Initialize() != MH_OK) {
        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    }

   /* AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);*/

    PlayLayer::mem_init();
    EditorPauseLayer::mem_init();
    ObjectToolbox::mem_init();
    //LevelEditorLayer::mem_init();
    EditorUI::mem_init();
    PauseLayer::mem_init();
    //EndLevelLayer::mem_init();
    Scheduler::mem_init();
    //HardStreak::mem_init();
    MenuLayer::mem_init();
    //LevelSettingsLayer::mem_init();

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xff130),
        reinterpret_cast<void**>(&UILayer_keyDown_H),
        reinterpret_cast<void**>(&UILayer_keyDown));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x2dc70),
        reinterpret_cast<void**>(&CustomizeObjectLayer_init_H),
        reinterpret_cast<void**>(&CustomizeObjectLayer_init));

    

    matdash::add_hook<&ColorSelectPopup_init>(gd::base + 0x29db0);
    matdash::add_hook<&ColorSelectPopup_dtor>(gd::base + 0x2b050);

    matdash::add_hook<&LevelInfoLayer_onClone>(gd::base + 0x9e2c0);
    matdash::add_hook<&EditLevelLayer_onClone>(gd::base + 0x3da30);

    auto cocos_ext = GetModuleHandleA("libExtensions.dll");
    
    matdash::add_hook<&cocos_hsv2rgb>(GetProcAddress(cocos_ext, "?RGBfromHSV@CCControlUtils@extension@cocos2d@@SA?AURGBA@23@UHSV@23@@Z"));

    auto cocos = GetModuleHandleA("libcocos2d.dll");

    

    MH_CreateHook(
        reinterpret_cast<void*>(GetProcAddress(cocos, "?end@CCDirector@cocos2d@@QAEXXZ")),
        reinterpret_cast<void**>(&CCDirector_end_H),
        reinterpret_cast<void**>(&CCDirector_end));

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
