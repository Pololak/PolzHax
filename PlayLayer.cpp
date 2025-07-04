#include "pch.h"
#include "state.h"
#include "PlayLayer.h"
//#include "Scheduler.h"
//#include "Labels.h"
#include "utils.hpp"
#include "CheckPoint.h"
#include "Hitboxes.h"
#include "imgui.h"
#include "SonicWaveFirstWave.hpp"
//#include "ObjectsIds.h"

CCArray* startPosArr;
gd::PlayLayer* playLayer;
ccColor3B playerColor1;
ccColor3B playerColor2;
ccColor3B playerColorG;

std::time_t t;
SYSTEMTIME st;
float NoclipAcc = 0.f;
bool isPlayerColliding = false;
bool isPlayerColorGot = false;
bool isPlayerDead = false;
gd::HardStreak* hardStreak;
int currentStartPos = 0;
bool fadedoutflag = 0;
int deaths = 0;
int currentBest = 0;
bool wasDead = 0;
float deathPos = 0.f;
float deathDifference = 0.f;

bool hasClicked = false;
std::vector<time_t> m_clickFrames;
int m_totalClicks = 0;
bool m_isHolding = false;

std::set<int>pulseObjects = {
    50, 51, 52, 53, 54, 60, 148, 149, 405,
    37 // that pulse things on sticks
};

std::set<int>portalObjects = {
    10, 11,					//gravity portals
    12, 13, 47, 111, 660,	//gamemode portals
    45, 46,					//mirror portals
    99, 101,				//mini/big portals
    286, 287				//dual portals
};

std::set<int>cornerObjects = {
    472, 473, 474
};

std::vector<gd::StartPosObject*> startPositions;
std::vector<gd::GameObject*> gravityPortals, dualPortals, gamemodePortals, miniPortals, speedChanges, mirrorPortals;
std::vector<bool> willFlip;

std::vector<CheckPoint> checkpoints;
std::vector<ObjectBlending> objBlending;
bool inPractice = false;
float updateRgb = 0.f;

std::vector<gd::GameObject*> m_startPosObjects = {};
int m_startPosIdx = 0;
bool m_canSwitch = true;

bool levelCompleted = false;

bool fromZero = true;

bool pressed = false;
int startPercent = 0;

std::set<int>invisibleObjects = {
    146, 174, 204, 206, 673, 674, 144, 205, 145, 459, 740, 741, 742
};

CCMenu* pauseLayerTogglerMenu;

ccColor3B getChromaColour() {
    return ColorUtility::hsvToRgb(cchsv((updateRgb * 180) / 10.f, 1.f, 1.f, true, true));
}

ccColor4F getChromaColour4() {
    auto color3b = ColorUtility::hsvToRgb(cchsv((updateRgb * 180) / 10.0f, 1.0f, 1.0f, true, true));
    return ccColor4F(color3b.r, color3b.g, color3b.b, 255);
}

int smoothOut;

void PlayLayer::onNextStartPos() {
    if (playLayer && !playLayer->hasCompletedLevel()) {
        auto spswitcherlbl = static_cast<CCLabelBMFont*>(playLayer->getChildByTag(45712));
        if (spswitcherlbl) {
            auto fadeout = CCSequence::create(CCDelayTime::create(2.f), CCFadeOut::create(0.5f), nullptr);
            if (!fadedoutflag)
            {
                spswitcherlbl->runAction(fadeout);
                fadedoutflag = !fadedoutflag;
            }
            if (startPosArr->count() > 1) {
                if (currentStartPos == startPosArr->count() - 1) currentStartPos = 0;
                else currentStartPos++;
                if (currentStartPos != 0) {
                    playLayer->setPlayerStartPosition(reinterpret_cast<gd::StartPosObject*>(startPosArr->objectAtIndex(currentStartPos))->getOrientedBox()->getCenterPoint());
                    playLayer->setStartPosObject(reinterpret_cast<gd::StartPosObject*>(startPosArr->objectAtIndex(currentStartPos)));
                }
                else {
                    playLayer->setPlayerStartPosition({ 0, 105 });
                    playLayer->setStartPosObject(nullptr);
                }
                if (currentStartPos == 0) {
                    from<bool>(playLayer, 0x2b8) = false;
                }
                else {
                    from<bool>(playLayer, 0x2b8) = true;
                }
                playLayer->resetLevel();
                spswitcherlbl->setString(CCString::createWithFormat("%d/%d", currentStartPos, startPosArr->count() - 1)->getCString());
                spswitcherlbl->stopAllActions();
                spswitcherlbl->setOpacity(255);
                spswitcherlbl->runAction(fadeout);
            }
        }
    }
}

void PlayLayer::onPrevStartPos() {
    if (playLayer && !playLayer->hasCompletedLevel()) {
        auto spswitcherlbl = static_cast<CCLabelBMFont*>(playLayer->getChildByTag(45712));
        if (spswitcherlbl) {
            auto fadeout = CCSequence::create(CCDelayTime::create(2.f), CCFadeOut::create(0.5f), nullptr);
            if (!fadedoutflag)
            {
                spswitcherlbl->runAction(fadeout);

                fadedoutflag = !fadedoutflag;
            }
            if (startPosArr->count() > 1) {
                if (currentStartPos == 0) currentStartPos = startPosArr->count() - 1;
                else currentStartPos--;
                if (currentStartPos != 0) {
                    playLayer->setPlayerStartPosition(reinterpret_cast<gd::StartPosObject*>(startPosArr->objectAtIndex(currentStartPos))->getOrientedBox()->getCenterPoint());
                    playLayer->setStartPosObject(reinterpret_cast<gd::StartPosObject*>(startPosArr->objectAtIndex(currentStartPos)));
                }
                else {
                    playLayer->setPlayerStartPosition({ 0, 105 });
                    playLayer->setStartPosObject(nullptr);
                }
                if (currentStartPos == 0) {
                    from<bool>(playLayer, 0x2b8) = false;
                }
                else {
                    from<bool>(playLayer, 0x2b8) = true;
                }
                playLayer->resetLevel();
                spswitcherlbl->setString(CCString::createWithFormat("%d/%d", currentStartPos, startPosArr->count() - 1)->getCString());
                spswitcherlbl->stopAllActions();
                spswitcherlbl->setOpacity(255);
                spswitcherlbl->runAction(fadeout);
            }
        }
    }
}

void PlayLayer::Callback::onNextPos(CCObject*) {
    PlayLayer::onNextStartPos();
}

void PlayLayer::Callback::onPrevPos(CCObject*) {
    PlayLayer::onPrevStartPos();
}

bool compareXPos(gd::GameObject* a, gd::GameObject* b) {
    return a->getPositionX() < b->getPositionX();
}

