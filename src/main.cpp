#include "include.h"

#include "Hooks/CustomizeObjectLayerHook.h"
#include "Hooks/EditButtonBarHook.h"
#include "Hooks/EditLevelLayerHook.h"
#include "Hooks/EditorPauseLayerHook.h"
#include "Hooks/EditorUIHook.h"
#include "Hooks/GameObjectHook.h"
#include "Hooks/LevelBrowserLayerHook.h"
#include "Hooks/LevelEditorLayerHook.h"
#include "Hooks/LevelInfoLayerHook.h"
#include "Hooks/LevelSettingsLayerHook.h"
#include "Hooks/SetGroupIDLayerHook.h"

#include "LevelShare.h"

#include "Setting.h"
#include "utils.h"

namespace MenuLayerHook {
    class Callback : public MenuLayer {
    public:
        void onImportLevel(CCObject*) {
            LevelShare::importLevel("/storage/emulated/0/PolzHaxMobile/19/levels/Bloodbath.gmd");
        }
    };

    inline bool(*init)(MenuLayer*);
    bool initH(MenuLayer* self) {
        if (!MenuLayerHook::init(self)) return false;

        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();

        auto menu = CCMenu::create();
        self->addChild(menu);

        auto onPolzHaxSpr = CCSprite::create("GJ_polzhaxBtn_001.png");
        onPolzHaxSpr->setScale(.85f);
        auto onPolzHax = CCMenuItemSpriteExtra::create(onPolzHaxSpr, self, menu_selector(MenuLayerHook::Callback::onImportLevel));
        onPolzHax->setPosition(menu->convertToNodeSpace({director->getScreenLeft() + 30.f, winSize.height / 2.f}));
        menu->addChild(onPolzHax);

        setting().save();

        return true;
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

        setting().load();
    }
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    create_directories("/storage/emulated/0/PolzHaxMobile/19");
    create_directories("/storage/emulated/0/PolzHaxMobile/19/levels");

    PatchManager patchManager;

    patchManager.addPatch(GDBASE, 0x1ac4fc, "00 20");
    patchManager.addPatch(GDBASE, 0x205b98, "01 21 00 bf");
    patchManager.Modify();

    CustomizeObjectLayerHook::mem_init();
    EditButtonBarHook::mem_init();
    EditLevelLayerHook::mem_init();
    EditorPauseLayerHook::mem_init();
    EditorUIHook::mem_init();
    GameObjectHook::mem_init();
    LevelBrowserLayerHook::mem_init();
    LevelEditorLayerHook::mem_init();
    LevelInfoLayerHook::mem_init();
    LevelSettingsLayerHook::mem_init();
    SetGroupIDLayerHook::mem_init();

	HOOK("_ZN12LoadingLayer15loadingFinishedEv", LoadingLayerHook::loadingFinishedH, LoadingLayerHook::loadingFinished);
	HOOK("_ZN9MenuLayer4initEv", MenuLayerHook::initH, MenuLayerHook::init);
	HOOK("_ZN16LevelSelectLayer4initEi", LevelSelectLayerHook::initH, LevelSelectLayerHook::init);

	return JNI_VERSION_1_6;
}   