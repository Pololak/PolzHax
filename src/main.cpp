#include "include.h"

#include "Hooks/CustomizeObjectLayerHook.h"
#include "Hooks/DrawGridLayerHook.h"
#include "Hooks/EditButtonBarHook.h"
#include "Hooks/EditLevelLayerHook.h"
#include "Hooks/EditorPauseLayerHook.h"
#include "Hooks/EditorUIHook.h"
#include "Hooks/FMODAudioEngineHook.h"
#include "Hooks/GameObjectHook.h"
#include "Hooks/LevelBrowserLayerHook.h"
#include "Hooks/LevelEditorLayerHook.h"
#include "Hooks/LevelInfoLayerHook.h"
#include "Hooks/LevelSettingsLayerHook.h"
#include "Hooks/PlayLayerHook.h"
#include "Hooks/SetGroupIDLayerHook.h"
#include "Hooks/UILayerHook.h"

#include "LevelShare.h"

#include "Setting.h"
#include "utils.h"
#include "ModPatches.h"
#include "Speedhack.h"
#include "Menu/SpeedHackLayer.h"

namespace MenuLayerHook {
    class Callback : public MenuLayer {
    public:
        void onCharacterFilter(CCObject*) {
            setting().onCharacterFilter = !setting().onCharacterFilter;
            ModPatches::onCharacterFilter();
        }

        void onIcons(CCObject*) {
            setting().onIcons = !setting().onIcons;
            ModPatches::onIcons();
        }

        void onMainLevels(CCObject*) {
            setting().onMainLevels = !setting().onMainLevels;
            ModPatches::onMainLevels();
        }

        void onSliderLimit(CCObject*) {
            setting().onSliderLimit = !setting().onSliderLimit;
            ModPatches::onSliderLimit();
        }

        void onTextLength(CCObject*) {
            setting().onTextLength = !setting().onTextLength;
            ModPatches::onTextLength();
        }

        void onNoDeathEffect(CCObject*) {
            setting().onNoDeathEffect = !setting().onNoDeathEffect;
            ModPatches::onNoDeathEffect();
        }

        void onSpeedhack(CCObject*) {
            setting().onSpeedhack = !setting().onSpeedhack;
            Speedhack::updateSpeedhack();
        }

        void onSpeedhackMusic(CCObject*) {
            setting().onSpeedhackMusic = !setting().onSpeedhackMusic;
            Speedhack::updateSpeedhackMusic();
        }

        void onSmartStartPos(CCObject*) {
            setting().onSmartStartPos = !setting().onSmartStartPos;
        }

        void onStartPosSwitcher(CCObject*) {
            setting().onStartPosSwitcher = !setting().onStartPosSwitcher;
        }

        void onHitboxBugFix(CCObject*) {
            setting().onHitboxBugFix = !setting().onHitboxBugFix;
        }

        void onNoTransition(CCObject*) {
            setting().onNoTransition = !setting().onNoTransition;
        }

        void onNoclip(CCObject*) {
            setting().onNoclip = !setting().onNoclip;
            ModPatches::onNoclip();
        }

        void onPracticeMusic(CCObject*) {
            setting().onPracticeMusic = !setting().onPracticeMusic;
            ModPatches::onPracticeMusic();
        }
    };

