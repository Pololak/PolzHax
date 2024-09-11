#include "PauseLayer.h"
#include "state.h"
#include "PlayLayer.h"

//gd::PlayLayer* playLayerObject;

auto playLayerObject = gd::GameManager::sharedState()->getPlayLayer();

class ExitAlertProtocol : public gd::FLAlertLayerProtocol {
protected:

    void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
    {
        if (btn2)
        {
            gd::GameManager::sharedState()->getPlayLayer()->onQuit();
            gd::GameSoundManager::sharedState()->playSound("quitSound_01.ogg");
            playLayerObject = nullptr;
        }
    }
};

ExitAlertProtocol eaProtocol;

bool __fastcall PauseLayer::customSetup_H(gd::CCBlockLayer* self) {
    layers().PauseLayerObject = self;
    if (setting().onHidePauseMenu)
        self->setVisible(false);
    else self->setVisible(true);

    bool result = PauseLayer::customSetup(self);

    auto director = CCDirector::sharedDirector();
    auto menu = CCMenu::create();

    auto bg = (extension::CCScale9Sprite*)self->getChildren()->objectAtIndex(0);
    /*auto progressBarLabel = (CCLabelBMFont*)self->getChildren()->objectAtIndex(11);
    progressBarLabel->setVisible(0);

    auto barLabel = CCLabelBMFont::create("Bar", "bigFont.fnt");
    barLabel->setPosition({ director->getScreenRight(), director->getScreenBottom() });
    self->addChild(barLabel);*/

    if (setting().onTransparentPause) {
        bg->setVisible(0);
    }
    else {
        bg->setVisible(1);
    }

    menu->setPosition({ director->getScreenRight(), director->getScreenTop() });

    auto optionsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    auto optionsBtn = gd::CCMenuItemSpriteExtra::create(optionsSpr, nullptr, self, menu_selector(gd::MenuLayer::onOptions));
    optionsBtn->setPosition({ -42, -42 });
    optionsSpr->setScale(.66f);
    menu->addChild(optionsBtn);
    self->addChild(menu);

    return result;
}

void __fastcall PauseLayer::onEdit_H(CCLayer* self)
{
    layers().PauseLayerObject = nullptr;
    playLayerObject = nullptr;
    PauseLayer::onEdit(self);
}

void __fastcall PauseLayer::onRestart_H(CCObject* sender) {

    CCEGLView::sharedOpenGLView()->showCursor(false);

    PauseLayer::onRestart(sender);
}

void __fastcall PauseLayer::onQuit_H(CCObject* btn)
{
    playLayerObject = nullptr;
    if (setting().onConfirmExit) {
        gd::FLAlertLayer::create(&eaProtocol, "Confirm", "Are you sure you want to exit the level?", "Cancel", "Exit", 320.f, false, 0)->show();
    }
    PauseLayer::onQuit(btn);
}

void PauseLayer::mem_init() {
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd5f50),
        PauseLayer::customSetup_H,
        reinterpret_cast<void**>(&PauseLayer::customSetup));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd7bf0),
        PauseLayer::onEdit_H,
        reinterpret_cast<void**>(&PauseLayer::onEdit));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd7b20),
        PauseLayer::onRestart_H,
        reinterpret_cast<void**>(&PauseLayer::onRestart));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd7f00),
        PauseLayer::onQuit_H,
        reinterpret_cast<void**>(&PauseLayer::onQuit));
}