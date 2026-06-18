#include "EditLevelLayerHook.h"
#include "../Layers/ExportLevelLayer.h"
#include "../Menu/PolzHaxLayer.h"

void EditLevelLayerHook::Callback::onLevelID(CCObject*) {
    return;
}

void EditLevelLayerHook::Callback::onLevelOrderUp(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto localLevelManager = LocalLevelManager::sharedState();
    auto levelOrder = localLevelManager->m_localLevels->indexOfObject(this->m_level);
    if (levelOrder > 0) {
        levelOrder--;
        localLevelManager->m_localLevels->removeObject(this->m_level);
        localLevelManager->m_localLevels->insertObject(this->m_level, levelOrder);
        localLevelManager->updateLevelOrder();
    }

    if (levelOrder == 0) {
        if (btn) {
            btn->setEnabled(false);
            btn->setOpacity(175);
            btn->setColor(ccGRAY);
        }
    }

    if (auto orderDownBtn = static_cast<CCMenuItemSpriteExtra*>(static_cast<CCMenu*>(this->getChildByTag(11))->getChildByTag(2))) {
        orderDownBtn->setEnabled(true);
        orderDownBtn->setOpacity(255);
        orderDownBtn->setColor(ccWHITE);
    }

    EditLevelLayerHook::updateLevelOrderLabel(this);
}

void EditLevelLayerHook::Callback::onLevelOrderDown(CCObject* sender) {
	auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
	auto localLevelManager = LocalLevelManager::sharedState();
	auto levelOrder = localLevelManager->m_localLevels->indexOfObject(this->m_level);
	if (levelOrder < (localLevelManager->m_localLevels->count() - 1)) {
		levelOrder++;
		localLevelManager->m_localLevels->removeObject(this->m_level);
		localLevelManager->m_localLevels->insertObject(this->m_level, levelOrder);
		localLevelManager->updateLevelOrder();
	}

	if (levelOrder == (localLevelManager->m_localLevels->count() - 1)) {
		if (btn) {
			btn->setEnabled(false);
			btn->setOpacity(175);
			btn->setColor(ccGRAY);
		}
	}

	if (auto orderUpBtn = static_cast<CCMenuItemSpriteExtra*>(static_cast<CCMenu*>(this->getChildByTag(11))->getChildByTag(1))) {
		orderUpBtn->setEnabled(true);
		orderUpBtn->setOpacity(255);
		orderUpBtn->setColor(ccWHITE);
	}

	updateLevelOrderLabel(this);
}

void EditLevelLayerHook::updateLevelOrderLabel(EditLevelLayer* self) {
	auto label = static_cast<CCLabelBMFont*>(self->getChildByTag(12));
	if (label) {
		label->setString(CCString::createWithFormat("%i", LocalLevelManager::sharedState()->m_localLevels->indexOfObject(self->m_level) + 1)->getCString());
	}
}

void EditLevelLayerHook::Callback::onExportLevel(CCObject*) {
    ExportLevelLayer::create(this->m_level)->show();
}

void EditLevelLayerHook::Callback::onMoveToTop(CCObject*) {
	auto flalert = FLAlertLayer::create(this, "Move To Top", "Move this level to the top of the created levels list?", "NO", "YES");
	flalert->setTag(0x1b);
	flalert->show();
}

void EditLevelLayerHook::Callback::onPolzHax(CCObject*) {
    PolzHaxLayer::create()->show();
}

