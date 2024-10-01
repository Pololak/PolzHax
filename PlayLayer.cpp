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
    PlayLayer::resetLevel(self);
}

bool __fastcall PlayLayer::init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level) {
    setting().beforeRestartCheatsCount = setting().cheatsCount;
    playLayer = self;
    isPlayerColorGot = false;
    fadedoutflag = 0;
    currentBest = 0;

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

    const auto bar = gd::GameManager::sharedState()->getShowProgressBar();

    auto percentLabel = CCLabelBMFont::create("0.00%", "bigFont.fnt");
    percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
    percentLabel->setScale(0.5f);
    percentLabel->setZOrder(5);
    percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });
    percentLabel->setTag(4571);
    self->addChild(percentLabel);


    auto playerDrawNode = CCDrawNode::create();
    playerDrawNode->setZOrder(1000);
    playerDrawNode->setTag(124);
    self->layer()->addChild(playerDrawNode);

    auto objDrawNode = CCDrawNode::create();
    objDrawNode->setZOrder(1000);
    objDrawNode->setTag(125);
    self->layer()->addChild(objDrawNode);

    if (setting().onHitboxes) {

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

    }

    auto labelsMenu = CCMenu::create();
    labelsMenu->setPosition({ 0, 0 });
    labelsMenu->setTag(7900);
    labelsMenu->setZOrder(5);
    labelsMenu->setAnchorPoint({ 0, 1 });
    labelsMenu->setScale(setting().labelsScale);
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
        auto messageLabel = CCLabelBMFont::create(setting().message, "bigFont.fnt");
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
    isPlayerDead = false;
    PlayLayer::update(self, dt);
    if (!isPlayerDead) wasDead = false;

    //const auto bar = gd::GameManager::sharedState()->getShowProgressBar();

    auto size = CCDirector::sharedDirector()->getWinSize();
    //auto audioScale = self->player1()->getAudioScale() > 1.f ? 1.f : self->player1()->getAudioScale();
    //auto percentLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(4571));
    auto spswitcherlbl = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(45712));
    //auto xPos = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69001));
    float fps = ImGui::GetIO().Framerate;

    auto secarr = self->getSections();
    auto arrcount = secarr->count();

    auto labelsMenu = reinterpret_cast<CCMenu*>(self->getChildByTag(7900));
    auto FPSLabel = reinterpret_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45076));
    auto CPSLabel = reinterpret_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45081));
    auto BestRunLabel = reinterpret_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45080));
    auto SessionTimeLabel = reinterpret_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45082));
    auto AttemptsLabel = reinterpret_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45073));
    auto Clock = reinterpret_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45075));
    auto MessageLabel = reinterpret_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45077));
    auto noclDeath = reinterpret_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45078));
    auto noclipAccLabel = reinterpret_cast<CCLabelBMFont*>(labelsMenu->getChildByTag(45079));
    auto xPos = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69060));
    auto yPos = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69061));
    auto vel = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69062));
    auto rot = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69063));
    auto psize = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69064));
    auto clickindi = reinterpret_cast<CCSprite*>(self->getChildByTag(141207));
    //auto speed = reinterpret_cast<CCLabelBMFont*>(self->getChildByTag(69065));

    if (setting().onAutoSafe && setting().cheatsCount > 0) safeModeON(), setting().isSafeMode = true;
    else if (!setting().onSafeMode) safeModeOFF(), setting().isSafeMode = false;

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

    if (labelsMenu) {
        if (setting().onHideLabels) {
            labelsMenu->setVisible(0);
        }
        else {
            labelsMenu->setVisible(1);
        }
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
        int newBest = self->getLastRunPercent();
        if (newBest >= currentBest) {
            currentBest = newBest;
            BestRunLabel->setString(CCString::createWithFormat("Best Run: %i%%", newBest)->getCString());
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
        const auto value = self->player1()->getPositionX() / self->levelLength() * 100.f;
        if (value < 100.0f) reinterpret_cast<CCLabelBMFont*>(noclipAccLabel)->setString(CCString::createWithFormat("%.2f%%", NoclipAcc)->getCString());
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

    //if (vel) {
    //    vel->setString(CCString::createWithFormat("Vel: %.06d%", self->player1()->m_yVelocity)->getCString());
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
    if ((setting().onPlayerHitbox && setting().onHitboxes) || (self->player1()->getIsDead() && setting().onHitboxesOnDeath))
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

    time_t currentTick = time::getTime();
    m_clickFrames.erase(std::remove_if(m_clickFrames.begin(), m_clickFrames.end(), [currentTick](float tick) {
        return currentTick - tick > 1000;
        }), m_clickFrames.end());

    hasClicked = false;
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
    auto playerDrawNode = reinterpret_cast<CCDrawNode*>(self->layer()->getChildByTag(124));
    auto objDrawNode = reinterpret_cast<CCDrawNode*>(self->layer()->getChildByTag(125));
    
    playerDrawNode->clear();
    objDrawNode->clear();

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
    PlayLayer::pushButton(self, idk1, idk2);
}

void __fastcall PlayLayer::releaseButton_H(gd::PlayLayer* self, void* edx, int idk1, bool idk2) {
    m_isHolding = false;
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
}

auto percentageTogglerSprite(CCSprite* toggleOn, CCSprite* toggleOff) {
    return (setting().onShowPercentage) ? toggleOn : toggleOff;
}

bool __fastcall PauseLayer::customSetup_H(gd::CCBlockLayer* self) {
    layers().PauseLayerObject = self;
    if (setting().onHidePauseMenu)
        self->setVisible(false);
    else self->setVisible(true);

    bool result = PauseLayer::customSetup(self);

    auto director = CCDirector::sharedDirector();
    auto menu = CCMenu::create();

    auto bg = (extension::CCScale9Sprite*)self->getChildren()->objectAtIndex(0);
    if (setting().onTransparentPause) {
        bg->setVisible(0);
    }
    else {
        bg->setVisible(1);
    }

    auto autoretry_lbl = (CCLabelBMFont*)self->getChildren()->objectAtIndex(9);
    auto autocheck_lbl = (CCLabelBMFont*)self->getChildren()->objectAtIndex(10);
    auto bar_lbl = (CCLabelBMFont*)self->getChildren()->objectAtIndex(11);
    bar_lbl->setVisible(0);
    auto toggler_menu = (CCMenu*)self->getChildren()->objectAtIndex(12);

    auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

    auto PercentageButton = gd::CCMenuItemToggler::create(percentageTogglerSprite(toggleOn, toggleOff), percentageTogglerSprite(toggleOff, toggleOn), self, menu_selector(PauseLayer::Callback::PercentageToggler));
    auto PercentageLabel = CCLabelBMFont::create("%", "bigFont.fnt");
    PercentageButton->setScale(0.6f);
    PercentageButton->setPosition({ 155, -135 });
    PercentageLabel->setScale(0.4f);
    PercentageLabel->setPosition({ 455,25 });
    PercentageLabel->setAnchorPoint({ 0.f, 0.5f });
    toggler_menu->addChild(PercentageButton);
    self->addChild(PercentageLabel);
    
    auto new_bar_lbl = CCLabelBMFont::create("Bar", "bigFont.fnt");
    new_bar_lbl->setPosition({ 402, 25 });
    new_bar_lbl->setScale(0.35f);
    self->addChild(new_bar_lbl);

    menu->setPosition({ director->getScreenRight(), director->getScreenTop() });

    auto optionsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    auto optionsBtn = gd::CCMenuItemSpriteExtra::create(optionsSpr, nullptr, self, menu_selector(gd::MenuLayer::onOptions));
    optionsBtn->setPosition({ -42, -42 });
    optionsSpr->setScale(.66f);
    menu->addChild(optionsBtn);
    self->addChild(menu);

    return result;
}

void __fastcall PauseLayer::onEdit_H(CCLayer* self)
{
    layers().PauseLayerObject = nullptr;
    playLayer = nullptr;
    PauseLayer::onEdit(self);
}

void __fastcall PauseLayer::onRestart_H(CCObject* sender) {
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

void __fastcall EndLevelLayer::customSetup_H(gd::GJDropDownLayer* self) {
    EndLevelLayer::customSetup(self);

    auto layerMain = (CCLayer*)self->getChildren()->objectAtIndex(0);
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

//void __fastcall GameObject::setOpacity_H(gd::GameObject* self, void* edx, GLubyte opacity) {
//    if (setting().onDontFade) {
//        return GameObject::setOpacity(self, 255.f);
//    }
//    GameObject::setOpacity((self->getObjectID() = 1), opacity);
//}

void PauseLayer::mem_init() {
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd5f50),
        PauseLayer::customSetup_H,
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
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xec510), PlayLayer::applyEnterEffect_H, reinterpret_cast<void**>(&PlayLayer::applyEnterEffect));
        // Hi.
}

void EndLevelLayer::mem_init() {
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x50430),
        EndLevelLayer::customSetup_H,
        reinterpret_cast<void**>(&EndLevelLayer::customSetup));
}

void PlayerObject::mem_init() {
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xdfff0), PlayerObject::updatePlayerFrame_H, reinterpret_cast<void**>(&PlayerObject::updatePlayerFrame));
    MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe0430), PlayerObject::updatePlayerRollFrame_H, reinterpret_cast<void**>(&PlayerObject::updatePlayerRollFrame));
}

void GameObject::mem_init() {
    //MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x72a60), GameObject::setOpacity_H, reinterpret_cast<void**>(&GameObject::setOpacity));
}