gd::GameObject* getClosestObject(std::vector<gd::GameObject*>& vec, gd::StartPosObject* startPos) {
    gd::GameObject* closest = nullptr;

    std::ranges::sort(vec.begin(), vec.end(), [](gd::GameObject* a, gd::GameObject* b) {
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

void setupStartPos(gd::StartPosObject* startPos) {
    gd::LevelSettingsObject* startPosSettings = startPos->levelSettingsObject();
    gd::LevelSettingsObject* levelSettings = gd::GameManager::sharedState()->getPlayLayer()->getLevelSettings();

    if (gd::GameManager::sharedState()->getPlayLayer()) {
        startPosSettings->setDual(levelSettings->getDual());
        startPosSettings->setGamemode(levelSettings->getGamemode());
        startPosSettings->setMini(levelSettings->getMini());
        startPosSettings->setSpeed(levelSettings->getSpeed());

        gd::GameObject* obj = getClosestObject(dualPortals, startPos);
        if (obj)
            startPosSettings->setDual(obj->getObjectID() == 286);

        obj = getClosestObject(gamemodePortals, startPos);
        if (obj) {
            switch (obj->getObjectID())
            {
            case 12:
                startPosSettings->setGamemode(0);
                break;
            case 13:
                startPosSettings->setGamemode(1);
                break;
            case 47:
                startPosSettings->setGamemode(2);
                break;
            case 111:
                startPosSettings->setGamemode(3);
                break;
            case 660:
                startPosSettings->setGamemode(4);
                break;
            }
        }

        obj = getClosestObject(miniPortals, startPos);
        if (obj)
            startPosSettings->setMini(obj->getObjectID() == 101);

        obj = getClosestObject(speedChanges, startPos);
        if (obj) {
            switch (obj->getObjectID())
            {
            case 200:
                startPosSettings->setSpeed(gd::Speed::kSpeedSlow);
                break;
            case 201:
                startPosSettings->setSpeed(gd::Speed::kSpeedNormal);
                break;
            case 202:
                startPosSettings->setSpeed(gd::Speed::kSpeedFast);
                break;
            case 203:
                startPosSettings->setSpeed(gd::Speed::kSpeedFaster);
                break;
            }
        }
    }
}

void updateShowLayout(gd::PlayLayer* self) {
    if (setting().onShowLayout) {
        ccColor3B bgColor = ccc3(40, 125, 255);
        ccColor3B gColor = ccc3(0, 102, 255);

        self->m_backgroundSprite->setColor(bgColor);
        self->m_bottomGround->groundSprite()->setColor(gColor);
        self->m_bottomGround->lineSprite()->setColor(ccc3(255, 255, 255));
        self->m_topGround->groundSprite()->setColor(gColor);
        self->m_topGround->lineSprite()->setColor(ccc3(255, 255, 255));

        self->m_backgroundFlash->setVisible(false);

        for (int i = self->m_firstVisibleSection - 1; i <= self->m_lastVisibleSection + 1; i++) {
            if (i < 0) continue;
            if (i >= self->m_levelSections->count()) break;

            auto objectAtIndex = self->m_levelSections->objectAtIndex(i);
            auto objArr = reinterpret_cast<CCArray*>(objectAtIndex);

            for (int j = 0; j < objArr->count(); j++) {
                auto obj = reinterpret_cast<gd::GameObject*>(objArr->objectAtIndex(j));
                if ((obj->m_objectType == gd::kGameObjectTypeDecoration || obj->m_objectType == gd::kGameObjectTypePulseObjects) && obj->isVisible()/* && (obj->m_objectID == 50 && obj->m_objectID == 51 && obj->m_objectID == 52 && obj->m_objectID == 53 && obj->m_objectID == 54 && obj->m_objectID == 60 && obj->m_objectID == 148 && obj->m_objectID == 149 && obj->m_objectID == 405) */ && (obj->m_objectID != 10 && obj->m_objectID != 11 && obj->m_objectID != 12 && obj->m_objectID != 13 && obj->m_objectID != 38 && obj->m_objectID != 44 && obj->m_objectID != 45 && obj->m_objectID != 46 && obj->m_objectID != 47 && obj->m_objectID != 99 && obj->m_objectID != 101 && obj->m_objectID != 111 && obj->m_objectID != 286 && obj->m_objectID != 287 && obj->m_objectID != 660 && obj->m_objectID != 745 && obj->m_objectID != 749) && obj != (gd::GameObject*)self->m_endPortalObject)
                    obj->setVisible(false);

                obj->setObjectColor({ 255, 255, 255 });
                if (obj->m_colorSprite) obj->m_colorSprite->setColor({ 255, 255, 255 });
                obj->m_customColorMode = (gd::CustomColorMode)9;
            }
        }
    }
}

void PlayLayer::updateShowHitboxes(gd::PlayLayer* self) {
    auto playerDrawNode = static_cast<CCDrawNode*>(self->layer()->getChildByTag(124));
    playerDrawNode->clear();
    auto objectDrawNode = static_cast<CCDrawNode*>(self->layer()->getChildByTag(125));
    objectDrawNode->clear();

    if ((self->m_player->m_isDead && setting().onHitboxesOnDeath) || setting().onHitboxes) {
        if (setting().onPlayerHitbox) {
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
                if (setting().onSolidsHitbox)
                    Hitboxes::drawSolidsObjectHitbox(obj, objectDrawNode);
                if (setting().onHazardsHitbox)
                    Hitboxes::drawHazardsObjectHitbox(obj, objectDrawNode);
                if (setting().onSpecialsHitbox)
                    Hitboxes::drawSpecialsObjectHitbox(obj, objectDrawNode);
            }
        }
    }
}

void __fastcall PlayLayer::destroyPlayer_H(gd::PlayLayer* self, void* edx, gd::PlayerObject* player)
{
    isPlayerDead = true;
    if (!wasDead) {
        deaths++;
    }
    else {
        deathDifference += player->getPositionX() - deathPos;
    }
    deathPos = player->getPositionX();
    wasDead = true;

    if (pressed) {
        pressed = false;
        keybd_event(VK_MENU, 0x38, 0, 0);
        keybd_event(setting().deafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event(setting().deafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
    }

    PlayLayer::destroyPlayer(self, player);
}

void __fastcall PlayLayer::resetLevel_H(gd::PlayLayer* self) {
    setting().beforeRestartCheatsCount = setting().cheatsCount;

    deaths = 0;
    wasDead = 0;
    deathDifference = 0.f;
    deathPos = 0.f;
    m_clickFrames.clear();
    m_totalClicks = 0;
    from<bool>(self, 0x2f9) = false;

    if (setting().onSmartStartPos) {
        if (from<bool>(self, 0xd0)) {
            for (gd::StartPosObject* object : startPositions) {
                if (object != nullptr) {
                    setupStartPos(object);
                }
            }
        }
    }

    fromZero = self->getLastRunPercent() == 0;

    if (pressed)
    {
        pressed = false;
        keybd_event(VK_MENU, 0x38, 0, 0);
        keybd_event(setting().deafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event(setting().deafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
    }

    if (self->getCheckpoints()->count() > 0) {
        auto porcodio = self->getCheckpoints()->lastObject();
        gd::CheckpointObject* go = reinterpret_cast<gd::CheckpointObject*>(porcodio);
        startPercent = (from<gd::PlayerCheckpoint*>(go, 0xec)->getPositionX() / self->levelLength()) * 100.f;
    }
    else {
        startPercent = (self->getPlayerStartPosition().x / self->levelLength()) * 100.f;
    }

    if (setting().onCheckpointLagFix && (self->m_practiceMode || self->m_testMode)) smoothOut = 2;

    PlayLayer::resetLevel(self);

    if (setting().onPracticeFix) {
        if (inPractice && checkpoints.size() > 0) {
            checkpoints.back().restore(self);
            objBlending.back().restore(self);
            self->m_needsReorderColor01 = true;
            self->m_needsReorderColor02 = true;
            self->m_needsReorderColor03 = true;
            self->m_needsReorderColor04 = true;
            self->m_needsReorderColorDL = true;
        }
    }
}

bool __fastcall PlayLayer::init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level) {
    smoothOut = 0;
    playLayer = self;
    if (!PlayLayer::init(self, level)) return false;

    setting().beforeRestartCheatsCount = setting().cheatsCount;
    isPlayerColorGot = false;
    fadedoutflag = 0;
    currentBest = 0;

    dualPortals.clear();
    gamemodePortals.clear();
    miniPortals.clear();
    miniPortals.clear();
    speedChanges.clear();
    mirrorPortals.clear();
    startPositions.clear();

    if (setting().onAutoPractice) self->togglePracticeMode(true);

    if (setting().onPracticeFix) {
        checkpoints.clear();
        objBlending.clear();
        inPractice = false;
    }

    if (setting().onSmartStartPos) {
        if (from<bool>(self, 0xd0)) {
            for (gd::StartPosObject* object : startPositions) {
                if (object != nullptr) {
                    setupStartPos(object);
                }
            }
        }
    }

    if (setting().onShowLayout) updateShowLayout(self);

    auto director = CCDirector::sharedDirector();
    auto size = CCDirector::sharedDirector()->getWinSize();

    auto secarr = self->getSections();
    auto objarr1 = self->getObjects();
    auto arrcount = secarr->count();

    if (startPosArr) delete startPosArr;
    auto sposarr = new CCArray;
    auto firstStartPosObj = gd::StartPosObject::create();
    sposarr->addObject(firstStartPosObj);
    startPosArr = sposarr;
    for (int i = 0; i < secarr->count(); i++)
    {
        auto objarr = reinterpret_cast<CCArray*>(secarr->objectAtIndex(i));
        for (int j = 0; j < objarr->count(); j++)
        {
            auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
            switch (obj->getObjectID())
            {
            case 31:
                startPosArr->addObject(obj);
                startPositions.push_back((gd::StartPosObject*)obj);
                break;
            case 12:
            case 13:
            case 47:
            case 111:
            case 660:
                gamemodePortals.push_back(obj);
                break;
            case 45:
            case 46:
                mirrorPortals.push_back(obj);
                break;
            case 99:
            case 101:
                miniPortals.push_back(obj);
                break;
            case 286:
            case 287:
                dualPortals.push_back(obj);
                break;
            case 200:
            case 201:
            case 202:
            case 203:
                speedChanges.push_back(obj);
                break;
            }
        }
    }
    currentStartPos = startPosArr->count() - 1;

    if (setting().onSPSwitcher)
    {
        auto spswitcher = CCLabelBMFont::create("", "bigFont.fnt");
        spswitcher->setZOrder(5);
        spswitcher->setScale(0.5f);
        spswitcher->setAnchorPoint({ 0.5f, 0.5f });
        spswitcher->setString(CCString::createWithFormat("%d/%d", currentStartPos, startPosArr->count() - 1)->getCString());
        spswitcher->setPosition({ CCDirector::sharedDirector()->getScreenRight() / 2, 20.f });
        spswitcher->setTag(45712);
        if (startPosArr->count() == 1) spswitcher->setVisible(0);
        self->addChild(spswitcher);
        if (spswitcher) {
            auto fadeout = CCSequence::create(CCDelayTime::create(2.f), CCFadeOut::create(0.5f), nullptr);
            if (!fadedoutflag)
            {
                spswitcher->runAction(fadeout);
                fadedoutflag = !fadedoutflag;
            }
            spswitcher->stopAllActions();
            spswitcher->setOpacity(255);
            spswitcher->runAction(fadeout);
        }
    }

    const auto bar = gd::GameManager::sharedState()->getShowProgressBar();
    const auto value = self->player1()->getPositionX() / self->levelLength() * 100.f;

    auto percentLabel = CCLabelBMFont::create("", "bigFont.fnt");
    switch (setting().accuratePercentage)
    {
    case 4:
        percentLabel->setString(CCString::createWithFormat("%.0f%%", value)->getCString());
        break;
    case 3:
        percentLabel->setString(CCString::createWithFormat("%.1f%%", value)->getCString());
        break;
    case 2:
        percentLabel->setString(CCString::createWithFormat("%.2f%%", value)->getCString());
        break;
    case 1:
        percentLabel->setString(CCString::createWithFormat("%.3f%%", value)->getCString());
        break;
    case 0:
        percentLabel->setString(CCString::createWithFormat("%.4f%%", value)->getCString());
        break;
    }
    percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
    percentLabel->setScale(0.5f);
    percentLabel->setZOrder(5);
    percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });
    percentLabel->setTag(4571);
    percentLabel->setVisible(setting().onShowPercentage);
    self->addChild(percentLabel);

    auto playerDrawNode = CCDrawNode::create();
    self->layer()->addChild(playerDrawNode, 1000, 124);

    auto objectDrawNode = CCDrawNode::create();
    self->layer()->addChild(objectDrawNode, 1000, 125);

    if (setting().onHitboxes) {
        if (setting().onPlayerHitbox) {
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
                if (setting().onSolidsHitbox)
                    Hitboxes::drawSolidsObjectHitbox(obj, objectDrawNode);
                if (setting().onHazardsHitbox)
                    Hitboxes::drawHazardsObjectHitbox(obj, objectDrawNode);
                if (setting().onSpecialsHitbox)
                    Hitboxes::drawSpecialsObjectHitbox(obj, objectDrawNode);
            }
        }
    }

    self->m_attemptsLabel->setVisible(!setting().onHideAttempts);

    self->m_player->setVisible(!setting().onHidePlayer);
    self->m_player2->setVisible(!setting().onHidePlayer);

    auto labelsMenu = CCMenu::create();
    labelsMenu->setPosition({ 0, 0 });
    labelsMenu->setTag(7900);
    labelsMenu->setZOrder(5);
    labelsMenu->setAnchorPoint({ 0, 1 });
    labelsMenu->setScale(setting().labelsScale);
    labelsMenu->setVisible(!setting().onHideLabels);
    self->addChild(labelsMenu);

    int labelCount = 0;

    if (setting().onCheatIndicator)
    {
        labelCount++;
        auto CheatIndicatorLabel = CCLabelBMFont::create(".", "bigFont.fnt");
        CheatIndicatorLabel->setScale(1.0f);
        CheatIndicatorLabel->setZOrder(5);
        CheatIndicatorLabel->setAnchorPoint({ 0.f, 0.f });
        CheatIndicatorLabel->setPosition({ 2.f, size.height - 13.f * labelCount });
        CheatIndicatorLabel->setTag(45072);
        //CheatIndicatorLabel->setAnchorPoint({ 0.5f, 0.5f });
        ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4F04E9), &setting().CurrentNoclipByte, 1, 0);
        if (setting().NoclipByte != setting().CurrentNoclipByte && setting().onNoclipOutOfMe == false) { setting().cheatsCount++; setting().beforeRestartCheatsCount++; setting().onNoclipOutOfMe = true; }
        else if (setting().NoclipByte == setting().CurrentNoclipByte && setting().onNoclipOutOfMe == true) { setting().cheatsCount--; setting().onNoclipOutOfMe = false; }
        if (setting().cheatsCount == 0 && !(setting().onSafeMode || setting().isSafeMode)) CheatIndicatorLabel->setColor({ 0, 255, 0 });
        else if (setting().cheatsCount == 0 && (setting().onSafeMode || setting().isSafeMode)) CheatIndicatorLabel->setColor({ 255, 255, 0 });
        else if (setting().cheatsCount != 0 && (setting().onSafeMode || setting().isSafeMode)) CheatIndicatorLabel->setColor({ 255, 128, 0 });
        else CheatIndicatorLabel->setColor({ 255, 0, 0 });


        //CheatIndicatorLabel->
        labelsMenu->addChild(CheatIndicatorLabel);


    }

    if (setting().onAutoSafe && setting().cheatsCount > 0) safeModeON(), setting().isSafeMode = true;
    else if (!setting().onSafeMode) safeModeOFF(), setting().isSafeMode = false;

    if (setting().onMessageLabel)
    {
        labelCount++;
        auto messageLabel = CCLabelBMFont::create("", "bigFont.fnt");
        messageLabel->setString(CCString::create(setting().message)->getCString());
        messageLabel->setScale(0.35f);
        messageLabel->setZOrder(5);
        messageLabel->setAnchorPoint({ 0.f, 0.f });
        messageLabel->setOpacity(setting().labelsOpacity);
        messageLabel->setTag(45077);
        messageLabel->setPosition({ 2.f, size.height - 11.f * labelCount });
        labelsMenu->addChild(messageLabel);
    }

    if (setting().onAttemptsLabel)
    {
        labelCount++;
        auto AttemptsLabel = CCLabelBMFont::create("Attempt 1", "bigFont.fnt");
        AttemptsLabel->setScale(0.35f);
        AttemptsLabel->setZOrder(5);
        AttemptsLabel->setAnchorPoint({ 0.f, 0.f });
        AttemptsLabel->setOpacity(setting().labelsOpacity);
        AttemptsLabel->setTag(45073);
        AttemptsLabel->setPosition({ 2.f, size.height - 11.f * labelCount });
        labelsMenu->addChild(AttemptsLabel);
    }


    if (setting().onBestRunLabel) {
        labelCount++;
        auto BestRunLabel = CCLabelBMFont::create("Best Run: 0%", "bigFont.fnt");
        BestRunLabel->setScale(0.35f);
        BestRunLabel->setZOrder(5);
        BestRunLabel->setAnchorPoint({ 0.f,0.f });
        BestRunLabel->setOpacity(setting().labelsOpacity);
        BestRunLabel->setTag(45080);
        BestRunLabel->setPosition({ 2.f, size.height - 11.f * labelCount });
        labelsMenu->addChild(BestRunLabel);
    }


    if (setting().onClock)
    {
        labelCount++;
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream s;
        s << std::put_time(&tm, "%H:%M:%S");

        auto Clock = CCLabelBMFont::create(":", "bigFont.fnt");
        Clock->setString(CCString::create(s.str().c_str())->getCString());
        Clock->setScale(0.35f);
        Clock->setZOrder(5);
        Clock->setAnchorPoint({ 0.f, 0.f });
        Clock->setOpacity(setting().labelsOpacity);
        Clock->setTag(45075);
        Clock->setPosition({ 2.f, size.height - 11.f * labelCount });
        labelsMenu->addChild(Clock);
    }

    if (setting().onSessionTime) {
        labelCount++;
        auto SessionTimeLabel = CCLabelBMFont::create("0s", "bigFont.fnt");
        SessionTimeLabel->setScale(0.35f);
        SessionTimeLabel->setZOrder(5);
        SessionTimeLabel->setAnchorPoint({ 0.f,0.f });
        SessionTimeLabel->setOpacity(setting().labelsOpacity);
        SessionTimeLabel->setTag(45082);
        SessionTimeLabel->setPosition({ 2.f, size.height - 11.f * labelCount });
        labelsMenu->addChild(SessionTimeLabel);
    }

    if (setting().onFPSLabel)
    {
        labelCount++;

        float fps = ImGui::GetIO().Framerate;
        auto FPSLabel = CCLabelBMFont::create("0 FPS", "bigFont.fnt");
        FPSLabel->setString(CCString::createWithFormat("%.f FPS", fps)->getCString());
        FPSLabel->setScale(0.35f);
        FPSLabel->setZOrder(5);
        FPSLabel->setAnchorPoint({ 0.f, 0.f });
        FPSLabel->setOpacity(setting().labelsOpacity);
        FPSLabel->setTag(45076);
        FPSLabel->setPosition({ 2.f, size.height - 11.f * labelCount });
        labelsMenu->addChild(FPSLabel);
    }

    if (setting().onCPSLabel) {
        labelCount++;
        auto CPSLabel = CCLabelBMFont::create("0/0 CPS", "bigFont.fnt");
        CPSLabel->setScale(0.35f);
        CPSLabel->setZOrder(5);
        CPSLabel->setAnchorPoint({ 0.f, 0.f });
        CPSLabel->setOpacity(setting().labelsOpacity);
        CPSLabel->setTag(45081);
        CPSLabel->setPosition({ 2.f, size.height - 11.f * labelCount });
        labelsMenu->addChild(CPSLabel);
    }

    if (setting().onNoclipAccuracy)
    {
        labelCount++;
        auto noclipAccLabel = CCLabelBMFont::create("100.00%", "bigFont.fnt");
        noclipAccLabel->setScale(0.35f);
        noclipAccLabel->setZOrder(5);
        noclipAccLabel->setAnchorPoint({ 0.f, 0.f });
        noclipAccLabel->setOpacity(setting().labelsOpacity);
        noclipAccLabel->setTag(45079);
        noclipAccLabel->setPosition({ 2.f, size.height - 11.f * labelCount });
        noclipAccLabel->setVisible(setting().onNoclip);
        labelsMenu->addChild(noclipAccLabel);
    }

    if (setting().onNoclipDeaths)
    {
        labelCount++;
        auto noclDeath = CCLabelBMFont::create("0 Deaths", "bigFont.fnt");
        noclDeath->setScale(0.35f);
        noclDeath->setZOrder(5);
        noclDeath->setAnchorPoint({ 0.f, 0.f });
        noclDeath->setOpacity(setting().labelsOpacity);
        noclDeath->setTag(45078);
        noclDeath->setPosition({ 2.f, size.height - 11.f * labelCount });
        noclDeath->setVisible(setting().onNoclip);
        labelsMenu->addChild(noclDeath);
    }

    if (setting().onMeta)
    {
        auto xPos = CCLabelBMFont::create("X: 0", "bigFont.fnt");
        xPos->setString(CCString::createWithFormat("X: %.f")->getCString());
        xPos->setScale(0.35f);
        xPos->setZOrder(5);
        xPos->setAnchorPoint({ 0.f, 0.f });
        xPos->setOpacity(100);
        xPos->setTag(69060);
        xPos->setPosition({ 2.f, 40 });
        self->addChild(xPos);

        auto yPos = CCLabelBMFont::create("Y: 0", "bigFont.fnt");
        yPos->setString(CCString::createWithFormat("Y: %.f")->getCString());
        yPos->setScale(0.35f);
        yPos->setZOrder(5);
        yPos->setAnchorPoint({ 0.f, 0.f });
        yPos->setOpacity(100);
        yPos->setTag(69061);
        yPos->setPosition({ 2.f, 30 });
        self->addChild(yPos);

        auto vel = CCLabelBMFont::create("yVel: 0", "bigFont.fnt");
        vel->setString(CCString::createWithFormat("yVel: %.f")->getCString());
        vel->setScale(0.35f);
        vel->setZOrder(5);
        vel->setAnchorPoint({ 0.f, 0.f });
        vel->setOpacity(100);
        vel->setTag(69062);
        vel->setPosition({ 2.f, 20 });
        self->addChild(vel);

        auto rot = CCLabelBMFont::create("Rot: 0", "bigFont.fnt");
        rot->setString(CCString::createWithFormat("Rot: %.f")->getCString());
        rot->setScale(0.35f);
        rot->setZOrder(5);
        rot->setAnchorPoint({ 0.f, 0.f });
        rot->setOpacity(100);
        rot->setTag(69063);
        rot->setPosition({ 2.f, 10 });
        self->addChild(rot);

        auto size = CCLabelBMFont::create("Size: 0", "bigFont.fnt");
        size->setString(CCString::createWithFormat("Size: %.f")->getCString());
        size->setScale(0.35f);
        size->setZOrder(5);
        size->setAnchorPoint({ 0.f, 0.f });
        size->setOpacity(100);
        size->setTag(69064);
        size->setPosition({ 2.f, 0 });
        self->addChild(size);
    }

    auto arrowMenu = CCMenu::create();
    arrowMenu->setZOrder(105);
    auto arrowSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    arrowSprite->setRotation(-90.f);
    arrowSprite->setScale(0.6f);
    auto showEndLayerButton = gd::CCMenuItemSpriteExtra::create(arrowSprite, nullptr, self, menu_selector(EndLevelLayer::Callback::onShowLayer));
    showEndLayerButton->setPosition(arrowMenu->convertToNodeSpace({ size.width / 2.f, director->getScreenTop() + 20.f }));
    arrowMenu->addChild(showEndLayerButton, 0, 5765);
    showEndLayerButton->setEnabled(false);
    self->addChild(arrowMenu, 105, 18257);
    auto xdlabel = CCLabelBMFont::create("Good Job! Your game will crash :)", "bigFont.fnt");
    showEndLayerButton->addChild(xdlabel);
    xdlabel->setScale(0.5f);
    xdlabel->setPosition({showEndLayerButton->getContentSize().width / 2.f, +35.f});
    xdlabel->setOpacity(50);

    if (gd::GameManager::sharedState()->getGameVariable("0024")) {
        from<gd::CCMenuItemSpriteExtra*>(self->m_uiLayer, 0x1a0)->setVisible(!setting().onHidePauseButton);
    }

    return true;
}

