#include "SpeedHackLayer.h"
#include "../Speedhack.h"
#include "../Setting.h"

bool SpeedHackLayer::init() {
    if (!this->initWithColor({0, 0, 0, 75})) return false;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    director->getTouchDispatcher()->incrementForcePrio();
    this->registerWithTouchDispatcher();

    auto layer = CCLayer::create();
    auto menu = CCMenu::create();
    this->m_mainLayer = layer;
    this->m_buttonMenu = menu;

    layer->addChild(menu);
    this->addChild(layer);

    auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
    bg->setContentSize({ 260.f, 160.f });
    bg->setPosition(winSize / 2.f);
    layer->addChild(bg, -1);

    auto title = CCLabelBMFont::create("Speedhack", "goldFont.fnt");
    title->setScale(.8f);
    title->setPosition(winSize.width / 2.f, winSize.height / 2.f + 65.f);
    layer->addChild(title);

    auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    onCloseSpr->setScale(.8f);
    auto onClose = CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(SpeedHackLayer::onClose));
    onClose->setPosition(-126.5f, 74.5f);
    menu->addChild(onClose);

    auto gm = GameManager::sharedState();

    auto valueLabel = CCLabelBMFont::create("Value:", "goldFont.fnt");
    valueLabel->setScale(.6f);
    valueLabel->setPosition(winSize.width / 2.f, winSize.height / 2.f + 30.f);
    layer->addChild(valueLabel);

    auto inputBg = extension::CCScale9Sprite::create("square02_small.png");
    inputBg->setOpacity(100);
    inputBg->setContentSize({70.f, 35.f});
    inputBg->setPosition(winSize / 2.f);
    layer->addChild(inputBg);

    m_input = CCTextInputNode::create(65.f, 35.f, "Val", "bigFont.fnt");
    m_input->setString(CCString::createWithFormat("%.2f", setting().speedhackValue)->getCString());
    m_input->setDelegate(this);
    m_input->setPosition(winSize / 2.f);
    m_input->setCharLimit(5);
    m_input->setAllowedChars("0123456789.");
    m_input->setMaxLabelScale(.8f);
    m_input->setLabelPlaceholderScale(.6f);
    m_input->setLabelPlaceholderColor(ccGRAY);
    layer->addChild(m_input);

    auto speedHackMusicLabel = CCLabelBMFont::create("Speedhack\nMusic:", "goldFont.fnt");
    speedHackMusicLabel->setScale(.5f);
    speedHackMusicLabel->setAlignment(kCCTextAlignmentCenter);
    speedHackMusicLabel->setPosition(winSize.width / 2.f + 83.f, winSize.height / 2.f + 35.f);
    layer->addChild(speedHackMusicLabel);

    auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_audioOffBtn_001.png");
    auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_audioOnBtn_001.png");
    auto onSpeedhackMusic = CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(SpeedHackLayer::onSpeedhackMusic));
    onSpeedhackMusic->toggle(setting().onSpeedhackMusic);
    onSpeedhackMusic->setPosition(85.f, 0.f);
    menu->addChild(onSpeedhackMusic);

    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);

    return true;
}

void SpeedHackLayer::keyBackClicked() {
    this->setTouchEnabled(false);
    this->setKeypadEnabled(false);
    this->removeFromParentAndCleanup(true);
}

void SpeedHackLayer::onClose(CCObject*) {
    this->keyBackClicked();
}

void SpeedHackLayer::textChanged(CCTextInputNode* input) {
    if (input == m_input) {
        setting().speedhackValue = std::atof(m_input->getString().c_str());
        Speedhack::updateSpeedhack();
    }
}

void SpeedHackLayer::onSpeedhackMusic(CCObject*) {
    setting().onSpeedhackMusic = !setting().onSpeedhackMusic;
    Speedhack::updateSpeedhackMusic();
}