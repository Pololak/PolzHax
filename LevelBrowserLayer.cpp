#include "LevelBrowserLayer.hpp"
#include "LevelShare.hpp"
#include "LevelCell.hpp"
#include "nfd.h"
#include <fstream>
#include "FindLevelPopup.hpp"
#include "GoToPagePopup.hpp"
#include "Setting.hpp"

std::string m_customSearchQuery;

void LevelBrowserLayer::setCustomSearchQuery(std::string query) {
	m_customSearchQuery = query;
}

void LevelBrowserLayer::updatePageButton(gd::LevelBrowserLayer* self) {
	auto menu = static_cast<CCMenu*>(self->getChildByTag(10));
	if (menu) {
		auto onGoToPage = static_cast<gd::CCMenuItemSpriteExtra*>(menu->getChildByTag(13));
		if (onGoToPage) {
			auto sprite = static_cast<CCSprite*>(onGoToPage->getChildren()->objectAtIndex(0));
			auto label = static_cast<CCLabelBMFont*>(sprite->getChildren()->objectAtIndex(0));
			if (label) {
				label->setString(CCString::createWithFormat("%i", self->m_searchObject->m_page + 1)->getCString());
				label->setScale(1.f);
				label->limitLabelWidth(32.f, .8f, 0.f);
			}
		}
	}
}

void LevelBrowserLayer::Callback::onSelectAll(CCObject* sender) {
	return;
	//auto btn = static_cast<gd::CCMenuItemToggler*>(sender);

	//auto selectedLevels = LevelCell::getSelectedLevels();
	//selectedLevels.clear();

	//auto customListView = static_cast<gd::CustomListView*>(this->m_list->getChildByTag(9));
	//if (customListView) {
	//	auto tableView = customListView->m_tableView;
	//	if (tableView) {
	//		auto contentLayer = tableView->m_contentLayer;
	//		if (contentLayer) {
	//			for (auto levelCell : CCArrayExt<gd::LevelCell*>(contentLayer->getChildren())) {
	//				if (levelCell) {
	//					auto menu = static_cast<CCMenu*>(levelCell->m_mainLayer->getChildByTag(120));
	//					if (menu) {
	//						auto toggle = static_cast<gd::CCMenuItemToggler*>(menu->getChildByTag(1));
	//						if (toggle) {
	//							toggle->toggle(!btn->m_toggled);
	//						}
	//					}

	//					selectedLevels.push_back(levelCell->m_level);
	//				}
	//			}
	//		}
	//	}
	//}
}

void LevelBrowserLayer::Callback::onDeleteSelected(CCObject*) {
	auto selectedLevels = LevelCell::getSelectedLevels();

	if (selectedLevels.empty()) {
		gd::FLAlertLayer::create("Nothing here...", "No levels selected.", "OK")->show();
	}
	else {
		auto desc = CCString::createWithFormat("Are you sure you want to <cr>delete</c> the <cy>%i</c> selected <cg>levels</c>?", selectedLevels.size())->getCString();
		auto layer = gd::FLAlertLayer::create(this, "Delete", desc, "Back", "Delete");
		layer->setTag(0x3);
		layer->show();
	}
}

void LevelBrowserLayer::Callback::onCancelSearch(CCObject*) {
	if (!m_customSearchQuery.empty()) {
		m_customSearchQuery.clear();
		this->loadPage(this->m_searchObject);
	}
}

void LevelBrowserLayer::Callback::onSearch(CCObject*) {
	FindLevelPopup::create(this)->show();
}

void LevelBrowserLayer::Callback::onFirstPage(CCObject* sender) {
	this->m_searchObject->m_page = 0;
	this->loadPage(this->m_searchObject);
}

void LevelBrowserLayer::Callback::onLastPage(CCObject* sender) {
	int totalItems = this->m_itemCount;

	this->m_searchObject->m_page = (totalItems % 10 == 0) ? totalItems / 10 - 1 : totalItems / 10;

	this->loadPage(this->m_searchObject);
}

void LevelBrowserLayer::Callback::onGoToPage(CCObject* sender) {
	GoToPagePopup::create(this)->show();
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

void LevelBrowserLayer::Callback::onFavorites(CCObject*) {
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, gd::LevelBrowserLayer::scene(gd::GJSearchObject::create(static_cast<gd::SearchType>(100)))));
}

void LevelBrowserLayer::Callback::onSaved(CCObject*) {
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, gd::LevelBrowserLayer::scene(gd::GJSearchObject::create(gd::SearchType::SavedLevels))));
}

