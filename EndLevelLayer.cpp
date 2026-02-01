#include "EndLevelLayer.hpp"
#include "Setting.hpp"

cocos2d::CCSprite* m_completeSprite;

void EndLevelLayer::Callback::onShowEndLayer(CCObject* sender) {
	auto director = CCDirector::sharedDirector();
	const CCPoint pos = this->m_endPosition;
	this->stopAllActions();
	this->runAction(CCFadeTo::create(.5f, 125));
	this->m_mainLayer->runAction(CCEaseInOut::create(CCMoveTo::create(.5, pos), 2.f));
	auto btn = static_cast<gd::CCMenuItemSpriteExtra*>(sender);
	btn->setEnabled(false);
	btn->runAction(CCSequence::create(CCEaseInOut::create(CCMoveTo::create(.5f, ccp(0.f, btn->getPositionY() + 35.f)), 2.f), CCFadeTo::create(.5f, 255), nullptr));
	auto onHideEndLayer = static_cast<gd::CCMenuItemSpriteExtra*>(this->m_actionsMenu->getChildByTag(54));
	if (onHideEndLayer) {
		onHideEndLayer->setEnabled(true);
	}
}

void EndLevelLayer::Callback::onHideEndLayer(CCObject* sender) {
	const CCPoint pos = this->m_startPosition;
	this->stopAllActions();
	this->runAction(CCFadeTo::create(.5f, 0));
	this->m_mainLayer->runAction(CCEaseInOut::create(CCMoveTo::create(.5, pos), 2.f));
	static_cast<gd::CCMenuItemSpriteExtra*>(sender)->setEnabled(false);
	auto showEndLayerMenu = static_cast<CCMenu*>(this->getChildByTag(55));
	if (showEndLayerMenu) {
		auto onShowEndLayer = static_cast<gd::CCMenuItemSpriteExtra*>(showEndLayerMenu->getChildByTag(56));
		if (onShowEndLayer) {
			onShowEndLayer->setEnabled(true);
			onShowEndLayer->runAction(CCSequence::create(CCEaseInOut::create(CCMoveTo::create(.5f, ccp(0.f, onShowEndLayer->getPositionY() - 35.f)), 2.f), CCFadeTo::create(.5f, 125), nullptr));
		}
	}
}

void EndLevelLayer::Callback::onLastCheckpoint(CCObject*) {
	if (auto pl = gd::GameManager::sharedState()->getPlayLayer()) {
		this->removeFromParent();
		pl->resetLevel();
		CCEGLView::sharedOpenGLView()->showCursor(gd::GameManager::sharedState()->getGameVariable("0024"));
	}
}

void __fastcall EndLevelLayer::customSetupH(gd::EndLevelLayer* self) {
	EndLevelLayer::customSetup(self);

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto practiceButton = gd::CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_practiceBtn_001.png"), self, menu_selector(EndLevelLayer::Callback::onLastCheckpoint));
	practiceButton->setPositionY(-125.f);

	if (setting().onReplayLastCheckpoint && self->m_actionsMenu && gd::GameManager::sharedState()->getPlayLayer()->m_practiceMode) {
		if (self->m_actionsMenu->getChildrenCount() == 2) {
			static_cast<gd::CCMenuItemSpriteExtra*>(self->m_actionsMenu->getChildren()->objectAtIndex(0))->setPositionX(-100.f);
			static_cast<gd::CCMenuItemSpriteExtra*>(self->m_actionsMenu->getChildren()->objectAtIndex(1))->setPositionX(100.f);
		}
		else {
			static_cast<gd::CCMenuItemSpriteExtra*>(self->m_actionsMenu->getChildren()->objectAtIndex(0))->setPositionX(-130.f);
			static_cast<gd::CCMenuItemSpriteExtra*>(self->m_actionsMenu->getChildren()->objectAtIndex(1))->setPositionX(130.f);
			static_cast<gd::CCMenuItemSpriteExtra*>(self->m_actionsMenu->getChildren()->objectAtIndex(2))->setPositionX(-45.f);
			practiceButton->setPositionX(45.f);
		}
		self->m_actionsMenu->addChild(practiceButton);
	}

	if (m_completeSprite) {
		if (setting().isSafeMode || setting().onSafeMode) {
			m_completeSprite->setVisible(false);

			auto safeModeLabel = CCLabelBMFont::create("Safe Mode", "bigFont.fnt");
			safeModeLabel->setPosition(m_completeSprite->getPosition());
			safeModeLabel->setScale(1.25f);
			self->m_mainLayer->addChild(safeModeLabel, 15);
		}
	}

	auto onHideEndLayerSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	onHideEndLayerSpr->setScale(.6f);
	onHideEndLayerSpr->setRotation(90.f);
	onHideEndLayerSpr->setFlipY(true);
	auto onHideEndLayer = gd::CCMenuItemSpriteExtra::create(onHideEndLayerSpr, self, menu_selector(EndLevelLayer::Callback::onHideEndLayer));
	onHideEndLayer->setPositionY(127.f);
	self->m_actionsMenu->addChild(onHideEndLayer, 0, 54);

	auto showEndLayerMenu = CCMenu::create();
	self->addChild(showEndLayerMenu, 0, 55);
	auto onShowEndLayerSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	onShowEndLayerSpr->setScale(.6f);
	onShowEndLayerSpr->setRotation(-90.f);
	auto onShowEndLayer = gd::CCMenuItemSpriteExtra::create(onShowEndLayerSpr, self, menu_selector(EndLevelLayer::Callback::onShowEndLayer));
	onShowEndLayer->setEnabled(false);
	onShowEndLayer->setPosition(showEndLayerMenu->convertToNodeSpace(ccp(winSize.width / 2.f, director->getScreenTop() + 20.f)));
	showEndLayerMenu->addChild(onShowEndLayer, 0, 56);
}

void __fastcall EndLevelLayer::completeSpriteH() {
	__asm {
		mov m_completeSprite, eax
	}
	EndLevelLayer::completeSprite();
}

void EndLevelLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x50430), EndLevelLayer::customSetupH, reinterpret_cast<void**>(&EndLevelLayer::customSetup));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x50503), EndLevelLayer::completeSpriteH, reinterpret_cast<void**>(&EndLevelLayer::completeSprite));
}