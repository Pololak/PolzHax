#include "EndLevelLayer.hpp"
#include "Setting.hpp"

void EndLevelLayer::Callback::onLastCheckpoint(CCObject*) {
	if (auto pl = gd::GameManager::sharedState()->getPlayLayer()) {
		this->removeFromParent();
		pl->resetLevel();
		CCEGLView::sharedOpenGLView()->showCursor(gd::GameManager::sharedState()->getGameVariable("0024"));
	}
}

void __fastcall EndLevelLayer::customSetupH(gd::EndLevelLayer* self) {
	EndLevelLayer::customSetup(self);

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
}

void EndLevelLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x50430), EndLevelLayer::customSetupH, reinterpret_cast<void**>(&EndLevelLayer::customSetup));
}