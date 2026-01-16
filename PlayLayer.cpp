#include "PlayLayer.hpp"
#include "Setting.hpp"
#include "Hitboxes.hpp"
#include "PracticeFix.hpp"
#include "utils.hpp"

std::vector<gd::GameObject*> m_coinsToPickup;

CCObject* m_hz;

bool m_deafenPressed = false;

int m_smoothFrames = 0; // https://github.com/qimiko/gdps-public/blob/238b71e9f3cd8fdf855556ce4cc7c498f22cf3c0/include/hooks/PlayLayer.hpp#L16

std::vector<Checkpoint> m_checkpoints;

std::vector<gd::StartPosObject*> m_startPositions;
std::vector<gd::GameObject*> m_dualPortals, m_gamemodePortals, m_miniPortals, m_speedChanges, m_mirrorPortals;

int currentStartPos = 0;
std::vector<gd::StartPosObject*> startPosObjects;

void pickStartPos(gd::PlayLayer* playLayer, int32_t index) { // Eclipse menu
	if (startPosObjects.empty()) return;

	auto count = static_cast<int32_t>(startPosObjects.size());
	if (index >= count) index = -1;
	else if (index < -1) index = count - 1;

	currentStartPos = index;

	auto* startPos = index >= 0 ? startPosObjects[index] : nullptr;
	playLayer->setStartPosObject(startPos);
	playLayer->m_testMode = index >= 0;

	if (currentStartPos >= 0) { // Taswert's thing
		playLayer->m_playerStartPos = playLayer->m_startPosObject->getOrientedBox()->m_center;
	}
	else {
		playLayer->m_playerStartPos = ccp(0, 105);
	}

	playLayer->resetLevel();
}

void PlayLayer::nextStartPos() {
	pickStartPos(gd::GameManager::sharedState()->getPlayLayer(), currentStartPos + 1);
}

void PlayLayer::prevStartPos() {
	pickStartPos(gd::GameManager::sharedState()->getPlayLayer(), currentStartPos - 1);
}

static gd::GameObject* getClosestObject(std::vector<gd::GameObject*>& vec, gd::StartPosObject* startPos) {
	gd::GameObject* closest = nullptr;

	std::ranges::sort(vec, [](gd::GameObject* a, gd::GameObject* b) {
		return a->getPositionX() < b->getPositionX();
		});

	for (auto obj : vec) {
		if (obj->getPositionX() - 10 > startPos->getPositionX())
			break;
		if (obj->getPositionX() - 10 < startPos->getPositionX())
			closest = obj;
	}

	return closest;
}

void setupStartPos(gd::StartPosObject* startPos) { // Eclipse menu https://github.com/EclipseMenu/EclipseMenu/blob/main/src/hacks/Level/SmartStartPos.cpp
	gd::LevelSettingsObject* startPosSettings = startPos->m_settings;
	gd::LevelSettingsObject* levelSettings = gd::GameManager::sharedState()->getPlayLayer()->m_levelSettings;

	startPosSettings->m_startDual = levelSettings->m_startDual;
	startPosSettings->m_startMode = levelSettings->m_startMode;
	startPosSettings->m_startMini = levelSettings->m_startMini;
	startPosSettings->m_startSpeed = levelSettings->m_startSpeed;

	gd::GameObject* obj = getClosestObject(m_dualPortals, startPos);
	if (obj)
		startPosSettings->m_startDual = obj->m_objectID == 286;

	obj = getClosestObject(m_gamemodePortals, startPos);

	if (obj) {
		switch (obj->m_objectID) {
		case 12: startPosSettings->m_startMode = 0;
			break;
		case 13: startPosSettings->m_startMode = 1;
			break;
		case 47: startPosSettings->m_startMode = 2;
			break;
		case 111: startPosSettings->m_startMode = 3;
			break;
		case 660: startPosSettings->m_startMode = 4;
			break;
		default: break;
		}
	}

	obj = getClosestObject(m_miniPortals, startPos);

	if (obj)
		startPosSettings->m_startMini = obj->m_objectID == 101;

	obj = getClosestObject(m_speedChanges, startPos);
	if (obj) {
		switch (obj->m_objectID) {
		case 200: startPosSettings->m_startSpeed = 1;
			break;
		case 201: startPosSettings->m_startSpeed = 0;
			break;
		case 202: startPosSettings->m_startSpeed = 2;
			break;
		case 203: startPosSettings->m_startSpeed = 3;
			break;
		default: break;
		}
	}
}

