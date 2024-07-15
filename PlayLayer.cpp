#include "pch.h"
#include "state.h"
#include "PlayLayer.h"

CCLayer* playLayer;
CCArray* startPosArr;

int currentStartPos = 0;
bool fadedoutflag = 0;

std::vector<bool> willFlip;
std::vector<gd::StartPosObject*> sp;
std::vector<gd::GameObject*> gravityPortals, dualPortals, gamemodePortals, miniPortals, speedChanges, mirrorPortals;



bool __fastcall PlayLayer::init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level) {
    layers().PauseLayerObject = self;
    playLayer = self;

    if (!PlayLayer::init(self, level)) return false;

    auto director = CCDirector::sharedDirector();
    auto size = CCDirector::sharedDirector()->getWinSize();

    auto secarr = self->getSections();
    auto objarr1 = self->getObjects();
    auto arrcount = secarr->count();



    /*if (startPosArr) delete startPosArr;
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
    }*/

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

    const auto bar = gd::GameManager::sharedState()->getShowProgressBar();

    auto size = CCDirector::sharedDirector()->getWinSize();

    auto percentLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(4571));
    auto spswitcherlbl = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45712));

    auto secarr = self->getSections();
    auto arrcount = secarr->count();
    /*if (spswitcherlbl)
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
    }*/

    if (percentLabel) {
        const auto value = self->player1()->getPositionX() / self->levelLength() * 100.f;

        percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
        percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });

        if (value < 100.0f) percentLabel->setString(CCString::createWithFormat("%.2f%%", value)->getCString());
        else percentLabel->setString(CCString::create("100.00%")->getCString());

    }

    if (setting().onHideAttempts) {
        self->attemptsLabel()->setVisible(false);
    }
    else {
        self->attemptsLabel()->setVisible(true);
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

    PlayLayer::update(self, dt);
}

bool __fastcall PlayLayer::resume_H(CCLayer* self) {
    layers().PauseLayerObject = nullptr;
    bool result = PlayLayer::resume(self);
    return result;
}

void __fastcall PlayLayer::onQuit_H(CCNode* self) {
    layers().PauseLayerObject = nullptr;
    playLayer = nullptr;
    PlayLayer::onQuit(self);
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
}