bool rKeyFlag = true;
bool lKeyFlag = true;

void __fastcall PlayLayer::togglePracticeModeH(gd::PlayLayer* self, void* edx, bool practice) {
    if (setting().onPracticeFix) {
        checkpoints.clear();
        inPractice = practice;
    }
    PlayLayer::togglePracticeMode(self, practice);
    
    if (setting().onHidePracticeBtn)
        self->m_uiLayer->m_checkpointMenu->setVisible(!setting().onHidePracticeBtn);
}

void __fastcall PlayLayer::createCheckpointH(gd::PlayLayer* self) {
    if (setting().onPracticeFix) {
        if (from<gd::PlayerObject*>(self, 0x2a4) != nullptr) {
            checkpoints.push_back({
                CheckPoint::from(self)
                });
            objBlending.push_back({
                ObjectBlending::from(self)
                });
        }
    }
    
    PlayLayer::createCheckpoint(self);
}

void __fastcall PlayLayer::removeLastCheckpointH(gd::PlayLayer* self) {
    if (setting().onPracticeFix) {
        if (checkpoints.size() > 0) {
            checkpoints.pop_back();
            objBlending.pop_back();
        }
    }
    /*g_orbCheckpoints.pop_back();*/
    PlayLayer::removeLastCheckpoint(self);
}

