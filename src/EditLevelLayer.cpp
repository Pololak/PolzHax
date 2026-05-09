#include "EditLevelLayer.hpp"
#include "Setting.hpp"

void PolzEditLevelLayer::onLevelOrderUp(CCObject* sender) {
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

    if (auto orderDownBtn = this->m_fields->m_orderDownBtn) {
        orderDownBtn->setEnabled(true);
        orderDownBtn->setOpacity(255);
        orderDownBtn->setColor(ccWHITE);
    }

    this->updateLevelOrderLabel();
}

void PolzEditLevelLayer::onLevelOrderDown(CCObject* sender) {
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

	if (auto orderUpBtn = this->m_fields->m_orderUpBtn) {
		orderUpBtn->setEnabled(true);
		orderUpBtn->setOpacity(255);
		orderUpBtn->setColor(ccWHITE);
	}

	this->updateLevelOrderLabel();
}

void PolzEditLevelLayer::updateLevelOrderLabel() {
    auto label = this->m_fields->m_orderLabel;
    if (label) {
        label->setString(CCString::createWithFormat("%i", LocalLevelManager::sharedState()->m_localLevels->indexOfObject(this->m_level) + 1)->getCString());
    }
}

void PolzEditLevelLayer::onMoveToTop(CCObject*) {
    auto flalert = FLAlertLayer::create(this, "Move To Top", "Move this level to the top of the created levels list?", "NO", "YES");
    flalert->setID("move-to-top-alert"_spr);
    flalert->show();
}

bool PolzEditLevelLayer::init(GJGameLevel* level) {
    if (!EditLevelLayer::init(level)) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    auto localLevels = LocalLevelManager::sharedState()->m_localLevels;

    auto levelActionsMenu = static_cast<CCMenu*>(this->getChildByID("level-actions-menu"));

    if (levelActionsMenu) {
        auto onMoveToTopSpr = CCSprite::create("GJ_orderUpBtn_001.png"_spr);
        onMoveToTopSpr->setScale(.925f);
        auto onMoveToTop = CCMenuItemSpriteExtra::create(onMoveToTopSpr, this, menu_selector(PolzEditLevelLayer::onMoveToTop));
        levelActionsMenu->addChild(onMoveToTop);
        levelActionsMenu->updateLayout();
    }

    auto orderMenu = CCMenu::create();
    orderMenu->setPosition(winSize.width / 2.f - 160.f, winSize.height / 2.f - 25.f);
    this->addChild(orderMenu);

    this->m_fields->m_orderLabel = CCLabelBMFont::create("", "bigFont.fnt");
    this->m_fields->m_orderLabel->setScale(.75f);
    this->m_fields->m_orderLabel->setPosition(winSize.width / 2.f - 160.f, winSize.height / 2.f - 25.f);
    this->addChild(this->m_fields->m_orderLabel);
    this->updateLevelOrderLabel();

    auto onLevelOrderUpSpr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
    this->m_fields->m_orderUpBtn = CCMenuItemSpriteExtra::create(onLevelOrderUpSpr, this, menu_selector(PolzEditLevelLayer::onLevelOrderUp));
    this->m_fields->m_orderUpBtn->setPositionY(22.5f);
    this->m_fields->m_orderUpBtn->setSizeMult(1.5f);
    if (localLevels->indexOfObject(level) == 0) {
        this->m_fields->m_orderUpBtn->setEnabled(false);
        this->m_fields->m_orderUpBtn->setOpacity(175);
        this->m_fields->m_orderUpBtn->setColor(ccGRAY);
    }
    orderMenu->addChild(this->m_fields->m_orderUpBtn);

    auto onLevelOrderDownSpr = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
    this->m_fields->m_orderDownBtn = CCMenuItemSpriteExtra::create(onLevelOrderDownSpr, this, menu_selector(PolzEditLevelLayer::onLevelOrderDown));
    this->m_fields->m_orderDownBtn->setPositionY(-22.5f);
    this->m_fields->m_orderDownBtn->setSizeMult(1.5f);
    if (localLevels->indexOfObject(level) == (localLevels->count() - 1)) {
        this->m_fields->m_orderDownBtn->setEnabled(false);
        this->m_fields->m_orderDownBtn->setOpacity(175);
        this->m_fields->m_orderDownBtn->setColor(ccGRAY);
    }
    orderMenu->addChild(this->m_fields->m_orderDownBtn);

    return true;
}

void PolzEditLevelLayer::onEdit(CCObject* sender) {
    if (setting().onNoRotation && !this->m_fields->m_noRotationPass) {
        auto flalert = FLAlertLayer::create(this, "No Rotation", "You have <cr>No Rotation</c> enabled, this will reset all rotated objects in your level if you save it. Do you still want to continue?", "NO", "YES");
        flalert->setID("no-rotation-alert"_spr);
        flalert->show();
        return;
    }
    EditLevelLayer::onEdit(sender);
}

void PolzEditLevelLayer::FLAlert_Clicked(FLAlertLayer* layer, bool btn2) {
    if ((layer->getID() == "move-to-top-alert"_spr) && btn2) {
        LocalLevelManager::sharedState()->m_localLevels->removeObject(this->m_level, true);
        LocalLevelManager::sharedState()->m_localLevels->insertObject(this->m_level, 0);
        LocalLevelManager::sharedState()->updateLevelOrder();
        this->updateLevelOrderLabel();
    }
    if ((layer->getID() == "no-rotation-alert"_spr) && btn2) {
        this->m_fields->m_noRotationPass = true;
        this->onEdit(nullptr);
        this->m_fields->m_noRotationPass = false;
    }
    EditLevelLayer::FLAlert_Clicked(layer, btn2);
}