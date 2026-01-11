#include "LevelInfoLayer.hpp"
#include "LevelShare.hpp"
#include "nfd.h"
#include <fstream>
#include "Setting.hpp"

void LevelInfoLayer::Callback::onExportLevel(CCObject*) {
	if (!this->m_level->m_levelString.size()) {
		gd::FLAlertLayer::create("Error", "Level string is empty!", "OK")->show();
		return;
	}

	nfdchar_t* path = nullptr;
	if (NFD_SaveDialog("gmd", CCFileUtils::sharedFileUtils()->getWritablePath2().c_str(), &path) == NFD_OKAY) {
		std::ofstream file(path);
		dumpLevel(this->m_level, file);
		free(path);
		gd::FLAlertLayer::create("Success", "The level has been saved.", "OK")->show();
	}
}

bool __fastcall LevelInfoLayer::initH(gd::LevelInfoLayer* self, void*, gd::GJGameLevel* level) {
	if (!LevelInfoLayer::init(self, level)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto actionsMenu = static_cast<CCMenu*>(self->m_likeBtn->getParent());

	auto onExportLevelSpr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
	if (!onExportLevelSpr->initWithFile("BE_Export_File.png")) {
		onExportLevelSpr->createWithSpriteFrameName("GJ_downloadBtn_001.png");
	}
	auto onExportLevel = gd::CCMenuItemSpriteExtra::create(onExportLevelSpr, self, menu_selector(LevelInfoLayer::Callback::onExportLevel));
	onExportLevel->setPosition(actionsMenu->convertToNodeSpace({ director->getScreenLeft() + 30.f, winSize.height / 2.f + 75.f }));
	actionsMenu->addChild(onExportLevel);

	return true;
}

void __fastcall LevelInfoLayer::onCloneH(gd::LevelInfoLayer* self, void*, CCObject* sender) {
	LevelInfoLayer::onClone(self, sender);

	if (!self->shouldDownloadLevel()) {
		auto level = static_cast<gd::GJGameLevel*>(gd::LocalLevelManager::sharedState()->m_localLevels->objectAtIndex(0));
		level->m_songID = self->m_level->m_songID;
	}
}

void LevelInfoLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9bc10), LevelInfoLayer::initH, reinterpret_cast<void**>(&LevelInfoLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9e2c0), LevelInfoLayer::onCloneH, reinterpret_cast<void**>(&LevelInfoLayer::onClone));
}