void __fastcall PlayLayer::update_H(gd::PlayLayer* self, void*, float dt) {
    layers().PauseLayerObject = nullptr;
    isPlayerDead = false;

    if (0 < smoothOut) {
        smoothOut--;
        dt = 0.01666667;
    }

    PlayLayer::update(self, dt);

    if (!isPlayerDead) wasDead = false;

    auto size = CCDirector::sharedDirector()->getWinSize();

    const auto value = self->player1()->getPositionX() / self->levelLength() * 100.f;
    auto percentLabel = static_cast<CCLabelBMFont*>(self->getChildByTag(4571));

    if (percentLabel) {
        if (value < 100.0f) switch (setting().accuratePercentage)
        {
        case 4:
            percentLabel->setString(CCString::createWithFormat("%.0f%%", value)->getCString());
            break;
        case 3:
            percentLabel->setString(CCString::createWithFormat("%.1f%%", value)->getCString());
            break;
        case 2:
            percentLabel->setString(CCString::createWithFormat("%.2f%%", value)->getCString());
            break;
        case 1:
            percentLabel->setString(CCString::createWithFormat("%.3f%%", value)->getCString());
            break;
        case 0:
            percentLabel->setString(CCString::createWithFormat("%.4f%%", value)->getCString());
            break;
        }
        else switch (setting().accuratePercentage)
        {
        case 4:
            percentLabel->setString(CCString::create("100%")->getCString());
            break;
        case 3:
            percentLabel->setString(CCString::create("100.0%")->getCString());
            break;
        case 2:
            percentLabel->setString(CCString::create("100.00%")->getCString());
            break;
        case 1:
            percentLabel->setString(CCString::create("100.000%")->getCString());
            break;
        case 0:
            percentLabel->setString(CCString::create("100.0000%")->getCString());
            break;
        } 
    }

    if ((setting().onAutoDeafen && !pressed) && (setting().deafenPercent > startPercent) && (value > setting().deafenPercent) && !self->isDead() && !self->hasCompletedLevel()) {
        pressed = true;
        keybd_event(VK_MENU, 0x38, 0, 0);
        keybd_event(setting().deafenKey, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event(setting().deafenKey, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
    }

    self->m_attemptsLabel->setVisible(!setting().onHideAttempts);

    auto spswitcherlbl = static_cast<CCLabelBMFont*>(self->getChildByTag(45712));
    float fps = ImGui::GetIO().Framerate;

    auto secarr = self->getSections();
    auto arrcount = secarr->count();

    auto labelsMenu = static_cast<CCMenu*>(self->getChildByTag(7900));
    auto FPSLabel = static_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45076));
    auto CPSLabel = static_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45081));
    auto BestRunLabel = static_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45080));
    auto SessionTimeLabel = static_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45082));
    auto AttemptsLabel = static_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45073));
    auto Clock = static_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45075));
    auto MessageLabel = static_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45077));
    auto noclDeath = static_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45078));
    auto noclipAccLabel = static_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45079));
    auto xPos = static_cast<CCLabelBMFont*>(self->getChildByTag(69060));
    auto yPos = static_cast<CCLabelBMFont*>(self->getChildByTag(69061));
    auto vel = static_cast<CCLabelBMFont*>(self->getChildByTag(69062));
    auto rot = static_cast<CCLabelBMFont*>(self->getChildByTag(69063));
    auto psize = static_cast<CCLabelBMFont*>(self->getChildByTag(69064));
    auto clickindi = static_cast<CCSprite*>(self->getChildByTag(141207));
    //auto speed = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69065));

    if (setting().onAutoSafe && setting().cheatsCount > 0) safeModeON(), setting().isSafeMode = true;
    else if (!setting().onSafeMode) safeModeOFF(), setting().isSafeMode = false;

    if (labelsMenu) {
        labelsMenu->setVisible(!setting().onHideLabels);
        labelsMenu->setScale(setting().labelsScale);
    }

    if (FPSLabel)
    {
        FPSLabel->setString(CCString::createWithFormat("%.f FPS", roundf(fps))->getCString());
        FPSLabel->setOpacity(setting().labelsOpacity);
    }

    if (CPSLabel) {
        std::string cpsCurrent = CCString::createWithFormat("%i", m_clickFrames.size())->getCString();
        std::string cpsTotal = CCString::createWithFormat("/%i", m_totalClicks)->getCString();
        std::string prefix = " CPS";
        CPSLabel->setString(std::string(cpsCurrent + cpsTotal + prefix).c_str());
        if (m_isHolding) {
            CPSLabel->setColor({ 0, 255, 0 });
        }
        else {
            CPSLabel->setColor({ 255, 255, 255 });
        }
        CPSLabel->setOpacity(setting().labelsOpacity);
    }

    if (SessionTimeLabel) {
        SessionTimeLabel->setString(CCString::createWithFormat("%.01fs", self->getClkTimer())->getCString());
        SessionTimeLabel->setOpacity(setting().labelsOpacity);
    }

    if (BestRunLabel) {
        if (!self->m_practiceMode && !self->m_testMode) {
            int newBest = self->getLastRunPercent();
            if (newBest >= currentBest) {
                currentBest = newBest;
                BestRunLabel->setString(CCString::createWithFormat("Best Run: %i%%", newBest)->getCString());
            }
            if (self->hasCompletedLevel()) {
                BestRunLabel->setString(CCString::createWithFormat("Best Run: 100%%")->getCString());
            }
        }
        BestRunLabel->setOpacity(setting().labelsOpacity);
    }

    if (AttemptsLabel)
    {
        AttemptsLabel->setString(CCString::createWithFormat("Attempt %d", self->attemptsCount())->getCString());
        AttemptsLabel->setOpacity(setting().labelsOpacity);
    }

    if (Clock)
    {
        t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream s;
        s << std::put_time(&tm, "%H:%M:%S");
        Clock->setString(CCString::create(s.str().c_str())->getCString());
        Clock->setOpacity(setting().labelsOpacity);
    }

    NoclipAcc = ((self->player1()->getPositionX() - deathDifference) / self->player1()->getPositionX()) * 100;
    if (noclipAccLabel)
    {
        if (value < 100.0f) static_cast<CCLabelBMFont*>(noclipAccLabel)->setString(CCString::createWithFormat("%.2f%%", NoclipAcc)->getCString());
        noclipAccLabel->setOpacity(setting().labelsOpacity);
    }

    if (noclDeath)
    {
        noclDeath->setString(CCString::createWithFormat("%d Deaths", deaths)->getCString());
        noclDeath->setOpacity(setting().labelsOpacity);
    }

    if (MessageLabel)
    {
        MessageLabel->setString(CCString::create(setting().message)->getCString());
        MessageLabel->setOpacity(setting().labelsOpacity);
    }

    if (xPos) {
        xPos->setString(CCString::createWithFormat("X: %.06f%", self->player1()->getPositionX())->getCString());
        if (setting().onHideLabels) {
            xPos->setVisible(0);
        }
        else {
            xPos->setVisible(1);
        }
    }

    if (yPos) {
        yPos->setString(CCString::createWithFormat("Y: %.06f%", self->player1()->getPositionY())->getCString());
        if (setting().onHideLabels) {
            yPos->setVisible(0);
        }
        else {
            yPos->setVisible(1);
        }
    }

    if (vel) {
        auto yVelocity = from<double>(self->player1(), 0x458);
        vel->setString(CCString::createWithFormat("Vel: %.06f%", yVelocity)->getCString());
        if (setting().onHideLabels) {
            vel->setVisible(0);
        }
        else {
            vel->setVisible(1);
        }
    }

    if (rot) {
        rot->setString(CCString::createWithFormat("Rot: %.06f%", self->player1()->getRealRotation())->getCString());
        if (setting().onHideLabels) {
            rot->setVisible(0);
        }
        else {
            rot->setVisible(1);
        }
    }

    if (psize) {
        psize->setString(CCString::createWithFormat("Size: %.06f%", self->player1()->getPlayerSize())->getCString());
        if (setting().onHideLabels) {
            psize->setVisible(0);
        }
        else {
            psize->setVisible(1);
        }
    }

    bool shouldKill = false;
    shouldKill |= value >= setting().killPercent;
    if (setting().onAutoKill) {
        if (shouldKill) {
            if (self->player1() && !self->player1()->getIsDead())
                self->destroyPlayer(self->player1());
        }
    }

    if (isPlayerColorGot == false)
    {
        playerColor1 = self->player1()->getFirstColor();
        playerColor2 = self->player1()->getSecondColor();
        playerColorG = self->player1()->getGlowColor();
        isPlayerColorGot = true;
    }

    if (setting().onSameDualColor)
    {
        self->player2()->setColor(self->player1()->getColor());
        self->player2()->setSecondColor(self->player1()->getSecondColor());
        self->player2()->setGlowColor(self->player1()->getGlowColor());
        self->player2()->setColorSecondVehicle(self->player1()->getColorSecondVehicle());
        self->player2()->setColorGlowVehicle(self->player1()->getColorGlowVehicle());
        self->player2()->setWaveTrailColor(self->player1()->getWaveTrailColor());
        self->player2()->motionStreak()->setColor(self->player1()->getSecondColor());
    }
    else
    {
        self->player2()->setColor(self->player1()->getSecondColor());
        self->player2()->setSecondColor(self->player1()->getColor());
        self->player2()->setGlowColor(self->player1()->getGlowColor());
        self->player2()->setColorSecondVehicle(self->player1()->getColor());
        self->player2()->setColorGlowVehicle(self->player1()->getGlowColor());
        self->player2()->setWaveTrailColor(self->player1()->getSecondColor());
        self->player2()->motionStreak()->setColor(self->player1()->getFirstColor());
    }

    if (setting().onLockCursor && !setting().show && !self->m_showingEndLayer && !self->isDead()) {
        SetCursorPos(size.width / 2, size.height / 2);
    }

    time_t currentTick = time::getTime();
    m_clickFrames.erase(std::remove_if(m_clickFrames.begin(), m_clickFrames.end(), [currentTick](float tick) {
        return currentTick - tick > 1000;
        }), m_clickFrames.end());

    hasClicked = false;

    if (setting().onRainbowIcon) {
        auto player1 = self->player1();
        auto player2 = self->player2();

        auto playerGlowNode = self->playerGlowNode();

        updateRgb += dt * setting().rgbSpeed;

        for (size_t i = 0; i < player1->getChildrenCount(); i++) {
            auto node = static_cast<CCSprite*>(player1->getChildren()->objectAtIndex(i));
            node->setColor(getChromaColour());
        }

        for (size_t i = 0; i < player2->getChildrenCount(); i++) {
            auto node = static_cast<CCSprite*>(player2->getChildren()->objectAtIndex(i));
            for (size_t i = 0; i < node->getChildrenCount(); i++) {
                auto node2 = static_cast<CCSprite*>(node->getChildren()->objectAtIndex(i));
                node2->setColor(getChromaColour());
            }
        }

        if (auto player2Glow = static_cast<CCSprite*>(playerGlowNode->getChildren()->objectAtIndex(playerGlowNode->getChildrenCount()))) {
            for (size_t i = 0; i < player2Glow->getChildrenCount(); i++) {
                auto node = static_cast<CCSprite*>(player2Glow->getChildren()->objectAtIndex(i));
                node->setColor(getChromaColour());
            }
        };
    }

    if (gd::GameManager::sharedState()->getGameVariable("0024")) {
        from<gd::CCMenuItemSpriteExtra*>(self->m_uiLayer, 0x1a0)->setVisible(!setting().onHidePauseButton);
    }

    self->m_player->m_hardStreak->setVisible(!setting().onNoWaveTrail);
    self->m_player2->m_hardStreak->setVisible(!setting().onNoWaveTrail);

    if (setting().onShowLayout) updateShowLayout(self);

    updateShowHitboxes(self);
}

