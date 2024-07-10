#define NOMINMAX
#include "pch.h"

#include "EditorPauseLayer.h"


#include <imgui-hook.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include "state.h"
#include "lvl-share.hpp"
//#include "preview-mode.hpp"

#include "menu.h"
#include "utils.hpp"

#include <matdash.hpp>
#include <matdash/minhook.hpp>

#include <cocos-ext.h>
#include <fstream>
#include <io.h>
#include <fcntl.h>
//#include "HitboxNode.h"
//#include "ObjectsIds.h"

CCLayer* LevelEditorLayerObject;
CCLayer* playLayer;
gd::EditorUI* editUI;
CCLayer* creatorLayer;
std::string savedClipboard;
CCLayer* PlayLayerObject;
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

CCPoint* EditorUI_getLimitedPosition(CCPoint* retVal, CCPoint point) {
    *retVal = point;
    return retVal;
}

class LevelEditorLayer : public CCLayer {
public:
    void hideui_btn(CCObject*);
};

bool hideui = false;

CCSprite* hideui_toggle_texture(CCSprite* unhide, CCSprite* hide) {
    return (hideui) ? unhide : hide;
}

void LevelEditorLayer::hideui_btn(CCObject*) {
    hideui = !hideui;
    if (hideui) {
        editUI->setVisible(0);
    }
    else {
        editUI->setVisible(1);
    }
}

bool(__thiscall* LevelEditorLayer_init)(gd::LevelEditorLayer* self, gd::GJGameLevel* level);
bool __fastcall LevelEditorLayer_init_H(gd::LevelEditorLayer* self, void*, gd::GJGameLevel* level) {
    LevelEditorLayerObject = self;
    if (!LevelEditorLayer_init(self, level)) return false;

    hideui = false;

    auto director = CCDirector::sharedDirector();
    auto menuh = CCMenu::create();

    auto unhide = CCSprite::create("BE_eye-off-btn.png");
    auto hide = CCSprite::create("BE_eye-on-btn.png");

    unhide->setOpacity(100);

    auto hideui_btn = gd::CCMenuItemToggler::create(
        hideui_toggle_texture(unhide, hide),
        hideui_toggle_texture(hide, unhide),
        self,
        menu_selector(LevelEditorLayer::hideui_btn));

    menuh->setZOrder(100);
    menuh->setScale(.5f);
    menuh->setPosition({ director->getScreenLeft() + 20, director->getScreenTop() - 100.5f });


    menuh->addChild(hideui_btn);
    self->addChild(menuh);
}

bool(__thiscall* EditorUI_init)(gd::EditorUI* self, CCLayer* editor);
bool __fastcall EditorUI_init_H(gd::EditorUI* self, void*, CCLayer* editor) {
    editUI = self;
    
    bool result = EditorUI_init(self, editor);
    
    auto sliderArr = from<CCArray*>(self, 0xA8);
    auto Slider = reinterpret_cast<gd::Slider*>(sliderArr->objectAtIndex(0));
    Slider->setPositionX(Slider->getPositionX() + 15);

    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();

    /*gd::EditorUI* selectedObject = from<gd::EditorUI*>(self, 0x220);
    auto objid = CCLabelBMFont::create("", "bigFont.fnt");
    objid->setString(CCString::createWithFormat("ID: %d", selectedObject)->getCString());
    objid->setPosition(director->getScreenLeft() + 90, director->getScreenBottom() + 80);
    objid->setScale(.4f);
    objid->setTag(4572);
    objid->setZOrder(1);
    objid->setAnchorPoint({ 0, 0.5f });
    self->addChild(objid);

    auto objcolorid = CCLabelBMFont::create("", "chatFont.fnt");
    objcolorid->setString(CCString::createWithFormat("Color ID: %d", 0)->getCString());
    objcolorid->setVisible(0);
    objcolorid->setTag(4580);
    objcolorid->setScale(0.66f);
    objcolorid->setPosition({ director->getScreenLeft() + 80, director->getScreenTop() - 60 });
    objcolorid->setAnchorPoint({ 0, 0.5f });
    self->addChild(objcolorid);*/

    if (setting().onPersClip)
    {
        if (!savedClipboard.empty()) {
            self->clipboard() = savedClipboard;
            self->updateButtons();
        }
    }

    return result;
}

