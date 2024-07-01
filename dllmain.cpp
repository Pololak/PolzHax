#include "pch.h"

#include "PauseLayer.h"
#include "MenuLayer.h"
#include "EditorPauseLayer.h"
#include "PlayLayer.h"

#include <imgui-hook.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include "state.h"

#include "menu.h"

//gd::EditorUI* editUI;
CCLayer* LevelEditorLayerObject;
CCLayer* playLayer;
gd::EditorUI* editUI;
CCLayer* creatorLayer;
static constexpr const int MENU_TAG = 0x44; // it was for clickable dots from BEv4, but i dropped it


// Input Lag Reducer // Begin

//void(__thiscall* fpMainLoop)(cocos2d::CCDirector* self);
//void(__thiscall* fpPollEvents)(cocos2d::CCEGLView* self);
//
//void(__fastcall hkMainLoop)(cocos2d::CCDirector* self) {
//    fpPollEvents(self->getOpenGLView());
//    fpMainLoop(self);
//}
//void __fastcall hkPollEvents(void* self) {};

// End

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

CCMenu* editorUIRightMenu = nullptr;

bool(__thiscall* EditorUI_init)(gd::EditorUI* self, CCLayer* editor);
bool __fastcall EditorUI_init_H(gd::EditorUI* self, void*, CCLayer* editor) {
    editUI = self;
    editorUIRightMenu = nullptr;
    bool result = EditorUI_init(self, editor);

    auto rMenuArr = from<CCArray*>(editorUIRightMenu, 0xA8);

    auto copyColorSpr = CCSprite::createWithSpriteFrameName("GJ_copyBtn_001.png");
    copyColorSpr->setScale(.85f);
    auto copyColorBtn = gd::CCMenuItemSpriteExtra::create(copyColorSpr, nullptr, 0, 0);
    copyColorBtn->setPosition({ -95, -44 });
    auto pasteColorSpr = CCSprite::createWithSpriteFrameName("GJ_pasteBtn_001.png");
    pasteColorSpr->setScale(.85f);
    auto pasteColorBtn = gd::CCMenuItemSpriteExtra::create(pasteColorSpr, nullptr, 0, 0);
    pasteColorBtn->setPosition({ -95, -88 });

    editorUIRightMenu->addChild(pasteColorBtn);
    editorUIRightMenu->addChild(copyColorBtn);
    

    auto sliderArr = from<CCArray*>(self, 0xA8);
    auto Slider = reinterpret_cast<gd::Slider*>(sliderArr->objectAtIndex(0));
    Slider->setPositionX(Slider->getPositionX() + 15);

    auto EditButtonBar_edit = reinterpret_cast<gd::EditButtonBar*>(sliderArr->objectAtIndex(19));
    //EditButtonBar_edit->setPositionX(EditButtonBar_edit->getPositionX() - 45);

    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();
    auto menuEUI = CCMenu::create();

    auto editorlbl = CCLabelBMFont::create("", "chatFont.fnt");
    editorlbl->setString(CCString::createWithFormat("EditorUI: %p", self)->getCString());
    editorlbl->setAnchorPoint({ 1.f, 0.5f });
    editorlbl->setPosition(CCDirector::sharedDirector()->getScreenRight() - 120, 130);
    editorlbl->setScale(0.5f);
    self->addChild(editorlbl);

    auto lvllbl = CCLabelBMFont::create("", "chatFont.fnt");
    lvllbl->setString(CCString::createWithFormat("LevelEditorLayer: %p", self->getLevelEditorLayer())->getCString());
    lvllbl->setAnchorPoint({ 1.f, 0.5f });
    lvllbl->setPosition(CCDirector::sharedDirector()->getScreenRight() - 120, 120);
    lvllbl->setScale(0.5f);
    lvllbl->setTag(4571);
    self->addChild(lvllbl);

    gd::EditorUI* selectedObject = from<gd::EditorUI*>(self, 0x220);
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
    self->addChild(objcolorid);

    

    //hideui_btn->setPosition({ director->getScreenLeft(), director->getScreenTop() });
    //toggleOff->setOpacity(100);

    auto advDelBtn = gd::ButtonSprite::create("Delete", 50, 0, 0.4f, false, "bigFont.fnt", "GJ_button_04.png", 50);

    auto DelSpr = CCSprite::create("GJ_trashBtn_001.png");
    auto DelBtn = gd::CCMenuItemSpriteExtra::create(DelSpr, nullptr, self, menu_selector(gd::EditorUI::onDeleteSelected));

   

    //director->getRunningScene()->addChild(menuEUI);

    menuEUI->setPosition({ director->getScreenLeft() + 104, director->getScreenTop() - 32.75f });
    menuEUI->setScale(0.925f);

    menuEUI->addChild(DelBtn);
    self->addChild(menuEUI);

    return result;
}