bool __fastcall LevelBrowserLayer::initH(gd::LevelBrowserLayer* self, void*, gd::GJSearchObject* searchObject) {
	if (!LevelBrowserLayer::init(self, searchObject)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();
	
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
		self->addChild(menu, 10, 10);

		auto onFirstPageSpr = CCSprite::createWithSpriteFrameName("edit_leftBtn2_001.png");
		auto onFirstPage = gd::CCMenuItemSpriteExtra::create(onFirstPageSpr, self, menu_selector(LevelBrowserLayer::Callback::onFirstPage));
		onFirstPage->setPosition(menu->convertToNodeSpace({ director->getScreenLeft() + 20.f, director->getScreenTop() - 70.f }));
		onFirstPage->setVisible(self->m_leftArrow->isVisible());
		menu->addChild(onFirstPage, 0, 11);

		auto onLastPageSpr = CCSprite::createWithSpriteFrameName("edit_rightBtn2_001.png");
		auto onLastPage = gd::CCMenuItemSpriteExtra::create(onLastPageSpr, self, menu_selector(LevelBrowserLayer::Callback::onLastPage));
		onLastPage->setPosition(menu->convertToNodeSpace({ director->getScreenRight() - 20.f, director->getScreenTop() - 70.f }));
		onLastPage->setVisible(self->m_rightArrow->isVisible());
		menu->addChild(onLastPage, 0, 12);

		auto onGoToPageSpr = CCSprite::create("GJ_button_02.png");
		onGoToPageSpr->setScale(.7f);
		auto onGoToPageSpr2 = CCLabelBMFont::create("", "bigFont.fnt");
		onGoToPageSpr2->setPosition(20.f, 21.f);
		onGoToPageSpr2->setScale(.8f);
		onGoToPageSpr->addChild(onGoToPageSpr2);
		auto onGoToPage = gd::CCMenuItemSpriteExtra::create(onGoToPageSpr, self, menu_selector(LevelBrowserLayer::Callback::onGoToPage));
		onGoToPage->setPosition(menu->convertToNodeSpace({ director->getScreenRight() - 20.f, director->getScreenTop() - 40.f }));
		menu->addChild(onGoToPage, 0, 13);

		updatePageButton(self);

		//auto onSearchSpr = CCSprite::create("gj_findBtn_001.png");
		//auto onSearch = gd::CCMenuItemSpriteExtra::create(onSearchSpr, self, menu_selector(LevelBrowserLayer::Callback::onSearch));
		//onSearch->setPosition(menu->convertToNodeSpace({ director->getScreenLeft() + 55.f, director->getScreenTop() - 70.f }));
		//onSearch->setVisible(m_customSearchQuery.empty());
		//menu->addChild(onSearch, 0, 14);

		//auto onCancelSearchSpr = CCSprite::create("gj_findBtnOff_001.png");
		//auto onCancelSearch = gd::CCMenuItemSpriteExtra::create(onCancelSearchSpr, self, menu_selector(LevelBrowserLayer::Callback::onCancelSearch));
		//onCancelSearch->setPosition(onSearch->getPosition());
		//onCancelSearch->setVisible(!m_customSearchQuery.empty());
		//menu->addChild(onCancelSearch, 0, 15);

		auto deleteMenu = CCMenu::create();
		self->addChild(deleteMenu, 2, 570);

		auto deleteSprite = CCSprite::createWithSpriteFrameName("edit_delBtn_001.png");
		auto onDeleteSelectedSpr = gd::ButtonSprite::create(deleteSprite, 0x22, false, 1.f, 0, "GJ_button_04.png", true, 36.f);
		onDeleteSelectedSpr->setScale(.4f);
		auto onDeleteSelected = gd::CCMenuItemSpriteExtra::create(onDeleteSelectedSpr, self, menu_selector(LevelBrowserLayer::Callback::onDeleteSelected));
		onDeleteSelected->setPosition(-145.f, -122.f);
		deleteMenu->addChild(onDeleteSelected);
	}

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

		//auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
		//toggleOff->setScale(.5f);
		//auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
		//toggleOn->setScale(.5f);

		//auto onSelectAll = gd::CCMenuItemToggler::create(toggleOff, toggleOn, self, menu_selector(LevelBrowserLayer::Callback::onSelectAll));
		//onSelectAll->setPosition(-125.f, -122.f);
		//deleteMenu->addChild(onSelectAll, 0, 1);
	}

	if (setting().onDeveloperMode) {
		if (searchObject->m_searchType == gd::SearchType::SavedLevels || searchObject->m_searchType == static_cast<gd::SearchType>(100)) {
			auto menu = CCMenu::create();
			self->addChild(menu);

			CCSprite* onFavoritesSpr = nullptr;
			SEL_MenuHandler callback = nullptr;
			if (searchObject->m_searchType == gd::SearchType::SavedLevels) {
				onFavoritesSpr = CCSprite::create("GJ_heartOn_001.png");
				callback = static_cast<SEL_MenuHandler>(&LevelBrowserLayer::Callback::onFavorites);
			}
			else {
				onFavoritesSpr = CCSprite::create("GJ_heartOff_001.png");
				callback = static_cast<SEL_MenuHandler>(&LevelBrowserLayer::Callback::onSaved);
			}

			auto onFavorites = gd::CCMenuItemSpriteExtra::create(onFavoritesSpr, self, callback);
			onFavorites->setPosition(menu->convertToNodeSpace({ director->getScreenLeft() + 68.f, director->getScreenBottom() + 30.f }));
			menu->addChild(onFavorites);
		}
	}

	return true;
}

