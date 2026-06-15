#include "ImportLevelLayer.h"
#include "../LevelShare.h"

std::string defaultPath = "/storage/emulated/0";

bool ManualImportLevelLayer::init() { // Dev thing
    if (!this->initWithColor({ 0, 0, 0, 75 })) return false;

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
    bg->setContentSize({ 260.f, 140.f });
    bg->setPosition(winSize / 2.f);
    layer->addChild(bg, -2);

    auto title = CCLabelBMFont::create("Manual import", "goldFont.fnt");
    title->setScale(.8f);
    title->setPosition(winSize.width / 2.f, winSize.height / 2.f + 55.f);
    layer->addChild(title);

    auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    onCloseSpr->setScale(.8f);
    auto onClose = CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(ManualImportLevelLayer::onClose));
    onClose->setPosition(-126.5f, 65.f);
    menu->addChild(onClose);

    auto fileNameLabel = CCLabelBMFont::create("Path to file:", "bigFont.fnt");
    fileNameLabel->setScale(.5f);
    fileNameLabel->setPosition(winSize.width / 2.f, winSize.height / 2.f + 25.f);
    layer->addChild(fileNameLabel);

    auto pathInputBg = extension::CCScale9Sprite::create("square02_small.png");
    pathInputBg->setContentSize({ 200.f, 30.f });
    pathInputBg->setOpacity(100);
    pathInputBg->setPosition(winSize.width / 2.f, winSize.height / 2.f);
    layer->addChild(pathInputBg, -1);

    m_pathInput = CCTextInputNode::create(180.f, 30.f, "Path", "bigFont.fnt");
    m_pathInput->setDelegate(this);
    m_pathInput->setString(defaultPath.c_str());
    m_pathInput->setAllowedChars(" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/.");
    m_pathInput->setPosition(winSize.width / 2.f, winSize.height / 2.f);
    m_pathInput->setMaxLabelScale(.65f);
    m_pathInput->setLabelPlaceholderScale(.5f);
    m_pathInput->setLabelPlaceholderColor(ccGRAY);
    layer->addChild(m_pathInput);
    
    auto onImportSpr = ButtonSprite::create("Import", 84, 0, .8f, true, "goldFont.fnt", "GJ_button_01.png", 30.f);
    auto onImport = CCMenuItemSpriteExtra::create(onImportSpr, this, menu_selector(ManualImportLevelLayer::onImport));
    onImport->setPosition(0.f, -45.f);
    menu->addChild(onImport);

    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);

    return true;
}

void ManualImportLevelLayer::keyBackClicked() {
    this->setTouchEnabled(false);
    this->setKeypadEnabled(false);
    this->removeFromParentAndCleanup(true);
}

void ManualImportLevelLayer::onClose(CCObject*) {
    this->keyBackClicked();
}

void ManualImportLevelLayer::textChanged(CCTextInputNode* input) {
    defaultPath = input->getString();
}

void ManualImportLevelLayer::onImport(CCObject*) {
    auto result = LevelShare::importLevel(m_pathInput->getString());
    if (result) {
        this->keyBackClicked();
    }
}