void __fastcall PlayLayer::spawnPlayer2_H(gd::PlayLayer* self) {
    self->player2()->setVisible(1);
    std::cout << "Player 2 Spawned" << std::endl;

    PlayLayer::spawnPlayer2(self);
}

void __fastcall PlayLayer::removePlayer2H(gd::PlayLayer* self) {
    std::cout << "Player 2 Removed" << std::endl;

    PlayLayer::removePlayer2(self);
}

bool __fastcall PlayLayer::resume_H(CCLayer* self) {
    layers().PauseLayerObject = nullptr;
    bool result = PlayLayer::resume(self);
    return result;
}

void __fastcall PlayLayer::onQuit_H(gd::PlayLayer* self, void* edx) {
    layers().PauseLayerObject = nullptr;
    auto playerDrawNode = static_cast<CCDrawNode*>(self->layer()->getChildByTag(124));
    auto objDrawNode = static_cast<CCDrawNode*>(self->layer()->getChildByTag(125));

    playerDrawNode->clear();
    objDrawNode->clear();

    if (setting().onPracticeFix) {
        checkpoints.clear();
        objBlending.clear();
    }
    
    playLayer = nullptr;
    PlayLayer::onQuit(self);
}

void __fastcall PlayLayer::pushButton_H(gd::PlayLayer* self, void* edx, int idk1, bool idk2) {
    m_isHolding = true;
    if (!hasClicked) {
        m_clickFrames.push_back(time::getTime());
        m_totalClicks++;
        hasClicked = true;
    }
    //std::cout << self->player1()->getRealPlayerPosX() << " 1" << std::endl;
    PlayLayer::pushButton(self, idk1, idk2);
}