void __fastcall LevelBrowserLayer::loadPageH(gd::LevelBrowserLayer* self, void*, gd::GJSearchObject* searchObject) {
	LevelBrowserLayer::loadPage(self, searchObject);

	if (searchObject->m_searchType == static_cast<gd::SearchType>(100)) {

	}

	//if (searchObject->m_searchQuery.size()) {
	//	if (searchObject->m_searchType == gd::SearchType::MyLevels) {
	//		auto localLevels = gd::LocalLevelManager::sharedState()->m_localLevels;

	//		auto filteredLevels = CCArray::create();
	//		for (auto level : CCArrayExt<gd::GJGameLevel*>(localLevels)) {
	//			if (level) {
	//				if (ci_contains(level->m_levelName.c_str(), searchObject->m_searchQuery.c_str())) {
	//					filteredLevels->addObject(level);
	//				}
	//			}
	//		}

	//		int page = searchObject->m_page;
	//		int uVar12 = page * 10; // idk how to name it

	//		auto filteredLevelsPerPage = CCArray::create();
	//		if (filteredLevels->count()) {
	//			self->m_itemCount = filteredLevels->count();

	//			for (int i = uVar12; i != uVar12 + 10 && i <= filteredLevels->count() - 1; ++i) {
	//				auto level = static_cast<gd::GJGameLevel*>(filteredLevels->objectAtIndex(i));
	//				filteredLevelsPerPage->addObject(level);
	//			}
	//		}

	//		if (filteredLevelsPerPage->count()) {
	//			self->m_array->release();
	//			self->m_array = filteredLevelsPerPage;
	//			self->setupLevelBrowser(filteredLevelsPerPage);
	//			self->updateLevelsLabel();
	//		}

	//		int totalItems = self->m_itemCount;
	//		if (self->m_searchObject->m_page == ((totalItems % 10 == 0) ? totalItems / 10 - 1 : totalItems / 10)) {
	//			self->m_rightArrow->setVisible(false);
	//		}
	//		if (self->m_searchObject->m_page == 0) {
	//			self->m_leftArrow->setVisible(false);
	//		}
	//	}
	//}

	auto menu = static_cast<CCMenu*>(self->getChildByTag(10));
	if (menu) {
		auto onFirstPage = static_cast<gd::CCMenuItemSpriteExtra*>(menu->getChildByTag(11));
		auto onLastPage = static_cast<gd::CCMenuItemSpriteExtra*>(menu->getChildByTag(12));
		if (onFirstPage && onLastPage) {
			onFirstPage->setVisible(self->m_leftArrow->isVisible());
			onLastPage->setVisible(self->m_rightArrow->isVisible());
		}

		//if (auto onSearchBtn = static_cast<gd::CCMenuItemSpriteExtra*>(menu->getChildByTag(14))) {
		//	onSearchBtn->setVisible(m_customSearchQuery.empty());
		//}
		//if (auto onCancelSearch = static_cast<gd::CCMenuItemSpriteExtra*>(menu->getChildByTag(15))) {
		//	onCancelSearch->setVisible(!m_customSearchQuery.empty());
		//}
	}

	updatePageButton(self);
}

void __fastcall LevelBrowserLayer::FLAlert_ClickedH(gd::LevelBrowserLayer* _self, void*, gd::FLAlertLayer* layer, bool btn2) {
	auto self = reinterpret_cast<gd::LevelBrowserLayer*>(reinterpret_cast<uintptr_t>(_self) - 0x11c);
	if ((layer->getTag() == 3) && btn2) {
		auto selectedLevels = LevelCell::getSelectedLevels();
		for (auto level : selectedLevels) {
			if (level) {
				if (level->m_levelType == gd::GJLevelType::Editor) {
					auto localLevelManager = gd::LocalLevelManager::sharedState();
					localLevelManager->m_localLevels->removeObject(level, true);
					localLevelManager->updateLevelOrder();
				}
				if (level->m_levelType == gd::GJLevelType::Saved) {
					auto savedLevels = gd::GameLevelManager::sharedState()->m_savedLevelsDict;
					savedLevels->removeObjectForKey(std::to_string(level->m_levelID));
				}
				selectedLevels.clear();
				self->loadPage(self->m_searchObject);
			}
		}
	}
	LevelBrowserLayer::FLAlert_Clicked(_self, layer, btn2);
}

void LevelBrowserLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x89590), LevelBrowserLayer::initH, reinterpret_cast<void**>(&LevelBrowserLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8a000), LevelBrowserLayer::loadPageH, reinterpret_cast<void**>(&LevelBrowserLayer::loadPage));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8b450), LevelBrowserLayer::FLAlert_ClickedH, reinterpret_cast<void**>(&LevelBrowserLayer::FLAlert_Clicked));
}