bool(__thiscall* EditorUI_dtor)(gd::EditorUI* self);
bool __fastcall EditorUI_dtor_H(gd::EditorUI* self) {
    //editUI = self;
    editUI = nullptr;
    if (setting().onPersClip) savedClipboard = self->clipboard();
    EditorUI_dtor(self);
}

void(__thiscall* LevelInfoLayer_onClone)(gd::LevelInfoLayer*, CCObject*);
void __stdcall LevelInfoLayer_onClone_H(gd::LevelInfoLayer* self, CCObject* sender) {
    LevelInfoLayer_onClone(self, sender);
    if (!self->shouldDownloadLevel()) {
        auto level = static_cast<gd::GJGameLevel*>(gd::LocalLevelManager::sharedState()->getLocalLevels()->objectAtIndex(0));
        level->songID() = self->level()->songID();
    }
}

void(__thiscall* EditLevelLayer_onClone)(gd::EditLevelLayer*);
void __stdcall EditLevelLayer_onClone_H(gd::EditLevelLayer* self) {
    EditLevelLayer_onClone(self);

    auto level = static_cast<gd::GJGameLevel*>(gd::LocalLevelManager::sharedState()->getLocalLevels()->objectAtIndex(0));
    level->songID() = self->level()->songID();
}

void(__thiscall* EditorPauseLayer_onSaveAndPlay)(gd::EditorPauseLayer*, CCObject*);
void __fastcall EditorPauseLayer_onSaveAndPlay_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
    EditorPauseLayer_onSaveAndPlay(self, sender);

    editUI = nullptr;
}

void(__thiscall* EditorPauseLayer_onSaveAndExit)(gd::EditorPauseLayer*, CCObject*);
void __fastcall EditorPauseLayer_onSaveAndExit_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
    EditorPauseLayer_onSaveAndExit(self, sender);

    editUI = nullptr;
}

void(__thiscall* EditorPauseLayer_onExitNoSave)(gd::EditorPauseLayer*, CCObject*);
void __fastcall EditorPauseLayer_onExitNoSave_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
    EditorPauseLayer_onExitNoSave(self, sender);

    editUI = nullptr;
}

bool(__thiscall* UILayer_keyDown)(gd::UILayer* self, enumKeyCodes key);
void __fastcall UILayer_keyDown_H(gd::UILayer* self, void*, enumKeyCodes key) {
    if ((key == KEY_R) && setting().onRetryBind) {
        auto pl = gd::GameManager::sharedState()->getPlayLayer();
        if (pl) {
            //here, should destroy pause smh
            //pl->removeMeAndCleanup();
            pl->resetLevel();
            /*if (layers().PauseLayerObject)
            {
                layers().PauseLayerObject->removeMeAndCleanup();
                layers().PauseLayerObject = nullptr;
            }*/
            pl->resume();
            return;
        }
    }
    UILayer_keyDown(self, key);
}

void(__thiscall* Scheduler_update)(void*, float);
void __fastcall Scheduler_update_H(void* self, float dt) {

        /*if (editUI) {
            auto objid = editUI->getChildByTag(4572);
            if (objid) {
                reinterpret_cast<CCLabelBMFont*>(objid)->setString(CCString::createWithFormat("ID: %d", editUI->selectedObjectID())->getCString());
            }
            auto objcolorid = editUI->getChildByTag(4580);
            if (objcolorid) {
                if (editUI->getSingleSelectedObj() == 0) objcolorid->setVisible(0);
                else
                {
                    reinterpret_cast<CCLabelBMFont*>(objcolorid)->setString(CCString::createWithFormat("Color ID: %d", editUI->getSingleSelectedObj()->getObjectColor())->getCString());
                    objcolorid->setVisible(1);
                }
                
            }
        }*/

    Scheduler_update(self, dt);
}

