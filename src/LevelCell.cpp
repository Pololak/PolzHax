#include "LevelCell.hpp"

std::vector<GJGameLevel*> m_selectedLevels;

std::vector<GJGameLevel*> PolzLevelCell2::getSelectedLevels() {
	return m_selectedLevels;
}

void PolzLevelCell::onSelectLevel(CCObject*) {
    bool addLevel = true;

    for (auto level : m_selectedLevels) {
        if (level) {
            if (level == this->m_level) {
                m_selectedLevels.erase(std::remove(m_selectedLevels.begin(), m_selectedLevels.end(), this->m_level), m_selectedLevels.end());
                addLevel = false;
            }
        }
    }

    if (addLevel) {
        m_selectedLevels.push_back(this->m_level);
    }

    for (auto level : m_selectedLevels) {
        if (level) {
            log::debug("{}", level->m_levelName);
        }
    } 
}

void PolzLevelCell::loadLocalLevelCell() {
    m_selectedLevels.clear();
    LevelCell::loadLocalLevelCell();

    auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    toggleOff->setScale(.6f);
    auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    toggleOn->setScale(.6f);

    auto menu = CCMenu::create();
    menu->setPosition(270.f, 35.f);
    this->m_mainLayer->addChild(menu, 0, 120);
    auto onSelectLevel = CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(PolzLevelCell::onSelectLevel));
    menu->addChild(onSelectLevel, 0, 1);
}

void PolzLevelCell::loadCustomLevelCell() {
    m_selectedLevels.clear();
    LevelCell::loadCustomLevelCell();

    if (this->m_level->m_levelType == GJLevelType::Saved) {
        auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
        toggleOff->setScale(.6f);
        auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
        toggleOn->setScale(.6f);

        auto menu = CCMenu::create();
        menu->setPosition(270.f, 45.f);
        this->m_mainLayer->addChild(menu, 0, 120);
        auto onSelectLevel = CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(PolzLevelCell::onSelectLevel));
        menu->addChild(onSelectLevel, 0, 1);
    }
}