#include "UILayerHook.h"
#include "PlayLayerHook.h"

void UILayerHook::Callback::onPrevStartPos(CCObject*) {
    PlayLayerHook::prevStartPos();
}

void UILayerHook::Callback::onNextStartPos(CCObject*) {
    PlayLayerHook::nextStartPos();
}

bool UILayerHook::initH(UILayer* self) {
    if (!UILayerHook::init(self)) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto startPosSwitcherMenu = CCMenu::create();
    self->addChild(startPosSwitcherMenu, 0, 125);

    auto startPosSwitcherLabel = CCLabelBMFont::create("", "bigFont.fnt");
    startPosSwitcherLabel->setPosition(startPosSwitcherMenu->convertToNodeSpace({ winSize.width / 2.f, director->getScreenBottom() + 20.f }));
    startPosSwitcherLabel->setScale(.5f);
    startPosSwitcherLabel->setOpacity(0);
    startPosSwitcherLabel->setVisible(false);
    startPosSwitcherMenu->addChild(startPosSwitcherLabel, 0, 1);

    auto onPrevStartPosSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    onPrevStartPosSpr->setScale(.65f);
    auto onPrevStartPos = CCMenuItemSpriteExtra::create(onPrevStartPosSpr, self, menu_selector(UILayerHook::Callback::onPrevStartPos));
    onPrevStartPos->setOpacity(0);
    onPrevStartPos->setVisible(false);
    onPrevStartPos->setPosition(startPosSwitcherMenu->convertToNodeSpace({ winSize.width / 2.f - 35.f, director->getScreenBottom() + 20.f }));
    startPosSwitcherMenu->addChild(onPrevStartPos, 0, 2);

    auto onNextStartPosSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    onNextStartPosSpr->setScale(.65f);
    onNextStartPosSpr->setFlipX(true);
    auto onNextStartPos = CCMenuItemSpriteExtra::create(onNextStartPosSpr, self, menu_selector(UILayerHook::Callback::onNextStartPos));
    onNextStartPos->setOpacity(0);
    onNextStartPos->setVisible(false);
    onNextStartPos->setPosition(startPosSwitcherMenu->convertToNodeSpace({ winSize.width / 2.f + 35.f, director->getScreenBottom() + 20.f }));
    startPosSwitcherMenu->addChild(onNextStartPos, 0, 3);

    return true;
}

void UILayerHook::onPauseH(UILayer* self, CCObject* sender) {
    UILayerHook::onPause(self, sender);
    // Void Click fix
    self->m_p1TouchId = -1;
    self->m_p2TouchId = -1;
    self->m_p1Jumping = false;
    self->m_p2Jumping = false;
}

void UILayerHook::mem_init() {
    HOOK("_ZN7UILayer4initEv", UILayerHook::initH, UILayerHook::init);
    HOOK("_ZN7UILayer7onPauseEPN7cocos2d8CCObjectE", UILayerHook::onPauseH, UILayerHook::onPause);
}