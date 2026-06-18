#include "LevelInfoLayerHook.h"
#include "../Layers/ExportLevelLayer.h"
#include "../Menu/PolzHaxLayer.h"

void LevelInfoLayerHook::Callback::onMoveToTop(CCObject*) {
    auto layer = FLAlertLayer::create(this, "Move To Top", "Move this level to the top of the levels list?", "NO", "YES", 300.f);
    layer->setTag(10);
    layer->show();
}

void LevelInfoLayerHook::Callback::onExportLevel(CCObject*) {
    if (!this->shouldDownloadLevel()) {
        ExportLevelLayer::create(this->m_level)->show();
    }
}

void LevelInfoLayerHook::Callback::onPolzHax(CCObject*) {
    PolzHaxLayer::create()->show();
}

bool LevelInfoLayerHook::initH(LevelInfoLayer* self, GJGameLevel* level) {
    if (!LevelInfoLayerHook::init(self, level)) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto actionsMenu = static_cast<CCMenu*>(self->m_likeBtn->getParent());

    auto onExportLevelSpr = CCSprite::create("BE_Export_File.png");
    auto onExportLevel = CCMenuItemSpriteExtra::create(onExportLevelSpr, self, menu_selector(LevelInfoLayerHook::Callback::onExportLevel));
    onExportLevel->setPosition(actionsMenu->convertToNodeSpace({ director->getScreenLeft() + 30.f, winSize.height / 2.f + 75.f }));
    actionsMenu->addChild(onExportLevel);

    auto onMoveToTopSpr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
    auto onMoveToTop = CCMenuItemSpriteExtra::create(onMoveToTopSpr, self, menu_selector(LevelInfoLayerHook::Callback::onMoveToTop));
    onMoveToTop->setPosition(actionsMenu->convertToNodeSpace(ccp(director->getScreenRight() - 80.f, director->getScreenBottom() + 70.f)));
    actionsMenu->addChild(onMoveToTop);

    auto onPolzHaxSpr = CCSprite::create("GJ_polzhaxBtn_001.png");
    onPolzHaxSpr->setScale(.85f);
    auto onPolzHax = CCMenuItemSpriteExtra::create(onPolzHaxSpr, self, menu_selector(LevelInfoLayerHook::Callback::onPolzHax));
    onPolzHax->setPosition(actionsMenu->convertToNodeSpace({director->getScreenLeft() + 70.f, director->getScreenTop() - 25.f}));
    actionsMenu->addChild(onPolzHax);

    return true;
}

void LevelInfoLayerHook::onCloneH(LevelInfoLayer* self, CCObject* sender) {
    LevelInfoLayerHook::onClone(self, sender);

    if (!self->shouldDownloadLevel()) {
        auto level = static_cast<GJGameLevel*>(LocalLevelManager::sharedState()->m_localLevels->objectAtIndex(0));
        level->m_songID = self->m_level->m_songID;
    }
}

void LevelInfoLayerHook::FLAlert_ClickedH(LevelInfoLayer* self, FLAlertLayer* layer, bool btn2) {
    if ((layer->getTag() == 10) && btn2) {
        auto savedLevels = GameLevelManager::sharedState()->m_savedLevelsDict;
        savedLevels->removeObjectForKey(CCString::createWithFormat("%i", self->m_level->m_levelID)->getCString());
        savedLevels->setObject(self->m_level, CCString::createWithFormat("%i", self->m_level->m_levelID)->getCString());
    }
    LevelInfoLayerHook::FLAlert_Clicked(self, layer, btn2);
}

void LevelInfoLayerHook::mem_init() {
    HOOK("_ZN14LevelInfoLayer4initEP11GJGameLevel", LevelInfoLayerHook::initH, LevelInfoLayerHook::init);
    HOOK("_ZN14LevelInfoLayer7onCloneEPN7cocos2d8CCObjectE", LevelInfoLayerHook::onCloneH, LevelInfoLayerHook::onClone);
    HOOK("_ZN14LevelInfoLayer15FLAlert_ClickedEP12FLAlertLayerb", LevelInfoLayerHook::FLAlert_ClickedH, LevelInfoLayerHook::FLAlert_Clicked);
}