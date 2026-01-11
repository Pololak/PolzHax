#include "PlayLayer.hpp"
#include "Setting.hpp"

std::vector<gd::GameObject*> m_coinsToPickup;

bool __fastcall PlayLayer::initH(gd::PlayLayer* self, void*, gd::GJGameLevel* level) {
	m_coinsToPickup.clear();

	if (!PlayLayer::init(self, level)) return false;

	//
	self->m_attemptsLabel->setVisible(!setting().onHideAttempts);

	if (gd::GameManager::sharedState()->getGameVariable("0024")) {
		self->m_uiLayer->m_pauseBtn->setVisible(!setting().onHidePauseButton);
	}

	self->m_player->setVisible(!setting().onHidePlayer);
	self->m_player2->setVisible(!setting().onHidePlayer);

	self->m_player->m_hardStreak->setVisible(!setting().onNoWaveTrail);
	self->m_player2->m_hardStreak->setVisible(!setting().onNoWaveTrail);

	if (setting().onShowTotalAttempts) {
		self->m_attemptsLabel->setString(CCString::createWithFormat("Attempt %i", level->m_attempts + 1)->getCString());
	}
	//

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto gm = gd::GameManager::sharedState();

	float playerPercentPos = self->m_player->getPositionX() / self->m_levelLength * 100.f;

	auto percentageLabel = CCLabelBMFont::create("", "bigFont.fnt");
	percentageLabel->setAnchorPoint({ (gm->m_showProgressBar ? 0.f : .5f), .5f });
	percentageLabel->setScale(.5f);
	percentageLabel->setVisible(setting().onShowPercentage);
	percentageLabel->setPosition(winSize.width / 2.f + (gm->m_showProgressBar ? 110.2f : 0.f), winSize.height - 8.f);
	percentageLabel->setString(CCString::createWithFormat("%.0f%%", playerPercentPos)->getCString());
	self->addChild(percentageLabel, 15, 301);

	return true;
}

void __fastcall PlayLayer::updateH(gd::PlayLayer* self, void*, float dt) {
	PlayLayer::update(self, dt);

	float playerPercentPos = self->m_player->getPositionX() / self->m_levelLength * 100.f;

	auto percentageLabel = static_cast<CCLabelBMFont*>(self->getChildByTag(301));
	if (percentageLabel) {
		if (playerPercentPos < 100.f) {
			percentageLabel->setString(CCString::createWithFormat("%.0f%%", playerPercentPos)->getCString());
		}
		else {
			percentageLabel->setString(CCString::create("100%")->getCString());
		}
	}

	if (setting().onLockCursor && !setting().show && !self->m_showingEndLayer && !self->m_isDead) {
		HWND hwnd = WindowFromDC(wglGetCurrentDC());
		RECT winSize; GetWindowRect(hwnd, &winSize);
		auto width = winSize.right - winSize.left;
		auto height = winSize.bottom - winSize.top;
		auto centerX = width / 2.f + winSize.left;
		auto centerY = height / 2.f + winSize.top;
		SetCursorPos(centerX, centerY);
	}
}

void __fastcall PlayLayer::resetLevelH(gd::PlayLayer* self) {
	PlayLayer::resetLevel(self);

	// Don't forget
	//for (auto* coin : m_coinsToPickup) {
	//	if (coin == nullptr) continue;

	//	coin->destroyObject();
	//	self->pickupItem(coin);
	//}
}

void __fastcall PlayLayer::addToSectionH(gd::PlayLayer* self, void*, gd::GameObject* object) {
	PlayLayer::addToSection(self, object);

	if (object->m_objectID == 142) {
		m_coinsToPickup.push_back(object);
	}
}

void __fastcall PlayLayer::togglePracticeModeH(gd::PlayLayer* self, void*, bool practice) {
	PlayLayer::togglePracticeMode(self, practice);

	if (setting().onHidePracticeButtons) {
		self->m_uiLayer->m_checkpointMenu->setVisible(!setting().onHidePracticeButtons);
	}
}

void __fastcall PlayLayer::showNewBestH(gd::PlayLayer* self) {
	if (!setting().onNoNewBestPopup) PlayLayer::showNewBest(self);
}

void __fastcall PlayLayer::updateVisibilityH(gd::PlayLayer* self) {
	PlayLayer::updateVisibility(self);

	//
	if (self->m_attemptsLabel->isVisible() && setting().onHideAttempts) {
		self->m_attemptsLabel->setVisible(false);
	}

	if (self->m_player->isVisible() && setting().onHidePlayer) {
		self->m_player->setVisible(false);
	}
	if (self->m_player2->isVisible() && setting().onHidePlayer) {
		self->m_player2->setVisible(false);
	}
	//

	if (setting().onNoWavePulse) {
		self->m_player->m_audioScale = 1.f;
		self->m_player2->m_audioScale = 1.f;
	}
}

void __fastcall PlayLayer::updateAttemptsH(gd::PlayLayer* self) {
	PlayLayer::updateAttempts(self);

	if (setting().onShowTotalAttempts) {
		self->m_attemptsLabel->setString(CCString::createWithFormat("Attempt %i", self->m_level->m_attempts + 1)->getCString());
	}
}

void PlayLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe35d0), PlayLayer::initH, reinterpret_cast<void**>(&PlayLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe9360), PlayLayer::updateH, reinterpret_cast<void**>(&PlayLayer::update));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf1f20), PlayLayer::resetLevelH, reinterpret_cast<void**>(&PlayLayer::resetLevel));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe9280), PlayLayer::addToSectionH, reinterpret_cast<void**>(&PlayLayer::addToSection));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf3610), PlayLayer::togglePracticeModeH, reinterpret_cast<void**>(&PlayLayer::togglePracticeMode));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe5ff0), PlayLayer::showNewBestH, reinterpret_cast<void**>(&PlayLayer::showNewBest));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xeb3f0), PlayLayer::updateVisibilityH, reinterpret_cast<void**>(&PlayLayer::updateVisibility));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf33a0), PlayLayer::updateAttemptsH, reinterpret_cast<void**>(&PlayLayer::updateAttempts));
}