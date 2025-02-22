#define NOMINMAX
#define UNICODE
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
#include <regex>
#include "nodes.hpp"
#include "LevelBrowserLayer.h"
#include "GJScoreCell.h"
#include "InfoLayer.hpp"
#include "CommentCell.h"
#include "GameVariables.hpp"
#include "share.hpp"
#include <nfd.h>

gd::MenuGameLayer* menuGameLayer;
gd::EditLevelLayer* editLevelLayer;
gd::LevelInfoLayer* levelInfoLayer;

int serverString;

enum class ServerAddresses {
    GHS = 808594997,
    ABSOLLLUTE = 1869832801, // 1953785198 old server
    PLATINUM = 1952541808
};

class ImportExportCB {
public:
    void onExportLevelELL(CCObject* obj) {
        auto* const level = reinterpret_cast<gd::EditLevelLayer*>(reinterpret_cast<CCNode*>(obj)->getParent()->getParent())->m_level;
        nfdchar_t* path = nullptr;
        if (NFD_SaveDialog("gmd", nullptr, &path) == NFD_OKAY) {
            std::ofstream file(path);
            dump_level(level, file);
            free(path);
            gd::FLAlertLayer::create(nullptr, "Success", "The level has been saved", "OK", nullptr, 300.f, false, 0)->show();
        }
    }
    void onExportLevelLIL(CCObject* obj) {
        auto* const level = reinterpret_cast<gd::LevelInfoLayer*>(reinterpret_cast<CCNode*>(obj)->getParent()->getParent())->m_level;
        if (static_cast<std::string>(level->m_levelString).empty()) return;
        nfdchar_t* path = nullptr;
        if (NFD_SaveDialog("gmd", nullptr, &path) == NFD_OKAY) {
            std::ofstream file(path);
            dump_level(level, file);
            free(path);
            gd::FLAlertLayer::create(nullptr, "Success", "The level has been saved", "OK", nullptr, 300.f, false, 0)->show();
        }
    }
};

void(__thiscall* fpMainLoop)(cocos2d::CCDirector* self);

void __fastcall hkMainLoop(cocos2d::CCDirector* self)
{
    ImGuiHook::poll(self->getOpenGLView());
    fpMainLoop(self);
}

void(__thiscall* LevelInfoLayer_onClone)(gd::LevelInfoLayer*, CCObject*);
void __fastcall LevelInfoLayer_onCloneH(gd::LevelInfoLayer* self, void*, CCObject* sender) {
    LevelInfoLayer_onClone(self, sender);
    if (!self->shouldDownloadLevel()) {
        auto level = static_cast<gd::GJGameLevel*>(gd::LocalLevelManager::sharedState()->getLocalLevels()->objectAtIndex(0));
        level->m_songID = self->m_level->m_songID;
        level->m_audioTrack = self->m_level->m_audioTrack;
    }
}

void(__thiscall* EditLevelLayer_onClone)(gd::EditLevelLayer*);
void __fastcall EditLevelLayer_onCloneH(gd::EditLevelLayer* self) {
    EditLevelLayer_onClone(self);
    auto level = static_cast<gd::GJGameLevel*>(gd::LocalLevelManager::sharedState()->getLocalLevels()->objectAtIndex(0));
    level->m_songID = self->m_level->m_songID;
    level->m_audioTrack = self->m_level->m_audioTrack;
}

