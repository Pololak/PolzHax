#include "EndLevelLayer.hpp"
#include "PlayLayer.hpp"
#include "Setting.hpp"

void PolzEndLevelLayer::updateCheatIndicator2() {
    auto playLayer = static_cast<PolzPlayLayer*>(GameManager::sharedState()->m_playLayer);

    bool isCheating = playLayer->isCheating();
    bool cheatingBeforeRestart = playLayer->getCheatingBeforeRestart();

    auto cheatIndicator = this->m_fields->m_cheatIndicator;

    if (cheatIndicator) {
        if (!cheatingBeforeRestart && !setting().isSafeMode) {
            cheatIndicator->setColor(ccGREEN);
        }
        else if (!cheatingBeforeRestart && setting().isSafeMode) {
            cheatIndicator->setColor(ccYELLOW);
        }
        else if ((!isCheating || isCheating) && cheatingBeforeRestart && setting().isSafeMode) {
            cheatIndicator->setColor(ccORANGE);
        }
        else if (isCheating || cheatingBeforeRestart && !setting().isSafeMode) {
            cheatIndicator->setColor(ccRED);
        }
    }
}

void PolzEndLevelLayer::customSetup() {
    EndLevelLayer::customSetup();

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto completeSprite = this->getChildByType<CCSprite*>(0);
    if (completeSprite && (setting().isSafeMode || setting().onSafeMode)) {
        completeSprite->setVisible(false);

        auto safeModeLabel = CCLabelBMFont::create("Safe Mode", "bigFont.fnt");
        safeModeLabel->setPosition(completeSprite->getPosition());
        safeModeLabel->setScale(1.25f);
        this->m_mainLayer->addChild(completeSprite, 15);
    }

    this->m_fields->m_cheatIndicator = CCLabelBMFont::create(".", "bigFont.fnt");
    this->m_fields->m_cheatIndicator->setAnchorPoint({ 0.f, 1.f });
    this->m_fields->m_cheatIndicator->setPosition(winSize.width / 2.f - 172.f, winSize.height / 2.f + 127.5f);
    this->m_mainLayer->addChild(this->m_fields->m_cheatIndicator, 15, 507);

    this->updateCheatIndicator2();
}