bool(__thiscall* EditorUI_rightMenu)();
void __fastcall EditorUI_rightMenu_H() {
    _asm {
        mov editorUIRightMenu, eax
    }
    EditorUI_rightMenu();
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

//class MenuGameDelegate : public CCLayer {
//public:
//    gd::MenuGameLayer* mgl = nullptr;
//    static inline MenuGameDelegate* instance = nullptr;;
//
//    bool init() {
//        if (!CCLayer::init()) return false;
//            
//            instance = this;
//
//            this->setKeyboardEnabled(true);
//            this->setTouchEnabled(true);
//
//            CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -69, true);
//
//            return true;
//    }
//    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
//        if (mgl) {
//            if (pTouch ? mgl->ccTouchBegan(pTouch, pEvent) : true) {
//                if (mgl->m_playerObject)
//                    mgl->m_playerObject->pushButton(1);
//                return true;
//            }
//        }
//        return false;
//    }
//
//    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent) {
//        if (mgl->m_playerObject)
//            mgl->m_playerObject->releaseButton(1);
//    }
//
//    ~MenuGameDelegate() {
//        instance = nullptr;
//    }
//
//    CREATE_FUNC(MenuGameDelegate);
//};
//
//bool(__thiscall* MenuGameLayer_init)(CCLayer* self);
//bool __fastcall MenuGameLayer_init_H(CCLayer* self) {
//    if (!MenuGameLayer_init) return false;
//    
//}

void(__thiscall* Scheduler_update)(void*, float);
void __fastcall Scheduler_update_H(void* self, float dt) {

        if (editUI) {
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
        }

    Scheduler_update(self, dt);
}



class OptionsLayer : public CCLayer {
public:
    void FLAlertRepBtn(CCObject*);
};

void OptionsLayer::FLAlertRepBtn(CCObject*) {
    auto flalertrep = gd::FLAlertLayer::create(nullptr, "Unavailable", "Gameplay recording is not supported on this device.", "OK", nullptr, 250.0f, false, 0);
    flalertrep->show();
}

bool(__thiscall* OptionsLayer_customSetup)(CCLayer* self);
void __fastcall OptionsLayer_customSetup_H(CCLayer* self) {

    auto layerMain = (cocos2d::CCLayer*)self->getChildren()->objectAtIndex(0);

    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();
    auto menu = CCMenu::create();

    auto input = gd::CCTextInputNode::create("Text", nullptr, "bigFont.fnt", 30.f, 20.f);

    auto RepLabel = CCLabelBMFont::create("Record\nReplays", "bigFont.fnt", 0.f, kCCTextAlignmentCenter);
    auto RepSpr = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    auto RepBtn = gd::CCMenuItemSpriteExtra::create(RepSpr, nullptr, self, menu_selector(OptionsLayer::FLAlertRepBtn));

    RepSpr->setScale(.8f);
    RepLabel->setScale(.4f);

    menu->setPosition({ size.width - 145, size.height - 200 });
    RepLabel->setPosition({ size.width - 145, size.height - 228 });

    menu->addChild(RepBtn);

    layerMain->addChild(RepLabel);
    layerMain->addChild(menu);

    //layerMain->addChild(input);

    OptionsLayer_customSetup(self);
}

//bool(__thiscall* ObjectToolbox_init)(CCNode* self);
//bool __fastcall ObjectToolbox_init_H(CCNode* self) {
//    if (!ObjectToolbox_init(self)) return false;
//
//    
//
//    return true;
//}

bool(__thiscall* SupportLayer_customSetup)(CCLayer* self);
void __fastcall SupportLayer_customSetup_H(CCLayer* self) {

    auto SupportLayerMain = (cocos2d::CCLayer*)self->getChildren()->objectAtIndex(0);

    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();

    auto PolzLabel = CCLabelBMFont::create("PolzHax by Pololak", "goldFont.fnt");

    PolzLabel->setPosition({ size.width / 2, size.height / 2 - 70});
    PolzLabel->setScale(.4f);

    SupportLayerMain->addChild(PolzLabel);

    SupportLayer_customSetup(self);
}

bool(__thiscall* PlayLayer_init)(gd::PlayLayer* self, gd::GJGameLevel* level);
bool __fastcall PlayLayer_init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level)
{
    playLayer = self;

    if (!PlayLayer_init(self, level)) return false;

    auto director = CCDirector::sharedDirector();
    auto size = CCDirector::sharedDirector()->getWinSize();

    auto cheatindicator = CCLabelBMFont::create(".", "bigFont.fnt");
    cheatindicator->setPosition({ director->getScreenLeft() + 8, director->getScreenTop() + 2 });
    cheatindicator->setScale(1.f);
    cheatindicator->setZOrder(100);
    cheatindicator->setColor({ 0, 255, 0 });

    auto lol = CCLabelBMFont::create("(totally legit)", "bigFont.fnt");
    lol->setPosition({ director->getScreenLeft() + 38, director->getScreenTop() - 7.5f });
    lol->setScale(.2f);
    lol->setZOrder(100);
    lol->setOpacity(100);

    auto polzlabel = CCLabelBMFont::create("PolzHax Alpha", "bigFont.fnt");
    polzlabel->setPosition({ director->getScreenLeft() + 55, director->getScreenTop() - 18 });
    polzlabel->setOpacity(100);
    polzlabel->setZOrder(100);
    polzlabel->setScale(.4f);

    self->addChild(polzlabel);
    self->addChild(cheatindicator);
    self->addChild(lol);
}

