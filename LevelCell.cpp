#include "LevelCell.hpp"

std::vector<gd::GJGameLevel*> m_selectedLevels;

std::vector<gd::GJGameLevel*> LevelCell::getSelectedLevels() {
	return m_selectedLevels;
}

void LevelCell::Callback::onSelectLevel(CCObject*) {
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

	std::cout << m_selectedLevels.size() << std::endl;

	for (auto level : m_selectedLevels) {
		if (level) {
			std::cout << level->m_levelName << std::endl;
		}
	}
}

void __fastcall LevelCell::loadLocalLevelCellH(gd::LevelCell* self) {
	m_selectedLevels.clear();
	LevelCell::loadLocalLevelCell(self);

	auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
	toggleOff->setScale(.6f);
	auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
	toggleOn->setScale(.6f);

	auto menu = CCMenu::create();
	self->m_mainLayer->addChild(menu, 0, 120);
	auto onSelectLevel = gd::CCMenuItemToggler::create(toggleOff, toggleOn, self, menu_selector(LevelCell::Callback::onSelectLevel));
	onSelectLevel->setPosition(-18.f, -125.f);
	menu->addChild(onSelectLevel, 0, 1);
}

void LevelCell::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x313b0), LevelCell::loadLocalLevelCellH, reinterpret_cast<void**>(&LevelCell::loadLocalLevelCell));
}