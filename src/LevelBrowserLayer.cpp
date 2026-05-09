#include "LevelBrowserLayer.hpp"
#include "LevelCell.hpp"

void PolzLevelBrowserLayer::onDeleteSelected(CCObject*) {
    auto selectedLevels = PolzLevelCell2::getSelectedLevels();

    if (selectedLevels.empty()) {
        FLAlertLayer::create("Nothing here...", "No levels selected.", "OK")->show();
    }
    else {
        auto desc = CCString::createWithFormat("Are you sure you want to <cr>delete</c> the <cy>%i</c> selected <cg>levels</c>?", selectedLevels.size())->getCString();
        auto layer = FLAlertLayer::create(this, "Delete", desc, "Back", "Delete");
        layer->setID("delete-levels-alert"_spr);
        layer->show();
    }
}

bool PolzLevelBrowserLayer::init(GJSearchObject* searchObject) {
    if (!LevelBrowserLayer::init(searchObject)) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    if (searchObject->m_searchType == SearchType::MyLevels || searchObject->m_searchType == SearchType::SavedLevels) {
        auto deleteMenu = CCMenu::create();
        this->addChild(deleteMenu, 2);

        auto deleteSprite = CCSprite::createWithSpriteFrameName("edit_delBtn_001.png");
        auto onDeleteSelectedSpr = ButtonSprite::create(deleteSprite, 0x22, false, 36.f, "GJ_button_04.png", 1.f);
        onDeleteSelectedSpr->setScale(.4f);
        auto onDeleteSelected = CCMenuItemSpriteExtra::create(onDeleteSelectedSpr, this, menu_selector(PolzLevelBrowserLayer::onDeleteSelected));
        onDeleteSelected->setPosition(-145.f, -122.f);
        deleteMenu->addChild(onDeleteSelected);
    }

    return true;
}

void PolzLevelBrowserLayer::FLAlert_Clicked(FLAlertLayer* layer, bool btn2) {
    if ((layer->getID() == "delete-levels-alert"_spr) && btn2) {
        auto selectedLevels = PolzLevelCell2::getSelectedLevels();
        for (auto level : selectedLevels) {
            if (level) {
                if (level->m_levelType == GJLevelType::Editor) {
                    auto localLevelManager = LocalLevelManager::sharedState();
                    localLevelManager->m_localLevels->removeObject(level, true);
                    localLevelManager->updateLevelOrder();
                }
                if (level->m_levelType == GJLevelType::Saved) {
                    auto savedLevels = GameLevelManager::sharedState()->m_savedLevelsDict;
                    savedLevels->removeObjectForKey(std::to_string(level->m_levelID));
                }
                selectedLevels.clear();
                this->loadPage(this->m_searchObject);
            }
        }
    }
    LevelBrowserLayer::FLAlert_Clicked(layer, btn2);
}