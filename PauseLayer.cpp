#include "PauseLayer.hpp"
#include "Setting.hpp"
#include "utils.hpp"

gd::PauseLayer* m_pauseLayer;
CCMenu* m_bottomMenu;
CCLabelBMFont* m_progressBarLabel;

gd::PauseLayer* PauseLayer::get() {
	return m_pauseLayer;
}

void PauseLayer::Callback::onShowPercentage(CCObject*) {
	setting().onShowPercentage = !setting().onShowPercentage;
	auto playLayer = gd::GameManager::sharedState()->getPlayLayer();
	if (playLayer) {
		auto percentageLabel = static_cast<CCLabelBMFont*>(playLayer->getChildByTag(301));
		if (percentageLabel) {
			percentageLabel->setVisible(setting().onShowPercentage);
		}
	}
}

void __fastcall PauseLayer::customSetupH(gd::PauseLayer* self) {
	m_pauseLayer = self;
	PauseLayer::customSetup(self);

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	self->setVisible(!setting().onHidePauseMenu);

	if (m_bottomMenu == nullptr) return;

	self->createToggleButton("%", menu_selector(PauseLayer::Callback::onShowPercentage), !setting().onShowPercentage, m_bottomMenu, ccp(winSize.width / 2.f + 155.f, director->getScreenBottom() + 25.f));
}

void __fastcall PauseLayer::onEditH(gd::PauseLayer* self, void*, CCObject* sender) {
	PauseLayer::onEdit(self, sender);
	m_bottomMenu = nullptr;
	m_pauseLayer = nullptr;
}

void __fastcall PauseLayer::onResumeH(gd::PauseLayer* self, void*, CCObject* sender) {
	PauseLayer::onResume(self, sender);
	m_bottomMenu = nullptr;
	m_pauseLayer = nullptr;
}

void __fastcall PauseLayer::onQuitH(gd::PauseLayer* self, void*, CCObject* sender) {
	PauseLayer::onQuit(self, sender);
	m_bottomMenu = nullptr;
	m_pauseLayer = nullptr;
}

void __fastcall PauseLayer::onRestartH(gd::PauseLayer* self, void*, CCObject* sender) {
	PauseLayer::onRestart(self, sender);
	m_bottomMenu = nullptr;
	m_pauseLayer = nullptr;
}

void __fastcall PauseLayer::onProgressBarH(gd::PauseLayer* self, void*, CCObject* sender) {
	PauseLayer::onProgressBar(self, sender);

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto gm = gd::GameManager::sharedState();
	auto playLayer = gm->getPlayLayer();

	if (playLayer) {
		auto percentageLabel = static_cast<CCLabelBMFont*>(playLayer->getChildByTag(301));
		if (percentageLabel) {
			percentageLabel->setAnchorPoint({ (gm->m_showProgressBar ? 0.f : .5f), .5f });
			percentageLabel->setPosition(winSize.width / 2.f + (gm->m_showProgressBar ? 110.2f : 0.f), winSize.height - 8.f);
		}
	}
}

void __fastcall PauseLayer::bottomMenuH() {
	__asm {
		mov m_bottomMenu, eax
	}
	PauseLayer::bottomMenu();
}

void PauseLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd5f50), PauseLayer::customSetupH, reinterpret_cast<void**>(&PauseLayer::customSetup));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd7bf0), PauseLayer::onEditH, reinterpret_cast<void**>(&PauseLayer::onEdit));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd7a80), PauseLayer::onResumeH, reinterpret_cast<void**>(&PauseLayer::onResume));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd7f00), PauseLayer::onQuitH, reinterpret_cast<void**>(&PauseLayer::onQuit));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd7b20), PauseLayer::onRestartH, reinterpret_cast<void**>(&PauseLayer::onRestart));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd8010), PauseLayer::onProgressBarH, reinterpret_cast<void**>(&PauseLayer::onProgressBar));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd66df), PauseLayer::bottomMenuH, reinterpret_cast<void**>(&PauseLayer::bottomMenu));
}