bool(__thiscall* ColorSelectPopup_init)(gd::ColorSelectPopup* self, gd::GameObject* obj, int colorid, int idk, int idk2);
void __fastcall ColorSelectPopup_init_H(gd::ColorSelectPopup* self, gd::GameObject* obj, int colorid, int idk, int idk2) {

    auto sprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    const auto menu = self->menu();
    //menu->addChild(sprite);

    ColorSelectPopup_init(self, obj, colorid, idk, idk2);
}

bool(__thiscall* ColorSelectPopup_dtor)(gd::ColorSelectPopup* self);
void __fastcall ColorSelectPopup_dtor_H(gd::ColorSelectPopup* self) {
    ColorSelectPopup_dtor(self);
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
    objectAdd(0x196);
    objectAdd(0x197);
    objectAdd(0x198);
    objectAdd(0x2D5);
    ObjectToolboxAdd_ChainTab();
}

bool(__thiscall* ObjectToolboxAdd_TriggerTab)();
void __fastcall ObjectToolboxAdd_TriggerTab_H() {
    objectAdd(0x37);
    objectAdd(0x8E);
    ObjectToolboxAdd_TriggerTab();
}

bool(__thiscall* ObjectToolboxAdd_RingTab)();
void __fastcall ObjectToolboxAdd_RingTab_H() {
    objectAdd(0x37);
    ObjectToolboxAdd_TriggerTab();
}

