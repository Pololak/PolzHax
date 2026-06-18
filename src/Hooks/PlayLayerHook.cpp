#include "PlayLayerHook.h"
#include "../Setting.h"

int m_currentStartPos = 0;
std::vector<StartPosObject*> m_startPositions;
std::vector<GameObject*> m_dualPortals, m_gamemodePortals, m_miniPortals, m_speedChanges, m_mirrorPortals;

static GameObject* getClosestObject(std::vector<GameObject*>& vec, StartPosObject* startPos) {
	GameObject* closest = nullptr;

	std::sort(vec.begin(), vec.end(), [](GameObject* a, GameObject* b) {
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

void pickStartPos(PlayLayer* playLayer, int32_t index) { // Eclipse menu
	if (playLayer->m_practiceMode) return;

	if (m_startPositions.empty()) return;

	auto count = static_cast<int32_t>(m_startPositions.size());
	if (index >= count) index = -1;
	else if (index < -1) index = count - 1;

	m_currentStartPos = index;

	auto* startPos = index >= 0 ? m_startPositions[index] : nullptr;
	playLayer->setStartPosObject(startPos);
	playLayer->m_testMode = index >= 0;

	if (m_currentStartPos >= 0) { // Taswert's thing
		playLayer->m_playerStartPos = playLayer->m_startPosObject->getOrientedBox()->m_center;
	}
	else {
		playLayer->m_playerStartPos = ccp(0, 105);
	}

	playLayer->resetLevel();

	PlayLayerHook::updateStartPosSwitcherLabel();
	//PlayLayer::updateStatusLabels();
}

void PlayLayerHook::nextStartPos() {
    pickStartPos(GameManager::sharedState()->getPlayLayer(), m_currentStartPos + 1);
}

void PlayLayerHook::prevStartPos() {
    pickStartPos(GameManager::sharedState()->getPlayLayer(), m_currentStartPos - 1);
}

void setupStartPos(StartPosObject* startPos) { // Eclipse menu https://github.com/EclipseMenu/EclipseMenu/blob/main/src/hacks/Level/SmartStartPos.cpp
	PlayLayer* playLayer = GameManager::sharedState()->getPlayLayer();
	LevelSettingsObject* startPosSettings = startPos->m_settings;
	LevelSettingsObject* levelSettings = playLayer->m_levelSettings;

	startPosSettings->m_startDual = levelSettings->m_startDual;
	startPosSettings->m_startMode = levelSettings->m_startMode;
	startPosSettings->m_startMini = levelSettings->m_startMini;
	startPosSettings->m_startSpeed = levelSettings->m_startSpeed;

	GameObject* obj = getClosestObject(m_dualPortals, startPos);
	if (obj) {
		startPosSettings->m_startDual = obj->m_objectID == 286;
	}

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

void PlayLayerHook::updateStartPosSwitcherLabel() {
	if (m_startPositions.empty() || !setting().onStartPosSwitcher) return;

	auto self = GameManager::sharedState()->getPlayLayer();
	if (!self) return;

    if (self->m_practiceMode) return;

	auto startPosSwitcherMenu = static_cast<CCMenu*>(self->m_uiLayer->getChildByTag(125));
	if (!startPosSwitcherMenu) return;

	auto gm = GameManager::sharedState();

	auto startPosSwitcherLabel = static_cast<CCLabelBMFont*>(startPosSwitcherMenu->getChildByTag(1));
	auto onPrevStartPos = static_cast<CCMenuItemSpriteExtra*>(startPosSwitcherMenu->getChildByTag(2));
	auto onNextStartPos = static_cast<CCMenuItemSpriteExtra*>(startPosSwitcherMenu->getChildByTag(3));

	if (!startPosSwitcherLabel || !onPrevStartPos || !onNextStartPos) return;

	startPosSwitcherLabel->setString(CCString::createWithFormat("%i/%i", m_currentStartPos + 1, m_startPositions.size())->getCString());
	startPosSwitcherLabel->setVisible(true);
	startPosSwitcherLabel->setOpacity(255);
	startPosSwitcherLabel->stopAllActions();
	startPosSwitcherLabel->runAction(CCSequence::create(CCDelayTime::create(1.f), CCFadeOut::create(.5f), nullptr));

	onPrevStartPos->setVisible(true);
	onPrevStartPos->setOpacity(255);
	onPrevStartPos->stopAllActions();
	onPrevStartPos->runAction(CCSequence::create(CCDelayTime::create(1.f), CCFadeOut::create(.5f), CCHide::create(), nullptr));

	onNextStartPos->setVisible(true);
	onNextStartPos->setOpacity(255);
	onNextStartPos->stopAllActions();
	onNextStartPos->runAction(CCSequence::create(CCDelayTime::create(1.f), CCFadeOut::create(.5f), CCHide::create(), nullptr));
}

bool PlayLayerHook::initH(PlayLayer* self, GJGameLevel* level) {
    m_currentStartPos = 0;
    m_startPositions.clear();
    m_dualPortals.clear();
    m_gamemodePortals.clear();
    m_miniPortals.clear();
    m_speedChanges.clear();
    m_mirrorPortals.clear();

    if (!PlayLayerHook::init(self, level)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto gm = GameManager::sharedState();

    float playerPercentPos = self->m_player->getPositionX() / self->m_levelLength * 100.f;

    auto percentageLabel = CCLabelBMFont::create("", "bigFont.fnt");
    percentageLabel->setAnchorPoint({ (gm->m_showProgressBar ? 0.f : .5f), .5f });
    percentageLabel->setScale(.5f);
    percentageLabel->setVisible(gm->getGameVariable(SHOW_PERCENTAGE));
    percentageLabel->setPosition(winSize.width / 2.f + (gm->m_showProgressBar ? 110.2f : 0.f), winSize.height - 8.f);
    std::string percentageString = "%.0f%%";
    percentageLabel->setString(CCString::createWithFormat(percentageString.c_str(), playerPercentPos)->getCString());
    self->addChild(percentageLabel, 15, 301);

    PlayLayerHook::updateStartPosSwitcherLabel();

    return true;
}

void PlayLayerHook::updateH(PlayLayer* self, float dt) {
	PlayLayerHook::update(self, dt);

	
}

void PlayLayerHook::updateProgressbarH(PlayLayer* self) {
	PlayLayerHook::updateProgressbar(self);

	float playerPercentPos = self->m_player->getPositionX() / self->m_levelLength * 100.f;
    std::string percentageString = "%.0f%%";

    auto percentageLabel = static_cast<CCLabelBMFont*>(self->getChildByTag(301));
    if (percentageLabel) {
		percentageLabel->setString(CCString::createWithFormat(percentageString.c_str(), (playerPercentPos < 100.f) ? playerPercentPos : 100.f)->getCString());
    }
}

void PlayLayerHook::resetLevelH(PlayLayer* self) {
    if (setting().onSmartStartPos) {
        for (StartPosObject* obj : m_startPositions) {
            if (obj) {
                setupStartPos(obj);
            }
        }
    }

    PlayLayerHook::resetLevel(self);

    PlayLayerHook::updateStartPosSwitcherLabel();
}

void PlayLayerHook::addToSectionH(PlayLayer* self, GameObject* object) {
    PlayLayerHook::addToSection(self, object);

    switch (object->m_objectID) {
    case 31:
        m_startPositions.push_back(static_cast<StartPosObject*>(object));
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
}

void PlayLayerHook::createObjectsFromSetupH(PlayLayer* self, std::string objects) {
    PlayLayerHook::createObjectsFromSetup(self, objects);
    if (m_startPositions.empty()) return;

    std::sort(m_startPositions.begin(), m_startPositions.end(), [](GameObject* a, GameObject* b) {
        return a->getPositionX() < b->getPositionX();
        });

    m_currentStartPos = -1;
    if (self->m_startPosObject) {
        auto it = std::find(m_startPositions.begin(), m_startPositions.end(), self->m_startPosObject);
        if (it != m_startPositions.end())
            m_currentStartPos = static_cast<int32_t>(std::distance(m_startPositions.begin(), it));
    }
}

void PlayLayerHook::togglePracticeModeH(PlayLayer* self, bool practice) {
	if (setting().onPracticeMusic && practice && !self->m_practiceMode) {
		self->m_practiceMode = practice;
		self->m_uiLayer->toggleCheckpointsMenu(practice);
		self->stopActionByTag(18);
	}
	else {
		PlayLayerHook::togglePracticeMode(self, practice);
	}

	if (practice) {
		auto startPosSwitcherMenu = static_cast<CCMenu*>(self->m_uiLayer->getChildByTag(125));
		static_cast<CCLabelBMFont*>(startPosSwitcherMenu->getChildByTag(1))->setVisible(false);
		static_cast<CCMenuItemSpriteExtra*>(startPosSwitcherMenu->getChildByTag(2))->setVisible(false);
		static_cast<CCMenuItemSpriteExtra*>(startPosSwitcherMenu->getChildByTag(3))->setVisible(false);
	}
}

void PlayLayerHook::mem_init() {
    HOOK("_ZN9PlayLayer4initEP11GJGameLevel", PlayLayerHook::initH, PlayLayerHook::init);
    HOOK("_ZN9PlayLayer6updateEf", PlayLayerHook::updateH, PlayLayerHook::update);
    HOOK("_ZN9PlayLayer17updateProgressbarEv", PlayLayerHook::updateProgressbarH, PlayLayerHook::updateProgressbar);
    HOOK("_ZN9PlayLayer10resetLevelEv", PlayLayerHook::resetLevelH, PlayLayerHook::resetLevel);
    HOOK("_ZN9PlayLayer12addToSectionEP10GameObject", PlayLayerHook::addToSectionH, PlayLayerHook::addToSection);
    HOOK("_ZN9PlayLayer22createObjectsFromSetupESs", PlayLayerHook::createObjectsFromSetupH, PlayLayerHook::createObjectsFromSetup);
    HOOK("_ZN9PlayLayer18togglePracticeModeEb", PlayLayerHook::togglePracticeModeH, PlayLayerHook::togglePracticeMode);
}