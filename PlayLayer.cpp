#include "pch.h"
#include "state.h"
#include "PlayLayer.h"
//#include "Scheduler.h"
//#include "Labels.h"
#include "utils.hpp"
#include "CheckPoint.h"
#include "Hitboxes.h"
#include "imgui.h"

CCArray* startPosArr;
gd::PlayLayer* playLayer;
ccColor3B playerColor1;
ccColor3B playerColor2;
ccColor3B playerColorG;

std::time_t t;
SYSTEMTIME st;

bool hasClicked = false;
//Labels* labels = nullptr;
bool isPlayerColorGot = false;
bool isPlayerDead = false;

gd::HardStreak* hardStreak;

int currentStartPos = 0;
bool fadedoutflag = 0;

int deaths = 0;
float NoclipAcc = 0.f;
float deathDifference = 0.f;
bool wasDead = 0;

float deathPos = 0.f;

//std::vector<CheckPoint> PlayLayer::checkpoints;
//bool PlayLayer::inPractice = false;

bool __fastcall PlayLayer::init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level) {
    setting().beforeRestartCheatsCount = setting().cheatsCount;
    layers().PauseLayerObject = self;
    playLayer = self;
    isPlayerColorGot = false;
    fadedoutflag = 0;

    if (!PlayLayer::init(self, level)) return false;

    /*if (setting().onPracticeFix) {
        PlayLayer::checkpoints.clear();
        PlayLayer::inPractice = false;
    };*/

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

    //auto ayaya = CCSprite::create("image0.png");
    //self->addChild(ayaya);
    // If you found this, please, don't ask me what this image was.

    //if (setting().onMeta) {
    //    auto xPos = CCLabelBMFont::create("", "bigFong.fnt");
    //    xPos->setZOrder(5);
    //    xPos->setScale(0.4f);
    //    xPos->setAnchorPoint({ -0.5, 0 });
    //    xPos->setString(CCString::createWithFormat("X: %f")->getCString());
    //    xPos->setPosition({ director->getScreenLeft(), director->getScreenBottom() });
    //    xPos->setTag(69001);
    //    self->addChild(xPos);
    //}

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

    auto playerDrawNode = CCDrawNode::create();
    playerDrawNode->setZOrder(1000);
    playerDrawNode->setTag(124);
    self->layer()->addChild(playerDrawNode);

    auto objDrawNode = CCDrawNode::create();
    objDrawNode->setZOrder(1000);
    objDrawNode->setTag(125);
    self->layer()->addChild(objDrawNode);

    if (setting().onSolidsHitbox)
    {
        for (int i = self->getFirstVisibleSection() + 1; i < self->getLastVisibleSection() - 1; i++)
        {
            if (i < 0) continue;
            if (i >= arrcount) break;
            auto objAtInd = secarr->objectAtIndex(i);
            auto objarr = reinterpret_cast<CCArray*>(objAtInd);

            for (int j = 0; j < objarr->count(); j++)
            {
                auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
                Hitboxes::drawSolidsObjectHitbox(obj, objDrawNode);
            }
        }
    }
    
    if (setting().onHazardsHitbox)
    {
        for (int i = self->getFirstVisibleSection() + 1; i < self->getLastVisibleSection() - 1; i++)
        {
            if (i < 0) continue;
            if (i >= arrcount) break;
            auto objAtInd = secarr->objectAtIndex(i);
            auto objarr = reinterpret_cast<CCArray*>(objAtInd);

            for (int j = 0; j < objarr->count(); j++)
            {
                auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
                Hitboxes::drawHazardsObjectHitbox(obj, objDrawNode);
            }
        }
    }

    if (setting().onSpecialsHitbox)
    {
        for (int i = self->getFirstVisibleSection() + 1; i < self->getLastVisibleSection() - 1; i++)
        {
            if (i < 0) continue;
            if (i >= arrcount) break;
            auto objAtInd = secarr->objectAtIndex(i);
            auto objarr = reinterpret_cast<CCArray*>(objAtInd);

            for (int j = 0; j < objarr->count(); j++)
            {
                auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
                Hitboxes::drawSpecialsObjectHitbox(obj, objDrawNode);
            }
        }
    }

    if (setting().onPlayerHitbox) {
        if (self->player1()) Hitboxes::drawPlayerHitbox(self->player1(), playerDrawNode);
        if (self->player2()) Hitboxes::drawPlayerHitbox(self->player2(), playerDrawNode);
    }

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
        if (setting().cheatsCount == 0 && !setting().onSafeMode) CheatIndicatorLabel->setColor({ 0, 255, 0 });
        else if (setting().cheatsCount == 0 && setting().onSafeMode) CheatIndicatorLabel->setColor({ 255, 255, 0 });
        else CheatIndicatorLabel->setColor({ 255, 0, 0 });

        
        //CheatIndicatorLabel->
        self->addChild(CheatIndicatorLabel);


    }

    if (setting().onMessageLabel)
    {
        labelCount++;
        auto messageLabel = CCLabelBMFont::create(setting().message, "bigFont.fnt");
        messageLabel->setScale(0.35f);
        messageLabel->setZOrder(5);
        messageLabel->setAnchorPoint({ 0.f, 0.f });
        messageLabel->setOpacity(100);
        messageLabel->setTag(45077);
        messageLabel->setPosition({ 2.f, size.height - 11.f * labelCount });
        self->addChild(messageLabel);
    }

    if (setting().onAttemptsLabel)
    {
        labelCount++;
        auto AttemptsLabel = CCLabelBMFont::create("Attempt 1", "bigFont.fnt");
        AttemptsLabel->setScale(0.35f);
        AttemptsLabel->setZOrder(5);
        AttemptsLabel->setAnchorPoint({ 0.f, 0.f });
        AttemptsLabel->setOpacity(100);
        AttemptsLabel->setTag(45073);
        AttemptsLabel->setPosition({ 2.f, size.height - 11.f * labelCount });
        self->addChild(AttemptsLabel);
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
        Clock->setOpacity(100);
        Clock->setTag(45075);
        Clock->setPosition({ 2.f, size.height - 11.f * labelCount });
        self->addChild(Clock);
    }

    if (setting().onFPSLabel)
    {
        labelCount++;
        //int fps = 1 / CCDirector::sharedDirector()->getAnimationInterval();

        float fps = ImGui::GetIO().Framerate;
        auto FPSLabel = CCLabelBMFont::create("0 FPS", "bigFont.fnt");
        FPSLabel->setString(CCString::createWithFormat("%.f FPS", fps)->getCString());
        FPSLabel->setScale(0.35f);
        FPSLabel->setZOrder(5);
        FPSLabel->setAnchorPoint({ 0.f, 0.f });
        FPSLabel->setOpacity(100);
        FPSLabel->setTag(45076);
        FPSLabel->setPosition({ 2.f, size.height - 11.f * labelCount });
        self->addChild(FPSLabel);
    }

    //if (setting().onNoclipAccuracy)
    //{
    //    labelCount++;
    //    auto noclipAccLabel = CCLabelBMFont::create("100.00%", "bigFont.fnt");
    //    noclipAccLabel->setScale(0.35f);
    //    noclipAccLabel->setZOrder(5);
    //    noclipAccLabel->setAnchorPoint({ 0.f, 0.f });
    //    noclipAccLabel->setOpacity(100);
    //    noclipAccLabel->setTag(45079);
    //    noclipAccLabel->setPosition({ 2.f, size.height - 13.f * labelCount });
    //    self->addChild(noclipAccLabel);
    //}

    //if (setting().onNoclipDeaths)
    //{
    //    labelCount++;
    //    auto noclDeath = CCLabelBMFont::create("0 Deaths", "bigFont.fnt");
    //    noclDeath->setScale(0.35f);
    //    noclDeath->setZOrder(5);
    //    noclDeath->setAnchorPoint({ 0.f, 0.f });
    //    noclDeath->setOpacity(100);
    //    noclDeath->setTag(45078);
    //    noclDeath->setPosition({ 2.f, size.height - 13.f * labelCount });
    //    self->addChild(noclDeath);
    //}

    

    


    if (setting().onMeta)
    {
        auto xPos = CCLabelBMFont::create("X: 0", "bigFont.fnt");
        xPos->setString(CCString::createWithFormat("X: %.f")->getCString());
        xPos->setScale(0.35f);
        xPos->setZOrder(5);
        xPos->setAnchorPoint({ 0.f, 0.f });
        xPos->setOpacity(100);
        xPos->setTag(69060);
        xPos->setPosition({ 2.f, 30 });
        self->addChild(xPos);

        auto yPos = CCLabelBMFont::create("Y: 0", "bigFont.fnt");
        yPos->setString(CCString::createWithFormat("Y: %.f")->getCString());
        yPos->setScale(0.35f);
        yPos->setZOrder(5);
        yPos->setAnchorPoint({ 0.f, 0.f });
        yPos->setOpacity(100);
        yPos->setTag(69061);
        yPos->setPosition({ 2.f, 20 });
        self->addChild(yPos);

        //auto vel = CCLabelBMFont::create("Vel: 0", "bigFont.fnt");
        //vel->setString(CCString::createWithFormat("Vel: %.d")->getCString());
        //vel->setScale(0.35f);
        //vel->setZOrder(5);
        //vel->setAnchorPoint({ 0.f, 0.f });
        //vel->setOpacity(100);
        //vel->setTag(69062);
        //vel->setPosition({ 2.f, 30 });
        //self->addChild(vel);

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

        //auto speed = CCLabelBMFont::create("Speed: 0", "bigFont.fnt");
        //speed->setString(CCString::createWithFormat("Speed: %.f")->getCString());
        //speed->setScale(0.35f);
        //speed->setZOrder(5);
        //speed->setAnchorPoint({ 0.f, 0.f });
        //speed->setOpacity(100);
        //speed->setTag(69065);
        //speed->setPosition({ 2.f, 0 });
        //self->addChild(speed);
    }

}