DWORD WINAPI my_thread(void* hModule) {

    // Level Bypass

    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A81AF), "\xE9\xA3\x02\x00\x00\x90", 6, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4A8C22), "\xE9\x98\x00\x00\x00\x90", 6, NULL);

    // Icon Bypass

    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x48000F), "\xE9\x74\x02\x00\x00\x90", 6, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E606), "\xE9\xDC\x00\x00\x00\x90", 6, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E098), "\x90\x90", 2, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E128), "\x90\x90", 2, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E1B8), "\x90\x90", 2, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E248), "\x90\x90", 2, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47E2D8), "\x90\x90", 2, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47e9be), "\xEB\x13", 2, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x47eaae), "\xEB\x13", 2, NULL);

    // fast alt-tab
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x28DFE), "\x90\x90\x90\x90\x90\x90\x90", 7, NULL);
    
    // no respawn blink
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DD62F), "\xE9\x99\x00\x00\x00\x90", 6, NULL);
    
    // no death effect
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4DDE71), "\xE9\xE7\x01\x00\x00\x90", 6, NULL);
    
    // esc during completion
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E531B), "\x00", 1, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4E2906), "\x90\x90\x90\x90\x90\x90", 6, NULL);
    
    // practice pulse
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x429975), "\x90\x90", 2, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EB441), "\xEB\x16", 2, NULL);

    // no orb ring
    //WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0xF6D40), "\xC3", 1, NULL);

    // Practice Music Hack
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F284F), "\x90\x90\x90\x90\x90\x90", 6, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F3663), "\x90\x90", 2, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F0699), "\x90\x90", 2, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F06CB), "\x90\x90", 2, NULL);
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F3691), "\x90\x90\x90\x90\x90", 5, NULL);

    // Restart Button
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4D64D9), "\x90\x90", 2, NULL);



    // # Editor Tweaks #

    // Free Scroll (Top)
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x44CA8C), "\xEB\x05", 2, NULL);

    // Grid Extension
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493861), "\x00\x20\x73", 3, NULL); // Grid V1
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x49399B), "\x00\x20\x73", 3, NULL); // Grid V2
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4939D3), "\x00\x20\x73", 3, NULL); // Grid V3
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493AED), "\x00\x20\x73", 3, NULL); // Grid H
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494112), "\x00\x20\x73", 3, NULL); // Portal Lines (Top)
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x494073), "\x00\x20\x73", 3, NULL); // Portal Lines (Bottom)
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493C72), "\x00\x20\x73", 3, NULL); // Music Lines

    // Play Music Button on LevelInfoLayer
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43A49B), "\xB8\x01\x00\x00\x00\x90\x90", 7, NULL); //B8 01 00 00 00 90 90

    // ImGui UI Settings




    DWORD cocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(cocosbase + 0xAC29D), "\xBB\xFF\x00\x00\x00\x90", 6, NULL); // trail cut fix


        
    if (MH_Initialize() != MH_OK) {
        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    }

   /* AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);*/

    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xD5F50), PauseLayer::hook, reinterpret_cast<void**>(&PauseLayer::init));
  
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
        reinterpret_cast<void*>(gd::base + 0xd0470),
        reinterpret_cast<void*>(&OptionsLayer_customSetup_H),
        reinterpret_cast<void**>(&OptionsLayer_customSetup));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xfd420),
        reinterpret_cast<void*>(&SupportLayer_customSetup_H),
        reinterpret_cast<void**>(&SupportLayer_customSetup));

    MH_CreateHook(
        reinterpret_cast<void*>(cocosbase + 0xff970),
        reinterpret_cast<void**>(&Scheduler_update_H),
        reinterpret_cast<void**>(&Scheduler_update));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x8c2c0),
        reinterpret_cast<void**>(&LevelEditorLayer_init_H),
        reinterpret_cast<void**>(&LevelEditorLayer_init));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3e3d0),
        EditorPauseLayer::EditorPauseLayer_customSetup_H,
        reinterpret_cast<void**>(&EditorPauseLayer::EditorPauseLayer_customSetup));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xaf5ad),
        MenuLayer::MenuLayer_bottomMenu_H,
        reinterpret_cast<void**>(&MenuLayer::MenuLayer_bottomMenu));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3e41d),
        EditorPauseLayer::EditorPauseLayer_centerMenu_H,
        reinterpret_cast<void**>(&EditorPauseLayer::EditorPauseLayer_centerMenu));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3e56b),
        EditorPauseLayer::EditorPauseLayer_bottomMenu_H,
        reinterpret_cast<void**>(&EditorPauseLayer::EditorPauseLayer_bottomMenu));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x408fc),
        reinterpret_cast<void**>(&EditorUI_rightMenu_H),
        reinterpret_cast<void**>(&EditorUI_rightMenu));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xAF210),
        MenuLayer::MenuLayer_init_H,
        reinterpret_cast<void**>(&MenuLayer::MenuLayer_init));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x29db0),
        reinterpret_cast<void**>(&ColorSelectPopup_init_H),
        reinterpret_cast<void**>(&ColorSelectPopup_init));
    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0x2b050),
    //    reinterpret_cast<void**>(&ColorSelectPopup_dtor_H),
    //    reinterpret_cast<void**>(&ColorSelectPopup_dtor));
    setup_imgui_menu();

    MH_EnableHook(MH_ALL_HOOKS);

    return TRUE;
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
