#include "PlayLayer.hpp"
#include "Setting.hpp"
#include "Hitboxes.hpp"
#include "PracticeFix.hpp"
#include "PlayerObject.hpp"
#include "Icons.hpp"
#include "utils.hpp"
#include "imgui.h"
#include <numeric>

std::vector<gd::GameObject*> m_coinsToPickup;

CCObject* m_deathObject;

void PlayLayer::setDeathObject(CCObject* object) {
	m_deathObject = object;
}

bool m_deafenPressed = false;

int m_smoothFrames = 0; // https://github.com/qimiko/gdps-public/blob/238b71e9f3cd8fdf855556ce4cc7c498f22cf3c0/include/hooks/PlayLayer.hpp#L16

std::vector<Checkpoint> m_checkpoints;

std::vector<gd::StartPosObject*> m_startPositions;
std::vector<gd::GameObject*> m_dualPortals, m_gamemodePortals, m_miniPortals, m_speedChanges, m_mirrorPortals;

int currentStartPos = 0;
std::vector<gd::StartPosObject*> startPosObjects;

gd::GameObject* m_portalRef;
gd::GameObject* m_dualPortalRef;

CCNode* m_labelsNode = nullptr;
CCLabelBMFont* m_cheatIndicatorLabel = nullptr;
CCLabelBMFont* m_messageLabel = nullptr;
CCLabelBMFont* m_attemptsLabel = nullptr;
CCLabelBMFont* m_fpsCounterLabel = nullptr;
float fps;
float _updateInterval = .5f;
float _timeLeft = _updateInterval;
float _accum = 0;
int _frames = 0;
CCLabelBMFont* m_cpsCounterLabel = nullptr;
bool m_hasClicked;
bool m_isHolding;
std::vector<time_t> m_clickFrames;
int m_totalClicks;
CCLabelBMFont* m_jumpsLabel = nullptr;
CCLabelBMFont* m_sessionTimeLabel = nullptr;
CCLabelBMFont* m_bestRunLabel = nullptr;
int m_lastRun;
int m_bestRunPercentage;
CCLabelBMFont* m_clockLabel = nullptr;
std::time_t clockTime;
SYSTEMTIME st;
CCLabelBMFont* m_noclipAccuracyLabel = nullptr;
float m_noclipAccuracy;
float m_deathPos;
float m_deathDifference;
CCLabelBMFont* m_noclipDeathsLabel = nullptr;
int m_deaths;
bool m_wasDead;
bool m_isPlayerDead;

bool m_cheatingBeforeRestart;

bool PlayLayer::getCheatingBeforeRestart() {
	return m_cheatingBeforeRestart;
}

bool PlayLayer::isCheating() {
	return
		setting().onInstantMirror ||
		setting().onNoMirror ||
		setting().onNoShadeEffect ||
		setting().onAutoPickupCoins ||
		setting().onEverythingHurts ||
		setting().onHitboxes ||
		setting().onInstantComplete ||
		setting().onJumpHack ||
		setting().onNoclip ||
		setting().onWaveSlide ||
		setting().onShowLayout ||
		setting().onSpeedhack;
}

