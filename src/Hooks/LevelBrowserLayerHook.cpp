#include "LevelBrowserLayerHook.h"
#include "../Layers/ImportLevelLayer.h"

void LevelBrowserLayerHook::updatePageButton(LevelBrowserLayer* self) {
	auto menu = static_cast<CCMenu*>(self->getChildByTag(10));
	if (menu) {
		auto onGoToPage = static_cast<CCMenuItemSpriteExtra*>(menu->getChildByTag(13));
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

void LevelBrowserLayerHook::Callback::onDeleteSelected(CCObject*) {
    return;
	// auto selectedLevels = LevelCell::getSelectedLevels();

	// if (selectedLevels.empty()) {
	// 	FLAlertLayer::create("Nothing here...", "No levels selected.", "OK")->show();
	// }
	// else {
	// 	auto desc = CCString::createWithFormat("Are you sure you want to <cr>delete</c> the <cy>%i</c> selected <cg>levels</c>?", selectedLevels.size())->getCString();
	// 	auto layer = FLAlertLayer::create(this, "Delete", desc, "Back", "Delete");
	// 	layer->setTag(0x3);
	// 	layer->show();
	// }
}

void LevelBrowserLayerHook::Callback::onFirstPage(CCObject* sender) {
	this->m_searchObject->m_page = 0;
	this->loadPage(this->m_searchObject);
}

void LevelBrowserLayerHook::Callback::onLastPage(CCObject* sender) {
	int totalItems = this->m_itemCount;

	this->m_searchObject->m_page = (totalItems % 10 == 0) ? totalItems / 10 - 1 : totalItems / 10;

	this->loadPage(this->m_searchObject);
}

void LevelBrowserLayerHook::Callback::onGoToPage(CCObject* sender) {
    return;
	//GoToPagePopup::create(this)->show();
}

void LevelBrowserLayerHook::Callback::onRefresh(CCObject* sender) {
	GameLevelManager::sharedState()->resetTimerForKey(this->m_searchObject->getKey());
	this->loadPage(this->m_searchObject);

	LOGD("%s", this->m_searchObject->getKey());
}

void LevelBrowserLayerHook::Callback::onImportLevel(CCObject*) {
	ManualImportLevelLayer::create()->show();
}

bool LevelBrowserLayerHook::initH(LevelBrowserLayer* self, GJSearchObject* searchObject) {
    if (!LevelBrowserLayerHook::init(self, searchObject)) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    if (!(searchObject->m_searchType == SearchType::MyLevels || searchObject->m_searchType == SearchType::SavedLevels || searchObject->m_searchType == SearchType::MapPack || searchObject->m_searchType == SearchType::MapPackOnClick)) {
        auto refreshMenu = CCMenu::create();
        refreshMenu->setPosition(director->getScreenRight(), director->getScreenBottom());
        self->addChild(refreshMenu, 2);

        auto onRefreshSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
        auto onRefresh = CCMenuItemSpriteExtra::create(onRefreshSpr, self, menu_selector(LevelBrowserLayerHook::Callback::onRefresh));
        onRefresh->setPosition(-30.f, 30.f);
        refreshMenu->addChild(onRefresh);
    }

    if (searchObject->m_searchType == SearchType::MyLevels || searchObject->m_searchType == SearchType::SavedLevels) {
        auto menu = CCMenu::create();
        self->addChild(menu, 10, 10);

        auto onFirstPageSpr = CCSprite::createWithSpriteFrameName("edit_leftBtn2_001.png");
        auto onFirstPage = CCMenuItemSpriteExtra::create(onFirstPageSpr, self, menu_selector(LevelBrowserLayerHook::Callback::onFirstPage));
        onFirstPage->setPosition(menu->convertToNodeSpace({ director->getScreenLeft() + 20.f, director->getScreenTop() - 70.f }));
        onFirstPage->setVisible(self->m_leftArrow->isVisible());
        menu->addChild(onFirstPage, 0, 11);

        auto onLastPageSpr = CCSprite::createWithSpriteFrameName("edit_rightBtn2_001.png");
        auto onLastPage = CCMenuItemSpriteExtra::create(onLastPageSpr, self, menu_selector(LevelBrowserLayerHook::Callback::onLastPage));
        onLastPage->setPosition(menu->convertToNodeSpace({ director->getScreenRight() - 20.f, director->getScreenTop() - 70.f }));
        onLastPage->setVisible(self->m_rightArrow->isVisible());
        menu->addChild(onLastPage, 0, 12);

        auto onGoToPageSpr = CCSprite::create("GJ_button_02.png");
        onGoToPageSpr->setScale(.7f);
        auto onGoToPageSpr2 = CCLabelBMFont::create("", "bigFont.fnt");
        onGoToPageSpr2->setPosition(20.f, 21.f);
        onGoToPageSpr2->setScale(.8f);
        onGoToPageSpr->addChild(onGoToPageSpr2);
        auto onGoToPage = CCMenuItemSpriteExtra::create(onGoToPageSpr, self, menu_selector(LevelBrowserLayerHook::Callback::onGoToPage));
        onGoToPage->setPosition(menu->convertToNodeSpace({ director->getScreenRight() - 20.f, director->getScreenTop() - 40.f }));
        menu->addChild(onGoToPage, 0, 13);

        updatePageButton(self);

        auto deleteMenu = CCMenu::create();
        self->addChild(deleteMenu, 2, 570);

        auto deleteSprite = CCSprite::createWithSpriteFrameName("edit_delBtn_001.png");
        auto onDeleteSelectedSpr = ButtonSprite::create(deleteSprite, 0x22, 0, 1.f, 36.f, true, "GJ_button_04.png");
        onDeleteSelectedSpr->setScale(.4f);
        auto onDeleteSelected = CCMenuItemSpriteExtra::create(onDeleteSelectedSpr, self, menu_selector(LevelBrowserLayerHook::Callback::onDeleteSelected));
        onDeleteSelected->setPosition(-145.f, -122.f);
        deleteMenu->addChild(onDeleteSelected);
    }

    if (searchObject->m_searchType == SearchType::MyLevels) {
        auto shareMenu = CCMenu::create();
        shareMenu->setPosition(director->getScreenRight(), director->getScreenBottom());
        self->addChild(shareMenu, 2);

        auto onExportLevelSpr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
        if (!onExportLevelSpr->initWithFile("BE_Import_File.png")) {
            onExportLevelSpr->createWithSpriteFrameName("GJ_downloadBtn_001.png");
        }
        auto onExportLevel = CCMenuItemSpriteExtra::create(onExportLevelSpr, self, menu_selector(LevelBrowserLayerHook::Callback::onImportLevel));
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

    return true;
}

void LevelBrowserLayerHook::loadPageH(LevelBrowserLayer* self, GJSearchObject* searchObject) {
    LevelBrowserLayerHook::loadPage(self, searchObject);

    auto menu = static_cast<CCMenu*>(self->getChildByTag(10));
    if (menu) {
        auto onFirstPage = static_cast<CCMenuItemSpriteExtra*>(menu->getChildByTag(11));
        auto onLastPage = static_cast<CCMenuItemSpriteExtra*>(menu->getChildByTag(12));
        if (onFirstPage && onLastPage) {
            onFirstPage->setVisible(self->m_leftArrow->isVisible());
            onLastPage->setVisible(self->m_rightArrow->isVisible());
        }
    }

    updatePageButton(self);
}

void LevelBrowserLayerHook::mem_init() {
    HOOK("_ZN17LevelBrowserLayer4initEP14GJSearchObject", LevelBrowserLayerHook::initH, LevelBrowserLayerHook::init);
    HOOK("_ZN17LevelBrowserLayer8loadPageEP14GJSearchObject", LevelBrowserLayerHook::loadPageH, LevelBrowserLayerHook::loadPage);
}