    inline bool(*init)(MenuLayer*);
    bool initH(MenuLayer* self) {
        if (!MenuLayerHook::init(self)) return false;

        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();

        auto menu = CCMenu::create();
        self->addChild(menu, 15);

        auto onPolzHaxSpr = CCSprite::create("GJ_polzhaxBtn_001.png");
        onPolzHaxSpr->setScale(.85f);
        auto onPolzHax = CCMenuItemSpriteExtra::create(onPolzHaxSpr, self, 0);
        onPolzHax->setPosition(menu->convertToNodeSpace({director->getScreenLeft() + 30.f, winSize.height / 2.f}));
        menu->addChild(onPolzHax);

        GameToolbox::createToggleButton(
            "Character Filter",
            menu_selector(Callback::onCharacterFilter),
            setting().onCharacterFilter,
            menu,
            ccp(75.f, director->getScreenTop() - 90.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );

        GameToolbox::createToggleButton(
            "Icons",
            menu_selector(Callback::onIcons),
            setting().onIcons,
            menu,
            ccp(75.f, director->getScreenTop() - 120.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );

        GameToolbox::createToggleButton(
            "Main Levels",
            menu_selector(Callback::onMainLevels),
            setting().onMainLevels,
            menu,
            ccp(75.f, director->getScreenTop() - 150.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );

        GameToolbox::createToggleButton(
            "Slider Limit",
            menu_selector(Callback::onSliderLimit),
            setting().onSliderLimit,
            menu,
            ccp(75.f, director->getScreenTop() - 180.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );

        GameToolbox::createToggleButton(
            "Text Length",
            menu_selector(Callback::onTextLength),
            setting().onTextLength,
            menu,
            ccp(75.f, director->getScreenTop() - 210.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );

        GameToolbox::createToggleButton(
            "No Death Effect",
            menu_selector(Callback::onNoDeathEffect),
            setting().onNoDeathEffect,
            menu,
            ccp(75.f, director->getScreenTop() - 240.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );

        GameToolbox::createToggleButton(
            "Speedhack",
            menu_selector(Callback::onSpeedhack),
            setting().onSpeedhack,
            menu,
            ccp(75.f, director->getScreenTop() - 270.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );

        GameToolbox::createToggleButton(
            "Speedhack Music",
            menu_selector(Callback::onSpeedhackMusic),
            setting().onSpeedhackMusic,
            menu,
            ccp(75.f, director->getScreenTop() - 300.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );
        //
        GameToolbox::createToggleButton(
            "Smart StartPos",
            menu_selector(Callback::onSmartStartPos),
            setting().onSmartStartPos,
            menu,
            ccp(195.f, director->getScreenTop() - 90.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );

        GameToolbox::createToggleButton(
            "StartPos Switcher",
            menu_selector(Callback::onStartPosSwitcher),
            setting().onStartPosSwitcher,
            menu,
            ccp(195.f, director->getScreenTop() - 120.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );

        GameToolbox::createToggleButton(
            "Hitbox Bug Fix",
            menu_selector(Callback::onHitboxBugFix),
            setting().onHitboxBugFix,
            menu,
            ccp(195.f, director->getScreenTop() - 150.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );

        GameToolbox::createToggleButton(
            "No Transition",
            menu_selector(Callback::onNoTransition),
            setting().onNoTransition,
            menu,
            ccp(195.f, director->getScreenTop() - 180.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );

        GameToolbox::createToggleButton(
            "Noclip",
            menu_selector(Callback::onNoclip),
            setting().onNoclip,
            menu,
            ccp(195.f, director->getScreenTop() - 210.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );

        GameToolbox::createToggleButton(
            "Practice Music",
            menu_selector(Callback::onPracticeMusic),
            setting().onPracticeMusic,
            menu,
            ccp(195.f, director->getScreenTop() - 240.f),
            self, self,
            .7f, .4f, 80.f,
            ccp(8.f, 0.f),
            "bigFont.fnt",
            false
        );

        setting().save();

        return true;
    }

    inline void(*onMoreGames)(MenuLayer*, CCObject*);
    void onMoreGamesH(MenuLayer* self, CCObject* sender) {
        SpeedHackLayer::create()->show();
    }
}

namespace LevelSelectLayerHook {
    inline bool(*init)(LevelSelectLayer*, int);
    bool initH(LevelSelectLayer* self, int p0) {
        if (!LevelSelectLayerHook::init(self, p0)) return false;

        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();

        auto menu = CCMenu::create();
        self->addChild(menu);

        auto onPolzHaxSpr = CCSprite::create("GJ_polzhaxBtn_001.png");
        onPolzHaxSpr->setScale(.85f);
        auto onPolzHax = CCMenuItemSpriteExtra::create(onPolzHaxSpr, self, 0);
        onPolzHax->setPosition(menu->convertToNodeSpace({director->getScreenLeft() + 30.f, director->getScreenTop() - 70.f}));
        menu->addChild(onPolzHax);

        return true;
    }
}

namespace LoadingLayerHook {
    inline void(*loadingFinished)(LoadingLayer*);
    void loadingFinishedH(LoadingLayer* self) {
        LoadingLayerHook::loadingFinished(self);

        Speedhack::updateSpeedhack();
    }
}

namespace AppDelegateHook {
    inline void(*applicationDidEnterBackground)(AppDelegate*);
    void applicationDidEnterBackgroundH(AppDelegate* self) {
        AppDelegateHook::applicationDidEnterBackground(self);

        setting().save();
    }

    inline void(*applicationWillEnterForeground)(AppDelegate*);
    void applicationWillEnterForegroundH(AppDelegate* self) {
        AppDelegateHook::applicationWillEnterForeground(self);

        setting().save();
    }

    inline void(*trySaveGame)(AppDelegate*);
    void trySaveGameH(AppDelegate* self) {
        AppDelegateHook::trySaveGame(self);

        setting().save();
    }
}

namespace CCTransitionSceneHook {
    inline bool(*initWithDuration)(CCTransitionScene*, float, CCScene*);
    bool initWithDurationH(CCTransitionScene* self, float duration, CCScene* scene) {
        return CCTransitionSceneHook::initWithDuration(self, setting().onNoTransition ? 0.f : duration, scene);
    }
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    create_directories("/storage/emulated/0/PolzHaxMobile/19");
    create_directories("/storage/emulated/0/PolzHaxMobile/19/levels");

    setting().load();

    ModPatches::loadPatches();

    PatchManager patchManager;

    patchManager.addPatch(GDBASE, 0x1ac4fc, "00 20");
    patchManager.addPatch(GDBASE, 0x205b98, "01 21 00 bf");
    patchManager.Modify();

    CustomizeObjectLayerHook::mem_init();
    DrawGridLayerHook::mem_init();
    EditButtonBarHook::mem_init();
    EditLevelLayerHook::mem_init();
    EditorPauseLayerHook::mem_init();
    EditorUIHook::mem_init();
    FMODAudioEngineHook::mem_init();
    GameObjectHook::mem_init();
    LevelBrowserLayerHook::mem_init();
    LevelEditorLayerHook::mem_init();
    LevelInfoLayerHook::mem_init();
    LevelSettingsLayerHook::mem_init();
    PlayLayerHook::mem_init();
    SetGroupIDLayerHook::mem_init();
    UILayerHook::mem_init();

	HOOK("_ZN11AppDelegate29applicationDidEnterBackgroundEv", AppDelegateHook::applicationDidEnterBackgroundH, AppDelegateHook::applicationDidEnterBackground);
	HOOK("_ZN11AppDelegate30applicationWillEnterForegroundEv", AppDelegateHook::applicationWillEnterForegroundH, AppDelegateHook::applicationWillEnterForeground);
	HOOK("_ZN11AppDelegate11trySaveGameEv", AppDelegateHook::trySaveGameH, AppDelegateHook::trySaveGame);
	HOOK("_ZN12LoadingLayer15loadingFinishedEv", LoadingLayerHook::loadingFinishedH, LoadingLayerHook::loadingFinished);
	HOOK("_ZN9MenuLayer4initEv", MenuLayerHook::initH, MenuLayerHook::init);
	HOOK("_ZN9MenuLayer11onMoreGamesEPN7cocos2d8CCObjectE", MenuLayerHook::onMoreGamesH, MenuLayerHook::onMoreGames);
	HOOK("_ZN16LevelSelectLayer4initEi", LevelSelectLayerHook::initH, LevelSelectLayerHook::init);
	HOOK("_ZN7cocos2d17CCTransitionScene16initWithDurationEfPNS_7CCSceneE", CCTransitionSceneHook::initWithDurationH, CCTransitionSceneHook::initWithDuration);

	return JNI_VERSION_1_6;
}   