bool(__thiscall* PlayLayer_init)(gd::PlayLayer* self, gd::GJGameLevel* level);
bool __fastcall PlayLayer_init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level)
{
    layers().PauseLayerObject = self;
    playLayer = self;

    if (!PlayLayer_init(self, level)) return false;

    auto director = CCDirector::sharedDirector();
    auto size = CCDirector::sharedDirector()->getWinSize();

    auto playerDrawNode = CCDrawNode::create();
    playerDrawNode->setZOrder(1000);
    playerDrawNode->setTag(124);
    self->layer()->addChild(playerDrawNode);

    auto objDrawNode = CCDrawNode::create();
    objDrawNode->setZOrder(1000);
    objDrawNode->setTag(125);
    self->layer()->addChild(objDrawNode);

    auto secarr = self->getSections();
    auto arrcount = secarr->count();

    

    if (setting().onShowPercentage)
    {
        const auto bar = gd::GameManager::sharedState()->getShowProgressBar();

        auto percentLabel = CCLabelBMFont::create("0.00%", "bigFont.fnt");
        percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
        percentLabel->setScale(0.5f);
        percentLabel->setZOrder(5);
        percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });
        percentLabel->setTag(4571);
        self->addChild(percentLabel);
    }

}

bool(__thiscall* PlayLayer_update)(gd::PlayLayer* self, float dt);
void __fastcall PlayLayer_update_H(gd::PlayLayer* self, void*, float dt) {
    layers().PauseLayerObject = nullptr;

    PlayLayer_update(self, dt);
    const auto bar = gd::GameManager::sharedState()->getShowProgressBar();

    auto size = CCDirector::sharedDirector()->getWinSize();

    auto percentLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(4571));

    if (percentLabel) {
        const auto value = self->player1()->getPositionX() / self->levelLength() * 100.f;

        percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
        percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });

        if (value < 100.0f) percentLabel->setString(CCString::createWithFormat("%.2f%%", value)->getCString());
        else percentLabel->setString(CCString::create("100.00%")->getCString());

    }

    if (setting().onHideAttempts) {
        self->attemptsLabel()->setVisible(false);
    }

    if (setting().onSameDualColor)
    {
        self->player2()->setColor(self->player1()->getColor());
        self->player2()->setSecondColor(self->player1()->getSecondColor());

        self->player2()->setGlowColor(self->player1()->getGlowColor());
        //self->player2()->setColor3(self->player1()->getColor3());

        self->player2()->setColorSecondVehicle(self->player1()->getColorSecondVehicle());
        //self->player2()->setColor5(self->player1()->getColor5());
        self->player2()->setColorGlowVehicle(self->player1()->getColorGlowVehicle());
        //self->player2()->setColor7(self->player1()->getColor7());

        self->player2()->setWaveTrailColor(self->player1()->getWaveTrailColor());
    }
    else
    {
        self->player2()->setColor(self->player1()->getSecondColor());
        self->player2()->setSecondColor(self->player1()->getColor());

        self->player2()->setGlowColor(self->player1()->getGlowColor());
        //self->player2()->setColor3(self->player1()->getColor3());

        self->player2()->setColorSecondVehicle(self->player1()->getColor());
        //self->player2()->setColor5(self->player1()->getColor5());
        self->player2()->setColorGlowVehicle(self->player1()->getGlowColor());
        //self->player2()->setColor7(self->player1()->getColor7());

        self->player2()->setWaveTrailColor(self->player1()->getSecondColor());
    }

    if (setting().onHidePlayer) {
        self->player1()->setVisible(0);
        self->player2()->setVisible(0);
    }
    else {
        self->player1()->setVisible(1);
        self->player2()->setVisible(1);
    }

}

//bool(__thiscall* HardStreak_updateStroke)(gd::HardStreak* self, float dt);
//void __fastcall HardStreak_updateStroke_H(gd::HardStreak* self, float dt) {
//    if (setting().onNoWavePulse) {
//        //self->m_pulseSize = 1;
//        dt = 1;
//    }
//    else {
//
//    }
//    
//    HardStreak_updateStroke(self, dt);
//}



bool(__thiscall* PauseLayer_customSetup)(gd::CCBlockLayer* self);
bool __fastcall PauseLayer_customSetup_H(gd::CCBlockLayer* self) {
    layers().PauseLayerObject = self;
    if (setting().onHidePauseMenu)
        self->setVisible(false);
    else self->setVisible(true);



    bool result = PauseLayer_customSetup(self);
    return result;
}

