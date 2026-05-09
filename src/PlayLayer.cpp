#include "PlayLayer.hpp"
#include "Setting.hpp"
#include "Hitboxes.hpp"
//#include "PracticeFix.hpp"
//#include "PlayerObject.hpp"
//#include "UILayer.hpp"
#include "PauseLayer.hpp"
// #include "Icons.hpp"
#include "utils.hpp"
#include "imgui.h"

bool PolzPlayLayer::getCheatingBeforeRestart() {
    return this->m_fields->m_cheatingBeforeRestart;
}

bool PolzPlayLayer::isCheating() {
    return
        setting().onInstantMirror ||
        setting().onNoMirror ||
        setting().onNoShadeEffect ||
        setting().onAutoPickupCoins ||
        setting().onEverythingHurts ||
        setting().onEverythingPulses ||
        setting().onFreezePlayer ||
        setting().onHitboxes ||
        setting().onInstantComplete ||
        setting().onJumpHack ||
        setting().onNoclip || setting().onNoclipUnstuck ||
        setting().onWaveSlide ||
        setting().onShowLayout ||
        setting().onPlayMacro ||
        setting().onRecordMacro ||
        setting().onShipcopter ||
        (setting().onSpeedhack && setting().speedhackValue != 1.f) ||
        setting().onHidePauseMenu;
}

void pickStartPos(PlayLayer* playLayer, int32_t index) { // Eclipse menu
	if (playLayer->m_practiceMode) return;

	if (static_cast<PolzPlayLayer*>(playLayer)->m_fields->m_startPositions.empty()) return;

	auto count = static_cast<int32_t>(static_cast<PolzPlayLayer*>(playLayer)->m_fields->m_startPositions.size());
	if (index >= count) index = -1;
	else if (index < -1) index = count - 1;

	static_cast<PolzPlayLayer*>(playLayer)->m_fields->m_currentStartPos = index;

	auto* startPos = index >= 0 ? static_cast<PolzPlayLayer*>(playLayer)->m_fields->m_startPositions[index] : nullptr;
	playLayer->setStartPosObject(startPos);
	playLayer->m_testMode = index >= 0;

	if (static_cast<PolzPlayLayer*>(playLayer)->m_fields->m_currentStartPos >= 0) { // Taswert's thing
		playLayer->m_startPos = playLayer->m_startPosObject->getOrientedBox()->m_center;
	}
	else {
		playLayer->m_startPos = ccp(0, 105);
	}

	playLayer->resetLevel();

	if (PolzPauseLayer::get()) {
		GameSoundManager::sharedManager()->stopBackgroundMusic();
	}

	static_cast<PolzPlayLayer*>(playLayer)->updateStartPosSwitcherLabel();
	static_cast<PolzPlayLayer*>(playLayer)->updateStatusLabels();
}

void PolzPlayLayer::nextStartPos() {
	pickStartPos(GameManager::sharedState()->m_playLayer, static_cast<PolzPlayLayer*>(GameManager::sharedState()->m_playLayer)->m_fields->m_currentStartPos + 1);
}

void PolzPlayLayer::prevStartPos() {
	pickStartPos(GameManager::sharedState()->m_playLayer, static_cast<PolzPlayLayer*>(GameManager::sharedState()->m_playLayer)->m_fields->m_currentStartPos - 1);
}

