#include "LevelInfoLayer.hpp"
#include "LevelShare.hpp"
#include "nfd.h"
#include <fstream>
#include "Setting.hpp"

gd::CustomSongWidget* m_songWidget;

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

void LevelInfoLayer::Callback::onGarage(CCObject*) {
	auto scene = gd::GJGarageLayer::scene();
	scene->setUserData(this->m_level);
	CCDirector::sharedDirector()->pushScene(CCTransitionMoveInT::create(.5f, scene));
	gd::GameManager::sharedState()->m_lastScene = static_cast<gd::LastGameScene>(3);
}

void LevelInfoLayer::Callback::onMoveToTop(CCObject*) {
	auto layer = gd::FLAlertLayer::create(this, "Move To Top", "Move this level to the top of the levels list?", "NO", "YES", 300.f);
	layer->setTag(10);
	layer->show();
}

bool __fastcall LevelInfoLayer::initH(gd::LevelInfoLayer* self, void*, gd::GJGameLevel* level) {
	if (!LevelInfoLayer::init(self, level)) return false;

	if (m_songWidget) {
		std::cout << m_songWidget << std::endl;
		
		if (setting().onAutoSongDownload) {
			m_songWidget->onDownload(nullptr);
		}
	}

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

	//auto onGarageSpr = CCSprite::createWithSpriteFrameName("gj_iconBtn_on_001.png");
	//auto onGarage = gd::CCMenuItemSpriteExtra::create(onGarageSpr, self, menu_selector(LevelInfoLayer::Callback::onGarage));
	//onGarage->setPosition(100, 50);
	//actionsMenu->addChild(onGarage);

	auto onMoveToTopSpr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
	auto onMoveToTop = gd::CCMenuItemSpriteExtra::create(onMoveToTopSpr, self, menu_selector(LevelInfoLayer::Callback::onMoveToTop));
	onMoveToTop->setPosition(actionsMenu->convertToNodeSpace(ccp(director->getScreenRight() - 80.f, director->getScreenBottom() + 70.f)));
	actionsMenu->addChild(onMoveToTop);

	return true;
}

void __fastcall LevelInfoLayer::onCloneH(gd::LevelInfoLayer* self, void*, CCObject* sender) {
	LevelInfoLayer::onClone(self, sender);

	if (!self->shouldDownloadLevel()) {
		auto level = static_cast<gd::GJGameLevel*>(gd::LocalLevelManager::sharedState()->m_localLevels->objectAtIndex(0));
		level->m_songID = self->m_level->m_songID;
	}
}

void __fastcall LevelInfoLayer::FLAlert_ClickedH(gd::LevelInfoLayer* _self, void*, gd::FLAlertLayer* layer, bool btn2) {
	auto self = reinterpret_cast<gd::LevelInfoLayer*>(reinterpret_cast<uintptr_t>(_self) - 0x128);
	if ((layer->getTag() == 10) && btn2) {
		auto savedLevels = gd::GameLevelManager::sharedState()->m_savedLevelsDict;
		savedLevels->removeObjectForKey(std::to_string(self->m_level->m_levelID));
		savedLevels->setObject(self->m_level, std::to_string(self->m_level->m_levelID));
	}
	LevelInfoLayer::FLAlert_Clicked(_self, layer, btn2);
}

void __fastcall LevelInfoLayer::songWidgetH() {
	__asm {
		mov m_songWidget, eax
	}
	LevelInfoLayer::songWidget();
}

void LevelInfoLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9bc10), LevelInfoLayer::initH, reinterpret_cast<void**>(&LevelInfoLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9e2c0), LevelInfoLayer::onCloneH, reinterpret_cast<void**>(&LevelInfoLayer::onClone));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9f2d0), LevelInfoLayer::FLAlert_ClickedH, reinterpret_cast<void**>(&LevelInfoLayer::FLAlert_Clicked));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9cb06), LevelInfoLayer::songWidgetH, reinterpret_cast<void**>(&LevelInfoLayer::songWidget));
}