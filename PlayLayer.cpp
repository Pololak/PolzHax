#include "pch.h"
#include "state.h"
#include "PlayLayer.h"
//#include "Scheduler.h"
//#include "Labels.h"
#include "utils.hpp"
#include "CheckPoint.h"

CCArray* startPosArr;
CCLayer* playLayer;
ccColor3B playerColor1;
ccColor3B playerColor2;
ccColor3B playerColorG;

std::time_t t;
SYSTEMTIME st;

bool hasClicked = false;
//Labels* labels = nullptr;
bool isPlayerColorGot = false;


int currentStartPos = 0;
bool fadedoutflag = 0;

std::vector<bool> willFlip;
std::vector<gd::StartPosObject*> sp;
std::vector<gd::GameObject*> gravityPortals, dualPortals, gamemodePortals, miniPortals, speedChanges, mirrorPortals;

std::vector<CheckPoint> PlayLayer::checkpoints;
bool PlayLayer::inPractice = false;

bool __fastcall PlayLayer::init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level) {
    
    

    setting().beforeRestartCheatsCount = setting().cheatsCount;
    layers().PauseLayerObject = self;
    playLayer = self;
    isPlayerColorGot = false;

    if (!PlayLayer::init(self, level)) return false;

    /*if (setting().onPracticeFix) {
        PlayLayer::checkpoints.clear();
        PlayLayer::inPractice = false;
    }*/

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
            if (obj->getObjectID() == 31) {
                startPosArr->addObject(obj);
            }
        }
    }
    currentStartPos = startPosArr->count() - 1;

    if (setting().onSPSwitcher)
    {
        auto spswitcher = CCLabelBMFont::create("", "bigFont.fnt");
        spswitcher->setZOrder(5);
        spswitcher->setScale(0.66f);
        spswitcher->setAnchorPoint({ 0.5f, 0.5f });
        spswitcher->setString(CCString::createWithFormat("%d/%d", currentStartPos, startPosArr->count() - 1)->getCString());
        spswitcher->setPosition({ CCDirector::sharedDirector()->getScreenRight() / 2, 15.f });
        spswitcher->setTag(45712);
        if (startPosArr->count() == 1) spswitcher->setVisible(0);
        self->addChild(spswitcher);
    }

    if (setting().onShowPercentage)
    {
        const auto bar = gd::GameManager::sharedState()->getShowProgressBar();

        auto percentLabel = CCLabelBMFont::create("0.00%", "bigFont.fnt");
        percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
        percentLabel->setScale(0.5f);
        percentLabel->setZOrder(5);
        percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });
        percentLabel->setTag(4571);
        self->addChild(percentLabel);
    }
}

bool rKeyFlag = true;
bool lKeyFlag = true;