bool EditLevelLayerHook::initH(EditLevelLayer* self, GJGameLevel* level) {
    if (!EditLevelLayerHook::init(self, level)) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    auto localLevels = LocalLevelManager::sharedState()->m_localLevels;

    auto levelActionsMenu = self->getChildByType<CCMenu*>(1);
    if (levelActionsMenu) {
        auto onCopy = static_cast<CCMenuItemSpriteExtra*>(levelActionsMenu->getChildByType<CCMenuItemSpriteExtra*>(2));

        auto onMoveToTopSpr = CCSprite::create("GJ_orderUpBtn_001.png");
        onMoveToTopSpr->setScale(.925f);
        auto onMoveToTop = CCMenuItemSpriteExtra::create(onMoveToTopSpr, self, menu_selector(EditLevelLayerHook::Callback::onMoveToTop));
        onMoveToTop->setPosition(onCopy->getPositionX(), onCopy->getPositionY() - 60.f);
        levelActionsMenu->addChild(onMoveToTop);
    }

    auto menu = CCMenu::create();
    self->addChild(menu);

    auto onPolzHaxSpr = CCSprite::create("GJ_polzhaxBtn_001.png");
    onPolzHaxSpr->setScale(.85f);
    auto onPolzHax = CCMenuItemSpriteExtra::create(onPolzHaxSpr, self, menu_selector(EditLevelLayerHook::Callback::onPolzHax));
    onPolzHax->setPosition(menu->convertToNodeSpace({director->getScreenLeft() + 30.f, winSize.height / 2.f}));
    menu->addChild(onPolzHax);

    auto shareMenu = CCMenu::create();
    shareMenu->setPosition(director->getScreenRight(), director->getScreenBottom());
    self->addChild(shareMenu, 1);

    auto onExportLevelSpr = CCSprite::create("BE_Export_File.png");
    auto onExportLevel = CCMenuItemSpriteExtra::create(onExportLevelSpr, self, menu_selector(EditLevelLayerHook::Callback::onExportLevel));
    onExportLevel->setPosition(-30.f, 30.f);
    shareMenu->addChild(onExportLevel);

    auto orderMenu = CCMenu::create();
    orderMenu->setPosition(winSize.width / 2.f - 160.f, winSize.height / 2.f - 25.f);
    self->addChild(orderMenu, 1, 11);

    auto orderLabel = CCLabelBMFont::create("", "bigFont.fnt");
    orderLabel->setScale(.75f);
    orderLabel->setPosition(winSize.width / 2.f - 160.f, winSize.height / 2.f - 25.f);
    self->addChild(orderLabel, 0, 12);
    updateLevelOrderLabel(self);

    auto onLevelOrderUpSpr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
    auto onLevelOrderUp = CCMenuItemSpriteExtra::create(onLevelOrderUpSpr, self, menu_selector(EditLevelLayerHook::Callback::onLevelOrderUp));
    onLevelOrderUp->setPositionY(22.5f);
    onLevelOrderUp->setSizeMult(1.5f);
    if (localLevels->indexOfObject(level) == 0) {
        onLevelOrderUp->setEnabled(false);
        onLevelOrderUp->setOpacity(175);
        onLevelOrderUp->setColor(ccGRAY);
    }
    orderMenu->addChild(onLevelOrderUp, 0, 1);

    auto onLevelOrderDownSpr = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
    auto onLevelOrderDown = CCMenuItemSpriteExtra::create(onLevelOrderDownSpr, self, menu_selector(EditLevelLayerHook::Callback::onLevelOrderDown));
    onLevelOrderDown->setPositionY(-22.5f);
    onLevelOrderDown->setSizeMult(1.5f);
    if (localLevels->indexOfObject(level) == (localLevels->count() - 1)) {
        onLevelOrderDown->setEnabled(false);
        onLevelOrderDown->setOpacity(175);
        onLevelOrderDown->setColor(ccGRAY);
    }
    orderMenu->addChild(onLevelOrderDown, 0, 2);

    return true;
}

void EditLevelLayerHook::FLAlert_ClickedH(EditLevelLayer* self, FLAlertLayer* layer, bool btn2) {
    if ((layer->getTag() == 0x1b) && btn2) {
        LocalLevelManager::sharedState()->m_localLevels->removeObject(self->m_level, true);
        LocalLevelManager::sharedState()->m_localLevels->insertObject(self->m_level, 0);
        LocalLevelManager::sharedState()->updateLevelOrder();
        EditLevelLayerHook::updateLevelOrderLabel(self);
    }
    EditLevelLayerHook::FLAlert_Clicked(self, layer, btn2);
}

void EditLevelLayerHook::onCloneH(EditLevelLayer* self) {
    EditLevelLayerHook::onClone(self);

    static_cast<GJGameLevel*>(LocalLevelManager::sharedState()->m_localLevels->objectAtIndex(0))->m_songID = self->m_level->m_songID;
}

void EditLevelLayerHook::mem_init() {
    HOOK("_ZN14EditLevelLayer4initEP11GJGameLevel", EditLevelLayerHook::initH, EditLevelLayerHook::init);
    HOOK("_ZN14EditLevelLayer15FLAlert_ClickedEP12FLAlertLayerb", EditLevelLayerHook::FLAlert_ClickedH, EditLevelLayerHook::FLAlert_Clicked);
    HOOK("_ZN14EditLevelLayer7onCloneEv", EditLevelLayerHook::onCloneH, EditLevelLayerHook::onClone);
}