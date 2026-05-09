#include "LevelInfoLayer.hpp"
#include "Setting.hpp"

void PolzLevelInfoLayer::onMoveToTop(CCObject*) {
    auto layer = FLAlertLayer::create(this, "Move To Top", "Move this level to the top of the levels list?", "NO", "YES", 300.f);
	layer->setID("move-to-top-alert"_spr);
	layer->show();
}

bool PolzLevelInfoLayer::init(GJGameLevel* level) {
    if (!LevelInfoLayer::init(level)) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto customSongWidget = static_cast<CustomSongWidget*>(this->getChildByID("custom-songs-widget"));
    if (setting().onAutoSongDownload) {
        customSongWidget->onDownload(nullptr);
    }

    auto actionsMenu = static_cast<CCMenu*>(this->getChildByID("right-side-menu"));

    auto onMoveToTopSpr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
    auto onMoveToTop = CCMenuItemSpriteExtra::create(onMoveToTopSpr, this, menu_selector(PolzLevelInfoLayer::onMoveToTop));
    onMoveToTop->setPosition(actionsMenu->convertToNodeSpace(ccp(director->getScreenRight() - 80.f, director->getScreenBottom() + 70.f)));
    actionsMenu->addChild(onMoveToTop);

    return true;
}

void PolzLevelInfoLayer::FLAlert_Clicked(FLAlertLayer* layer, bool btn2) {
    if ((layer->getID() == "move-to-top-alert"_spr) && btn2) {
        auto savedLevels = GameLevelManager::sharedState()->m_savedLevelsDict;
        savedLevels->removeObjectForKey(std::to_string(this->m_level->m_levelID));
        savedLevels->setObject(this->m_level, std::to_string(this->m_level->m_levelID));
    }
    LevelInfoLayer::FLAlert_Clicked(layer, btn2);
}