#include "PauseLayerHook.h"
#include "../Setting.h"
#include "../utils.h"
#include "../Menu/PolzHaxLayer.h"

bool m_hide;

void PauseLayerHook::Callback::onShowPercentage(CCObject*) {
    GameManager::sharedState()->toggleGameVariable(SHOW_PERCENTAGE);
    auto playLayer = GameManager::sharedState()->getPlayLayer();
    if (playLayer) {
        auto percentageLabel = static_cast<CCLabelBMFont*>(playLayer->getChildByTag(301));
        if (percentageLabel) {
            percentageLabel->setVisible(GameManager::sharedState()->getGameVariable(SHOW_PERCENTAGE));
        }
    }
}

void PauseLayerHook::Callback::onHide(CCObject*) {
    m_hide = !m_hide;

    CCARRAY_FOREACH_B_TYPE(this->getChildren(), node, CCNode) {
        if (node && node != this->getChildByTag(45)) {
            node->setVisible(!m_hide);
        }
    }

    this->setOpacity(m_hide ? 0 : 75);
}

void PauseLayerHook::Callback::onPolzHax(CCObject*) {
    PolzHaxLayer::create()->show();
}

void PauseLayerHook::customSetupH(PauseLayer* self) {
    PauseLayerHook::customSetup(self);

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto togglerMenu = self->getChildByType<CCMenu*>(1);

    if (togglerMenu) {
        self->createToggleButton(
            "%",
            menu_selector(PauseLayerHook::Callback::onShowPercentage),
            !GameManager::sharedState()->getGameVariable(SHOW_PERCENTAGE),
            togglerMenu,
            ccp(winSize.width / 2.f + 155.f, director->getScreenBottom() + 25.f)
        );
    }

    auto mainMenu = self->getChildByType<CCMenu*>(0);

    if (mainMenu) {
        auto onPolzHaxSpr = CCSprite::create("GJ_polzhaxBtn_001.png");
        onPolzHaxSpr->setScale(.85f);
        auto onPolzHax = CCMenuItemSpriteExtra::create(onPolzHaxSpr, self, menu_selector(PauseLayerHook::Callback::onPolzHax));
        onPolzHax->setPosition(mainMenu->convertToNodeSpace({director->getScreenLeft() + 40.f, winSize.height / 2.f}));
        mainMenu->addChild(onPolzHax);
    }

    auto hideMenu = CCMenu::create();
    self->addChild(hideMenu, 0, 45);

    auto onHideSpr = CCSprite::create("hideBtn_001.png");
    onHideSpr->setOpacity(50);
    auto onHide = CCMenuItemSpriteExtra::create(onHideSpr, self, menu_selector(PauseLayerHook::Callback::onHide));
    onHide->setPosition(hideMenu->convertToNodeSpace({director->getScreenLeft() + 20.f, director->getScreenBottom() + 15.f}));
    hideMenu->addChild(onHide);
}

void PauseLayerHook::onProgressBarH(PauseLayer* self, CCObject* sender) {
    PauseLayerHook::onProgressBar(self, sender);

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto gm = GameManager::sharedState();
    auto playLayer = gm->getPlayLayer();

    if (playLayer) {
        auto percentageLabel = static_cast<CCLabelBMFont*>(playLayer->getChildByTag(301));
        if (percentageLabel) {
            percentageLabel->setAnchorPoint({ (gm->m_showProgressBar ? 0.f : .5f), .5f });
            percentageLabel->setPosition(winSize.width / 2.f + (gm->m_showProgressBar ? 110.2f : 0.f), winSize.height - 8.f);
        }
    }
}

void PauseLayerHook::mem_init() {
    HOOK("_ZN10PauseLayer11customSetupEv", PauseLayerHook::customSetupH, PauseLayerHook::customSetup);
    HOOK("_ZN10PauseLayer13onProgressBarEPN7cocos2d8CCObjectE", PauseLayerHook::onProgressBarH, PauseLayerHook::onProgressBar);
}