void PlayLayer::updateShowLayout() {
	auto self = gd::GameManager::sharedState()->getPlayLayer();

	if (self == nullptr) return;

	ccColor3B bgColor = ccc3(setting().layoutBGR, setting().layoutBGG, setting().layoutBGB);
	ccColor3B gColor = ccc3(setting().layoutGR, setting().layoutGG, setting().layoutGB);

	self->m_backgroundSprite->setColor(bgColor);
	self->m_bottomGround->m_groundSprite->setColor(gColor);
	self->m_bottomGround->m_line->setColor(ccWHITE);
	self->m_topGround->m_groundSprite->setColor(gColor);
	self->m_topGround->m_line->setColor(ccWHITE);

	self->m_backgroundFlash->setVisible(false);

	for (int i = self->m_firstVisibleSection - 1; i <= self->m_lastVisibleSection + 1; i++) {
		if (i < 0) continue;
		if (i >= self->m_levelSections->count()) break;

		auto objectAtIndex = self->m_levelSections->objectAtIndex(i);
		auto objArr = reinterpret_cast<CCArray*>(objectAtIndex);

		for (int j = 0; j < objArr->count(); j++) {
			auto obj = reinterpret_cast<gd::GameObject*>(objArr->objectAtIndex(j));
			if ((obj->m_objectType == gd::GameObjectType::Decoration || obj->m_objectType == gd::GameObjectType::PulsingDecoration) && obj->isVisible()/* && (obj->m_objectID == 50 && obj->m_objectID == 51 && obj->m_objectID == 52 && obj->m_objectID == 53 && obj->m_objectID == 54 && obj->m_objectID == 60 && obj->m_objectID == 148 && obj->m_objectID == 149 && obj->m_objectID == 405) */ && (obj->m_objectID != 10 && obj->m_objectID != 11 && obj->m_objectID != 12 && obj->m_objectID != 13 && obj->m_objectID != 38 && obj->m_objectID != 44 && obj->m_objectID != 45 && obj->m_objectID != 46 && obj->m_objectID != 47 && obj->m_objectID != 99 && obj->m_objectID != 101 && obj->m_objectID != 111 && obj->m_objectID != 286 && obj->m_objectID != 287 && obj->m_objectID != 660 && obj->m_objectID != 745 && obj->m_objectID != 749) && obj != self->m_endPortalObject) {
				obj->setVisible(false);
			}

			obj->setObjectColor(ccWHITE);
			if (obj->m_colorSprite) obj->m_colorSprite->setColor(ccWHITE);
			obj->m_customColorMode = static_cast<gd::GJCustomColorMode>(9);
		}
	}
}

void PlayLayer::updateShowHitboxes() {
	auto self = gd::GameManager::sharedState()->getPlayLayer();

	if (self == nullptr) return;

	auto playerDrawNode = static_cast<CCDrawNode*>(self->m_gameLayer->getChildByTag(124));
	playerDrawNode->clear();
	auto objectDrawNode = static_cast<CCDrawNode*>(self->m_gameLayer->getChildByTag(125));
	objectDrawNode->clear();

	if ((self->m_player->m_isDead && setting().onHitboxesOnDeath) || setting().onHitboxes) {
		if (setting().onPlayerHitboxes) {
			if (self->m_player) Hitboxes::drawPlayerHitbox(self->m_player, playerDrawNode);
			if (self->m_player2) Hitboxes::drawPlayerHitbox(self->m_player2, playerDrawNode);
		}

		for (int i = self->m_firstVisibleSection + 1; i <= self->m_lastVisibleSection - 1; i++) {
			if (i < 0) continue;
			if (i >= self->m_levelSections->count()) break;

			auto objectAtIndex = self->m_levelSections->objectAtIndex(i);
			auto objArr = reinterpret_cast<CCArray*>(objectAtIndex);

			for (int j = 0; j < objArr->count(); j++) {
				auto obj = reinterpret_cast<gd::GameObject*>(objArr->objectAtIndex(j));
				if (setting().onSolidHitboxes)
					Hitboxes::drawSolidsObjectHitbox(obj, objectDrawNode);
				if (setting().onHazardHitboxes)
					Hitboxes::drawHazardsObjectHitbox(obj, objectDrawNode);
				if (setting().onSpecialHitboxes)
					Hitboxes::drawSpecialsObjectHitbox(obj, objectDrawNode);
			}
		}
	}
}