void __fastcall PlayLayer::update_H(gd::PlayLayer* self, void*, float dt) {
    layers().PauseLayerObject = nullptr;

    //const auto bar = gd::GameManager::sharedState()->getShowProgressBar();

    auto size = CCDirector::sharedDirector()->getWinSize();
    auto audioScale = self->player1()->getAudioScale() > 1.f ? 1.f : self->player1()->getAudioScale();
    //auto percentLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(4571));
    auto spswitcherlbl = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45712));

    auto secarr = self->getSections();
    auto arrcount = secarr->count();
        

    if (spswitcherlbl)
    {
        auto fadeout = CCSequence::create(CCDelayTime::create(2.f), CCFadeOut::create(0.5f), nullptr);
        if (!fadedoutflag)
        {
            spswitcherlbl->runAction(fadeout);
            fadedoutflag = !fadedoutflag;
        }
        if (GetAsyncKeyState(0x45) && rKeyFlag && startPosArr->count() > 1)
        {
            rKeyFlag = false;
            if (currentStartPos == startPosArr->count() - 1) currentStartPos = 0;
            else currentStartPos++;
            if (currentStartPos != 0) {
                self->setPlayerStartPosition(reinterpret_cast<gd::StartPosObject*>(startPosArr->objectAtIndex(currentStartPos))->getOrientedBox()->getCenterPoint());
                self->setStartPosObject(reinterpret_cast<gd::StartPosObject*>(startPosArr->objectAtIndex(currentStartPos)));
            }
            else {
                self->setPlayerStartPosition({ 0, 105 });
                self->setStartPosObject(nullptr);
            }
            self->resetLevel();
            spswitcherlbl->setString(CCString::createWithFormat("%d/%d", currentStartPos, startPosArr->count() - 1)->getCString());
            spswitcherlbl->stopAllActions();
            spswitcherlbl->setOpacity(255);
            spswitcherlbl->runAction(fadeout);
        }
        else if (!GetAsyncKeyState(0x45)) rKeyFlag = true;

        if (GetAsyncKeyState(0x51) && lKeyFlag && startPosArr->count() > 1)
        {
            lKeyFlag = false;
            if (currentStartPos == 0) currentStartPos = startPosArr->count() - 1;
            else currentStartPos--;
            if (currentStartPos != 0) {
                self->setPlayerStartPosition(reinterpret_cast<gd::StartPosObject*>(startPosArr->objectAtIndex(currentStartPos))->getOrientedBox()->getCenterPoint());
                self->setStartPosObject(reinterpret_cast<gd::StartPosObject*>(startPosArr->objectAtIndex(currentStartPos)));
            }
            else {
                self->setPlayerStartPosition({ 0, 105 });
                self->setStartPosObject(nullptr);
            }
            self->resetLevel();
            spswitcherlbl->setString(CCString::createWithFormat("%d/%d", currentStartPos, startPosArr->count() - 1)->getCString());
            spswitcherlbl->stopAllActions();
            spswitcherlbl->setOpacity(255);
            spswitcherlbl->runAction(fadeout);
        }
        else if (!GetAsyncKeyState(0x51)) lKeyFlag = true;
    }

    /*if (percentLabel) {
        const auto value = self->player1()->getPositionX() / self->levelLength() * 100.f;

        percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
        percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });

        if (value < 100.0f) percentLabel->setString(CCString::createWithFormat("%.2f%%", value)->getCString());
        else percentLabel->setString(CCString::create("100.00%")->getCString());

    }*/

    if (setting().onHideAttempts) {
        self->attemptsLabel()->setVisible(false);
    }
    else {
        self->attemptsLabel()->setVisible(true);
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
    }
    else
    {
        self->player2()->setColor(self->player1()->getSecondColor());
        self->player2()->setSecondColor(self->player1()->getColor());
        self->player2()->setGlowColor(self->player1()->getGlowColor());
        self->player2()->setColorSecondVehicle(self->player1()->getColor());
        self->player2()->setColorGlowVehicle(self->player1()->getGlowColor());
        self->player2()->setWaveTrailColor(self->player1()->getSecondColor());
    }

    if (setting().onHidePlayer) {
        self->player1()->setVisible(0);
        self->player2()->setVisible(0);
    }
    else {
        self->player1()->setVisible(1);
        self->player2()->setVisible(1);
    }

    if (setting().onLockCursor) {
        SetCursorPos(size.width / 2, size.height / 2);
    }

    if (setting().onPrimaryPulse)
    {
        unsigned char
            RR = playerColor1.r + ((setting().PrimaryPulse[0] * 255 - playerColor1.r) * audioScale),
            RG = playerColor1.g + ((setting().PrimaryPulse[1] * 255 - playerColor1.g) * audioScale),
            RB = playerColor1.b + ((setting().PrimaryPulse[2] * 255 - playerColor1.b) * audioScale);
        self->player1()->setColor({ RR, RG, RB });
    }
    if (setting().onSecondaryPulse)
    {
        unsigned char
            RR = playerColor2.r + ((setting().SecondaryPulse[0] * 255 - playerColor2.r) * audioScale),
            RG = playerColor2.g + ((setting().SecondaryPulse[1] * 255 - playerColor2.g) * audioScale),
            RB = playerColor2.b + ((setting().SecondaryPulse[2] * 255 - playerColor2.b) * audioScale);
        self->player1()->setSecondColor({ RR, RG, RB });
        self->player1()->setColorSecondVehicle({ RR, RG, RB });
    }

    if (setting().onGlowPulse)
    {
        unsigned char
            RR = playerColorG.r + ((setting().GlowPulse[0] * 255 - playerColorG.r) * audioScale),
            RG = playerColorG.g + ((setting().GlowPulse[1] * 255 - playerColorG.g) * audioScale),
            RB = playerColorG.b + ((setting().GlowPulse[2] * 255 - playerColorG.b) * audioScale);
        self->player1()->setGlowColor({ RR, RG, RB });
        self->player1()->setColorGlowVehicle({ RR, RG, RB });
    }

    if (setting().onWaveTrailPulse)
    {
        unsigned char
            RR = playerColor1.r + ((setting().WaveTrailPulse[0] * 255 - playerColor1.r) * audioScale),
            RG = playerColor1.g + ((setting().WaveTrailPulse[1] * 255 - playerColor1.g) * audioScale),
            RB = playerColor1.b + ((setting().WaveTrailPulse[2] * 255 - playerColor1.b) * audioScale);
        self->player1()->setWaveTrailColor({ RR, RG, RB });
    }

    PlayLayer::update(self, dt);
}