bool(__thiscall* CCKeyboardDispatcher_dispatchKeyboardMSG)(cocos2d::CCKeyboardDispatcher* self, int key, bool down);
void __fastcall CCKeyboardDispatcher_dispatchKeyboardMSG_H(CCKeyboardDispatcher* self, void* edx, int key, bool down) {
    auto pl = gd::GameManager::sharedState()->getPlayLayer();
    if (down) {
        if ((key == setting().retryKeybind) && setting().onRetryBind) {
            if (pl) {
                if (!pl->hasCompletedLevel()) {
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
        if (pl) {
            if (setting().onSPSwitcher) {
                if (key == setting().spsPrevKey)
                    PlayLayer::onPrevStartPos();
                if (key == setting().spsNextKey)
                    PlayLayer::onNextStartPos();
            }
        }
    }
    CCKeyboardDispatcher_dispatchKeyboardMSG(self, key, down);
}

class CustomizeObjectLayer : public gd::FLAlertLayer {
public:
    gd::GameObject* m_targetObject; // 0x1bc
    cocos2d::CCArray* m_selectedObjects; // 0x1c0
    cocos2d::CCArray* m_buttonsArray; // 0x1c4

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

    self->m_buttonsArray->addObject(spr_3dl);
    self->m_buttonsArray->addObject(spr_wht);

    self->m_buttonMenu->addChild(btn_3dl);
    self->m_buttonMenu->addChild(btn_wht);

    for (auto obj2 : CCArrayExt<gd::GameObject*>(objs)) {
        if (obj2) {
            switch (obj2->getColorMode())
            {
            case gd::CustomColorMode::Default:
                break;

            case gd::CustomColorMode::DL:
                self->hightlightSelected(spr_3dl);
                break;
            case gd::CustomColorMode::White:
                self->hightlightSelected(spr_wht);
                break;
            }
        }
    }

    return true;
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

void(__thiscall* CCLabelBMFont_setString)(CCLabelBMFont*, const char*);
void __fastcall CCLabelBMFont_setStringH(CCLabelBMFont* self, void* edx, const char* newString) {
    return CCLabelBMFont_setString(self, "eeffoc");
}

void(__thiscall* CCLabelBMFont_create)(CCLabelBMFont*, const char*, const char*);
void __fastcall CCLabelBMFont_createH(CCLabelBMFont* self, void* edx, const char* label, const char* font) {
    return CCLabelBMFont_create(self, "eeffoc", font);
}

class CustomSongWidgetCallback : public gd::FLAlertLayer, public cocos2d::CCTextFieldDelegate, public gd::FLAlertLayerProtocol {
public:
    void onCopy(CCObject*) {
        std::string songInfo = dynamic_cast<CCLabelProtocol*>(this->getChildren()->objectAtIndex(4))->getString();
        std::smatch matches;

        std::regex_search(songInfo, matches, std::regex(R"(\w+: (\d+) .*)"));

        ImGui::SetClipboardText(matches[1].str().c_str());
    }
};

bool(__thiscall* CustomSongWidget_init)(gd::CustomSongWidget*, gd::SongInfoObject*, gd::LevelSettingsObject*, bool, bool, bool, bool, bool);
bool __fastcall CustomSongWidget_initH(gd::CustomSongWidget* self, void* edx, gd::SongInfoObject* infoObject, gd::LevelSettingsObject* settingsObject, bool idk1, bool idk2, bool idk3, bool idk4, bool idk5) {
    if (!CustomSongWidget_init(self, infoObject, settingsObject, idk1, idk2, idk3, idk4, idk5)) return false;
    auto menu = from<CCMenu*>(self, 0xf4);

    if (reinterpret_cast<CCSprite*>(from<CCLabelBMFont*>(self, 0x100)->getChildren()->objectAtIndex(0))->isVisible()) {
        auto moreButtonPos = from<gd::CCMenuItemSpriteExtra*>(self, 0x11c)->getPosition();

        auto copyButton_spr = gd::ButtonSprite::create("Copy", 0x32, 0, 1.5f, true, "bigFont.fnt", "GJ_button_04.png", 25.f);
        auto copyButton_btn = gd::CCMenuItemSpriteExtra::create(copyButton_spr, copyButton_spr, self, menu_selector(CustomSongWidgetCallback::onCopy));
        copyButton_spr->getLabel()->setScale(.65f);
        copyButton_spr->setScale(0.7f);

        copyButton_btn->setSizeMult(1.f);
        copyButton_btn->setPosition({ moreButtonPos.x + 50.f, moreButtonPos.y });
        menu->addChild(copyButton_btn);
    }

    return true;
}

gd::LevelSearchLayer* levelSearchLayer;

class LevelSearchLayerCB {
public:
    void onSearchPlayer(CCObject*) {
        auto textInput = from<gd::CCTextInputNode*>(levelSearchLayer, 0x120);
        auto inputString = textInput->getString();
        if (!std::string(inputString).empty()) {
            auto searchObject = gd::GJSearchObject::create(gd::SearchType::UsersLevels, inputString);
            gd::LevelBrowserLayer::scene(searchObject);
        }
    }
};

bool(__thiscall* LevelSearchLayer_init)(gd::LevelSearchLayer*);
bool __fastcall LevelSearchLayer_initH(gd::LevelSearchLayer* self) {
    if (!LevelSearchLayer_init(self)) return false;
    levelSearchLayer = self;

    auto baseSpr = CCSprite::create("GJ_button_01.png");
    baseSpr->setScale(0.7f);
    auto secondSpr = gd::SimplePlayer::create(4);
    secondSpr->updatePlayerFrame(4, gd::IconType::Cube);
    secondSpr->setScale(0.9f);
    secondSpr->setPosition({ baseSpr->getContentSize() / 2.f });
    baseSpr->addChild(secondSpr);
    auto button = gd::CCMenuItemSpriteExtra::create(baseSpr, nullptr, self, menu_selector(LevelSearchLayerCB::onSearchPlayer));
    button->setPosition({ 200, 120 });

    auto menu = CCMenu::create();
    menu->addChild(button);
    self->addChild(menu);

    return true;
}

//class MoreSearchLayerCB {
//public:
//    void onExtraLong(CCObject*) {
//        gd::GameManager::sharedState()->toggleGameVariable(GameVariable::EXTRA_LONG);
//    }
//};
//
//auto extraLongToggleSpr(CCSprite* toggleOn, CCSprite* toggleOff) {
//    bool extraLong_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::EXTRA_LONG);
//    return (extraLong_enabled) ? toggleOn : toggleOff;
//}
//
//bool(__thiscall* MoreSearchLayer_init)(gd::FLAlertLayer*);
//bool __fastcall MoreSearchLayer_initH(gd::FLAlertLayer* self) {
//    if (!MoreSearchLayer_init(self)) return false;
//
//    auto menu = self->m_buttonMenu;
//
//    auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
//    auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
//
//    auto ELToggler = gd::CCMenuItemToggler::create(extraLongToggleSpr(toggleOn, toggleOff), extraLongToggleSpr(toggleOff, toggleOn), self, menu_selector(MoreSearchLayerCB::onExtraLong));
//    ELToggler->setPosition({ 70.f, -215.f });
//    ELToggler->setScale(0.8f);
//    auto ELLabel = CCLabelBMFont::create("Extra-Long", "bigFont.fnt");
//    ELLabel->setPosition({ ELToggler->getPositionX() + 96.f, CCDirector::sharedDirector()->getWinSize().height / 2.f - 80.f });
//    ELLabel->setScale(0.5f);
//    ELLabel->setAnchorPoint({ 0.f, 0.5f });
//    menu->addChild(ELToggler);
//    self->m_mainLayer->addChild(ELLabel);
//
//    return true;
//}

class LeaderboardsLayerCB {
public:
    void onRefreshLeaderboards(CCObject*) {
        auto gameLevelManager = gd::GameLevelManager::sharedState();
        gameLevelManager->updateUserScore();
        gameLevelManager->m_storedLevels->removeObjectForKey("leaderboard_top");
        gameLevelManager->m_storedLevels->removeObjectForKey("leaderboard_week");
        gameLevelManager->m_storedLevels->removeObjectForKey("leaderboard_creator");
        gameLevelManager->m_storedLevels->removeObjectForKey("leaderboard_global");
    }
};

bool(__thiscall* LeaderboardsLayer_init)(gd::LeaderboardsLayer*, gd::LeaderboardState);
bool __fastcall LeaderboardsLayer_initH(gd::LeaderboardsLayer* self, void*, gd::LeaderboardState state) {
    if (!LeaderboardsLayer_init(self, state)) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto menu = CCMenu::create();
    auto sprite = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
    auto button = gd::CCMenuItemSpriteExtra::create(sprite, nullptr, self, menu_selector(LeaderboardsLayerCB::onRefreshLeaderboards));

    menu->setPosition({ director->getScreenRight() - 27.f, director->getScreenBottom() + 27.f });
    menu->setZOrder(5);

    menu->addChild(button);
    self->addChild(menu);

    auto GJListLayer = from<gd::GJListLayer*>(self, 0x11c);
    auto CustomListView = reinterpret_cast<gd::CustomListView*>(GJListLayer->getChildren()->objectAtIndex(0));
    auto TableView = from<CCLayer*>(CustomListView, 0x120);
    auto CCContentLayer = from<gd::CCContentLayer*>(TableView, 0x144);

    return true;
}

//bool(__thiscall* MenuGameLayer_init)(gd::MenuGameLayer*);
//bool __fastcall MenuGameLayer_initH(gd::MenuGameLayer* self) {
//    if (!MenuGameLayer_init(self)) return false;
//    menuGameLayer = self;
//    std::cout << serverString << std::endl;
//    return true;
//}
//
//void(__thiscall* MenuGameLayer_tryJump)(gd::MenuGameLayer*, float);
//void __fastcall MenuGameLayer_tryJumpH(gd::MenuGameLayer* self, void* edx, float idk) {
//    if (!setting().onMenuGameplay) MenuGameLayer_tryJump(self, idk);
//}
//
//void(__thiscall* MenuGameLayer_update)(gd::MenuGameLayer*, float);
//void __fastcall MenuGameLayer_updateH(gd::MenuGameLayer* self, void* edx, float dt) {
//    if (setting().onMenuGameplay) {
//        if (self->getPlayerObject()->isRoll() && GetAsyncKeyState(KEY_W)) self->destroyPlayer(); // Destroys player if it's ball gamemode.
//
//        if ((GetAsyncKeyState(KEY_W))) {
//            self->getPlayerObject()->pushButton(gd::PlayerButton::Jump);
//        }
//        else {
//            self->getPlayerObject()->releaseButton(gd::PlayerButton::Jump);
//        }
//    }
//
//    MenuGameLayer_update(self, dt);
//}

cocos2d::CCObject* levelObject;
cocos2d::CCArray* localLevelArray;

class MoveToTopProtocol : public gd::FLAlertLayerProtocol {
public:
	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override {
		if (btn2) {
			localLevelArray->removeObject(levelObject, true);
			localLevelArray->insertObject(levelObject, 0);
			gd::LocalLevelManager::sharedState()->updateLevelOrder();
		}
	}
};

MoveToTopProtocol moveToTopProtocol;

class EditLevelLayerCB {
public:
    void onMoveToTop(CCObject*) {
        gd::FLAlertLayer::create(&moveToTopProtocol, "Move To Top", "Move this level to the top of the created levels list?", "NO", "YES", 300.f, false, 140.f)->show();
    }
    void onLevelID(CCObject*) {
        gd::LevelBrowserLayer::scene(gd::GJSearchObject::create(gd::SearchType::Search, std::to_string(editLevelLayer->m_level->m_levelID)));
    }
};

class MoveToTopProtocolLIL : public gd::FLAlertLayerProtocol {
public:
    void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override {
        if (btn2) {

        }
    }
};

MoveToTopProtocolLIL moveToTopProtocolLIL;

class LevelInfoLayerCB {
public:
    void onMoveToTop(CCObject*) {
        gd::FLAlertLayer::create(&moveToTopProtocolLIL, "Move To Top", "Move this level to the top of the created levels list?", "NO", "YES", 300.f, false, 140.f)->show();
    }
    void onGarage(CCObject*) {
        auto garageScene = gd::GJGarageLayer::scene();
        CCScene* scene = CCTransitionMoveInT::create(0.5f, garageScene);
        CCDirector::sharedDirector()->pushScene(scene);
    }
};

bool(__thiscall* EditLevelLayer_init_O)(gd::EditLevelLayer*, gd::GJGameLevel*);
bool __fastcall EditLevelLayer_init_H(gd::EditLevelLayer* self, void*, gd::GJGameLevel* gameLevel) {
    if (!EditLevelLayer_init_O(self, gameLevel)) return false;
    //gd::GameManager::sharedState()->m_lastScene2 = static_cast<gd::LastGameScene>(99);
    //gd::GameManager::sharedState()->m_premiumPopup = gameLevel;
    editLevelLayer = self;

    localLevelArray = gd::LocalLevelManager::sharedState()->getLocalLevels();
	levelObject = (cocos2d::CCObject*)gameLevel;

	auto director = CCDirector::sharedDirector();

	auto menu = CCMenu::create();

	menu->setZOrder(1);
	menu->setPosition({ director->getScreenRight(), director->getScreenTop() });
	self->addChild(menu);

	auto onMoveToTop_spr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
	if (!onMoveToTop_spr->initWithFile("GJ_orderUpBtn_001.png")) {
		onMoveToTop_spr->createWithSpriteFrameName("edit_upBtn_001.png");
	}
	onMoveToTop_spr->setScale(0.925f);
	auto onMoveToTop_btn = gd::CCMenuItemSpriteExtra::create(onMoveToTop_spr, onMoveToTop_spr, self, menu_selector(EditLevelLayerCB::onMoveToTop));
	onMoveToTop_btn->setPosition({ -30, -210 });

    //auto garageRope_spr = CCSprite::createWithSpriteFrameName("GJ_garageBtn_001.png");
    //garageRope_spr->setScale(0.6f);
    //auto garageRope = gd::CCMenuItemSpriteExtra::create(garageRope_spr, garageRope_spr, self, menu_selector(LevelInfoLayerCB::onGarage));
    //garageRope->setPosition(menu->convertToNodeSpace({ director->getScreenRight() - 84.f, director->getScreenTop() - 30.f }));
    //menu->addChild(garageRope);

	menu->addChild(onMoveToTop_btn);

    auto centerMenu = self->m_buttonMenu;
    reinterpret_cast<CCLabelBMFont*>(self->getChildren()->objectAtIndex(13))->setVisible(0);
    auto idLabel = CCLabelBMFont::create("ID: ", "goldFont.fnt");
    std::string original = std::to_string(self->m_level->m_originalLevel);
    std::string levelID = std::to_string(self->m_level->m_levelID);
    if ((self->m_level->m_levelID == 0) && (self->m_level->m_originalLevel == 0)) {
        idLabel->setString("ID: na");
    }
    else if ((self->m_level->m_originalLevel != 0) && (self->m_level->m_levelID == 0)) {
        idLabel->setString(std::string("ID: na (" + original + ")").c_str());
    }
    else if ((self->m_level->m_levelID != 0) && (self->m_level->m_originalLevel != 0)) {
        idLabel->setString(std::string("ID: " + levelID + " (" + original + ")").c_str());
    }
    else {
        idLabel->setString(CCString::createWithFormat("ID: %i", self->m_level->m_levelID)->getCString());
    }
    idLabel->setScale(0.6f);
    auto onLevelID = gd::CCMenuItemSpriteExtra::create(idLabel, nullptr, self, menu_selector(EditLevelLayerCB::onLevelID));
    if (self->m_level->m_levelID == 0) {
        onLevelID->setEnabled(false);
    }
    else {
        onLevelID->setEnabled(true);
    }
    centerMenu->addChild(onLevelID);
    onLevelID->setPosition({ 75.f, -121.f });

    auto shareMenu = CCMenu::create();

    auto btn_spr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
    if (!btn_spr->initWithFile("BE_Export_File.png")) {
        btn_spr->initWithSpriteFrameName("GJ_downloadBtn_001.png");
    }
    auto button = gd::CCMenuItemSpriteExtra::create(btn_spr, nullptr, self, menu_selector(ImportExportCB::onExportLevelELL));
    button->setPosition({ -30, +30 });

    shareMenu->setZOrder(1);
    shareMenu->setPosition({ director->getScreenRight(), director->getScreenBottom() });
    shareMenu->addChild(button);
    self->addChild(shareMenu);

    return true;
}

//class NoRotationAlertProtocol : public gd::FLAlertLayerProtocol {
//    void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override {
//        if (btn2) {
//            auto idk = CCTransitionFade::create(0.5f, gd::LevelEditorLayer::scene(editLevelLayer->level()));
//            CCDirector::sharedDirector()->replaceScene(idk);
//        }
//    }
//};
//
//NoRotationAlertProtocol noRotAlert;
//
//void(__thiscall* EditLevelLayer_onEdit)(gd::EditLevelLayer*, CCObject*);
//void __fastcall EditLevelLayer_onEditH(gd::EditLevelLayer* self, void*, CCObject* sender) {
//    if (setting().onNoRotation) {
//        gd::FLAlertLayer::create(&noRotAlert, "No Rotation", "'No Rotation' is turned on, it can <cr>break your level</c> if you will save it with this option.", "Cancel", "Go", 320.f, false, 120.f)->show();
//    }
//}

CCArray* savedLevelsArray;
CCObject* savedGameLevel;

bool LevelInfoLayer_init(gd::LevelInfoLayer* self, gd::GJGameLevel* level) {
	if (!matdash::orig<&LevelInfoLayer_init>(self, level)) return false;

    //gd::GameManager::sharedState()->m_lastScene2 = static_cast<gd::LastGameScene>(98);
    //gd::GameManager::sharedState()->m_premiumPopup = level;

    levelInfoLayer = self;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

    //savedGameLevel = (CCObject*)level;

    //auto menu = CCMenu::create();
    //auto moveToTopSpr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
    //auto moveToTop = gd::CCMenuItemSpriteExtra::create(moveToTopSpr, nullptr, self, menu_selector(LevelInfoLayerCB::onMoveToTop));
    //menu->addChild(moveToTop);
    //self->addChild(menu);

    if (setting().onAutoDownloadSong) {
        auto songWidget = reinterpret_cast<gd::CustomSongWidget*>(self->getChildren()->objectAtIndex(26)); // FUCK YOU
        if (songWidget && from<gd::CCMenuItemSpriteExtra*>(songWidget, 0x108) && from<gd::CCMenuItemSpriteExtra*>(songWidget, 0x108)->isVisible()) {
            songWidget->onDownload(nullptr);
        }
    }

    //auto garageRope_spr = CCSprite::createWithSpriteFrameName("GJ_garageBtn_001.png");
    //garageRope_spr->setScale(0.6f);
    //auto garageRope = gd::CCMenuItemSpriteExtra::create(garageRope_spr, garageRope_spr, self, menu_selector(LevelInfoLayerCB::onGarage));
    //garageRope->setPosition(self->m_likeBtn->getParent()->convertToNodeSpace({ director->getScreenRight() - 84.f, director->getScreenTop() - 30.f}));
    //self->m_likeBtn->getParent()->addChild(garageRope);

    auto rightMenu = static_cast<CCMenu*>(self->m_likeBtn->getParent());
    auto btn_spr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
    if (!btn_spr->initWithFile("BE_Export_File.png")) {
        btn_spr->initWithSpriteFrameName("GJ_downloadBtn_001.png");
    }
    auto button = gd::CCMenuItemSpriteExtra::create(btn_spr, nullptr, self, menu_selector(ImportExportCB::onExportLevelLIL));

    button->setPosition({ rightMenu->convertToNodeSpace({director->getScreenLeft() + 30.f, winSize.height / 2.f + 75.f}) });
    rightMenu->addChild(button);

    std::cout << serverString << std::endl;

	return true;
}

class CustomSongLayerCB {
public:
    void onReupload(CCObject*) {
        if (serverString == static_cast<int>(ServerAddresses::GHS)) { // R.I.P.
            CCApplication::sharedApplication()->openURL("http://85.209.2.73:25573/custom/uploadsong");
        }
        else if (serverString == static_cast<int>(ServerAddresses::ABSOLLLUTE)) {
            CCApplication::sharedApplication()->openURL("https://absolllute.com/gdps/gdapi/tools/song-add/");
        }
        else if (serverString == static_cast<int>(ServerAddresses::PLATINUM)) {
            CCApplication::sharedApplication()->openURL("https://platinum.141412.xyz/dashboard/songs/");
        }
    }
};

bool(__thiscall* CustomSongLayer_init)(gd::FLAlertLayer*, gd::LevelSettingsObject*);
bool __fastcall CustomSongLayer_initH(gd::FLAlertLayer* self, void*, gd::LevelSettingsObject* levelSettings) {
    if (!CustomSongLayer_init(self, levelSettings)) return false;

    if ((serverString == static_cast<int>(ServerAddresses::GHS)) || (serverString == static_cast<int>(ServerAddresses::ABSOLLLUTE)) || (serverString == static_cast<int>(ServerAddresses::PLATINUM))) {
        auto menu = self->m_buttonMenu;

        auto newgroundsButton = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(4));
        newgroundsButton->setPositionX(newgroundsButton->getPositionX() - 55.f);

        auto buttonSprite = gd::ButtonSprite::create("Reupload", 84, 0, 1.f, false, "bigFont.fnt", "GJ_button_04.png", 25.f);
        auto reuploadButton = gd::CCMenuItemSpriteExtra::create(buttonSprite, nullptr, self, menu_selector(CustomSongLayerCB::onReupload));
        reuploadButton->setPosition({ 332.f, -111.f });

        menu->addChild(reuploadButton);
    }

    return true;
}

void(__thiscall* GJDropDownLayer_showLayer)(gd::GJDropDownLayer*, bool);
void __fastcall GJDropDownLayer_showLayerH(gd::GJDropDownLayer* self, void*, bool noTransition) {
    if (setting().onFastMenu)
        GJDropDownLayer_showLayer(self, true);
    else
        GJDropDownLayer_showLayer(self, noTransition);
}

void(__thiscall* GJDropDownLayer_hideLayer)(gd::GJDropDownLayer*, bool);
void __fastcall GJDropDownLayer_hideLayerH(gd::GJDropDownLayer* self, void*, bool noTransition) {
    if (setting().onFastMenu)
        GJDropDownLayer_hideLayer(self, true);
    else
        GJDropDownLayer_hideLayer(self, noTransition);
}

void(__thiscall* CCNode_sortAllChildren)(CCNode*);
void __fastcall CCNode_sortAllChildrenH(CCNode* self) {
    //CCNode_sortAllChildren(self);
}

void(__thiscall* AppDelegate_trySaveGame)(gd::AppDelegate* self);
void __fastcall AppDelegate_trySaveGame_H(gd::AppDelegate* self) {
    if (setting().onAutoSave)
    {
        setting().saveState();
    }
    AppDelegate_trySaveGame(self);
}

DWORD WINAPI my_thread(void* hModule) {
    //setting().loadState();

    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x43A49B), "\xB8\x01\x00\x00\x00\x90\x90", 7, NULL); // Play Music Button
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x428bd5), "\x6a\x00", 2, NULL); // rgba8888 format (better texture quality and colors)

    ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F04E9), &setting().NoclipByte, 1, 0);
    ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x54083C), &serverString, 8, 0);

    if (MH_Initialize() != MH_OK) {
        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    }

    AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

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
    SimplePlayer::mem_init();
    GJGarageLayer::mem_init();
    LevelEditorLayer::mem_init();
    SetGroupIDLayer::mem_init();
    ColorSelectPopup::mem_init();
    LevelBrowserLayer::mem_init();

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

    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x38bd0), CustomSongWidget_initH, reinterpret_cast<void**>(&CustomSongWidget_init));

    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xad430), MenuGameLayer_createH, reinterpret_cast<void**>(&MenuGameLayer_create));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xfd420), SupportLayer_customSetupH, reinterpret_cast<void**>(&SupportLayer_customSetup));

    MH_CreateHook(reinterpret_cast<void*>(GetProcAddress(cocos, "?initWithDuration@CCTransitionScene@cocos2d@@UAE_NMPAVCCScene@2@@Z")), CCTransitionScene_initWithDurationH, reinterpret_cast<void**>(&CCTransitionScene_initWithDuration));
    MH_CreateHook(reinterpret_cast<void*>(GetProcAddress(cocos, "?draw@CCParticleSystemQuad@cocos2d@@UAEXXZ")), CCParticleSystemQuad_drawH, reinterpret_cast<void**>(&CCParticleSystemQuad_draw));
    //MH_CreateHook(reinterpret_cast<void*>(GetProcAddress(cocos, "?sortAllChildren@CCNode@cocos2d@@UAEXXZ")), CCNode_sortAllChildrenH, reinterpret_cast<void**>(&CCNode_sortAllChildren));
    //MH_CreateHook(reinterpret_cast<void*>(GetProcAddress(cocos, "?setString@CCLabelBMFont@cocos2d@@UAEXPBD@Z")), CCLabelBMFont_setStringH, reinterpret_cast<void**>(&CCLabelBMFont_setString));
    //MH_CreateHook(reinterpret_cast<void*>(GetProcAddress(cocos, "?create@CCLabelBMFont@cocos2d@@SAPAV12@PBD0@Z")), CCLabelBMFont_createH, reinterpret_cast<void**>(&CCLabelBMFont_create));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9e2c0), LevelInfoLayer_onCloneH, reinterpret_cast<void**>(&LevelInfoLayer_onClone));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3da30), EditLevelLayer_onCloneH, reinterpret_cast<void**>(&EditLevelLayer_onClone));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x97050), LevelSettingsLayer::initH, reinterpret_cast<void**>(&LevelSettingsLayer::init));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9f830), LevelSearchLayer_initH, reinterpret_cast<void**>(&LevelSearchLayer_init));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x84080), InfoLayer::initH, reinterpret_cast<void**>(&InfoLayer::init));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x87fc0), LeaderboardsLayer_initH, reinterpret_cast<void**>(&LeaderboardsLayer_init));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3b5a0), EditLevelLayer_init_H, reinterpret_cast<void**>(&EditLevelLayer_init_O));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9bc10), LevelInfoLayer_initH, reinterpret_cast<void**>(&LevelInfoLayer_init));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3d440), EditLevelLayer_onEditH, reinterpret_cast<void**>(&EditLevelLayer_onEdit));

    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xadff0), MenuGameLayer_tryJumpH, reinterpret_cast<void**>(&MenuGameLayer_tryJump));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xad4d0), MenuGameLayer_initH, reinterpret_cast<void**>(&MenuGameLayer_init));

    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x355f0), GJScoreCell::loadFromScoreH, reinterpret_cast<void**>(&GJScoreCell::loadFromScore));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x859c0), InfoLayer::onLevelInfoH, reinterpret_cast<void**>(&InfoLayer::onLevelInfo));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x342e0), CommentCell::loadFromCommentH, reinterpret_cast<void**>(&CommentCell::loadFromComment));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x36540), CustomSongLayer_initH, reinterpret_cast<void**>(&CustomSongLayer_init));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xA3620), MoreSearchLayer_initH, reinterpret_cast<void**>(&MoreSearchLayer_init));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7bff0), GJDropDownLayer_hideLayerH, reinterpret_cast<void**>(&GJDropDownLayer_hideLayer));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7bf20), GJDropDownLayer_showLayerH, reinterpret_cast<void**>(&GJDropDownLayer_showLayer));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x92f20), DrawGridLayer::addToSpeedObjectsH, reinterpret_cast<void**>(&DrawGridLayer::addToSpeedObjects));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x93710), DrawGridLayer::drawH, reinterpret_cast<void**>(&DrawGridLayer::draw));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x93710), DrawGridLayer::drawH, reinterpret_cast<void**>(&DrawGridLayer::draw));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe1270), PlayerObject::placeStreakPointH, reinterpret_cast<void**>(&PlayerObject::placeStreakPoint));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd9b50), PlayerObject::updateH, reinterpret_cast<void**>(&PlayerObject::update));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0de0), PlayerObject::fadeOutStreak2H, reinterpret_cast<void**>(&PlayerObject::fadeOutStreak2));


    matdash::add_hook<&cocos_hsv2rgb>(GetProcAddress(cocos_ext, "?RGBfromHSV@CCControlUtils@extension@cocos2d@@SA?AURGBA@23@UHSV@23@@Z"));

    matdash::add_hook<&LevelInfoLayer_init>(gd::base + 0x9bc10);

    //MH_CreateHook(
    //    reinterpret_cast<void*>(GetProcAddress(cocos, "?end@CCDirector@cocos2d@@QAEXXZ")),
    //    reinterpret_cast<void**>(&CCDirector_end_H),
    //    reinterpret_cast<void**>(&CCDirector_end));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x293f0), 
        reinterpret_cast<void**>(&AppDelegate_trySaveGame_H), 
        reinterpret_cast<void**>(&AppDelegate_trySaveGame));

    preview_mode::init();

    setup_imgui_menu();

    MH_EnableHook(MH_ALL_HOOKS);

    MH_CreateHook(reinterpret_cast<LPVOID>(reinterpret_cast<uintptr_t>(cocos) + 0xfc240), hkMainLoop, reinterpret_cast<LPVOID*>(&fpMainLoop));

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