void __fastcall PlayLayer::releaseButton_H(gd::PlayLayer* self, void* edx, int idk1, bool idk2) {
    m_isHolding = false;
    //std::cout << self->player1()->getRealPlayerPosX() << " 0" << std::endl;
    PlayLayer::releaseButton(self, idk1, idk2);
}

void __fastcall PlayLayer::applyEnterEffect_H(gd::PlayLayer* self, void* edx, gd::GameObject* obj) {
    PlayLayer::applyEnterEffect(self, obj);
}

class ExitAlertProtocol : public gd::FLAlertLayerProtocol {
protected:
	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
	{
		if (btn2)
		{
			gd::GameManager::sharedState()->getPlayLayer()->onQuit();
			gd::GameSoundManager::sharedState()->playSound("quitSound_01.ogg");
			playLayer = nullptr;
		}
	}
};

ExitAlertProtocol eaProtocol;

void PauseLayer::Callback::PercentageToggler(CCObject*) {
    setting().onShowPercentage = !setting().onShowPercentage;
    const auto bar = gd::GameManager::sharedState()->getShowProgressBar();
    auto size = CCDirector::sharedDirector()->getWinSize();
    if (gd::GameManager::sharedState()->getPlayLayer()) {
        auto percentLabel = static_cast<CCLabelBMFont*>(gd::GameManager::sharedState()->getPlayLayer()->getChildByTag(4571));
        if (percentLabel) {
            percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
            percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });
            percentLabel->setVisible(setting().onShowPercentage);
        }
    }
}

auto percentageTogglerSprite(CCSprite* toggleOn, CCSprite* toggleOff) {
    return (setting().onShowPercentage) ? toggleOn : toggleOff;
}

