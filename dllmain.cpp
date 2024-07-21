#define NOMINMAX
#include "pch.h"

//#include "EditorPauseLayer.h"
#include "ObjectToolbox.h"
//#include "EditorUI.h"
#include "PauseLayer.h"
#include "EndLevelLayer.h"
//#include "Scheduler.h"

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
#include "HitboxNode.h"
#include "ObjectsIds.h"
gd::ColorSelectPopup* colorSelectPopup;

// Hitboxes thing by Taswert

//void drawRect(CCDrawNode* drawer, CCRect const& rect, ccColor4F col) {
//    constexpr size_t N = 4;
//    CCPoint vert[N];
//
//    vert[0] = CCPointMake(rect.getMinX(), rect.getMinY());
//    vert[1] = CCPointMake(rect.getMinX(), rect.getMaxY());
//    vert[2] = CCPointMake(rect.getMaxX(), rect.getMaxY());
//    vert[3] = CCPointMake(rect.getMaxX(), rect.getMinY());
//
//    auto colorA = ccc4FFromccc4B(ccc4(0, 0, 0, 0));
//
//    drawer->drawPolygon(vert, N, colorA, 0.5, col);
//}
//
//void drawTriangleObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col) {
//
//    ob->updateOrientedBox();
//    if (!ob->getOrientedBox()) return;
//
//    CCPoint triangle[3];
//    auto isFlippedX = ob->getIsFlippedX();
//    auto isFlippedY = ob->getIsFlippedY();
//    auto point1 = ob->getOrientedBox()->getPoint1();
//    auto point2 = ob->getOrientedBox()->getPoint2();
//    auto point3 = ob->getOrientedBox()->getPoint3();
//    auto point4 = ob->getOrientedBox()->getPoint4();
//
//    if (!isFlippedX && !isFlippedY) {
//        triangle[0] = point1;
//        triangle[1] = point2;
//        triangle[2] = point3;
//    }
//    else if (isFlippedX && !isFlippedY) {
//        triangle[0] = point2;
//        triangle[1] = point3;
//        triangle[2] = point4;
//    }
//    else if (!isFlippedX && isFlippedY) {
//        triangle[0] = point1;
//        triangle[1] = point2;
//        triangle[2] = point4;
//    }
//    else if (isFlippedX && isFlippedY) {
//        triangle[0] = point1;
//        triangle[1] = point3;
//        triangle[2] = point4;
//    }
//
//    drawer->drawPolygon(triangle, 3, { 0, 0, 0, 0 }, 0.5, col);
//}
//
//void drawRectObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col) {
//    ob->updateOrientedBox();
//    if (!ob->getOrientedBox()) return;
//
//    CCPoint vert[4];
//    vert[0] = ob->getOrientedBox()->getPoint1();
//    vert[1] = ob->getOrientedBox()->getPoint2();
//    vert[2] = ob->getOrientedBox()->getPoint3();
//    vert[3] = ob->getOrientedBox()->getPoint4();
//
//    drawer->drawPolygon(vert, 4, { 0, 0, 0, 0 }, 0.5, col);
//}
//
//void drawCircleObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col) {
//    ob->updateOrientedBox();
//    if (!ob->getOrientedBox()) return;
//    auto rad = ob->getObjectRadius();
//
//    constexpr size_t N = 64;
//    constexpr float PI = 3.14159265;
//
//    CCPoint vert[N];
//    for (size_t i = 0; i < N; ++i) {
//        vert[i] = ob->getOrientedBox()->getCenterPoint() + CCPointMake(std::cos(2 * PI * i / N), std::sin(2 * PI * i / N)) * rad;
//    }
//    drawer->drawPolygon(vert, N, { 0, 0, 0, 0 }, 0.5, col);
//}
//
//void drawPlayerHitbox(gd::PlayerObject* player, CCDrawNode* drawNode)
//{
//    CCPoint pointRectangle[4];
//    CCRect rectRectangle;
//    CCRect rectRectangleSmall;
//    pointRectangle[0] = player->getOrientedBox()->getPoint1();
//    pointRectangle[1] = player->getOrientedBox()->getPoint2();
//    pointRectangle[2] = player->getOrientedBox()->getPoint3();
//    pointRectangle[3] = player->getOrientedBox()->getPoint4();
//
//    auto p1x = pointRectangle[0].x - pointRectangle[1].x;
//    auto p1y = pointRectangle[0].y - pointRectangle[1].y;
//    auto distance1 = sqrt(p1x * p1x + p1y * p1y);
//
//    auto p2x = pointRectangle[1].x - pointRectangle[2].x;
//    auto p2y = pointRectangle[1].y - pointRectangle[2].y;
//    auto distance2 = sqrt(p2x * p2x + p2y * p2y);
//
//    auto distanceS1 = distance1 / 4;
//    auto distanceS2 = distance2 / 4;
//
//    rectRectangle.setRect(player->getPositionX() - distance1 / 2, player->getPositionY() - distance2 / 2, distance1, distance2);
//    rectRectangleSmall.setRect(player->getPositionX() - distanceS1 / 2, player->getPositionY() - distanceS2 / 2, distanceS1, distanceS2);
//
//    drawRect(drawNode, rectRectangleSmall, { 0, 0, 1, 1 });
//    drawNode->drawPolygon(pointRectangle, 4, { 0, 0, 0, 0 }, 0.5, { 0.5, 0, 0, 1 });
//    drawRect(drawNode, rectRectangle, { 1, 0, 0, 1 });
//}
//
//void drawObjectHitbox(gd::GameObject* obj, CCDrawNode* drawNode) {
//    if (hazard.contains(obj->getObjectID()))
//        drawRectObj(drawNode, obj, { 1, 0, 0, 1 });
//    else if (orbsnportals.contains(obj->getObjectID()))
//        drawRectObj(drawNode, obj, { 0, 1, 0, 1 });
//    else if (solids.contains(obj->getObjectID()))
//        drawRectObj(drawNode, obj, { 0, 0, 1, 1 });
//    else if (ramps.contains(obj->getObjectID()))
//        drawTriangleObj(drawNode, obj, { 0, 0, 1, 1 });
//    else if (saws.contains(obj->getObjectID()))
//        drawCircleObj(drawNode, obj, { 1, 0, 0, 1 });
//}

// End of Hitboxes thing



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

    auto sprite = gd::ButtonSprite::create("3DL", 250, 0, 0.4, false, "bigFont.fnt", "GJ_button_04.png", 25.0);
    auto button = gd::CCMenuItemSpriteExtra::create(sprite, nullptr, self, union_cast<SEL_MenuHandler>(gd::base + 0x2e600));
    button->setTag(static_cast<int>(gd::CustomColorMode::DL));
    button->setPosition(100, 0);
    from<CCArray*>(self, 0x1c4)->addObject(sprite);
    self->getMenu()->addChild(button);

    if (obj && obj->getColorMode() == gd::CustomColorMode::DL) {
        self->hightlightSelected(sprite);
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
    EndLevelLayer::mem_init();
    Scheduler::mem_init();

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
        return TRUE;
    }