static GameObject* getClosestObject(std::vector<GameObject*>& vec, StartPosObject* startPos) {
	GameObject* closest = nullptr;

	std::ranges::sort(vec, [](GameObject* a, GameObject* b) {
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

void setupStartPos(StartPosObject* startPos) { // Eclipse menu https://github.com/EclipseMenu/EclipseMenu/blob/main/src/hacks/Level/SmartStartPos.cpp
	LevelSettingsObject* startPosSettings = startPos->m_settings;
	LevelSettingsObject* levelSettings = GameManager::sharedState()->getPlayLayer()->m_levelSettings;

	startPosSettings->m_startDual = levelSettings->m_startDual;
	startPosSettings->m_startMode = levelSettings->m_startMode;
	startPosSettings->m_startMini = levelSettings->m_startMini;
	startPosSettings->m_startSpeed = levelSettings->m_startSpeed;

	GameObject* obj = getClosestObject(static_cast<PolzPlayLayer*>(GameManager::sharedState()->m_playLayer)->m_fields->m_dualPortals, startPos);
	if (obj)
		startPosSettings->m_startDual = obj->m_objectID == 286;

	obj = getClosestObject(static_cast<PolzPlayLayer*>(GameManager::sharedState()->m_playLayer)->m_fields->m_gamemodePortals, startPos);

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

	obj = getClosestObject(static_cast<PolzPlayLayer*>(GameManager::sharedState()->m_playLayer)->m_fields->m_miniPortals, startPos);

	if (obj)
		startPosSettings->m_startMini = obj->m_objectID == 101;

	obj = getClosestObject(static_cast<PolzPlayLayer*>(GameManager::sharedState()->m_playLayer)->m_fields->m_speedChanges, startPos);
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

void PolzPlayLayer::updateShowLayout() {
	ccColor3B bgColor = ccc3(setting().layoutBGR, setting().layoutBGG, setting().layoutBGB);
	ccColor3B gColor = ccc3(setting().layoutGR, setting().layoutGG, setting().layoutGB);

	this->m_bgSprite->setColor(bgColor);
	this->m_groundBottomLayer->m_groundSprite->setColor(gColor);
	this->m_groundBottomLayer->m_line->setColor(ccWHITE);
	this->m_groundTopLayer->m_groundSprite->setColor(gColor);
	this->m_groundTopLayer->m_line->setColor(ccWHITE);

	this->m_pUnknown7->setVisible(false);

	for (int i = this->m_firstVisibleSection - 1; i <= this->m_lastVisibleSection + 1; i++) {
		if (i < 0) continue;
		if (i >= this->m_levelSections->count()) break;

		auto objectAtIndex = this->m_levelSections->objectAtIndex(i);
		auto objArr = reinterpret_cast<CCArray*>(objectAtIndex);

		for (int j = 0; j < objArr->count(); j++) {
			auto obj = reinterpret_cast<GameObject*>(objArr->objectAtIndex(j));
			if ((obj->m_objectType == GameObjectType::Decoration || obj->m_objectType == GameObjectType::PulsingDecoration) && obj->isVisible() && (obj->m_objectID != 10 && obj->m_objectID != 11 && obj->m_objectID != 12 && obj->m_objectID != 13 && obj->m_objectID != 38 && obj->m_objectID != 44 && obj->m_objectID != 45 && obj->m_objectID != 46 && obj->m_objectID != 47 && obj->m_objectID != 99 && obj->m_objectID != 101 && obj->m_objectID != 111 && obj->m_objectID != 286 && obj->m_objectID != 287 && obj->m_objectID != 660 && obj->m_objectID != 745 && obj->m_objectID != 749) && obj != this->m_endPortalObject) {
				obj->setVisible(false);
			}

			obj->setObjectColor(ccWHITE);
			if (obj->m_colorSprite) obj->m_colorSprite->setColor(ccWHITE);
			obj->m_customColorMode = static_cast<GJCustomColorMode>(9);
		}
	}
}

void PolzPlayLayer::updateShowHitboxes() {
	auto playerDrawNode = static_cast<CCDrawNode*>(this->m_gameLayer->getChildByTag(124));
	auto objectDrawNode = static_cast<CCDrawNode*>(this->m_gameLayer->getChildByTag(125));
	playerDrawNode->clear();
	objectDrawNode->clear();

	if ((this->m_player->m_isDead && setting().onHitboxesOnDeath) || setting().onHitboxes) {
		if (setting().onPlayerHitboxes) {
			if (this->m_player) Hitboxes::drawPlayerHitbox(this->m_player, playerDrawNode);
			if (this->m_player2) Hitboxes::drawPlayerHitbox(this->m_player2, playerDrawNode);
		}

		for (int i = this->m_firstVisibleSection + 1; i <= this->m_lastVisibleSection - 1; i++) {
			if (i < 0) continue;
			if (i >= this->m_levelSections->count()) break;

			auto objectAtIndex = this->m_levelSections->objectAtIndex(i);
			auto objArr = static_cast<CCArray*>(objectAtIndex);

			for (int j = 0; j < objArr->count(); j++) {
				auto obj = typeinfo_cast<GameObject*>(objArr->objectAtIndex(j));
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

void PolzPlayLayer::clearHitboxes() {
	auto playerDrawNode = static_cast<CCDrawNode*>(this->m_gameLayer->getChildByTag(124));
	auto objectDrawNode = static_cast<CCDrawNode*>(this->m_gameLayer->getChildByTag(125));
	playerDrawNode->clear();
	objectDrawNode->clear();
}

void PolzPlayLayer::updatePlayerColors() {
	if (this->m_player2 == nullptr) return;

	auto gm = GameManager::sharedState();

	this->m_player2->setColor(setting().onSameDualColor ? gm->colorForIdx(gm->m_playerColor) : gm->colorForIdx(gm->m_playerColor2));
	this->m_player2->setSecondColor(setting().onSameDualColor ? gm->colorForIdx(gm->m_playerColor2) : gm->colorForIdx(gm->m_playerColor));

	this->m_player2->updateGlowColor();
}

void PolzPlayLayer::updateCheatIndicator() {
	bool isCheating = PolzPlayLayer::isCheating();
	bool cheatingBeforeRestart = PolzPlayLayer::getCheatingBeforeRestart();

	if (this->m_fields->m_cheatIndicatorLabel && this->m_fields->m_cheatIndicatorLabel->isVisible()) {
		if (!cheatingBeforeRestart && !setting().isSafeMode) {
			this->m_fields->m_cheatIndicatorLabel->setColor(ccGREEN);
		}
		else if (!cheatingBeforeRestart && setting().isSafeMode) {
			this->m_fields->m_cheatIndicatorLabel->setColor(ccYELLOW);
		}
		else if ((!isCheating || isCheating) && cheatingBeforeRestart && setting().isSafeMode) {
			this->m_fields->m_cheatIndicatorLabel->setColor(ccORANGE);
		}
		else if (isCheating || cheatingBeforeRestart && !setting().isSafeMode) {
			this->m_fields->m_cheatIndicatorLabel->setColor(ccRED);
		}
	}
}

void PolzPlayLayer::updateMessageLabel() {
    if (this->m_fields->m_messageLabel && this->m_fields->m_messageLabel->isVisible()) {
        this->m_fields->m_messageLabel->setString(setting().message.c_str());
    }
}

void PolzPlayLayer::updateAttemptsLabel() {
    if (this->m_fields->m_attemptsLabel && this->m_fields->m_attemptsLabel->isVisible()) {
        std::string prefix;

        if (setting().attemptsPrefix) {
            prefix = "Attempt ";
        }
        else {
            prefix.clear();
        }

        int attempts;

        if (!setting().onShowTotalAttempts) {
            attempts = this->m_attempts;
        }
        else {
            attempts = this->m_level->m_attempts + 1;
        }

        if (attempts < 1) {
            attempts = 1;
        }

        this->m_fields->m_attemptsLabel->setString((prefix + std::to_string(attempts)).c_str());
    }
}

void PolzPlayLayer::updateFPSLabel() {
	if (this->m_fields->m_fpsCounterLabel && this->m_fields->m_fpsCounterLabel->isVisible()) {
		std::string prefix;

		if (setting().fpsPrefix) {
			prefix = setting().onTPSBypass ? " TPS" : " FPS";
		}
		else {
			prefix.clear();
		}

		if (setting().useImGuiFps) {
			this->m_fields->m_fpsCounterLabel->setString((std::to_string(static_cast<int>(roundf(ImGui::GetIO().Framerate))) + prefix).c_str());
		}
		else {
			this->m_fields->m_fpsCounterLabel->setString((std::to_string(static_cast<int>(roundf(this->m_fields->fps))) + prefix).c_str());
		}
	}
}

void PolzPlayLayer::updateCPSLabel() {
	if (this->m_fields->m_cpsCounterLabel && this->m_fields->m_cpsCounterLabel->isVisible()) {
		std::string prefix;

		if (setting().cpsPrefix) {
			prefix = " CPS";
		}
		else {
			prefix.clear();
		}

		std::string cpsTotal;

		if (setting().cpsTotal) {
			cpsTotal = "/" + std::to_string(this->m_fields->m_totalClicks);
		}

		if (this->m_fields->m_isHolding) {
			this->m_fields->m_cpsCounterLabel->setColor(ccc3(64, 255, 64));
		}
		else {
			this->m_fields->m_cpsCounterLabel->setColor(ccc3(255, 255, 255));
		}

		this->m_fields->m_cpsCounterLabel->setString((std::to_string(this->m_fields->m_clickFrames.size()) + cpsTotal + prefix).c_str());
	}
}

void PolzPlayLayer::updateJumpsLabel() {
	if (this->m_fields->m_jumpsLabel && this->m_fields->m_jumpsLabel->isVisible()) {
		std::string prefix;

		if (setting().jumpsPrefix) {
			prefix = " Jumps";
		}
		else {
			prefix.clear();
		}

		this->m_fields->m_jumpsLabel->setString((std::to_string(this->m_jumps) + prefix).c_str());
	}
}

void PolzPlayLayer::updateSessionTimeLabel() {
	if (this->m_fields->m_sessionTimeLabel && this->m_fields->m_sessionTimeLabel->isVisible()) {
		this->m_fields->m_sessionTimeLabel->setString(CCString::createWithFormat("%.01fs", this->m_clkTimer)->getCString());
	}
}

void PolzPlayLayer::updateBestRunLabel() {
	if (this->m_fields->m_bestRunLabel && this->m_fields->m_bestRunLabel->isVisible()) {
		std::string prefix;

		if (setting().bestRunPrefix) {
			prefix = "Best Run: ";
		}
		else {
			prefix.clear();
		}

		float newBest = roundf(this->m_fields->m_lastRun);

		if (this->m_fields->m_startPositions.size()) {
			if (this->m_fields->m_currentStartPos > -1) {
				int startPosLevelPos = static_cast<int>(roundf(this->m_fields->m_startPositions[this->m_fields->m_currentStartPos]->getPositionX() / this->m_levelLength * 100.f));
				int newStartPosBest = static_cast<int>(roundf(this->m_fields->m_startPositionsBestRun[this->m_fields->m_startPositions[this->m_fields->m_currentStartPos]].first));

				if (newStartPosBest > 0) {
					this->m_fields->m_bestRunLabel->setString((prefix + std::to_string(startPosLevelPos) + "%-" + std::to_string(newStartPosBest) + "%").c_str());
				}
				else {
					this->m_fields->m_bestRunLabel->setString((prefix + "None").c_str());
				}

				return;
			}
		}

		if (newBest >= this->m_fields->m_bestRunPercentage) {
			this->m_fields->m_bestRunPercentage = newBest;
			this->m_fields->m_bestRunLabel->setString((prefix + std::to_string(static_cast<int>(newBest)) + "%").c_str());
		}

		if (newBest == 0) {
			this->m_fields->m_bestRunLabel->setString((prefix + "None").c_str());
		}
	}
}

void PolzPlayLayer::updateClockLabel() {
	if (this->m_fields->m_clockLabel && this->m_fields->m_clockLabel->isVisible()) {
		this->m_fields->clockTime = std::time(nullptr);
		auto tm = *std::localtime(&this->m_fields->clockTime);
		std::ostringstream s;
		s << std::put_time(&tm, "%H:%M:%S");
		this->m_fields->m_clockLabel->setString(s.str().c_str());
	}
}

void PolzPlayLayer::updateNoclipAccuracyLabel(bool tintRed) {
	if (this->m_fields->m_noclipAccuracyLabel && this->m_fields->m_noclipAccuracyLabel->isVisible()) {
		std::string prefix;

		if (setting().nocAccPrefix) {
			prefix = "Accuracy: ";
		}
		else {
			prefix.clear();
		}

		if (tintRed) {
			this->m_fields->m_noclipAccuracyLabel->stopAllActions();
			this->m_fields->m_noclipAccuracyLabel->setColor(ccc3(255, 64, 64));
			this->m_fields->m_noclipAccuracyLabel->runAction(CCTintTo::create(.1f, 255, 255, 255));
		}

		float accuracy = 100.f;
		if (this->m_fields->m_noclipFrames != 0) {
			accuracy = (float(this->m_fields->m_noclipFrames - this->m_fields->m_deaths) / float(this->m_fields->m_noclipFrames)) * 100.f;
		}

		if (!this->m_endTriggered) {
			this->m_fields->m_noclipAccuracyLabel->setString(CCString::createWithFormat((prefix + "%.2f%%").c_str(), accuracy)->getCString());
		}
	}
}

void PolzPlayLayer::updateNoclipDeathsLabel(bool tintRed) {
	if (this->m_fields->m_noclipDeathsLabel && this->m_fields->m_noclipDeathsLabel->isVisible()) {
		std::string prefix;

		if (setting().nocDeathsPrefix) {
			prefix = " Deaths";
		}
		else {
			prefix.clear();
		}

		if (tintRed) {
			this->m_fields->m_noclipDeathsLabel->stopAllActions();
			this->m_fields->m_noclipDeathsLabel->setColor(ccc3(255, 64, 64));
			this->m_fields->m_noclipDeathsLabel->runAction(CCTintTo::create(.1f, 255, 255, 255));
		}

		this->m_fields->m_noclipDeathsLabel->setString((std::to_string(this->m_fields->m_deathsFull) + prefix).c_str());
	}
}

void PolzPlayLayer::updateStatusLabels() {
	auto director = CCDirector::sharedDirector();

	auto leftStatusXPos = (setting().labelsScale < 1.f) ? director->getScreenLeft() + setting().labelsScale * 5 : director->getScreenLeft() + 5;
	auto rightStatusXPos = (setting().labelsScale < 1.f) ? director->getScreenRight() - setting().labelsScale * 5 : director->getScreenRight() - 5;

	this->m_fields->m_labelsNode->setVisible(!setting().onHideLabels);

	this->m_fields->m_cheatIndicatorLabel->setVisible(setting().onCheatIndicator);
	this->m_fields->m_cheatIndicatorLabel->setTag(setting().cheatIndicatorPos);

	this->m_fields->m_messageLabel->setVisible(setting().onMessageLabel);
	this->m_fields->m_messageLabel->setTag(setting().messagePos);

	this->m_fields->m_attemptsLabel->setVisible(setting().onAttemptsLabel);
	this->m_fields->m_attemptsLabel->setTag(setting().attemptsPos);

	this->m_fields->m_fpsCounterLabel->setVisible(setting().onFPSCounter);
	this->m_fields->m_fpsCounterLabel->setTag(setting().fpsCounterPos);

	this->m_fields->m_cpsCounterLabel->setVisible(setting().onCPSCounter);
	this->m_fields->m_cpsCounterLabel->setTag(setting().cpsCounterPos);

	this->m_fields->m_jumpsLabel->setVisible(setting().onJumpsLabel);
	this->m_fields->m_jumpsLabel->setTag(setting().jumpsPos);

	this->m_fields->m_sessionTimeLabel->setVisible(setting().onSessionTime);
	this->m_fields->m_sessionTimeLabel->setTag(setting().sessionTimePos);

	this->m_fields->m_bestRunLabel->setVisible(setting().onBestRunLabel);
	this->m_fields->m_bestRunLabel->setTag(setting().bestRunPos);

	this->m_fields->m_clockLabel->setVisible(setting().onClockLabel);
	this->m_fields->m_clockLabel->setTag(setting().clockPos);

	this->m_fields->m_noclipAccuracyLabel->setVisible(setting().onNoclipAccuracy && setting().onNoclip);
	this->m_fields->m_noclipAccuracyLabel->setTag(setting().nocAccPos);

	this->m_fields->m_noclipDeathsLabel->setVisible(setting().onNoclipDeaths && setting().onNoclip);
	this->m_fields->m_noclipDeathsLabel->setTag(setting().nocDeathsPos);

	int topLeftLabelsCount = 0;
	int topRightLabelsCount = 0;
	int bottomRightLabelsCount = 0;
	int bottomLeftLabelsCount = 0;

	for (int i = 0; i < this->m_fields->m_labelsNode->getChildrenCount(); i++) {
		auto label = static_cast<CCLabelBMFont*>(this->m_fields->m_labelsNode->getChildren()->objectAtIndex(i));
		if (label && label->isVisible()) {
			label->setScale((1.f - .6f) * setting().labelsScale);
			if (label != this->m_fields->m_cheatIndicatorLabel) {
				label->setOpacity((255 - 191) * setting().labelsOpacity);
			}

			if (label->getTag() == 0) { // Top-Left
				label->setAnchorPoint(ccp(0.f, 1.f));
				if (label != this->m_fields->m_cheatIndicatorLabel) {
					label->m_pAlignment = kCCTextAlignmentLeft;
				}
				label->setPosition(leftStatusXPos, (director->getScreenTop() - 2.f) - (topLeftLabelsCount * setting().labelsScale * 13.f));
				topLeftLabelsCount++;
			}

			if (label->getTag() == 1) { // Top-Right
				label->setAnchorPoint(ccp(1.f, 1.f));
				if (label != this->m_fields->m_cheatIndicatorLabel) {
					label->m_pAlignment = kCCTextAlignmentRight;
				}
				label->setPosition(rightStatusXPos, (director->getScreenTop() - 2.f) - (topRightLabelsCount * setting().labelsScale * 13.f));
				topRightLabelsCount++;
			}

			if (label->getTag() == 2) { // Bottom-Right
				label->setAnchorPoint(ccp(1.f, 0.f));
				if (label != this->m_fields->m_cheatIndicatorLabel) {
					label->m_pAlignment = kCCTextAlignmentRight;
				}
				label->setPosition(rightStatusXPos, (director->getScreenBottom() + 2.f) + (bottomRightLabelsCount * setting().labelsScale * 13.f));
				bottomRightLabelsCount++;
			}

			if (label->getTag() == 3) { // Bottom-Left
				label->setAnchorPoint(ccp(0.f, 0.f));
				if (label != this->m_fields->m_cheatIndicatorLabel) {
					label->m_pAlignment = kCCTextAlignmentLeft;
				}
				label->setPosition(leftStatusXPos, (director->getScreenBottom() + 2.f) + (bottomLeftLabelsCount * setting().labelsScale * 13.f));
				bottomLeftLabelsCount++;
			}
		}
	}

	this->updateCheatIndicator();
	this->updateMessageLabel();
	this->updateAttemptsLabel();
	this->updateFPSLabel();
	this->updateCPSLabel();
	this->updateJumpsLabel();
	this->updateSessionTimeLabel();
	this->updateBestRunLabel();
	this->updateClockLabel();
	this->updateNoclipAccuracyLabel();
	this->updateNoclipDeathsLabel();
}

void PolzPlayLayer::updateStartPosSwitcherLabel() {
	if (this->m_fields->m_startPositions.empty()) return;

	auto startPosSwitcherMenu = static_cast<CCMenu*>(this->m_uiLayer->getChildByTag(125));
	if (!startPosSwitcherMenu) return;

	auto gm = GameManager::sharedState();

	auto startPosSwitcherLabel = static_cast<CCLabelBMFont*>(startPosSwitcherMenu->getChildByTag(1));
	auto onPrevStartPos = static_cast<CCMenuItemSpriteExtra*>(startPosSwitcherMenu->getChildByTag(2));
	auto onNextStartPos = static_cast<CCMenuItemSpriteExtra*>(startPosSwitcherMenu->getChildByTag(3));

	if (!startPosSwitcherLabel || !onPrevStartPos || !onNextStartPos) return;

	startPosSwitcherLabel->setString(CCString::createWithFormat("%i/%i", this->m_fields->m_currentStartPos + 1, this->m_fields->m_startPositions.size())->getCString());
	startPosSwitcherLabel->setVisible(true);
	startPosSwitcherLabel->setOpacity(255);
	startPosSwitcherLabel->stopAllActions();
	startPosSwitcherLabel->runAction(CCSequence::create(CCDelayTime::create(1.f), CCFadeOut::create(.5f), nullptr));

	onPrevStartPos->setVisible(gm->getGameVariable("0024"));
	onPrevStartPos->setOpacity(255);
	onPrevStartPos->stopAllActions();
	onPrevStartPos->runAction(CCSequence::create(CCDelayTime::create(1.f), CCFadeOut::create(.5f), CCHide::create(), nullptr));

	onNextStartPos->setVisible(gm->getGameVariable("0024"));
	onNextStartPos->setOpacity(255);
	onNextStartPos->stopAllActions();
	onNextStartPos->runAction(CCSequence::create(CCDelayTime::create(1.f), CCFadeOut::create(.5f), CCHide::create(), nullptr));
}

bool PolzPlayLayer::init(GJGameLevel* level) {
    if (!PlayLayer::init(level)) return false;

    //
    this->m_attemptsLabel->setVisible(!setting().onHideAttempts);

    if (GameManager::sharedState()->getGameVariable("0024")) {
        this->m_uiLayer->m_pauseBtn->setVisible(!setting().onHidePauseButton);
    }

    this->m_player->setVisible(!setting().onHidePlayer);
    this->m_player2->setVisible(!setting().onHidePlayer);

    this->m_player->m_hardStreak->setVisible(!setting().onNoWaveTrail);
    this->m_player2->m_hardStreak->setVisible(!setting().onNoWaveTrail);

    if (setting().onShowTotalAttempts) {
        this->m_attemptsLabel->setString(CCString::createWithFormat("Attempt %i", level->m_attempts + 1)->getCString());
    }

    if (setting().onAutoPracticeMode) this->togglePracticeMode(true);

    this->m_player->m_birdDragParticle->setVisible(!setting().onNoVehicleParticles);
    this->m_player2->m_birdDragParticle->setVisible(!setting().onNoVehicleParticles);
    this->m_player->m_dragParticle2->setVisible(!setting().onNoVehicleParticles);
    this->m_player2->m_dragParticle2->setVisible(!setting().onNoVehicleParticles);
    this->m_player->m_burstParticle->setVisible(!setting().onNoVehicleParticles);
    this->m_player2->m_burstParticle->setVisible(!setting().onNoVehicleParticles);

	if (setting().onMiniCubeIcon) {
		this->m_player->updatePlayerFrame(0);
		this->m_player2->updatePlayerFrame(0);
	}
    //

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto gm = GameManager::sharedState();

    auto playerDrawNode = CCDrawNode::create();
    this->m_gameLayer->addChild(playerDrawNode, 1000, 124);
    auto objectDrawNode = CCDrawNode::create();
    this->m_gameLayer->addChild(objectDrawNode, 1000, 125);

    if (setting().onHitboxes) {
        this->updateShowHitboxes();
    }

    if (setting().onShowLayout) {
        this->updateShowLayout();
    }

    this->updatePlayerColors();

    if ((setting().onAutoSafeMode && (this->isCheating() || this->m_fields->m_cheatingBeforeRestart)) || setting().onSafeMode) {
        safeModeON();
        setting().isSafeMode = true;
    }
    else {
        safeModeOFF();
        setting().isSafeMode = false;
    }

    this->updateStartPosSwitcherLabel();

    auto noclipTint = CCLayerColor::create(ccc4(setting().noclipTintR, setting().noclipTintG, setting().noclipTintB, 255), winSize.width, winSize.height);
    noclipTint->setOpacity(0);
    this->addChild(noclipTint, 11, 875);

    this->m_fields->m_labelsNode = CCNode::create();
    this->addChild(this->m_fields->m_labelsNode, 99, 72615);

    this->m_fields->m_cheatIndicatorLabel = CCLabelBMFont::create(". ", "bigFont.fnt");
    static_cast<CCSprite*>(this->m_fields->m_cheatIndicatorLabel->getChildren()->objectAtIndex(0))->setScale(3.f);
    static_cast<CCSprite*>(this->m_fields->m_cheatIndicatorLabel->getChildren()->objectAtIndex(0))->setAnchorPoint(ccp(.25f, .25f));
    this->m_fields->m_labelsNode->addChild(this->m_fields->m_cheatIndicatorLabel);

    this->m_fields->m_messageLabel = CCLabelBMFont::create("", "bigFont.fnt");
    this->m_fields->m_labelsNode->addChild(this->m_fields->m_messageLabel);

    this->m_fields->m_bestRunLabel = CCLabelBMFont::create("", "bigFont.fnt");
    this->m_fields->m_labelsNode->addChild(this->m_fields->m_bestRunLabel);

    this->m_fields->m_attemptsLabel = CCLabelBMFont::create("", "bigFont.fnt");
    this->m_fields->m_labelsNode->addChild(this->m_fields->m_attemptsLabel);

    this->m_fields->m_fpsCounterLabel = CCLabelBMFont::create("", "bigFont.fnt");
    this->m_fields->m_labelsNode->addChild(this->m_fields->m_fpsCounterLabel);

    this->m_fields->m_cpsCounterLabel = CCLabelBMFont::create("", "bigFont.fnt");
    this->m_fields->m_labelsNode->addChild(this->m_fields->m_cpsCounterLabel);

    this->m_fields->m_jumpsLabel = CCLabelBMFont::create("", "bigFont.fnt");
    this->m_fields->m_labelsNode->addChild(this->m_fields->m_jumpsLabel);

    this->m_fields->m_clockLabel = CCLabelBMFont::create("", "bigFont.fnt");
    this->m_fields->m_labelsNode->addChild(this->m_fields->m_clockLabel);

    this->m_fields->m_sessionTimeLabel = CCLabelBMFont::create("", "bigFont.fnt");
    this->m_fields->m_labelsNode->addChild(this->m_fields->m_sessionTimeLabel);

    this->m_fields->m_noclipAccuracyLabel = CCLabelBMFont::create("", "bigFont.fnt");
    this->m_fields->m_labelsNode->addChild(this->m_fields->m_noclipAccuracyLabel);

    this->m_fields->m_noclipDeathsLabel = CCLabelBMFont::create("", "bigFont.fnt");
    this->m_fields->m_labelsNode->addChild(this->m_fields->m_noclipDeathsLabel);

    this->updateStatusLabels();

    if (setting().onSolidPlayerGlow) {
        //this->m_batchNodeAddPlayer->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
    }

    return true;
}

void PolzPlayLayer::update(float dt) {
	this->m_fields->_timeLeft -= dt;
	this->m_fields->_accum += 1 / dt;
	this->m_fields->_frames++;

	if (this->m_fields->_timeLeft <= 0) {
		this->m_fields->fps = this->m_fields->_accum / this->m_fields->_frames;
		this->m_fields->_timeLeft = this->m_fields->_updateInterval;
		this->m_fields->_accum = 0;
		this->m_fields->_frames = 0;
	}

	PlayLayer::update(dt);

	float playerPercentPos = this->m_player->getPositionX() / this->m_levelLength * 100.f;

	if (this->isCheating()) {
		this->m_fields->m_cheatingBeforeRestart = this->isCheating();
	}

	if ((setting().onAutoSafeMode && (this->isCheating() || this->m_fields->m_cheatingBeforeRestart)) || setting().onSafeMode) {
		safeModeON();
		setting().isSafeMode = true;
	}
	else {
		safeModeOFF();
		setting().isSafeMode = false;
	}

	if (setting().onLockCursor && !setting().show && !this->m_showingEndLayer && !this->m_isDead) {
		HWND hwnd = WindowFromDC(wglGetCurrentDC());
		RECT winSize; GetWindowRect(hwnd, &winSize);
		auto width = winSize.right - winSize.left;
		auto height = winSize.bottom - winSize.top;
		auto centerX = width / 2.f + winSize.left;
		auto centerY = height / 2.f + winSize.top;
		SetCursorPos(centerX, centerY);
	}

	if (setting().onAutoKill && (setting().killPercentage <= playerPercentPos)) {
		if (!this->m_isDead) {
			this->destroyPlayer(this->m_player);
		}
	}

	this->updateShowHitboxes();

	if (setting().onShowLayout) {
		this->updateShowLayout();
	}

	if ((setting().onAutoDeafen && !this->m_fields->m_deafenPressed) && (playerPercentPos > setting().deafenPercent) && !this->m_isDead && !this->m_endTriggered) {
		if ((this->m_practiceMode && !setting().onPracticeDeafen) || (this->m_testMode && !setting().onTestmodeDeafen)) return;

		this->m_fields->m_deafenPressed = true;
		keybd_event(VK_MENU, 0x38, 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}

	time_t currentTick = time::getTime();
	this->m_fields->m_clickFrames.erase(std::remove_if(this->m_fields->m_clickFrames.begin(), this->m_fields->m_clickFrames.end(), [currentTick](float tick) {
		return currentTick - tick > 1000;
		}), this->m_fields->m_clickFrames.end());
	this->m_fields->m_hasClicked = false;

	float playerXPos = this->m_player->getPositionX();

	if (playerXPos != this->m_fields->m_prevX) {
		this->m_fields->m_noclipFrames++;
		this->m_fields->m_totalDelta += dt;
	}

	if (this->m_fields->m_wouldDie) {
		this->m_fields->m_wouldDie = false;
		if (this->m_fields->m_totalDelta >= .1f && playerXPos != this->m_fields->m_prevX) {
			this->m_fields->m_deaths += 1;
		}
		if (this->m_fields->m_totalDelta >= .1f && playerXPos != this->m_fields->m_prevX && !this->m_fields->m_prevDied) {
			this->m_fields->m_deathsFull += 1;
		}
		this->m_fields->m_prevDied = true;
	}
	else {
		this->m_fields->m_prevDied = false;
	}

	this->m_fields->m_prevX = playerXPos;

	this->updateCheatIndicator();
	this->updateMessageLabel();
	this->updateAttemptsLabel();
	this->updateFPSLabel();
	this->updateCPSLabel();
	this->updateBestRunLabel();
	this->updateJumpsLabel();
	this->updateSessionTimeLabel();
	this->updateClockLabel();
	this->updateNoclipAccuracyLabel();
	this->updateNoclipDeathsLabel();
}

void PolzPlayLayer::resetLevel() {
	if (setting().onSmartStartPos) {
		for (StartPosObject* obj : this->m_fields->m_startPositions) {
			if (obj) {
				setupStartPos(obj);
			}
		}
	}

	this->m_fields->m_clickFrames.clear();
	this->m_fields->m_totalClicks = 0;

	if (this->m_endTriggered) {
		this->m_endTriggered = false;
		this->stopAllActions();
		this->m_player->stopAllActions();
		this->m_player2->stopAllActions();
	}

	PlayLayer::resetLevel();

	this->m_fields->m_cheatingBeforeRestart = this->isCheating();

	if (setting().onPracticeFix) {
		if (this->m_practiceMode && this->m_fields->m_checkpoints.size() > 0) {
			this->m_fields->m_checkpoints.back().restore(this);
		}
	}

	if (this->m_fields->m_deafenPressed) {
		this->m_fields->m_deafenPressed = false;
		keybd_event(VK_MENU, 0x38, 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}

	if (setting().onAutoPickupCoins) {
		for (auto* coin : this->m_fields->m_coinsToPickup) {
			if (coin == nullptr) continue;

			coin->destroyObject();
			this->pickupItem(coin);
		}
	}

	if (!this->m_practiceMode) {
		this->m_fields->m_prevX = this->m_player->getPositionX();
		this->m_fields->m_noclipFrames = 0;
		this->m_fields->m_totalDelta = 0;
		this->m_fields->m_deaths = 0;
		this->m_fields->m_deathsFull = 0;
		this->m_fields->m_wouldDie = false;
	}
}

void PolzPlayLayer::addToSection(GameObject* object) {
	PlayLayer::addToSection(object);

	switch (object->m_objectID) {
	case 31:
		this->m_fields->m_startPositions.push_back(static_cast<StartPosObject*>(object));
		break;
	case 12:
	case 13:
	case 47:
	case 111:
	case 660: this->m_fields->m_gamemodePortals.push_back(object);
		break;
	case 45:
	case 46: this->m_fields->m_mirrorPortals.push_back(object);
		break;
	case 99:
	case 101: this->m_fields->m_miniPortals.push_back(object);
		break;
	case 286:
	case 287: this->m_fields->m_dualPortals.push_back(object);
		break;
	case 200:
	case 201:
	case 202:
	case 203: this->m_fields->m_speedChanges.push_back(object);
		break;
	default: break;
	}

	if (object->m_objectID == 142) {
		this->m_fields->m_coinsToPickup.push_back(object);
	}
}

void PolzPlayLayer::createObjectsFromSetup(gd::string objects) {
	PlayLayer::createObjectsFromSetup(objects);
	if (this->m_fields->m_startPositions.empty()) return;

	std::ranges::sort(this->m_fields->m_startPositions, [](GameObject* a, GameObject* b) {
		return a->getPositionX() < b->getPositionX();
		});

	this->m_fields->m_currentStartPos = -1;
	if (this->m_startPosObject) {
		auto it = std::ranges::find(this->m_fields->m_startPositions, this->m_startPosObject);
		if (it != this->m_fields->m_startPositions.end())
			this->m_fields->m_currentStartPos = static_cast<int32_t>(std::distance(this->m_fields->m_startPositions.begin(), it));
	}
}

void PolzPlayLayer::togglePracticeMode(bool practice) {
	this->m_fields->m_checkpoints.clear();
	PlayLayer::togglePracticeMode(practice);

	if (setting().onHidePracticeButtons) {
		this->m_uiLayer->m_checkpointMenu->setVisible(!setting().onHidePracticeButtons);
	}
}

void PolzPlayLayer::showNewBest() {
	if (!setting().onNoNewBestPopup) PlayLayer::showNewBest();
}

void PolzPlayLayer::updateVisibility() {
	PlayLayer::updateVisibility();

	//
	if (this->m_attemptsLabel->isVisible() && setting().onHideAttempts) {
		this->m_attemptsLabel->setVisible(false);
	}

	if (this->m_player->isVisible() && setting().onHidePlayer) {
		this->m_player->setVisible(false);
	}
	if (this->m_player2->isVisible() && setting().onHidePlayer) {
		this->m_player2->setVisible(false);
	}

	if (setting().onNoWavePulse) {
		this->m_player->m_audioScale = 1.f;
		this->m_player2->m_audioScale = 1.f;
	}
	//
}

void PolzPlayLayer::updateAttempts() {
	PlayLayer::updateAttempts();

	if (setting().onShowTotalAttempts) {
		this->m_attemptsLabel->setString(CCString::createWithFormat("Attempt %i", this->m_level->m_attempts + 1)->getCString());
	}
}

void PolzPlayLayer::processItems() {
	if (!setting().onSafeMode) PlayLayer::processItems();
}

void PolzPlayLayer::destroyPlayer(PlayerObject* player) {
	PlayLayer::destroyPlayer(player);

	this->m_fields->m_wouldDie = true;

	if (setting().onNoclipTint && setting().onNoclip) {
		auto noclipTint = static_cast<CCLayerColor*>(this->getChildByTag(875));
		if (noclipTint) {
			noclipTint->runAction(CCSequence::create(CCFadeTo::create(0.f, 65), CCFadeTo::create(.1f, 0), nullptr));
		}
	}

	//PlayLayer::updateShowHitboxes();

	if (this->m_fields->m_deafenPressed) {
		this->m_fields->m_deafenPressed = false;
		keybd_event(VK_MENU, 0x38, 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}

	if (setting().onRespawnTime) {
		float respawnTime = setting().respawnValue / 1000.f;

		if (auto* respawnSequence = this->getActionByTag(0x10)) {
			this->stopAction(respawnSequence);

			auto* delayedSequence = CCSequence::create(CCDelayTime::create(respawnTime), CCCallFunc::create(this, callfunc_selector(PlayLayer::delayedResetLevel)), nullptr);

			delayedSequence->setTag(0x10);
			this->runAction(delayedSequence);
		}
	}

	if (!this->m_practiceMode && !this->m_testMode && !setting().onNoclip) {
		this->m_fields->m_lastRun = this->m_player->getPositionX() / this->m_levelLength * 100.f;
	}

	if (!this->m_practiceMode && this->m_testMode && !setting().onNoclip) {
		if (this->m_fields->m_startPositions.size() && this->m_fields->m_currentStartPos > -1) {
			StartPosObject* currentStartPosObject = this->m_fields->m_startPositions[this->m_fields->m_currentStartPos];
			if (currentStartPosObject) {
				float playerLastRun = this->m_player->getPositionX() / this->m_levelLength * 100.f;

				if (playerLastRun >= this->m_fields->m_startPositionsBestRun[currentStartPosObject].second) {
					this->m_fields->m_startPositionsBestRun[currentStartPosObject] = { playerLastRun, playerLastRun };
				}
			}
		}
	}

	if (setting().onNoclip) {
		this->updateNoclipDeathsLabel(true);
		this->updateNoclipAccuracyLabel(true);
	}

	this->updateBestRunLabel();
}

void PolzPlayLayer::levelComplete() {
	PlayLayer::levelComplete();

	if (!this->m_practiceMode && this->m_testMode) {
		if (this->m_fields->m_startPositions.size() && this->m_fields->m_currentStartPos > -1) {
			StartPosObject* currentStartPosObject = this->m_fields->m_startPositions[this->m_fields->m_currentStartPos];
			if (currentStartPosObject) {
				this->m_fields->m_startPositionsBestRun[currentStartPosObject] = { 100.f, 100.f };
			}
		}
	}

	if (!this->m_practiceMode && !this->m_testMode) {
		this->m_fields->m_lastRun = 100.f;
	}

	if (this->m_fields->m_deafenPressed) {
		this->m_fields->m_deafenPressed = false;
		keybd_event(VK_MENU, 0x38, 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}
}

void PolzPlayLayer::pauseGame(bool idk) {
	PlayLayer::pauseGame(idk);

	if (setting().onPauseUndeafen && this->m_fields->m_deafenPressed) {
		this->m_fields->m_deafenPressed = false;
		keybd_event(VK_MENU, 0x38, 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(setting().m_autoDeafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}
}

CheckpointObject* PolzPlayLayer::createCheckpoint() {
	if (this->m_player != nullptr) {
		this->m_fields->m_checkpoints.push_back({ Checkpoint::from(this) });
	}

	return PlayLayer::createCheckpoint();
}

void PolzPlayLayer::removeLastCheckpoint() {
	PlayLayer::removeLastCheckpoint();
	
	if (this->m_fields->m_checkpoints.size() > 0) {
		this->m_fields->m_checkpoints.pop_back();
	}
}

void PolzPlayLayer::spawnPlayer2() {
	PlayLayer::spawnPlayer2();

	if (setting().onInvisibleDualFix) {
		this->m_player2->setVisible(true);
	}
}

void PolzPlayLayer::pushButton(int p0, bool p1) {
	this->m_fields->m_isHolding = true;
	if (!this->m_fields->m_hasClicked) {
		this->m_fields->m_clickFrames.push_back(time::getTime());
		this->m_fields->m_totalClicks++;
		this->m_fields->m_hasClicked = true;
	}
	PlayLayer::pushButton(p0, p1);
}

void PolzPlayLayer::releaseButton(int p0, bool p1) {
	this->m_fields->m_isHolding = false;
	PlayLayer::releaseButton(p0, p1);
}