void __fastcall PauseLayer::customSetupH(gd::PauseLayer* self) {
    pauseLayerTogglerMenu = nullptr;
    PauseLayer::customSetup(self);
    layers().PauseLayerObject = self;
    if (setting().onHidePauseMenu)
        self->setVisible(false);
    else self->setVisible(true);

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    auto menu = CCMenu::create();

    CCLabelBMFont* barText = nullptr;
    if (self->getChildren()->count() > 20) {
        barText = reinterpret_cast<CCLabelBMFont*>(self->getChildren()->objectAtIndex(14));
    }
    else {
        barText = reinterpret_cast<CCLabelBMFont*>(self->getChildren()->objectAtIndex(11));
    }
    barText->setString("Bar");
    barText->setScale(0.42f);

    auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

    auto PercentageButton = gd::CCMenuItemToggler::create(percentageTogglerSprite(toggleOn, toggleOff), percentageTogglerSprite(toggleOff, toggleOn), self, menu_selector(PauseLayer::Callback::PercentageToggler));
    auto PercentageLabel = CCLabelBMFont::create("%", "bigFont.fnt");
    PercentageButton->setScale(0.6f);
    PercentageButton->setPosition({ pauseLayerTogglerMenu->convertToNodeSpace({(winSize.width / 2) + 155.f, +25}) });
    PercentageLabel->setScale(0.4f);
    PercentageLabel->setPosition({ (winSize.width / 2) + 170.5f, +25 });
    PercentageLabel->setAnchorPoint({ 0.f, 0.5f });
    pauseLayerTogglerMenu->addChild(PercentageButton);
    self->addChild(PercentageLabel);

    menu->setPosition({ director->getScreenRight(), director->getScreenTop() });

    auto optionsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    auto optionsBtn = gd::CCMenuItemSpriteExtra::create(optionsSpr, nullptr, self, menu_selector(gd::MenuLayer::onOptions));
    optionsBtn->setPosition({ -42, -42 });
    optionsSpr->setScale(.66f);
    menu->addChild(optionsBtn);
    self->addChild(menu);

    if (pressed) {
        pressed = false;
        keybd_event(VK_MENU, 0x38, 0, 0);
        keybd_event(setting().deafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event(setting().deafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
    }
}

void __fastcall PauseLayer::togglerMenuH() {
    __asm {
        mov pauseLayerTogglerMenu, eax
    }
    PauseLayer::togglerMenu();
}

void __fastcall PauseLayer::onProgressBarH(gd::PauseLayer* self, void*, CCObject* obj) {
    const auto bar = gd::GameManager::sharedState()->getShowProgressBar();
    auto size = CCDirector::sharedDirector()->getWinSize();
    if (gd::GameManager::sharedState()->getPlayLayer()) {
        auto percentLabel = static_cast<CCLabelBMFont*>(gd::GameManager::sharedState()->getPlayLayer()->getChildByTag(4571));
        if (percentLabel) {
            percentLabel->setAnchorPoint({ bar ? 0.5f : 0.f, 0.5f });
            percentLabel->setPosition({ size.width / 2.f + (bar ? 0.f : 107.2f), size.height - 8.f });
            percentLabel->setVisible(setting().onShowPercentage);
        }
    }
    PauseLayer::onProgressBar(self, obj);
}

void __fastcall PauseLayer::onEdit_H(CCLayer* self)
{
    layers().PauseLayerObject = nullptr;
    playLayer = nullptr;
    PauseLayer::onEdit(self);
}

void __fastcall PauseLayer::onRestart_H(CCObject* sender) {
    if (!gd::GameManager::sharedState()->getGameVariable("0024"))
        CCEGLView::sharedOpenGLView()->showCursor(false);
    layers().PauseLayerObject = nullptr;
    PauseLayer::onRestart(sender);
}

void __fastcall PauseLayer::onQuit_H(CCObject* btn)
{
    playLayer = nullptr;
    if (setting().onConfirmExit) {
        gd::FLAlertLayer::create(&eaProtocol, "Confirm", "Are you sure you want to <cr>exit</c> the level?", "Cancel", "Exit", 320.f, false, 0)->show();
    }
    PauseLayer::onQuit(btn);
}

void __fastcall PlayLayer::levelCompleteH(gd::PlayLayer* self) {
    if (setting().onPracticeFix) {
        checkpoints.clear();
        objBlending.clear();
        inPractice = false;
    }
    from<bool>(self, 0x2f9) = true;
    if (setting().onZeroPracticeComplete) {
        if (fromZero && self->getPracticeMode()) from<bool>(self, 0x2b9) = false;
    }

    PlayLayer::levelComplete(self);

    if (pressed) {
        pressed = false;
        keybd_event(VK_MENU, 0x38, 0, 0);
        keybd_event(setting().deafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event(setting().deafenKey, 0x50, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
    }
}

void __fastcall PlayLayer::updateVisibilityH(gd::PlayLayer* self) {
    PlayLayer::updateVisibility(self);
    if (setting().onNoWavePulse) {
        self->m_player->m_audioScale = 1.f;
        self->m_player2->m_audioScale = 1.f;
    }
}

void EndLevelLayer::Callback::onLastCheckpoint(CCObject* sender) {
    if (auto pl = gd::GameManager::sharedState()->getPlayLayer()) {
        reinterpret_cast<gd::EndLevelLayer*>(reinterpret_cast<CCNode*>(sender))->getParent()->getParent()->getParent()->removeFromParent();
        pl->resetLevel();
        CCEGLView::sharedOpenGLView()->showCursor(gd::GameManager::sharedState()->getGameVariable("0024"));
    }
}

void EndLevelLayer::Callback::onHideLayer(CCObject* obj) {
    if (auto pl = gd::GameManager::sharedState()->getPlayLayer()) {
        auto showEndLayerButton = static_cast<gd::CCMenuItemSpriteExtra*>(pl->getChildByTag(18257)->getChildByTag(5765));
        if (showEndLayerButton) {
            this->exitLayer(nullptr);
            showEndLayerButton->setEnabled(true);
            showEndLayerButton->runAction(CCEaseSineInOut::create(CCMoveTo::create(0.5f, { 0, showEndLayerButton->getPositionY() - 35.f })));
            showEndLayerButton->runAction(CCSequence::create(CCDelayTime::create(0.5f), CCFadeTo::create(0.5f, 50), nullptr));
        }
    }
}

void EndLevelLayer::Callback::onShowLayer(CCObject* obj) {
    if (auto pl = gd::GameManager::sharedState()->getPlayLayer()) {
        pl->showEndLayer();
        auto showEndLayerButton = static_cast<gd::CCMenuItemSpriteExtra*>(pl->getChildByTag(18257)->getChildByTag(5765));
        if (showEndLayerButton) {
            showEndLayerButton->setEnabled(false);
            showEndLayerButton->runAction(CCEaseSineInOut::create(CCMoveTo::create(0.5f, { 0, showEndLayerButton->getPositionY() + 35.f })));
            showEndLayerButton->runAction(CCSequence::create(CCDelayTime::create(0.5f), CCFadeTo::create(0.5f, 255), nullptr));
        }
    }
}

void __fastcall EndLevelLayer::customSetup_H(gd::EndLevelLayer* self) {
    EndLevelLayer::customSetup(self);

    auto layerMain = reinterpret_cast<CCLayer*>(self->getChildren()->objectAtIndex(0));
    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();

    if (setting().isSafeMode || setting().onSafeMode) {
        auto safeModeLabel = CCLabelBMFont::create("Safe Mode", "bigFont.fnt");
        safeModeLabel->setPosition({ size.width / 2, director->getScreenTop() - 85 });
        safeModeLabel->setScale(1.25f);
        layerMain->addChild(safeModeLabel);
        auto levelCompleteSpr = (CCSprite*)layerMain->getChildren()->objectAtIndex(4);
        levelCompleteSpr->setVisible(0);
    }

    auto menu = CCMenu::create();
    menu->setZOrder(5);
    auto arrowSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    arrowSprite->setRotation(90.f);
    arrowSprite->setScale(0.6f);
    auto arrowButton = gd::CCMenuItemSpriteExtra::create(arrowSprite, nullptr, self, menu_selector(EndLevelLayer::Callback::onHideLayer));
    arrowButton->setPositionY(128.f);
    menu->addChild(arrowButton);
    layerMain->addChild(menu);

    auto practiceButton = gd::CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_practiceBtn_001.png"),
        nullptr, self,
        menu_selector(EndLevelLayer::Callback::onLastCheckpoint));
    practiceButton->setPositionY(-125.f);

    auto bottomMenu = from<CCMenu*>(self, 0x1c0);

    if (setting().onLastCheckpoint && bottomMenu && gd::GameManager::sharedState()->getPlayLayer()->m_practiceMode) {
        if (bottomMenu->getChildrenCount() == 2) {
            static_cast<gd::CCMenuItemSpriteExtra*>(bottomMenu->getChildren()->objectAtIndex(0))->setPositionX(-100.f);
            static_cast<gd::CCMenuItemSpriteExtra*>(bottomMenu->getChildren()->objectAtIndex(1))->setPositionX(100.f);
        }
        else {
            static_cast<gd::CCMenuItemSpriteExtra*>(bottomMenu->getChildren()->objectAtIndex(0))->setPositionX(-130.f);
            static_cast<gd::CCMenuItemSpriteExtra*>(bottomMenu->getChildren()->objectAtIndex(1))->setPositionX(130.f);
            static_cast<gd::CCMenuItemSpriteExtra*>(bottomMenu->getChildren()->objectAtIndex(2))->setPositionX(-45.f);
            practiceButton->setPositionX(45.f);
        }
        bottomMenu->addChild(practiceButton);
    }
}

void __fastcall PlayerObject::updatePlayerFrame_H(gd::PlayerObject* self, void* edx, int frameID) {
    if (setting().onNoMiniIcon) {
        return PlayerObject::updatePlayerFrame(self, gd::GameManager::sharedState()->getPlayerFrame());
    }
    PlayerObject::updatePlayerFrame(self, frameID);
}

void __fastcall PlayerObject::updatePlayerRollFrame_H(gd::PlayerObject* self, void* edx, int frameID) {
    if (setting().onNoMiniIcon) {
        return PlayerObject::updatePlayerRollFrame(self, gd::GameManager::sharedState()->getPlayerBall());
    }
    PlayerObject::updatePlayerRollFrame(self, frameID);
}

void __fastcall CCCircleWave::drawH(gd::CCCircleWave* self) {
    if (!setting().onNoEffectCircle) CCCircleWave::draw(self);
}

void __fastcall GameObject::getEditorColorH(gd::GameObject* self, void* edx, cocos2d::ccColor4B color) {
    int* a = GameObject::getEditorColor(self, color);
    switch (self->getActiveColor())
    {
    case 0:
        *a = 0x0AFFFFFF;
        break;
    case 1:
        *a = 0x0AFF96AF;
        break;
    case 2:
        *a = 0x0A9696FF;
        break;
    case 3:
        *a = 0x0AFF96FF;
        break;
    case 4:
        *a = 0x0A96FFFF;
        break;
    case 5:
        *a = 0x0AFFAF4B;
        break;
    case 6:
        *a = 0x0AFFFF96;
        break;
    case 7:
        *a = 0x0A96FF96;
        break;
    case 8:
        *a = 0x0A00FFFF;
        break;
    default:
        *a = 0x0AFFFFFF;
        break;
    }
    if (self->isInvisible()) *a = 0x0A007FFF;
}

//bool m_realVisible;
//_ccColor3B m_realColor;
//
//void __fastcall GameObject::setVisibleH(gd::GameObject* self, void*, bool visible) {
//    if (setting().onShowLayout) {
//        m_realVisible = visible;
//
//        if (self->m_objectType == gd::GameObjectType::kGameObjectTypeDecoration) {
//            GameObject::setVisible(self, false);
//        }
//        else {
//            GameObject::setVisible(self, visible);
//            GameObject::setVisible(self, true);
//        }
//    }
//    else GameObject::setVisible(self, visible);
//}
//
//void __fastcall GameObject::setOpacityH(gd::GameObject* self, void*, GLubyte opacity) {
//    GameObject::setOpacity(self, opacity);
//
//    //if (setting().onShowLayout && self->m_objectType != gd::GameObjectType::kGameObjectTypeDecoration) {
//    //    self->setOpacity(255);
//    //    if (self->m_glowSprite)
//    //        self->m_glowSprite->setOpacity(255);
//    //}
//}
//
//void __fastcall GameObject::setGlowColorH(gd::GameObject* self, void*, _ccColor3B const& color) {
//    if (setting().onShowLayout)
//        GameObject::setGlowColor(self, ccc3(255, 255, 255));
//    else
//        GameObject::setGlowColor(self, color);
//}
//
//void __fastcall GameObject::setObjectColorH(gd::GameObject* self, void*, _ccColor3B const& color) {
//    if (setting().onShowLayout) {
//        GameObject::setObjectColor(self, ccc3(255, 255, 255));
//        if (self->m_glowSprite)
//            self->m_glowSprite->setColor(ccc3(255, 255, 255));
//    }
//    else {
//        GameObject::setObjectColor(self, color);
//        if (self->m_glowSprite)
//            self->m_glowSprite->setColor(color);
//    }
//}

void __fastcall PlayerObject::runBallRotation2H(gd::PlayerObject* self) {
    if (setting().onBallRotationFix) {
        self->stopActionByTag(1);
    }
    PlayerObject::runBallRotation2(self);
}

void __fastcall PlayerObject::togglePlayerScaleH(gd::PlayerObject* self, void*, bool p0) {
    PlayerObject::togglePlayerScale(self, p0);
    if (gd::GameManager::sharedState()->getPlayerGlow() == 2)
        if (self->m_playerScale == 1.f)
            from<float>(self->m_playerStreak, 0x114) = 14.f;
}

void __fastcall PlayerObject::toggleDartModeH(gd::PlayerObject* self, void*, bool p0) {
    PlayerObject::toggleDartMode(self, p0);
    if (setting().onNoWaveTrailBehind) from<bool>(self->m_playerStreak, 0xfe) = false;
}

void PauseLayer::mem_init() {
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd5f50),
        PauseLayer::customSetupH,
        reinterpret_cast<void**>(&PauseLayer::customSetup));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd7bf0),
        PauseLayer::onEdit_H,
        reinterpret_cast<void**>(&PauseLayer::onEdit));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd7b20),
        PauseLayer::onRestart_H,
        reinterpret_cast<void**>(&PauseLayer::onRestart));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd7f00),
        PauseLayer::onQuit_H,
        reinterpret_cast<void**>(&PauseLayer::onQuit));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd66df), PauseLayer::togglerMenuH, reinterpret_cast<void**>(&PauseLayer::togglerMenu));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd8010), PauseLayer::onProgressBarH, reinterpret_cast<void**>(&PauseLayer::onProgressBar));
}