bool __fastcall PlayLayer::initH(gd::PlayLayer* self, void*, gd::GJGameLevel* level) {
	m_coinsToPickup.clear();
	m_checkpoints.clear();

	m_dualPortals.clear();
	m_gamemodePortals.clear();
	m_miniPortals.clear();
	m_speedChanges.clear();
	m_mirrorPortals.clear();
	m_startPositions.clear();
	startPosObjects.clear();
	currentStartPos = 0;

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

	if (setting().onAutoPracticeMode) self->togglePracticeMode(true);
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

	auto playerDrawNode = CCDrawNode::create();
	self->m_gameLayer->addChild(playerDrawNode, 1000, 124);
	auto objectDrawNode = CCDrawNode::create();
	self->m_gameLayer->addChild(objectDrawNode, 1000, 125);

	if (setting().onHitboxes) {
		PlayLayer::updateShowHitboxes();
	}

	if (setting().onShowLayout) {
		PlayLayer::updateShowLayout();
	}

	if (setting().onAutoSafeMode && setting().cheatsCount > 0) safeModeON(), setting().isSafeMode = true;
	else if (!setting().onSafeMode) safeModeOFF(), setting().isSafeMode = false;

	return true;
}

void __fastcall PlayLayer::updateH(gd::PlayLayer* self, void*, float dt) {
	if (self->m_practiceMode || self->m_testMode) {
		if (m_smoothFrames > 0) {
			auto ideal_dt = CCDirector::sharedDirector()->getAnimationInterval();

			if (dt - ideal_dt < 1) {
				m_smoothFrames--;
			}

			dt = ideal_dt;
		}
	}

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

	if ((setting().onAutoSafeMode || setting().onSafeMode) && setting().cheatsCount > 0) safeModeON(), setting().isSafeMode = true;
	else if (!setting().onSafeMode) safeModeOFF(), setting().isSafeMode = false;

	if (setting().onLockCursor && !setting().show && !self->m_showingEndLayer && !self->m_isDead) {
		HWND hwnd = WindowFromDC(wglGetCurrentDC());
		RECT winSize; GetWindowRect(hwnd, &winSize);
		auto width = winSize.right - winSize.left;
		auto height = winSize.bottom - winSize.top;
		auto centerX = width / 2.f + winSize.left;
		auto centerY = height / 2.f + winSize.top;
		SetCursorPos(centerX, centerY);
	}

	if (setting().onAutoKill && (setting().killPercentage <= playerPercentPos)) {
		if (!self->m_isDead) {
			self->destroyPlayer(self->m_player);
		}
	}

	if ((setting().onAutoDeafen && !m_deafenPressed) && (playerPercentPos > setting().deafenPercent) && !self->m_isDead && !self->m_endTriggered) {
		if ((self->m_practiceMode && !setting().onPracticeDeafen) || (self->m_testMode && !setting().onTestmodeDeafen)) return;

		m_deafenPressed = true;
		keybd_event(VK_MENU, 0x38, 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}

	PlayLayer::updateShowHitboxes();

	if (setting().onShowLayout) {
		PlayLayer::updateShowLayout();
	}
}

void __fastcall PlayLayer::resetLevelH(gd::PlayLayer* self) {
	if (setting().onSmartStartPos) {
		for (gd::StartPosObject* obj : m_startPositions) {
			if (obj) {
				setupStartPos(obj);
			}
		}
	}

	if (setting().onCheckpointLagFix) {
		if (self->m_practiceMode || self->m_testMode) {
			m_smoothFrames = 2;
		}
	}

	PlayLayer::resetLevel(self);

	if (setting().onPracticeFix) {
		if (self->m_practiceMode && m_checkpoints.size() > 0) {
			m_checkpoints.back().restore(self);
		}
	}

	if (m_deafenPressed) {
		m_deafenPressed = false;
		keybd_event(VK_MENU, 0x38, 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}

	if (setting().onAutoPickupCoins) {
		for (auto* coin : m_coinsToPickup) {
			if (coin == nullptr) continue;

			coin->destroyObject();
			self->pickupItem(coin);
		}
	}
}

void __fastcall PlayLayer::addToSectionH(gd::PlayLayer* self, void*, gd::GameObject* object) {
	PlayLayer::addToSection(self, object);

	if (object->m_objectID == 31) {
		startPosObjects.push_back(static_cast<gd::StartPosObject*>(object));
	}

	switch (object->m_objectID) {
	case 31:
		m_startPositions.push_back(static_cast<gd::StartPosObject*>(object));
		break;
	case 12:
	case 13:
	case 47:
	case 111:
	case 660: m_gamemodePortals.push_back(object);
		break;
	case 45:
	case 46: m_mirrorPortals.push_back(object);
		break;
	case 99:
	case 101: m_miniPortals.push_back(object);
		break;
	case 286:
	case 287: m_dualPortals.push_back(object);
		break;
	case 200:
	case 201:
	case 202:
	case 203: m_speedChanges.push_back(object);
		break;
	default: break;
	}

	if (object->m_objectID == 142) {
		m_coinsToPickup.push_back(object);
	}
}

void __fastcall PlayLayer::createObjectsFromSetupH(gd::PlayLayer* self, void*, gd::string objects) {
	PlayLayer::createObjectsFromSetup(self, objects);
	if (startPosObjects.empty()) return;

	std::ranges::sort(startPosObjects, [](gd::GameObject* a, gd::GameObject* b) {
		return a->getPositionX() < b->getPositionX();
		});

	currentStartPos = -1;
	if (self->m_startPosObject) {
		auto it = std::ranges::find(startPosObjects, self->m_startPosObject);
		if (it != startPosObjects.end())
			currentStartPos = static_cast<int32_t>(std::distance(startPosObjects.begin(), it));
	}
}

void __fastcall PlayLayer::togglePracticeModeH(gd::PlayLayer* self, void*, bool practice) {
	m_checkpoints.clear();
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

	if (setting().onNoWavePulse) {
		self->m_player->m_audioScale = 1.f;
		self->m_player2->m_audioScale = 1.f;
	}
	//
}

void __fastcall PlayLayer::updateAttemptsH(gd::PlayLayer* self) {
	PlayLayer::updateAttempts(self);

	if (setting().onShowTotalAttempts) {
		self->m_attemptsLabel->setString(CCString::createWithFormat("Attempt %i", self->m_level->m_attempts + 1)->getCString());
	}
}

void __fastcall PlayLayer::processItemsH(gd::PlayLayer* self) {
	if (!setting().onSafeMode) PlayLayer::processItems(self);
}

void __fastcall PlayLayer::destroyPlayerH(gd::PlayLayer* self, void*, gd::PlayerObject* player) {
	PlayLayer::destroyPlayer(self, player);

	if (m_deafenPressed) {
		m_deafenPressed = false;
		keybd_event(VK_MENU, 0x38, 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}

	if (m_hz) {
		std::cout << "Death hazard: " << m_hz << std::endl;
	}
}

void __fastcall PlayLayer::collidedWithObjectH(gd::PlayerObject* self, void*, gd::GameObject* obj) {
	PlayLayer::collidedWithObject(self, obj);
	std::cout << obj << std::endl;
}

void __fastcall PlayLayer::levelCompleteH(gd::PlayLayer* self) {
	PlayLayer::levelComplete(self);

	if (m_deafenPressed) {
		m_deafenPressed = false;
		keybd_event(VK_MENU, 0x38, 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}
}

void __fastcall PlayLayer::pauseGameH(gd::PlayLayer* self, void*, bool idk) {
	PlayLayer::pauseGame(self, idk);

	if (setting().onPauseUndeafen && m_deafenPressed) {
		m_deafenPressed = false;
		keybd_event(VK_MENU, 0x38, 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}
}

void __fastcall PlayLayer::loadLastCheckpointH(gd::PlayLayer* self) {
	if (setting().onPracticeFix) {
		if (self->m_checkpoints->count() > 0) {
			auto checkpoint = static_cast<gd::CheckpointObject*>(self->m_checkpoints->lastObject());

			self->updateCustomColorBlend(3, checkpoint->m_customColor01Action->m_blend);
			self->updateCustomColorBlend(4, checkpoint->m_customColor02Action->m_blend);
			self->updateCustomColorBlend(6, checkpoint->m_customColor03Action->m_blend);
			self->updateCustomColorBlend(7, checkpoint->m_customColor04Action->m_blend);
			self->updateCustomColorBlend(8, checkpoint->m_dLineColorAction->m_blend);
		}
	}

	PlayLayer::loadLastCheckpoint(self);
}

gd::CheckpointObject* __fastcall PlayLayer::createCheckpointH(gd::PlayLayer* self) {
	if (self->m_player != nullptr) {
		m_checkpoints.push_back({ Checkpoint::from(self) });
	}

	return PlayLayer::createCheckpoint(self);
}

void __fastcall PlayLayer::removeLastCheckpointH(gd::PlayLayer* self) {
	PlayLayer::removeLastCheckpoint(self);

	if (m_checkpoints.size() > 0) {
		m_checkpoints.pop_back();
	}
}

void __fastcall PlayLayer::spawnPlayer2H(gd::PlayLayer* self) {
	PlayLayer::spawnPlayer2(self);

	if (setting().onInvisibleDualFix) {
		self->m_player2->setVisible(true);
	}
}

void __fastcall PlayLayer::gjH() {
	__asm {
		mov m_hz, eax
	}
	PlayLayer::gj();
}

inline void(__thiscall* gj2)();
void __fastcall gj2H() {
	__asm {
		mov m_hz, eax
	}
	gj2();
}

void PlayLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe35d0), PlayLayer::initH, reinterpret_cast<void**>(&PlayLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe9360), PlayLayer::updateH, reinterpret_cast<void**>(&PlayLayer::update));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf1f20), PlayLayer::resetLevelH, reinterpret_cast<void**>(&PlayLayer::resetLevel));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe9280), PlayLayer::addToSectionH, reinterpret_cast<void**>(&PlayLayer::addToSection));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe81c0), PlayLayer::createObjectsFromSetupH, reinterpret_cast<void**>(&PlayLayer::createObjectsFromSetup));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf3610), PlayLayer::togglePracticeModeH, reinterpret_cast<void**>(&PlayLayer::togglePracticeMode));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe5ff0), PlayLayer::showNewBestH, reinterpret_cast<void**>(&PlayLayer::showNewBest));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xeb3f0), PlayLayer::updateVisibilityH, reinterpret_cast<void**>(&PlayLayer::updateVisibility));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf33a0), PlayLayer::updateAttemptsH, reinterpret_cast<void**>(&PlayLayer::updateAttempts));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xee230), PlayLayer::processItemsH, reinterpret_cast<void**>(&PlayLayer::processItems));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf04a0), PlayLayer::destroyPlayerH, reinterpret_cast<void**>(&PlayLayer::destroyPlayer));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe52e0), PlayLayer::levelCompleteH, reinterpret_cast<void**>(&PlayLayer::levelComplete));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf38c0), PlayLayer::pauseGameH, reinterpret_cast<void**>(&PlayLayer::pauseGame));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf15b0), PlayLayer::loadLastCheckpointH, reinterpret_cast<void**>(&PlayLayer::loadLastCheckpoint));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf1010), PlayLayer::createCheckpointH, reinterpret_cast<void**>(&PlayLayer::createCheckpoint));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf1d70), PlayLayer::removeLastCheckpointH, reinterpret_cast<void**>(&PlayLayer::removeLastCheckpoint));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xef0d0), PlayLayer::spawnPlayer2H, reinterpret_cast<void**>(&PlayLayer::spawnPlayer2));

	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdc510), PlayLayer::collidedWithObjectH, reinterpret_cast<void**>(&PlayLayer::collidedWithObject));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xeb28f), PlayLayer::gjH, reinterpret_cast<void**>(&PlayLayer::gj));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdbb09), gj2H, reinterpret_cast<void**>(&gj2));
}