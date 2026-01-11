#include "LevelBrowserLayer.hpp"
#include "LevelShare.hpp"
#include "nfd.h"
#include <fstream>

void LevelBrowserLayer::Callback::onFirstPage(CCObject*) {
	this->m_searchObject->m_page = 0;
	this->loadPage(this->m_searchObject);
}

void LevelBrowserLayer::Callback::onRefresh(CCObject* sender) {
	gd::GameLevelManager::sharedState()->resetTimerForKey(this->m_searchObject->getKey());
	this->loadPage(this->m_searchObject);

	std::cout << this->m_searchObject->getKey() << std::endl;
}

void LevelBrowserLayer::Callback::onImportLevel(CCObject*) {
	nfdchar_t* path = nullptr;
	if (NFD_OpenDialog("gmd", CCFileUtils::sharedFileUtils()->getWritablePath2().c_str(), &path) == NFD_OKAY) {
		std::ifstream file(path);
		auto* const level = importLevel(file);
		free(path);
		if (!level) {
			gd::FLAlertLayer::create("Error", "Failed to import.", "OK")->show();
			return;
		}
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, gd::EditLevelLayer::scene(level)));
	}
}

bool __fastcall LevelBrowserLayer::initH(gd::LevelBrowserLayer* self, void*, gd::GJSearchObject* searchObject) {
	if (!LevelBrowserLayer::init(self, searchObject)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	if (searchObject->m_searchType == gd::SearchType::MyLevels) {
		auto shareMenu = CCMenu::create();
		shareMenu->setPosition(director->getScreenRight(), director->getScreenBottom());
		self->addChild(shareMenu, 2);

		auto onExportLevelSpr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
		if (!onExportLevelSpr->initWithFile("BE_Import_File.png")) {
			onExportLevelSpr->createWithSpriteFrameName("GJ_downloadBtn_001.png");
		}
		auto onExportLevel = gd::CCMenuItemSpriteExtra::create(onExportLevelSpr, self, menu_selector(LevelBrowserLayer::Callback::onImportLevel));
		onExportLevel->setPosition(-30.f, 90.f);
		shareMenu->addChild(onExportLevel);
	}
	
	if (!(searchObject->m_searchType == gd::SearchType::MyLevels || searchObject->m_searchType == gd::SearchType::SavedLevels || searchObject->m_searchType == gd::SearchType::MapPack || searchObject->m_searchType == gd::SearchType::MapPackOnClick)) {
		auto refreshMenu = CCMenu::create();
		refreshMenu->setPosition(director->getScreenRight(), director->getScreenBottom());
		self->addChild(refreshMenu, 2);

		auto onRefreshSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
		auto onRefresh = gd::CCMenuItemSpriteExtra::create(onRefreshSpr, self, menu_selector(LevelBrowserLayer::Callback::onRefresh));
		onRefresh->setPosition(-30.f, 30.f);
		refreshMenu->addChild(onRefresh);
	}

	if (searchObject->m_searchType == gd::SearchType::MyLevels || searchObject->m_searchType == gd::SearchType::SavedLevels) {
		auto menu = CCMenu::create();
		self->addChild(menu, 10);

		auto onFirstPageSpr = CCSprite::createWithSpriteFrameName("edit_leftBtn2_001.png");
		auto onFirstPage = gd::CCMenuItemSpriteExtra::create(onFirstPageSpr, self, menu_selector(LevelBrowserLayer::Callback::onFirstPage));
		menu->addChild(onFirstPage);
	}

	return true;
}

void LevelBrowserLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x89590), LevelBrowserLayer::initH, reinterpret_cast<void**>(&LevelBrowserLayer::init));
}