void PlayLayer::mem_init() {
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xe35d0),
        PlayLayer::init_H,
        reinterpret_cast<void**>(&PlayLayer::init));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xe9360),
        PlayLayer::update_H,
        reinterpret_cast<void**>(&PlayLayer::update));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf39b0),
        PlayLayer::resume_H,
        reinterpret_cast<void**>(&PlayLayer::resume));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf3b80),
        PlayLayer::onQuit_H,
        reinterpret_cast<void**>(&PlayLayer::onQuit));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf1f20),
        PlayLayer::resetLevel_H,
        reinterpret_cast<void**>(&PlayLayer::resetLevel));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf04a0),
        PlayLayer::destroyPlayer_H,
        reinterpret_cast<void**>(&PlayLayer::destroyPlayer));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf0a00),
        PlayLayer::pushButton_H,
        reinterpret_cast<void**>(&PlayLayer::pushButton));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf0af0),
        PlayLayer::releaseButton_H,
        reinterpret_cast<void**>(&PlayLayer::releaseButton));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf3610), PlayLayer::togglePracticeModeH, reinterpret_cast<void**>(&PlayLayer::togglePracticeMode));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf1010), PlayLayer::createCheckpointH, reinterpret_cast<void**>(&PlayLayer::createCheckpoint));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf1d70), PlayLayer::removeLastCheckpointH, reinterpret_cast<void**>(&PlayLayer::removeLastCheckpoint));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe52e0), PlayLayer::levelCompleteH, reinterpret_cast<void**>(&PlayLayer::levelComplete));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xef0d0), PlayLayer::spawnPlayer2_H, reinterpret_cast<void**>(&PlayLayer::spawnPlayer2));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xeb3f0), PlayLayer::updateVisibilityH, reinterpret_cast<void**>(&PlayLayer::updateVisibility));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xef120), PlayLayer::removePlayer2H, reinterpret_cast<void**>(&PlayLayer::removePlayer2));
}

void EndLevelLayer::mem_init() {
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x50430),
        EndLevelLayer::customSetup_H,
        reinterpret_cast<void**>(&EndLevelLayer::customSetup));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x50380), EndLevelLayer::dtorH, reinterpret_cast<void**>(&EndLevelLayer::dtor));
}

void PlayerObject::mem_init() {
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdfff0), PlayerObject::updatePlayerFrame_H, reinterpret_cast<void**>(&PlayerObject::updatePlayerFrame));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0430), PlayerObject::updatePlayerRollFrame_H, reinterpret_cast<void**>(&PlayerObject::updatePlayerRollFrame));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdad10), PlayerObject::runBallRotation2H, reinterpret_cast<void**>(&PlayerObject::runBallRotation2));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe12e0), PlayerObject::togglePlayerScaleH, reinterpret_cast<void**>(&PlayerObject::togglePlayerScale));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdee80), PlayerObject::toggleDartModeH, reinterpret_cast<void**>(&PlayerObject::toggleDartMode));
}

void GameObject::mem_init() {
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x756b0), GameObject::getEditorColorH, reinterpret_cast<void**>(&GameObject::getEditorColor));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x72d20), GameObject::setVisibleH, reinterpret_cast<void**>(&GameObject::setVisible));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x72a60), GameObject::setOpacityH, reinterpret_cast<void**>(&GameObject::setOpacity));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x72a60), GameObject::setGlowColorH, reinterpret_cast<void**>(&GameObject::setGlowColor));
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x75560), GameObject::setObjectColorH, reinterpret_cast<void**>(&GameObject::setObjectColor));
}

void CCCircleWave::mem_init() {
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xb4b0), CCCircleWave::drawH, reinterpret_cast<void**>(&CCCircleWave::draw));
}