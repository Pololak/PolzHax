#include "SURVEY_PROGRAM.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/MenuGameLayer.hpp>
#include <stdlib.h>

std::string executable_name() {
    #if defined(_WIN32)
        char buf[MAX_PATH];
        GetModuleFileNameA(nullptr, buf, MAX_PATH);
        return buf;
    #endif
}

bool platinum_exe = false;
bool platinum_url = false;

int serverString;

class $modify(LoadingLayer) {
    void loadingFinished() {
        if (!Mod::get()->getSavedValue<bool>("090526-load") || Mod::get()->getSavedValue<bool>("platinum-check"))
            CCDirector::get()->replaceScene(SURVEY_PROGRAM_LAYER::scene());
        else {
            HWND hwnd = WindowFromDC(wglGetCurrentDC());
            SetWindowText(hwnd, "Geometry Dash");
            LoadingLayer::loadingFinished();
        }
    }
};

SURVEY_PROGRAM_LAYER* SURVEY_PROGRAM_LAYER::create() {
    SURVEY_PROGRAM_LAYER* ret = new SURVEY_PROGRAM_LAYER();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCScene* SURVEY_PROGRAM_LAYER::scene() {
    auto layer = SURVEY_PROGRAM_LAYER::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

bool SURVEY_PROGRAM_LAYER::init() {
    if (!CCLayer::init()) return false;

    auto director = CCDirector::get();
    auto winSize = director->getWinSize();

    this->setKeypadEnabled(true);

    auto exe_name = executable_name();
    log::debug("{}", executable_name().c_str());
    if (exe_name.find("Platinum") != std::string::npos) {
        platinum_exe = true;
    }
    if ((serverString == 1952541808) && !platinum_exe) {
        platinum_url = true;
    }

    HWND hwnd = WindowFromDC(wglGetCurrentDC());
    SetWindowText(hwnd, "SURVEY_PROGRAM");

    //SetClassLong(hwnd, GCL_HICON, (LONG)LoadIcon(NULL, "DELTARUNE_152.ico"_spr));

    GameSoundManager::sharedManager()->stopBackgroundMusic();
    FMODAudioEngine::sharedEngine()->playBackgroundMusic("AUDIO_ANOTHERHIM.mp3"_spr, true, false);

    m_background = CCSprite::create("IMAGE_DEPTH.png"_spr);
    m_background->setOpacity(0);
    m_background->setPosition(winSize / 2.f);
    m_background->setScale(3.f);
    m_background->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

    m_background->runAction(CCRepeat::create(CCSequence::create(CCSpawn::create(CCScaleTo::create(2.5f, 4.f), CCFadeTo::create(2.5f, 100.f), nullptr), CCSpawn::create(CCScaleTo::create(2.5f, 5.f), CCFadeTo::create(2.5f, 0), nullptr), CCScaleTo::create(0.f, 3.f), nullptr), kCCRepeatForever));

    this->addChild(m_background, -2);

    m_background2 = CCSprite::create("IMAGE_DEPTH.png"_spr);
    m_background2->setOpacity(0);
    m_background2->setPosition(winSize / 2.f);
    m_background2->setScale(3.f);
    m_background2->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

    auto repeatForever2 = CCRepeat::create(CCSequence::create(CCSpawn::create(CCScaleTo::create(2.5f, 4.f), CCFadeTo::create(2.5f, 100.f), nullptr), CCSpawn::create(CCScaleTo::create(2.5f, 5.f), CCFadeTo::create(2.5f, 0), nullptr), CCScaleTo::create(0.f, 3.f), nullptr), kCCRepeatForever);
    m_background2->runAction(CCSequence::create(CCDelayTime::create(.5f), repeatForever2, nullptr));

    this->addChild(m_background2, -2);

    m_background3 = CCSprite::create("IMAGE_DEPTH.png"_spr);
    m_background3->setOpacity(0);
    m_background3->setPosition(winSize / 2.f);
    m_background3->setScale(3.f);
    m_background3->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

    auto repeatForever3 = CCRepeat::create(CCSequence::create(CCSpawn::create(CCScaleTo::create(2.5f, 4.f), CCFadeTo::create(2.5f, 100.f), nullptr), CCSpawn::create(CCScaleTo::create(2.5f, 5.f), CCFadeTo::create(2.5f, 0), nullptr), CCScaleTo::create(0.f, 3.f), nullptr), kCCRepeatForever);
    m_background3->runAction(CCSequence::create(CCDelayTime::create(1.f), repeatForever3, nullptr));

    this->addChild(m_background3, -2);

    m_background4 = CCSprite::create("IMAGE_DEPTH.png"_spr);
    m_background4->setOpacity(0);
    m_background4->setPosition(winSize / 2.f);
    m_background4->setScale(3.f);
    m_background4->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

    auto repeatForever4 = CCRepeat::create(CCSequence::create(CCSpawn::create(CCScaleTo::create(2.5f, 4.f), CCFadeTo::create(2.5f, 100.f), nullptr), CCSpawn::create(CCScaleTo::create(2.5f, 5.f), CCFadeTo::create(2.5f, 0), nullptr), CCScaleTo::create(0.f, 3.f), nullptr), kCCRepeatForever);
    m_background4->runAction(CCSequence::create(CCDelayTime::create(1.5f), repeatForever4, nullptr));

    this->addChild(m_background4, -2);

    m_background5 = CCSprite::create("IMAGE_DEPTH.png"_spr);
    m_background5->setOpacity(0);
    m_background5->setPosition(winSize / 2.f);
    m_background5->setScale(3.f);
    m_background5->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

    auto repeatForever5 = CCRepeat::create(CCSequence::create(CCSpawn::create(CCScaleTo::create(2.5f, 4.f), CCFadeTo::create(2.5f, 100.f), nullptr), CCSpawn::create(CCScaleTo::create(2.5f, 5.f), CCFadeTo::create(2.5f, 0), nullptr), CCScaleTo::create(0.f, 3.f), nullptr), kCCRepeatForever);
    m_background5->runAction(CCSequence::create(CCDelayTime::create(2.f), repeatForever5, nullptr));

    this->addChild(m_background5, -2);

    m_background6 = CCSprite::create("IMAGE_DEPTH.png"_spr);
    m_background6->setOpacity(0);
    m_background6->setPosition(winSize / 2.f);
    m_background6->setScale(3.f);
    m_background6->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

    auto repeatForever6 = CCRepeat::create(CCSequence::create(CCSpawn::create(CCScaleTo::create(2.5f, 4.f), CCFadeTo::create(2.5f, 100.f), nullptr), CCSpawn::create(CCScaleTo::create(2.5f, 5.f), CCFadeTo::create(2.5f, 0), nullptr), CCScaleTo::create(0.f, 3.f), nullptr), kCCRepeatForever);
    m_background6->runAction(CCSequence::create(CCDelayTime::create(2.5f), repeatForever6, nullptr));

    this->addChild(m_background6, -2);

    m_background7 = CCSprite::create("IMAGE_DEPTH.png"_spr);
    m_background7->setOpacity(0);
    m_background7->setPosition(winSize / 2.f);
    m_background7->setScale(3.f);
    m_background7->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

    auto repeatForever7 = CCRepeat::create(CCSequence::create(CCSpawn::create(CCScaleTo::create(2.5f, 4.f), CCFadeTo::create(2.5f, 100.f), nullptr), CCSpawn::create(CCScaleTo::create(2.5f, 5.f), CCFadeTo::create(2.5f, 0), nullptr), CCScaleTo::create(0.f, 3.f), nullptr), kCCRepeatForever);
    m_background7->runAction(CCSequence::create(CCDelayTime::create(3.f), repeatForever7, nullptr));

    this->addChild(m_background7, -2);

    m_background8 = CCSprite::create("IMAGE_DEPTH.png"_spr);
    m_background8->setOpacity(0);
    m_background8->setPosition(winSize / 2.f);
    m_background8->setScale(3.f);
    m_background8->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

    auto repeatForever8 = CCRepeat::create(CCSequence::create(CCSpawn::create(CCScaleTo::create(2.5f, 4.f), CCFadeTo::create(2.5f, 100.f), nullptr), CCSpawn::create(CCScaleTo::create(2.5f, 5.f), CCFadeTo::create(2.5f, 0), nullptr), CCScaleTo::create(0.f, 3.f), nullptr), kCCRepeatForever);
    m_background8->runAction(CCSequence::create(CCDelayTime::create(3.5f), repeatForever8, nullptr));

    this->addChild(m_background8, -2);

    m_background9 = CCSprite::create("IMAGE_DEPTH.png"_spr);
    m_background9->setOpacity(0);
    m_background9->setPosition(winSize / 2.f);
    m_background9->setScale(3.f);
    m_background9->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

    auto repeatForever9 = CCRepeat::create(CCSequence::create(CCSpawn::create(CCScaleTo::create(2.5f, 4.f), CCFadeTo::create(2.5f, 100.f), nullptr), CCSpawn::create(CCScaleTo::create(2.5f, 5.f), CCFadeTo::create(2.5f, 0), nullptr), CCScaleTo::create(0.f, 3.f), nullptr), kCCRepeatForever);
    m_background9->runAction(CCSequence::create(CCDelayTime::create(4.f), repeatForever9, nullptr));

    this->addChild(m_background9, -2);

    m_background10 = CCSprite::create("IMAGE_DEPTH.png"_spr);
    m_background10->setOpacity(0);
    m_background10->setPosition(winSize / 2.f);
    m_background10->setScale(3.f);
    m_background10->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

    auto repeatForever10 = CCRepeat::create(CCSequence::create(CCSpawn::create(CCScaleTo::create(2.5f, 4.f), CCFadeTo::create(2.5f, 100.f), nullptr), CCSpawn::create(CCScaleTo::create(2.5f, 5.f), CCFadeTo::create(2.5f, 0), nullptr), CCScaleTo::create(0.f, 3.f), nullptr), kCCRepeatForever);
    m_background10->runAction(CCSequence::create(CCDelayTime::create(4.5f), repeatForever10, nullptr));

    this->addChild(m_background10, -2);

    std::string playerName = (std::string)GameManager::get()->m_playerName;
    std::transform(playerName.begin(), playerName.end(), playerName.begin(), ::toupper);

    m_polzspeech = CCLabelBMFont::create("", "RunedeUniform.fnt"_spr);
    m_polzspeech->setPosition(winSize.width / 2.f, winSize.height / 2.f + 60.f);
    m_polzspeech->setString(CCString::createWithFormat("WELCOME AGAIN, %s", playerName.c_str())->getCString());
    m_polzspeech->setOpacity(0);
    m_polzspeech->setAlignment(kCCTextAlignmentCenter);
    m_polzspeech->setAnchorPoint({.5f, 1.f});
    this->addChild(m_polzspeech);

    if ((platinum_exe || platinum_url) && Mod::get()->getSavedValue<bool>("platinum-check")) this->_friend();

    m_polzspeech->runAction(CCSequence::create(
        CCDelayTime::create(5.f), CCFadeIn::create(1.f), CCDelayTime::create(3.f), CCFadeOut::create(1.f), CCDelayTime::create(1.f), CCCallFunc::create(this, callfunc_selector(SURVEY_PROGRAM_LAYER::speech)),
        CCDelayTime::create(1.f), CCFadeIn::create(1.f), CCDelayTime::create(3.f), CCFadeOut::create(1.f), CCDelayTime::create(1.f), CCCallFunc::create(this, callfunc_selector(SURVEY_PROGRAM_LAYER::speech)), 
        CCDelayTime::create(1.f), CCFadeIn::create(1.f), CCDelayTime::create(3.f), CCFadeOut::create(1.f), CCDelayTime::create(1.f), CCCallFunc::create(this, callfunc_selector(SURVEY_PROGRAM_LAYER::speech)),
        CCDelayTime::create(1.f), CCFadeIn::create(1.f), CCDelayTime::create(3.f), CCFadeOut::create(1.f), CCDelayTime::create(1.f), CCCallFunc::create(this, callfunc_selector(SURVEY_PROGRAM_LAYER::speech)),
        CCDelayTime::create(1.f), CCFadeIn::create(1.f), CCDelayTime::create(4.5f), CCFadeOut::create(1.f), CCDelayTime::create(1.f), CCCallFunc::create(this, callfunc_selector(SURVEY_PROGRAM_LAYER::speech)),
        CCDelayTime::create(1.f), CCFadeIn::create(1.f), CCDelayTime::create(3.f), CCFadeOut::create(1.f), CCDelayTime::create(6.f), CCCallFunc::create(this, callfunc_selector(SURVEY_PROGRAM_LAYER::goToMenu)), nullptr));

    log::debug("{}", m_textId);
    log::debug("{}", platinum_exe);
    log::debug("{}", platinum_url);
    
    return true;
}

void SURVEY_PROGRAM_LAYER::speech() {
    std::string m;

    log::debug("{}", m_textId);
    log::debug("{}", platinum_exe);
    log::debug("{}", platinum_url);

    if (platinum_exe) {
        switch (m_textId)
        {
        case 0:
            m = "A PLATINUM GDPS PLAYER"; break;
        case 1:
            m = "YOU REALLY\nWANT TO\nUSE POLZHAX\nWITH GEODE"; break;
        case 2:
            m = "BUT I WONT ALLOW\nTHIS EXECUTABLE\nTO RUN IT"; break;
        case 3:
            m = "HOPE YOU WILL ENJOY\nTHE EMPTINESS\nAND THINK ABOUT\nWHAT YOUVE DONE"; break;
        case 4:
            m = "NOW\nYOU WILL STAY HERE\nFOREVER\nFOREVER\nFOREVER"; break;
        }
        this->runAction(CCSequence::create(CCDelayTime::create(37.f), CCCallFunc::create(this, callfunc_selector(SURVEY_PROGRAM_LAYER::_friend)), nullptr));
    }
    else if (platinum_url) {
        switch (m_textId)
        {
        case 0:
            m = "VERY\nVERY\nSMART"; break;
        case 1:
            m = "YOU REALLY\nWANT TO\nUSE POLZHAX\nWITH GEODE"; break;
        case 2:
            m = "BUT I WONT ALLOW\nYOU TO DO THIS\nANYWAY"; break;
        case 3:
            m = "HOPE YOU WILL ENJOY\nTHE EMPTINESS AGAIN\nAND THINK ABOUT\nWHAT YOUVE DONE"; break;
        case 4:
            m = "NOW\nYOU WILL STAY HERE\nFOREVER\nFOREVER\nFOREVER"; break;
        }
        this->runAction(CCSequence::create(CCDelayTime::create(37.f), CCCallFunc::create(this, callfunc_selector(SURVEY_PROGRAM_LAYER::_friend)), nullptr));
    }
    else {
        switch (m_textId)
        {
        case 0:
            m = "WELCOME TO POLZHAX U9"; break;
        case 1:
            m = "AFTER ALMOST A YEAR\nI HAVE A NEW BETA FOR YOU"; break;
        case 2:
            m = "ITS VERY BIG\nAND MUCH BIGGER\nTHAN PREVIOUS ONE"; break;
        case 3:
            m = "HOPE YOU WILL HELP ME\nAND ENJOY THE BETA"; break;
        case 4:
            m = "NOW\nI WILL SEND YOU\nTO MAIN MENU"; break;
        }
    }
    log::debug("{}", m.c_str());

    m_polzspeech->setString(m.c_str());

    m_textId++;

    log::debug("{}", m_textId);
}

void SURVEY_PROGRAM_LAYER::goToMenu() {
    if (platinum_exe || platinum_url) return;

    Mod::get()->setSavedValue<bool>("090526-load", true);
    CCScene* scene = CCTransitionFade::create(.5f, MenuLayer::scene(false));
    CCDirector::get()->replaceScene(scene);
}

void SURVEY_PROGRAM_LAYER::keyBackClicked() {
    if (platinum_exe || platinum_url) return;

    CCScene* scene = CCTransitionFade::create(.5f, MenuLayer::scene(false));
    CCDirector::get()->replaceScene(scene);
}

void SURVEY_PROGRAM_LAYER::_friend() {
    Mod::get()->setSavedValue<bool>("platinum-check", true);
    Mod::get()->saveData();

    GameSoundManager::sharedManager()->stopBackgroundMusic();
    m_background->setVisible(false);
    m_background2->setVisible(false);
    m_background3->setVisible(false);
    m_background4->setVisible(false);
    m_background5->setVisible(false);
    m_background6->setVisible(false);
    m_background7->setVisible(false);
    m_background8->setVisible(false);
    m_background9->setVisible(false);
    m_background10->setVisible(false);

    auto winSize = CCDirector::get()->getWinSize();
    
    auto image_friend = CCSprite::create("SPR_NONDESCRIPT_ROOM___FACE.png"_spr);
    image_friend->setPosition(winSize / 2.f);
    image_friend->setScale(4.f);
    this->addChild(image_friend);
    FMODAudioEngine::sharedEngine()->m_backgroundMusicVolume = 100.f;
    FMODAudioEngine::sharedEngine()->playBackgroundMusic("snd_crowd_laugh.mp3"_spr, false, false);

    this->runAction(CCSequence::create(CCDelayTime::create(1.5f), CCCallFunc::create(this, callfunc_selector(SURVEY_PROGRAM_LAYER::closeTheGame)), nullptr));
}

void SURVEY_PROGRAM_LAYER::closeTheGame() {
    abort();
}

$execute {
    ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x54083C), &serverString, 8, 0);
}