bool(__thiscall* PlayLayer_resume)(CCLayer* self);
bool __fastcall PlayLayer_resume_H(CCLayer* self)
{
    layers().PauseLayerObject = nullptr;
    bool result = PlayLayer_resume(self);
    return result;
}

bool(__thiscall* PauseLayer_onEdit)(CCLayer* self);
void __fastcall PauseLayer_onEdit_H(CCLayer* self)
{
    layers().PauseLayerObject = nullptr;
    PlayLayerObject = nullptr;
    PauseLayer_onEdit(self);
}

bool(__thiscall* PlayLayer_onQuit)(CCNode* self);
void __fastcall PlayLayer_onQuit_H(CCNode* self) {
    layers().PauseLayerObject = nullptr;
    PlayLayerObject = nullptr;
    PlayLayer_onQuit(self);
}

class ExitAlertProtocol : public gd::FLAlertLayerProtocol {
protected:

    void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
    {
        if (btn2)
        {
            gd::GameManager::sharedState()->getPlayLayer()->onQuit();
            gd::GameSoundManager::sharedState()->playSound("quitSound_01.ogg");
        }
    }
};
ExitAlertProtocol eaProtocol;

bool(__thiscall* PauseLayer_onQuit)(CCObject* btn);
void __fastcall PauseLayer_onQuit_H(CCObject* btn)
{
    PlayLayerObject = nullptr;
    if (setting().onConfirmExit) gd::FLAlertLayer::create(&eaProtocol, "Confirm", "Are you sure you want to exit the level?", "Cancel", "Exit", 320.f, false, 0)->show();
    PauseLayer_onQuit(btn);
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


bool(__thiscall* EditorUI_scrollWheel)(gd::EditorUI* _self, float dy, float dx);
void __fastcall EditorUI_scrollWheel_H(gd::EditorUI* _self, void* edx, float dy, float dx) {
    auto self = reinterpret_cast<gd::EditorUI*>(reinterpret_cast<uintptr_t>(_self) - 0xf8);
    auto layer = reinterpret_cast<gd::LevelEditorLayer*>(self->getParent())->gameLayer();
    auto zoom = layer->getScale();

    static_assert(offsetof(CCDirector, m_pKeyboardDispatcher) == 0x4c, "it wrong!");
    auto kb = CCDirector::sharedDirector()->m_pKeyboardDispatcher;
    if (kb->getControlKeyPressed()) {
        zoom = static_cast<float>(std::pow(2.71828182845904523536, std::log(std::max(zoom, 0.001f)) - dy * 0.01f));
        // zoom limit
        zoom = std::max(zoom, 0.01f);
        zoom = std::min(zoom, 1000000.f);
        self->updateZoom(zoom);
    }
    else if (kb->getShiftKeyPressed()) {
        layer->setPositionX(layer->getPositionX() - dy * 1.f);
    }
    else {
        EditorUI_scrollWheel(_self, dy, dx);
    }
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

// Unlisted Objects

void getCreateBtn() {
    return reinterpret_cast<void(__fastcall*)()>(gd::base + 0x47200)();
}

void objectAdd(int id)
{
    __asm {
        push ecx
        push 0x4
        push id
        mov ecx, ebx
        call getCreateBtn
        push eax
        mov ecx, esi
        call edi
    }
}

bool(__thiscall* ObjectToolboxAdd_RampTab)();
void __fastcall ObjectToolboxAdd_RampTab_H() {
    objectAdd(0x173);
    objectAdd(0x174);
    objectAdd(0x175);
    objectAdd(0x176);
    ObjectToolboxAdd_RampTab();
}

bool(__thiscall* ObjectToolboxAdd_SpikesTab)();
void __fastcall ObjectToolboxAdd_SpikesTab_H() {
    objectAdd(0x1A5);
    objectAdd(0x1A6);
    ObjectToolboxAdd_SpikesTab();
}

bool(__thiscall* ObjectToolboxAdd_DecoSpikesTab)();
void __fastcall ObjectToolboxAdd_DecoSpikesTab_H() {
    objectAdd(0x1A3);
    objectAdd(0x1A4);
    ObjectToolboxAdd_DecoSpikesTab();
}

bool(__thiscall* ObjectToolboxAdd_ChainTab)();
void __fastcall ObjectToolboxAdd_ChainTab_H() {
    objectAdd(0x1d2); // another brushes that don't overlaps player
    objectAdd(0x1cd);
    objectAdd(0x1ce);
    objectAdd(0x1cf);
    objectAdd(0x1d0);
    objectAdd(0x1d1);
    objectAdd(0x196); // grass
    objectAdd(0x197);
    objectAdd(0x198);
    objectAdd(0x2D5); // dot
    ObjectToolboxAdd_ChainTab();
}

bool(__thiscall* ObjectToolboxAdd_TriggerTab)();
void __fastcall ObjectToolboxAdd_TriggerTab_H() {
    objectAdd(0x37);
    objectAdd(0x8E);
    ObjectToolboxAdd_TriggerTab();
}

DWORD cocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");

DWORD WINAPI my_thread(void* hModule) {

    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43A49B), "\xB8\x01\x00\x00\x00\x90\x90", 7, NULL); //B8 01 00 00 00 90 90

    if (MH_Initialize() != MH_OK) {
        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    }

   /* AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);*/

    MH_CreateHook
    (reinterpret_cast<void*>(gd::base + 0x3f170),
        reinterpret_cast<void**>(&EditorPauseLayer_onSaveAndPlay_H),
        reinterpret_cast<void**>(&EditorPauseLayer_onSaveAndPlay));
    MH_CreateHook
    (reinterpret_cast<void*>(gd::base + 0x3f340),
        reinterpret_cast<void**>(&EditorPauseLayer_onSaveAndExit_H),
        reinterpret_cast<void**>(&EditorPauseLayer_onSaveAndExit));
    MH_CreateHook
    (reinterpret_cast<void*>(gd::base + 0x3f420),
        reinterpret_cast<void**>(&EditorPauseLayer_onExitNoSave_H),
        reinterpret_cast<void**>(&EditorPauseLayer_onExitNoSave));
  
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xe35d0),
        reinterpret_cast<void**>(&PlayLayer_init_H),
        reinterpret_cast<void**>(&PlayLayer_init));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd7f00),
        reinterpret_cast<void*>(&PauseLayer_onQuit_H),
        reinterpret_cast<void**>(&PauseLayer_onQuit));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd5f50),
        reinterpret_cast<void**>(&PauseLayer_customSetup_H),
        reinterpret_cast<void**>(&PauseLayer_customSetup));
    /*MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xcfc90),
        reinterpret_cast<void**>(&ObjectToolbox_gridNodeSizeForKey_H),
        reinterpret_cast<void**>(&ObjectToolbox_gridNodeSizeForKey));*/


    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3fb90),
        reinterpret_cast<void**>(&EditorUI_dtor_H),
        reinterpret_cast<void**>(&EditorUI_dtor));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x4ee90),
        reinterpret_cast<void**>(&EditorUI_scrollWheel_H),
        reinterpret_cast<void**>(&EditorUI_scrollWheel));
    /*MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x833e0),
        reinterpret_cast<void**>(&HardStreak_updateStroke_H), 
        reinterpret_cast<void**>(&HardStreak_updateStroke));*/

    /*MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd5f50),
        reinterpret_cast<void**>(&EditorUI_getLimitedPosition_H),
        reinterpret_cast<void**>(&EditorUI_getLimitedPosition));*/

    /*MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xad4d0),
        reinterpret_cast<void**>(&MenuGameLayer_init_H),
        reinterpret_cast<void**>(&MenuGameLayer_init));*/

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x44964),
        reinterpret_cast<void*>(&ObjectToolboxAdd_RampTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_RampTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x44f99),
        reinterpret_cast<void*>(&ObjectToolboxAdd_SpikesTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_SpikesTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x45f4b),
        reinterpret_cast<void*>(&ObjectToolboxAdd_DecoSpikesTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_DecoSpikesTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x464d6),
        reinterpret_cast<void*>(&ObjectToolboxAdd_ChainTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_ChainTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x46d37),
        reinterpret_cast<void*>(&ObjectToolboxAdd_TriggerTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_TriggerTab));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3fdc0),
        reinterpret_cast<void**>(&EditorUI_init_H),
        reinterpret_cast<void**>(&EditorUI_init));

    MH_CreateHook(
        reinterpret_cast<void*>(cocosbase + 0xff970),
        reinterpret_cast<void**>(&Scheduler_update_H),
        reinterpret_cast<void**>(&Scheduler_update));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x8c2c0),
        reinterpret_cast<void**>(&LevelEditorLayer_init_H),
        reinterpret_cast<void**>(&LevelEditorLayer_init));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf39b0),
        reinterpret_cast<void**>(&PlayLayer_resume_H),
        reinterpret_cast<void**>(&PlayLayer_resume));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf3b80),
        reinterpret_cast<void*>(&PlayLayer_onQuit_H),
        reinterpret_cast<void**>(&PlayLayer_onQuit));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd7bf0),
        reinterpret_cast<void**>(&PauseLayer_onEdit_H),
        reinterpret_cast<void**>(&PauseLayer_onEdit));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3e3d0),
        EditorPauseLayer::EditorPauseLayer_customSetup_H,
        reinterpret_cast<void**>(&EditorPauseLayer::EditorPauseLayer_customSetup));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3f570),
        EditorPauseLayer::EditorPauseLayer_keyDown_H,
        reinterpret_cast<void**>(&EditorPauseLayer::EditorPauseLayer_keyDown));


    /*MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3e41d),
        EditorPauseLayer::EditorPauseLayer_centerMenu_H,
        reinterpret_cast<void**>(&EditorPauseLayer::EditorPauseLayer_centerMenu));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3e56b),
        EditorPauseLayer::EditorPauseLayer_bottomMenu_H,
        reinterpret_cast<void**>(&EditorPauseLayer::EditorPauseLayer_bottomMenu));*/

    
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xff130),
        reinterpret_cast<void**>(&UILayer_keyDown_H),
        reinterpret_cast<void**>(&UILayer_keyDown));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xe9360),
        reinterpret_cast<void**>(&PlayLayer_update_H),
        reinterpret_cast<void**>(&PlayLayer_update));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x2dc70),
        reinterpret_cast<void**>(&CustomizeObjectLayer_init_H),
        reinterpret_cast<void**>(&CustomizeObjectLayer_init));

    /*MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x9e2c0),
        reinterpret_cast<void**>(&LevelInfoLayer_onClone_H),
        reinterpret_cast<void**>(&LevelInfoLayer_onClone));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3da30),
        reinterpret_cast<void**>(&EditLevelLayer_onClone_H),
        reinterpret_cast<void**>(&EditLevelLayer_onClone));*/

    matdash::add_hook<&EditorUI_getLimitedPosition, matdash::Stdcall>(gd::base + 0x4b500);

    matdash::add_hook<&ColorSelectPopup_init>(gd::base + 0x29db0);
    matdash::add_hook<&ColorSelectPopup_dtor>(gd::base + 0x2b050);

        auto cocos_ext = GetModuleHandleA("libExtensions.dll");
    
        matdash::add_hook<&cocos_hsv2rgb>(GetProcAddress(cocos_ext, "?RGBfromHSV@CCControlUtils@extension@cocos2d@@SA?AURGBA@23@UHSV@23@@Z"));

    auto cocos = GetModuleHandleA("libcocos2d.dll");

    //matdash::add_hook<&LevelInfoLayer_onClone>(gd::base + 0x9e2c0);
    //matdash::add_hook<&EditLevelLayer_onClone>(gd::base + 0x3da30);

    

    /*MH_CreateHook(
        reinterpret_cast<void*>(GetProcAddress(cocos, "?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z")),
        reinterpret_cast<void**>(&CCKeyboardDispatcher_dispatchKeyboardMSG_H),
        reinterpret_cast<void**>(&CCKeyboardDispatcher_dispatchKeyboardMSG));*/

    MH_CreateHook(
        reinterpret_cast<void*>(GetProcAddress(cocos, "?end@CCDirector@cocos2d@@QAEXXZ")),
        reinterpret_cast<void**>(&CCDirector_end_H),
        reinterpret_cast<void**>(&CCDirector_end));

    lvl_share::init();
    //preview_mode::init();

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
