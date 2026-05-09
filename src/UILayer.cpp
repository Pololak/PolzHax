#include "UILayer.hpp"
#include "PlayLayer.hpp"
#include "Setting.hpp"

void PolzUILayer::onPrevStartPos(CCObject*) {
    static_cast<PolzPlayLayer*>(GameManager::sharedState()->m_playLayer)->prevStartPos();
}

void PolzUILayer::onNextStartPos(CCObject*) {
    static_cast<PolzPlayLayer*>(GameManager::sharedState()->m_playLayer)->nextStartPos();
}

bool PolzUILayer::init() {
    if (!UILayer::init()) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto startPosSwitcherMenu = CCMenu::create();
    this->addChild(startPosSwitcherMenu, 0, 125);

    auto startPosSwitcherLabel = CCLabelBMFont::create("", "bigFont.fnt");
    startPosSwitcherLabel->setPosition(startPosSwitcherMenu->convertToNodeSpace({ winSize.width / 2.f, director->getScreenBottom() + 20.f }));
    startPosSwitcherLabel->setScale(.5f);
    startPosSwitcherLabel->setOpacity(0);
    startPosSwitcherLabel->setVisible(false);
    startPosSwitcherMenu->addChild(startPosSwitcherLabel, 0, 1);

    auto onPrevStartPosSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    onPrevStartPosSpr->setScale(.4f);
    auto onPrevStartPos = CCMenuItemSpriteExtra::create(onPrevStartPosSpr, this, menu_selector(PolzUILayer::onPrevStartPos));
    onPrevStartPos->setOpacity(0);
    onPrevStartPos->setVisible(false);
    onPrevStartPos->setPosition(startPosSwitcherMenu->convertToNodeSpace({ winSize.width / 2.f - 35.f, director->getScreenBottom() + 20.f }));
    startPosSwitcherMenu->addChild(onPrevStartPos, 0, 2);

    auto onNextStartPosSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    onNextStartPosSpr->setScale(.4f);
    onNextStartPosSpr->setFlipX(true);
    auto onNextStartPos = CCMenuItemSpriteExtra::create(onNextStartPosSpr, this, menu_selector(PolzUILayer::onNextStartPos));
    onNextStartPos->setOpacity(0);
    onNextStartPos->setVisible(false);
    onNextStartPos->setPosition(startPosSwitcherMenu->convertToNodeSpace({ winSize.width / 2.f + 35.f, director->getScreenBottom() + 20.f }));
    startPosSwitcherMenu->addChild(onNextStartPos, 0, 3);

    return true;
}

void PolzUILayer::keyDown(enumKeyCodes key) {
    auto playLayer = GameManager::sharedState()->m_playLayer;

    if (key == setting().m_p1click) {
        if (playLayer) playLayer->pushButton(1, false);
    }
    else if (key == setting().m_p2click) {
        if (playLayer) playLayer->pushButton(1, true);
    }
    else {
        return UILayer::keyDown(key);
    }
}

void PolzUILayer::keyUp(enumKeyCodes key) {
    auto playLayer = GameManager::sharedState()->m_playLayer;

    if (key == setting().m_p1click) {
        if (playLayer) playLayer->releaseButton(1, false);
    }
    else if (key == setting().m_p2click) {
        if (playLayer) playLayer->releaseButton(1, true);
    }
    else {
        return UILayer::keyUp(key);
    }
}