bool rKeyFlag = true;
bool lKeyFlag = true;

inline bool playerTouchesObject(CCRect* playerRect, CCRect* hitboxRect)
{
    float playerMaxX = playerRect->getMaxX();

}

void __fastcall PlayLayer::update_H(gd::PlayLayer* self, void*, float dt) {
    layers().PauseLayerObject = nullptr;

    //const auto bar = gd::GameManager::sharedState()->getShowProgressBar();

    auto size = CCDirector::sharedDirector()->getWinSize();
    //auto audioScale = self->player1()->getAudioScale() > 1.f ? 1.f : self->player1()->getAudioScale();
    //auto percentLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(4571));
    auto spswitcherlbl = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45712));
    //auto xPos = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69001));
    float fps = ImGui::GetIO().Framerate;

    auto secarr = self->getSections();
    auto arrcount = secarr->count();

    auto FPSLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45076));
    auto AttemptsLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45073));
    auto Clock = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45075));
    auto MessageLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45077));
    //auto noclDeath = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45078));
    //auto noclipAccLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45079));
    auto xPos = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69060));
    auto yPos = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69061));
    //auto vel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69062));
    auto rot = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69063));
    auto psize = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69064));
    //auto speed = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69065));

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

    if (FPSLabel)
    {
        FPSLabel->setString(CCString::createWithFormat("%.f FPS", roundf(fps)/*CCDirector::sharedDirector()->getTotalFrames()*/)->getCString());
        if (setting().onHideLabels) {
            FPSLabel->setVisible(0);
        }
        else {
            FPSLabel->setVisible(1);
        }
    }

    if (AttemptsLabel)
    {
        AttemptsLabel->setString(CCString::createWithFormat("Attempt %d", self->attemptsCount())->getCString());
        if (setting().onHideLabels) {
            AttemptsLabel->setVisible(0);
        }
        else {
            AttemptsLabel->setVisible(1);
        }
    }

    if (Clock)
    {
        t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream s;
        s << std::put_time(&tm, "%H:%M:%S");
        Clock->setString(CCString::create(s.str().c_str())->getCString());
        if (setting().onHideLabels) {
            Clock->setVisible(0);
        }
        else {
            Clock->setVisible(1);
        }
    }

    //NoclipAcc = ((self->player1()->getPositionX() - deathDifference) / self->player1()->getPositionX()) * 100;
    //if (noclipAccLabel)
    //{
    //    const auto value = self->player1()->getPositionX() / self->levelLength() * 100.f;
    //    if (value < 100.0f) reinterpret_cast<CCLabelBMFont*>(noclipAccLabel)->setString(CCString::createWithFormat("%.2f%%", NoclipAcc)->getCString());
    //}

    //if (noclDeath)
    //{
    //    noclDeath->setString(CCString::createWithFormat("%d Deaths", deaths)->getCString());
    //}

    if (MessageLabel)
    {
        MessageLabel->setString(CCString::create(setting().message)->getCString());
        if (setting().onHideLabels) {
            MessageLabel->setVisible(0);
        }
        else {
            MessageLabel->setVisible(1);
        }
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

    //if (vel) {
    //    vel->setString(CCString::createWithFormat("Vel: %.06d%", self->player1()->getVelocity())->getCString());
    //}

    if (rot) {
        rot->setString(CCString::createWithFormat("Rot: %.06f%", self->player1()->getRotation())->getCString());
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

    //if (speed) {
    //    speed->setString(CCString::createWithFormat("Speed: %.06f%", self->player1()->getPlayerSpeed())->getCString());
    //}

    /*if (percentLabel) {
        const auto value = self->player1()->getPositionX() / self->levelLength() * 100.f;

        percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
        percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });

        if (value < 100.0f) percentLabel->setString(CCString::createWithFormat("%.2f%%", value)->getCString());
        else percentLabel->setString(CCString::create("100.00%")->getCString());

    }*/

    auto playerDrawNode = reinterpret_cast<CCDrawNode*>(self->layer()->getChildByTag(124));
    playerDrawNode->clear();

    if ((self->player1()->getIsDead() && setting().onHitboxesOnDeath) || setting().onHitboxes) {
        if (setting().onPlayerHitbox)
        {
            if (self->player1())
            {
                Hitboxes::drawPlayerHitbox(self->player1(), playerDrawNode);
            }
            if (self->player2())
            {
                Hitboxes::drawPlayerHitbox(self->player2(), playerDrawNode);
            }
        }
    }

    auto objDrawNode = reinterpret_cast<CCDrawNode*>(self->layer()->getChildByTag(125));
    objDrawNode->clear();

    if ((self->player1()->getIsDead() && setting().onHitboxesOnDeath) || setting().onHitboxes) {
        if (setting().onSolidsHitbox)
        {
            for (int i = self->getFirstVisibleSection() + 1; i < self->getLastVisibleSection() - 1; i++)
            {
                if (i < 0) continue;
                if (i >= arrcount) break;
                auto objAtInd = secarr->objectAtIndex(i);
                auto objarr = reinterpret_cast<CCArray*>(objAtInd);

                for (int j = 0; j < objarr->count(); j++)
                {
                    auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
                    Hitboxes::drawSolidsObjectHitbox(obj, objDrawNode);
                }
            }
        }

        if (setting().onHazardsHitbox)
        {
            for (int i = self->getFirstVisibleSection() + 1; i < self->getLastVisibleSection() - 1; i++)
            {
                if (i < 0) continue;
                if (i >= arrcount) break;
                auto objAtInd = secarr->objectAtIndex(i);
                auto objarr = reinterpret_cast<CCArray*>(objAtInd);

                for (int j = 0; j < objarr->count(); j++)
                {
                    auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
                    Hitboxes::drawHazardsObjectHitbox(obj, objDrawNode);
                }
            }
        }

        if (setting().onSpecialsHitbox)
        {
            for (int i = self->getFirstVisibleSection() + 1; i < self->getLastVisibleSection() - 1; i++)
            {
                if (i < 0) continue;
                if (i >= arrcount) break;
                auto objAtInd = secarr->objectAtIndex(i);
                auto objarr = reinterpret_cast<CCArray*>(objAtInd);

                for (int j = 0; j < objarr->count(); j++)
                {
                    auto obj = reinterpret_cast<gd::GameObject*>(objarr->objectAtIndex(j));
                    Hitboxes::drawSpecialsObjectHitbox(obj, objDrawNode);
                }
            }
        }
    }

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

    if (setting().onSwapWaveColors) {
        self->player1()->setWaveTrailColor(self->player2()->getWaveTrailColor());
        self->player2()->setWaveTrailColor(self->player1()->getWaveTrailColor());
    }
    else {
        self->player1()->setWaveTrailColor(self->player1()->getWaveTrailColor());
        self->player2()->setWaveTrailColor(self->player2()->getWaveTrailColor());
    }

    if (setting().onLockCursor && !setting().show && !self->hasCompletedLevel() && !self->isDead()) {
        SetCursorPos(size.width / 2, size.height / 2);
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

void __fastcall PlayLayer::onQuit_H(gd::PlayLayer* self, void* edx) {
    layers().PauseLayerObject = nullptr;
    //if (setting().onPracticeFix) {
    //    PlayLayer::checkpoints.clear();
    //}
    auto playerDrawNode = reinterpret_cast<CCDrawNode*>(self->layer()->getChildByTag(124));
    auto objDrawNode = reinterpret_cast<CCDrawNode*>(self->layer()->getChildByTag(125));
    
    playerDrawNode->clear();
    objDrawNode->clear();

    playLayer = nullptr;
    PlayLayer::onQuit(self);
}

void __fastcall PlayLayer::resetLevel_H(gd::PlayLayer* self) {
    setting().beforeRestartCheatsCount = setting().cheatsCount;
    deaths = 0;
    wasDead = 0;
    deathDifference = 0.f;
    deathPos = 0.f;
    //if (setting().onPracticeFix) {
    //    if (PlayLayer::inPractice && PlayLayer::checkpoints.size() > 0) {
    //        PlayLayer::checkpoints.pop_back();
    //    }
    //}

    PlayLayer::resetLevel(self);
}

void __fastcall PlayLayer::pushButton_H(int idk1, bool idk2) {
    hasClicked = true;
    PlayLayer::pushButton(idk1, idk2);
}

void __fastcall PlayLayer::releaseButton_H(int idk1, bool idk2) {
    hasClicked = false;
    PlayLayer::releaseButton(idk1, idk2);
}

void __fastcall HardStreak::updateStroke_H(gd::HardStreak* self, float dt) {

    //self->m_pulseSize = 1;
    hardStreak = self;

    HardStreak::updateStroke(self, dt);
}

void __fastcall PlayLayer::togglePracticeMode_H(gd::PlayLayer* self, int edx, bool practice) {
 /*   if (setting().onPracticeFix) {
        PlayLayer::checkpoints.clear();
        PlayLayer::inPractice = practice;
    }*/

    PlayLayer::togglePracticeMode(self, practice);
}

void __fastcall PlayLayer::createCheckpoint_H(gd::PlayLayer* self) {
    //if (setting().onPracticeFix) {
    //    if (*(void**)((char*)self + 0x2a4) != nullptr) {
    //        PlayLayer::checkpoints.push_back({
    //            CheckPoint::from(self)
    //            });
    //    }
    //}
    
    PlayLayer::createCheckpoint(self);

    //if (!PlayLayer::createCheckpoint) return false;
}

void __fastcall PlayLayer::removeLastCheckpoint_H(gd::PlayLayer* self) {
    //if (setting().onPracticeFix) {
    //    if (PlayLayer::checkpoints.size() > 0) {
    //        PlayLayer::checkpoints.pop_back();
    //    }
    //}

    PlayLayer::removeLastCheckpoint(self);
}

void __fastcall PlayLayer::levelComplete_H(gd::PlayLayer* self) {

    //if (setting().onPracticeFix) {
    //    PlayLayer::checkpoints.clear();
    //    PlayLayer::inPractice = false;
    //}

    PlayLayer::levelComplete(self);
}

void __fastcall PlayLayer::destroyPlayer_H(gd::PlayLayer* self, void*, gd::PlayerObject* player)
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
    PlayLayer::destroyPlayer(self, player);
}

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
    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0xf3610),
    //    PlayLayer::togglePracticeMode_H,
    //    reinterpret_cast<void**>(&PlayLayer::togglePracticeMode));
    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0xf1010),
    //    PlayLayer::createCheckpoint_H,
    //    reinterpret_cast<void**>(&PlayLayer::createCheckpoint));
    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0xf1d70),
    //    PlayLayer::removeLastCheckpoint_H,
    //    reinterpret_cast<void**>(&PlayLayer::removeLastCheckpoint));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf1f20),
        PlayLayer::resetLevel_H,
        reinterpret_cast<void**>(&PlayLayer::resetLevel));
    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0xf04a0),
    //    PlayLayer::destroyPlayer_H,
    //    reinterpret_cast<void**>(&PlayLayer::destroyPlayer));
    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0xe52e0),
    //    PlayLayer::levelComplete_H,
    //    reinterpret_cast<void**>(&PlayLayer::levelComplete));

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