void pickStartPos(gd::PlayLayer* playLayer, int32_t index) { // Eclipse menu
	if (playLayer->m_practiceMode) return;

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

void PlayLayer::clearHitboxes() {
	auto self = gd::GameManager::sharedState()->getPlayLayer();

	if (self == nullptr) return;

	auto playerDrawNode = static_cast<CCDrawNode*>(self->m_gameLayer->getChildByTag(124));
	playerDrawNode->clear();
	auto objectDrawNode = static_cast<CCDrawNode*>(self->m_gameLayer->getChildByTag(125));
	objectDrawNode->clear();
}

void PlayLayer::updatePlayerColors() {
	auto self = gd::GameManager::sharedState()->getPlayLayer();

	if (self == nullptr) return;

	if (self->m_player2 == nullptr) return;

	auto gm = gd::GameManager::sharedState();

	self->m_player2->setColor(setting().onSameDualColor ? gm->colorForIdx(gm->m_playerColor) : gm->colorForIdx(gm->m_playerColor2));
	self->m_player2->setSecondColor(setting().onSameDualColor ? gm->colorForIdx(gm->m_playerColor2) : gm->colorForIdx(gm->m_playerColor));

	self->m_player2->updateGlowColor();
}

void updateCheatIndicator() {
	bool isCheating = PlayLayer::isCheating();
	bool cheatingBeforeRestart = PlayLayer::getCheatingBeforeRestart();

	if (m_cheatIndicatorLabel && m_cheatIndicatorLabel->isVisible()) {
		if (!cheatingBeforeRestart && !setting().isSafeMode) {
			m_cheatIndicatorLabel->setColor(ccGREEN);
		}
		else if (!cheatingBeforeRestart && setting().isSafeMode) {
			m_cheatIndicatorLabel->setColor(ccYELLOW);
		}
		else if ((!isCheating || isCheating) && cheatingBeforeRestart && setting().isSafeMode) {
			m_cheatIndicatorLabel->setColor(ccORANGE);
		}
		else if (isCheating || cheatingBeforeRestart && !setting().isSafeMode) {
			m_cheatIndicatorLabel->setColor(ccRED);
		}
	}
}

void updateMessageLabel() {
	if (m_messageLabel && m_messageLabel->isVisible()) {
		m_messageLabel->setString(setting().message.c_str());
	}
}

void updateAttemptsLabel() {
	if (m_attemptsLabel && m_attemptsLabel->isVisible()) {
		auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

		std::string prefix;

		if (setting().attemptsPrefix) {
			prefix = "Attempt ";
		}
		else {
			prefix.clear();
		}

		int attempts = 1;

		if (!setting().onShowTotalAttempts) {
			attempts = playLayer->m_attempts;
		}
		else {
			attempts = playLayer->m_level->m_attempts + 1;
		}

		m_attemptsLabel->setString((prefix + std::to_string(attempts)).c_str());
	}
}

void updateFPSLabel() {
	if (m_fpsCounterLabel && m_fpsCounterLabel->isVisible()) {
		std::string prefix;

		if (setting().fpsPrefix) {
			prefix = " FPS";
		}
		else {
			prefix.clear();
		}

		m_fpsCounterLabel->setString((std::to_string(static_cast<int>(fps)) + prefix).c_str());
	}
}

void updateCPSLabel() {
	if (m_cpsCounterLabel && m_cpsCounterLabel->isVisible()) {
		std::string prefix;

		if (setting().cpsPrefix) {
			prefix = " CPS";
		}
		else {
			prefix.clear();
		}

		//std::string cpsMax;

		//if (setting().cpsMax) {
		//	cpsMax = "/" + std::to_string(m_totalClicks);
		//}
		//else {
		//	cpsMax.clear();
		//}

		std::string cpsTotal;

		if (setting().cpsTotal) {
			cpsTotal = "/" + std::to_string(m_totalClicks);
		}

		if (m_isHolding) {
			m_cpsCounterLabel->setColor(ccc3(64, 255, 64));
		}
		else {
			m_cpsCounterLabel->setColor(ccc3(255, 255, 255));
		}

		m_cpsCounterLabel->setString((std::to_string(m_clickFrames.size()) + cpsTotal + prefix).c_str());
	}
}

void updateJumpsLabel() {
	if (m_jumpsLabel && m_jumpsLabel->isVisible()) {
		auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

		std::string prefix;

		if (setting().jumpsPrefix) {
			prefix = " Jumps";
		}
		else {
			prefix.clear();
		}

		m_jumpsLabel->setString((std::to_string(playLayer->m_jumps) + prefix).c_str());
	}
}

void updateSessionTimeLabel() {
	if (m_sessionTimeLabel && m_sessionTimeLabel->isVisible()) {
		auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

		m_sessionTimeLabel->setString(CCString::createWithFormat("%.01fs", playLayer->m_clkTime)->getCString());
	}
}

void updateBestRunLabel() {
	if (m_bestRunLabel && m_bestRunLabel->isVisible()) {
		std::string prefix;

		if (setting().bestRunPrefix) {
			prefix = "Best Run: ";
		}
		else {
			prefix.clear();
		}

		int newBest = m_lastRun;

		if (newBest >= m_bestRunPercentage) {
			m_bestRunPercentage = newBest;
			m_bestRunLabel->setString((prefix + std::to_string(newBest) + "%").c_str());
		}
	}
}

void updateClockLabel() {
	if (m_clockLabel && m_clockLabel->isVisible()) {
		clockTime = std::time(nullptr);
		auto tm = *std::localtime(&clockTime);
		std::ostringstream s;
		s << std::put_time(&tm, "%H:%M:%S");
		m_clockLabel->setString(s.str().c_str());
	}
}

void updateNoclipAccuracyLabel(bool tintRed = false) {
	if (m_noclipAccuracyLabel && m_noclipAccuracyLabel->isVisible()) {
		auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

		std::string prefix;

		if (setting().nocAccPrefix) {
			prefix = "Accuracy: ";
		}
		else {
			prefix.clear();
		}

		m_noclipAccuracy = ((playLayer->m_player->getPositionX() - m_deathDifference) / playLayer->m_player->getPositionX()) * 100.f;

		if (!playLayer->m_endTriggered) {
			m_noclipAccuracyLabel->setString(CCString::createWithFormat((prefix + "%.2f%%").c_str(), m_noclipAccuracy)->getCString());
		}
	}
}

void updateNoclipDeathsLabel(bool tintRed = false) {
	if (m_noclipDeathsLabel && m_noclipDeathsLabel->isVisible()) {
		std::string prefix;

		if (setting().nocDeathsPrefix) {
			prefix = " Deaths";
		}
		else {
			prefix.clear();
		}

		m_noclipDeathsLabel->setString((std::to_string(m_deaths) + prefix).c_str());
	}
}

void PlayLayer::updateStatusLabels() {
	auto self = gd::GameManager::sharedState()->getPlayLayer();

	if (!self) return;

	auto director = CCDirector::sharedDirector();

	auto leftStatusXPos = (setting().labelsScale < 1.f) ? director->getScreenLeft() + setting().labelsScale * 5 : director->getScreenLeft() + 5;
	auto rightStatusXPos = (setting().labelsScale < 1.f) ? director->getScreenRight() - setting().labelsScale * 5 : director->getScreenRight() - 5;

	m_labelsNode->setVisible(!setting().onHideLabels);

	m_cheatIndicatorLabel->setVisible(setting().onCheatIndicator);
	m_cheatIndicatorLabel->setTag(setting().cheatIndicatorPos);

	m_messageLabel->setVisible(setting().onMessageLabel);
	m_messageLabel->setTag(setting().messagePos);

	m_attemptsLabel->setVisible(setting().onAttemptsLabel);
	m_attemptsLabel->setTag(setting().attemptsPos);

	m_fpsCounterLabel->setVisible(setting().onFPSCounter);
	m_fpsCounterLabel->setTag(setting().fpsCounterPos);

	m_cpsCounterLabel->setVisible(setting().onCPSCounter);
	m_cpsCounterLabel->setTag(setting().cpsCounterPos);

	m_jumpsLabel->setVisible(setting().onJumpsLabel);
	m_jumpsLabel->setTag(setting().jumpsPos);

	m_sessionTimeLabel->setVisible(setting().onSessionTime);
	m_sessionTimeLabel->setTag(setting().sessionTimePos);

	m_bestRunLabel->setVisible(setting().onBestRunLabel);
	m_bestRunLabel->setTag(setting().bestRunPos);

	m_clockLabel->setVisible(setting().onClockLabel);
	m_clockLabel->setTag(setting().clockPos);

	m_noclipAccuracyLabel->setVisible(setting().onNoclipAccuracy);
	m_noclipAccuracyLabel->setTag(setting().nocAccPos);

	m_noclipDeathsLabel->setVisible(setting().onNoclipDeaths);
	m_noclipDeathsLabel->setTag(setting().nocDeathsPos);

	int topLeftLabelsCount = 0;
	int topRightLabelsCount = 0;
	int bottomRightLabelsCount = 0;
	int bottomLeftLabelsCount = 0;

	for (int i = 0; i < m_labelsNode->getChildrenCount(); i++) {
		auto label = static_cast<CCLabelBMFont*>(m_labelsNode->getChildren()->objectAtIndex(i));
		if (label && label->isVisible()) {
			label->setScale((1.f - .6f) * setting().labelsScale);
			if (label != m_cheatIndicatorLabel) {
				label->setOpacity((255 - 191) * setting().labelsOpacity);
			}

			if (label->getTag() == 0) { // Top-Left
				label->setAnchorPoint(ccp(0.f, 1.f));
				if (label != m_cheatIndicatorLabel) {
					label->m_pAlignment = kCCTextAlignmentLeft;
				}
				label->setPosition(leftStatusXPos, (director->getScreenTop() - 2.f) - (topLeftLabelsCount * setting().labelsScale * 13.f));
				topLeftLabelsCount++;
			}

			if (label->getTag() == 1) { // Top-Right
				label->setAnchorPoint(ccp(1.f, 1.f));
				if (label != m_cheatIndicatorLabel) {
					label->m_pAlignment = kCCTextAlignmentRight;
				}
				label->setPosition(rightStatusXPos, (director->getScreenTop() - 2.f) - (topRightLabelsCount * setting().labelsScale * 13.f));
				topRightLabelsCount++;
			}

			if (label->getTag() == 2) { // Bottom-Right
				label->setAnchorPoint(ccp(1.f, 0.f));
				if (label != m_cheatIndicatorLabel) {
					label->m_pAlignment = kCCTextAlignmentRight;
				}
				label->setPosition(rightStatusXPos, (director->getScreenBottom() + 2.f) + (bottomRightLabelsCount * setting().labelsScale * 13.f));
				bottomRightLabelsCount++;
			}

			if (label->getTag() == 3) { // Bottom-Left
				label->setAnchorPoint(ccp(0.f, 0.f));
				if (label != m_cheatIndicatorLabel) {
					label->m_pAlignment = kCCTextAlignmentLeft;
				}
				label->setPosition(leftStatusXPos, (director->getScreenBottom() + 2.f) + (bottomLeftLabelsCount * setting().labelsScale * 13.f));
				bottomLeftLabelsCount++;
			}
		}
	}

	updateCheatIndicator();
	updateMessageLabel();
	updateAttemptsLabel();
	updateFPSLabel();
	updateCPSLabel();
	updateJumpsLabel();
	updateSessionTimeLabel();
	updateBestRunLabel();
	updateClockLabel();
	updateNoclipAccuracyLabel();
	updateNoclipDeathsLabel();
}

CCLabelBMFont* m_debugLabel = nullptr;

bool __fastcall PlayLayer::initH(gd::PlayLayer* self, void*, gd::GJGameLevel* level) {
	m_debugLabel = nullptr;

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

	m_portalRef = nullptr;
	m_dualPortalRef = nullptr;

	m_cheatingBeforeRestart = PlayLayer::isCheating();

	m_deathObject = nullptr;

	fps = ImGui::GetIO().Framerate;
	_updateInterval = .5f;
	_timeLeft = _updateInterval;
	_accum = 0;
	_frames = 0;

	m_hasClicked = false;
	m_clickFrames.clear();
	m_totalClicks = 0;
	m_isHolding = false;

	m_lastRun = 0;
	m_bestRunPercentage = 0;

	m_noclipAccuracy = 0.f;
	m_deathPos = 0.f;
	m_deathDifference = 0.f;

	m_isPlayerDead = false;
	m_deaths = 0;
	m_wasDead = false;

	setting().beforeRestartCheatsCount = setting().cheatsCount;

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
	std::string percentageString = "%." + std::to_string((setting().onAccuratePercentage ? setting().decimalPlaces : 0)) + "f%%";
	percentageLabel->setString(CCString::createWithFormat(percentageString.c_str(), playerPercentPos)->getCString());
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

	PlayLayer::updatePlayerColors();

	if ((setting().onAutoSafeMode && (PlayLayer::isCheating() || m_cheatingBeforeRestart)) || setting().onSafeMode) {
		safeModeON();
		setting().isSafeMode = true;
	}
	else {
		safeModeOFF();
		setting().isSafeMode = false;
	}

	//if (setting().onAutoSafeMode && setting().cheatsCount > 0) safeModeON(), setting().isSafeMode = true;
	//else if (!setting().onSafeMode) safeModeOFF(), setting().isSafeMode = false;

	auto noclipTint = CCLayerColor::create(ccc4(setting().noclipTintR, setting().noclipTintG, setting().noclipTintB, 255), winSize.width, winSize.height);
	noclipTint->setOpacity(0);
	self->addChild(noclipTint, 11, 875);

	//if (setting().onDeveloperMode) {
	//	m_debugLabel = CCLabelBMFont::create("", "chatFont.fnt");
	//	m_debugLabel->setAnchorPoint({ 0.f, 0.f });
	//	m_debugLabel->setScale(.5f);
	//	m_debugLabel->setOpacity(100);
	//	m_debugLabel->setString(CCString::createWithFormat("FPS: %.0f X: %.2f Y: %.2f isSafeMode: %d isCheating: %d m_cheatingBeforeRestart: %d",
	//		ImGui::GetIO().Framerate,
	//		self->m_player->getPositionX(),
	//		self->m_player->getPositionY(),
	//		setting().isSafeMode,
	//		PlayLayer::isCheating(),
	//		m_cheatingBeforeRestart
	//	)->getCString());
	//	self->addChild(m_debugLabel, 9999);
	//}

	m_labelsNode = CCNode::create();
	self->addChild(m_labelsNode, 99);

	m_cheatIndicatorLabel = CCLabelBMFont::create(". ", "bigFont.fnt");
	static_cast<CCSprite*>(m_cheatIndicatorLabel->getChildren()->objectAtIndex(0))->setScale(3.f);
	static_cast<CCSprite*>(m_cheatIndicatorLabel->getChildren()->objectAtIndex(0))->setAnchorPoint(ccp(.25f, .25f));
	m_labelsNode->addChild(m_cheatIndicatorLabel);

	m_messageLabel = CCLabelBMFont::create("", "bigFont.fnt");
	m_labelsNode->addChild(m_messageLabel);

	m_bestRunLabel = CCLabelBMFont::create("", "bigFont.fnt");
	m_labelsNode->addChild(m_bestRunLabel);

	m_attemptsLabel = CCLabelBMFont::create("", "bigFont.fnt");
	m_labelsNode->addChild(m_attemptsLabel);

	m_fpsCounterLabel = CCLabelBMFont::create("", "bigFont.fnt");
	m_labelsNode->addChild(m_fpsCounterLabel);

	m_cpsCounterLabel = CCLabelBMFont::create("", "bigFont.fnt");
	m_labelsNode->addChild(m_cpsCounterLabel);

	m_jumpsLabel = CCLabelBMFont::create("", "bigFont.fnt");
	m_labelsNode->addChild(m_jumpsLabel);

	m_clockLabel = CCLabelBMFont::create("", "bigFont.fnt");
	m_labelsNode->addChild(m_clockLabel);

	m_sessionTimeLabel = CCLabelBMFont::create("", "bigFont.fnt");
	m_labelsNode->addChild(m_sessionTimeLabel);

	m_noclipAccuracyLabel = CCLabelBMFont::create("", "bigFont.fnt");
	m_labelsNode->addChild(m_noclipAccuracyLabel);

	m_noclipDeathsLabel = CCLabelBMFont::create("", "bigFont.fnt");
	m_labelsNode->addChild(m_noclipDeathsLabel);

	PlayLayer::updateStatusLabels();

	if (setting().onSolidPlayerGlow) {
		self->m_batchNodeAddPlayer->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
	}

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

	_timeLeft -= dt;
	_accum += 1 / dt;
	_frames++;

	if (_timeLeft <= 0) {
		fps = _accum / _frames;
		_timeLeft = _updateInterval;
		_accum = 0;
		_frames = 0;
	}

	m_isPlayerDead = false;

	PlayLayer::update(self, dt);

	if (!m_isPlayerDead) m_wasDead = false;

	float playerPercentPos = self->m_player->getPositionX() / self->m_levelLength * 100.f;
	std::string percentageString = "%." + std::to_string((setting().onAccuratePercentage ? setting().decimalPlaces : 0)) + "f%%";

	auto percentageLabel = static_cast<CCLabelBMFont*>(self->getChildByTag(301));
	if (percentageLabel) {
		if (playerPercentPos < 100.f) {
			percentageLabel->setString(CCString::createWithFormat(percentageString.c_str(), playerPercentPos)->getCString());
		}
		else {
			percentageLabel->setString(CCString::createWithFormat(percentageString.c_str(), 100.f)->getCString());
		}
	}

	if (PlayLayer::isCheating()) {
		m_cheatingBeforeRestart = PlayLayer::isCheating();
	}

	if ((setting().onAutoSafeMode && (PlayLayer::isCheating() || m_cheatingBeforeRestart)) || setting().onSafeMode) {
		safeModeON();
		setting().isSafeMode = true;
	}
	else {
		safeModeOFF();
		setting().isSafeMode = false;
	}

	//if ((setting().onAutoSafeMode || setting().onSafeMode) && setting().cheatsCount > 0) safeModeON(), setting().isSafeMode = true;
	//else if (!setting().onSafeMode) safeModeOFF(), setting().isSafeMode = false;

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

	PlayLayer::updateShowHitboxes();

	if (setting().onShowLayout) {
		PlayLayer::updateShowLayout();
	}

	if ((setting().onAutoDeafen && !m_deafenPressed) && (playerPercentPos > setting().deafenPercent) && !self->m_isDead && !self->m_endTriggered) {
		if ((self->m_practiceMode && !setting().onPracticeDeafen) || (self->m_testMode && !setting().onTestmodeDeafen)) return;

		m_deafenPressed = true;
		keybd_event(VK_MENU, 0x38, 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}

	//if (setting().onDeveloperMode) {
	//	if (m_debugLabel) {
	//		m_debugLabel->setString(CCString::createWithFormat("FPS: %.0f X: %.2f Y: %.2f isSafeMode: %d isCheating: %d m_cheatingBeforeRestart: %d",
	//			ImGui::GetIO().Framerate,
	//			self->m_player->getPositionX(),
	//			self->m_player->getPositionY(),
	//			setting().isSafeMode,
	//			PlayLayer::isCheating(),
	//			m_cheatingBeforeRestart
	//		)->getCString());
	//	}
	//}

	time_t currentTick = time::getTime();
	m_clickFrames.erase(std::remove_if(m_clickFrames.begin(), m_clickFrames.end(), [currentTick](float tick) {
		return currentTick - tick > 1000;
		}), m_clickFrames.end());
	m_hasClicked = false;

	updateCheatIndicator();
	updateMessageLabel();
	updateAttemptsLabel();
	updateFPSLabel();
	updateCPSLabel();
	updateBestRunLabel();
	updateJumpsLabel();
	updateSessionTimeLabel();
	updateClockLabel();
	updateNoclipAccuracyLabel();
	updateNoclipDeathsLabel();
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

	m_clickFrames.clear();
	m_totalClicks = 0;

	if (!self->m_practiceMode) {
		m_deaths = 0;
		m_deathDifference = 0.f;
		m_deathPos = 0.f;
	}
	m_wasDead = false;

	PlayLayer::resetLevel(self);

	m_cheatingBeforeRestart = PlayLayer::isCheating();

	//PlayLayer::clearHitboxes();

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

	if (setting().onIconRandomizer) {
		auto gm = gd::GameManager::sharedState();

		srand(time(0));

		auto randColor1 = rand() % (31 - 0 + 1) + 0;
		auto randColor2 = rand() % (31 - 0 + 1) + 0;

		if (setting().onRandomizeColor1) {
			self->m_player->setColor(gm->colorForIdx(randColor1));
			self->m_player2->setSecondColor(gm->colorForIdx(randColor1));

			self->m_player->updateGlowColor();
			self->m_player2->updateGlowColor();
		}
		if (setting().onRandomizeColor2) {
			self->m_player->setSecondColor(gm->colorForIdx(randColor2));
			self->m_player2->setColor(gm->colorForIdx(randColor2));

			self->m_player->updateGlowColor();
			self->m_player2->updateGlowColor();
		}

		int cubeIcon = rand() % (Icons::getCount("player", "001") - 1 + 1) + 1;

		PlayerObject::setCubeIcon(cubeIcon);
		PlayerObject::setShipIcon(rand() % (Icons::getCount("ship", "001") - 0 + 1) + 0);
		PlayerObject::setRollIcon(rand() % (Icons::getCount("player_ball", "001") - 0 + 1) + 0);
		PlayerObject::setBirdIcon(rand() % (Icons::getCount("bird", "001") - 0 + 1) + 0);
		PlayerObject::setDartIcon(rand() % (Icons::getCount("dart", "001") - 0 + 1) + 0);

		self->m_player->updatePlayerFrame(cubeIcon);
		self->m_player2->updatePlayerFrame(cubeIcon);
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
	m_isPlayerDead = true;
	if (setting().onNoclip) {
		if (!m_wasDead) {
			m_deaths++;
		}
		else {
			m_deathDifference += player->getPositionX() - m_deathPos;
		}
	}
	m_deathPos = player->getPositionX();
	m_wasDead = true;

	PlayLayer::destroyPlayer(self, player);

	if (setting().onNoclipTint && setting().onNoclip) {
		auto noclipTint = static_cast<CCLayerColor*>(self->getChildByTag(875));
		if (noclipTint) {
			noclipTint->runAction(CCSequence::create(CCFadeTo::create(0.f, 65), CCFadeTo::create(.25f, 0), nullptr));
		}
	}

	//PlayLayer::updateShowHitboxes();

	if (m_deafenPressed) {
		m_deafenPressed = false;
		keybd_event(VK_MENU, 0x38, 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}

	//std::cout << "Death hazard: " << m_deathObject << std::endl;
	//if (m_deathObject) {
	//	auto objectDrawNode = static_cast<CCDrawNode*>(self->m_gameLayer->getChildByTag(125));

	//	if (setting().onSolidHitboxes)
	//		Hitboxes::drawSolidsObjectHitbox(reinterpret_cast<gd::GameObject*>(m_deathObject), objectDrawNode);
	//	if (setting().onHazardHitboxes)
	//		Hitboxes::drawHazardsObjectHitbox(reinterpret_cast<gd::GameObject*>(m_deathObject), objectDrawNode);
	//	if (setting().onSpecialHitboxes)
	//		Hitboxes::drawSpecialsObjectHitbox(reinterpret_cast<gd::GameObject*>(m_deathObject), objectDrawNode);
	//}

	if (setting().onRespawnTime) {
		float respawnTime = setting().respawnValue / 1000.f;

		if (auto* respawnSequence = self->getActionByTag(0x10)) {
			self->stopAction(respawnSequence);

			auto* delayedSequence = CCSequence::create(CCDelayTime::create(respawnTime), CCCallFunc::create(self, callfunc_selector(gd::PlayLayer::delayedResetLevel)), nullptr);

			delayedSequence->setTag(0x10);
			self->runAction(delayedSequence);
		}
	}

	if (!self->m_practiceMode && !self->m_testMode) {
		m_lastRun = self->m_player->getPositionX() / self->m_levelLength * 100.f;
	}

	updateBestRunLabel();

	std::cout << "Deaths: " << m_deaths << std::endl;
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

	if (setting().onPracticeFix) {
		self->m_cameraPortal = m_portalRef;
		self->m_dualModeCamera = m_dualPortalRef;
	}
}

gd::CheckpointObject* __fastcall PlayLayer::createCheckpointH(gd::PlayLayer* self) {
	if (self->m_player != nullptr) {
		m_checkpoints.push_back({ Checkpoint::from(self) });
	}

	m_portalRef = self->m_cameraPortal;
	m_dualPortalRef = self->m_dualModeCamera;

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

//inline bool(__thiscall* hazardDeathObject)();
//void __fastcall hazardDeathObjectH() {
//	__asm {
//		mov m_deathObject, eax
//	}
//	hazardDeathObject();
//}

//inline bool(__thiscall* solidDeathObject)();
//void __fastcall solidDeathObjectH() {
//	__asm {
//		mov m_deathObject, eax
//	}
//	solidDeathObject();
//}

//inline bool(__thiscall* slopeDeathObject)();
//void __fastcall slopeDeathObjectH() {
//	__asm {
//		mov m_deathObject, ebx
//	}
//	slopeDeathObject();
//}

void __fastcall PlayLayer::pushButtonH(gd::PlayLayer* self, void*, int p0, bool p1) {
	m_isHolding = true;
	if (!m_hasClicked) {
		m_clickFrames.push_back(time::getTime());
		m_totalClicks++;
		m_hasClicked = true;
	}
	PlayLayer::pushButton(self, p0, p1);
}

void __fastcall PlayLayer::releaseButtonH(gd::PlayLayer* self, void*, int p0, bool p1) {
	m_isHolding = false;
	PlayLayer::releaseButton(self, p0, p1);
}

void __fastcall PlayLayer::destructorH(gd::PlayLayer* self) {
	PlayLayer::destructor(self);
	m_labelsNode = nullptr;
	m_cheatIndicatorLabel = nullptr;
	m_messageLabel = nullptr;
	m_attemptsLabel = nullptr;
	m_fpsCounterLabel = nullptr;
	m_cpsCounterLabel = nullptr;
	m_jumpsLabel = nullptr;
	m_sessionTimeLabel = nullptr;
	m_clockLabel = nullptr;
	m_noclipAccuracyLabel = nullptr;
	m_noclipDeathsLabel = nullptr;
}

void PlayLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe35d0), PlayLayer::initH, reinterpret_cast<void**>(&PlayLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe3170), PlayLayer::destructorH, reinterpret_cast<void**>(&PlayLayer::destructor));
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
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf0a00), PlayLayer::pushButtonH, reinterpret_cast<void**>(&PlayLayer::pushButton));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf0af0), PlayLayer::releaseButtonH, reinterpret_cast<void**>(&PlayLayer::releaseButton));

	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdc510), PlayLayer::collidedWithObjectH, reinterpret_cast<void**>(&PlayLayer::collidedWithObject));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xeb28f), hazardDeathObjectH, reinterpret_cast<void**>(&hazardObject));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdc52a), solidDeathObjectH, reinterpret_cast<void**>(&solidDeathObject));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdb5e7), slopeDeathObjectH, reinterpret_cast<void**>(&slopeDeathObject));
}