void __fastcall PlayLayer::spawnPlayer2_H(gd::PlayLayer* self) {

    self->player2()->setVisible(1);

    PlayLayer::spawnPlayer2(self);
}

bool __fastcall PlayLayer::resume_H(CCLayer* self) {
    layers().PauseLayerObject = nullptr;
    bool result = PlayLayer::resume(self);
    return result;
}

void __fastcall PlayLayer::onQuit_H(CCNode* self) {
    layers().PauseLayerObject = nullptr;
    /*if (setting().onPracticeFix) {
        PlayLayer::checkpoints.clear();
    }*/
    playLayer = nullptr;
    PlayLayer::onQuit(self);
}

void __fastcall PlayLayer::resetLevel_H(gd::PlayLayer* self) {
    setting().beforeRestartCheatsCount = setting().cheatsCount;

    /*if (setting().onPracticeFix) {
        if (PlayLayer::inPractice && PlayLayer::checkpoints.size() > 0) {
            PlayLayer::checkpoints.pop_back();
        }
    }*/

    PlayLayer::resetLevel(self);
}

void __fastcall PlayLayer::pushButton_H(int idk1, bool idk2) {
    
    PlayLayer::pushButton(idk1, idk2);
}

void __fastcall PlayLayer::releaseButton_H(int idk1, bool idk2) {

    PlayLayer::releaseButton(idk1, idk2);
}

void __fastcall HardStreak::updateStroke_H(gd::HardStreak* self, float dt) {

    //self->m_pulseSize = 1;

    HardStreak::updateStroke(self, dt);
}

//void __fastcall PlayLayer::togglePracticeMode_H(gd::PlayLayer* self, int edx, bool practice) {
//    if (setting().onPracticeFix) {
//        PlayLayer::checkpoints.clear();
//        PlayLayer::inPractice = practice;
//    }
//
//    return PlayLayer::togglePracticeMode(self, practice);
//}
//
//int __fastcall PlayLayer::createCheckpoint_H(gd::PlayLayer* self) {
//    if (setting().onPracticeFix) {
//        if (*(void**)((char*)self + 0x2a4) != nullptr) {
//            PlayLayer::checkpoints.push_back({
//                CheckPoint::from(self)
//                });
//        }
//    }
//    
//    return PlayLayer::createCheckpoint(self);
//
//    //if (!PlayLayer::createCheckpoint) return false;
//}
//
//int __fastcall PlayLayer::removeLastCheckpoint_H(gd::PlayLayer* self) {
//    if (setting().onPracticeFix) {
//        if (PlayLayer::checkpoints.size() > 0) {
//            PlayLayer::checkpoints.pop_back();
//        }
//    }
//
//    return PlayLayer::removeLastCheckpoint(self);
//}
//
//bool __fastcall PlayLayer::levelComplete_H(gd::PlayLayer* self) {
//
//    if (setting().onPracticeFix) {
//        PlayLayer::checkpoints.clear();
//        PlayLayer::inPractice = false;
//    }
//    return PlayLayer::levelComplete(self);
//}

//DWORD cocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");

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
    /*MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf3610),
        PlayLayer::togglePracticeMode_H,
        reinterpret_cast<void**>(&PlayLayer::togglePracticeMode));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf1010),
        PlayLayer::createCheckpoint_H,
        reinterpret_cast<void**>(&PlayLayer::createCheckpoint));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf1d70),
        PlayLayer::removeLastCheckpoint_H,
        reinterpret_cast<void**>(&PlayLayer::removeLastCheckpoint));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf1f20),
        PlayLayer::resetLevel_H,
        reinterpret_cast<void**>(&PlayLayer::resetLevel));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xe52e0),
        PlayLayer::levelComplete_H,
        reinterpret_cast<void**>(&PlayLayer::levelComplete));*/

    /*MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf0a00),
        PlayLayer::pushButton_H,
        reinterpret_cast<void**>(&PlayLayer::pushButton));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf0af0),
        PlayLayer::releaseButton_H,
        reinterpret_cast<void**>(&PlayLayer::releaseButton));*/
}

void HardStreak::mem_init() {
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x833e0),
        HardStreak::updateStroke_H,
        reinterpret_cast<void**>(&HardStreak::updateStroke));
}
