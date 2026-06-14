#include "include.h"

#include "Hooks/CustomizeObjectLayerHook.h"
#include "Hooks/EditorPauseLayerHook.h"
#include "Hooks/EditorUIHook.h"
#include "Hooks/GameObjectHook.h"
#include "Hooks/LevelEditorLayerHook.h"

namespace MenuLayerHook {
    inline bool(*init)(MenuLayer*);
    bool initH(MenuLayer* self) {
        if (!MenuLayerHook::init(self)) return false;

        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();

        auto label = CCLabelBMFont::create("Test", "bigFont.fnt");
        label->setPosition(winSize / 2.f);
        self->addChild(label, 100);

        auto menu = CCMenu::create();
        self->addChild(menu, 105);

        GameToolbox::createToggleButton(
            "Idk",
            nullptr,
            false,
            menu,
            ccp(0.f, 0.f),
            self, self,
            .8f, .8f, 60.f,
            ccp(5.f, 0.f),
            "bigFonf.fnt",
            false
        );

        return true;
    }
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    PatchManager patchManager;

    patchManager.addPatch(GDBASE, 0x1ac4fc, "00 20");
    patchManager.addPatch(GDBASE, 0x205b98, "01 21 00 bf");
    patchManager.Modify();

    CustomizeObjectLayerHook::mem_init();
    EditorPauseLayerHook::mem_init();
    EditorUIHook::mem_init();
    GameObjectHook::mem_init();
    LevelEditorLayerHook::mem_init();

	HOOK("_ZN9MenuLayer4initEv", MenuLayerHook::initH, MenuLayerHook::init);

	return JNI_VERSION_1_6;
}   