#include "ExportLevelLayer.h"
#include "../LevelShare.h"

bool ExportLevelLayer::init(GJGameLevel* level) {
    m_level = level;
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
    bg->setContentSize({ 240.f, 170.f });
    bg->setPosition(winSize / 2.f);
    layer->addChild(bg, -2);

    auto title = CCLabelBMFont::create("Export level", "goldFont.fnt");
    title->setScale(.8f);
    title->setPosition(winSize.width / 2.f, winSize.height / 2.f + 70.f);
    layer->addChild(title);

    auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    onCloseSpr->setScale(.8f);
    auto onClose = CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(ExportLevelLayer::onClose));
    onClose->setPosition(-116.5f, 79.5f);
    menu->addChild(onClose);

    auto fileNameLabel = CCLabelBMFont::create("File name:", "bigFont.fnt");
    fileNameLabel->setScale(.5f);
    fileNameLabel->setPosition(winSize.width / 2.f, winSize.height / 2.f + 45.f);
    layer->addChild(fileNameLabel);

    auto nameInputBg = extension::CCScale9Sprite::create("square02_small.png");
    nameInputBg->setContentSize({ 180.f, 30.f });
    nameInputBg->setOpacity(100);
    nameInputBg->setPosition(winSize.width / 2.f, winSize.height / 2.f + 20.f);
    layer->addChild(nameInputBg, -1);

    m_nameInput = CCTextInputNode::create(160.f, 30.f, "Name", "bigFont.fnt");
    m_nameInput->setString(m_level->m_levelName);
    m_nameInput->setPosition(winSize.width / 2.f, winSize.height / 2.f + 20.f);
    m_nameInput->setMaxLabelScale(.65f);
    m_nameInput->setLabelPlaceholderScale(.5f);
    m_nameInput->setLabelPlaceholderColor(ccGRAY);
    layer->addChild(m_nameInput);

    auto pathLabel = CCLabelBMFont::create("Path:", "bigFont.fnt");
    pathLabel->setScale(.5f);
    pathLabel->setPosition(winSize.width / 2.f, winSize.height / 2.f - 5.f);
    layer->addChild(pathLabel);

    auto exportPathInputBg = extension::CCScale9Sprite::create("square02_small.png");
    exportPathInputBg->setContentSize({ 240.f, 30.f });
    exportPathInputBg->setOpacity(100);
    exportPathInputBg->setPosition(winSize.width / 2.f, winSize.height / 2.f - 27.5f);
    exportPathInputBg->setScale(.75f);
    layer->addChild(exportPathInputBg, -1);

    auto exportPathLabel = CCLabelBMFont::create("storage/emulated/0/PolzHaxMobile/19/levels/", "chatFont.fnt");
    exportPathLabel->setScale(.5f);
    exportPathLabel->setPosition(winSize.width / 2.f, winSize.height / 2.f - 27.5f);
    layer->addChild(exportPathLabel);

    auto onExportSpr = ButtonSprite::create("Export", 84, 0, .8f, true, "goldFont.fnt", "GJ_button_01.png", 30.f);
    auto onExport = CCMenuItemSpriteExtra::create(onExportSpr, this, menu_selector(ExportLevelLayer::onExport));
    onExport->setPosition(0.f, -60.f);
    menu->addChild(onExport);

    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);

    return true;
}

void ExportLevelLayer::keyBackClicked() {
    this->setTouchEnabled(false);
    this->setKeypadEnabled(false);
    this->removeFromParentAndCleanup(true);
}

void ExportLevelLayer::onClose(CCObject*) {
    this->keyBackClicked();
}

void ExportLevelLayer::FLAlert_Clicked(FLAlertLayer* layer, bool btn2) {
    if (btn2) {
        LevelShare::exportLevel(m_level, m_nameInput->getString(), true);
        this->keyBackClicked();
    }
}

void ExportLevelLayer::onExport(CCObject*) {
    if (m_level->m_levelString.empty()) {
        FLAlertLayer::create("Error", "Level string is empty!", "OK")->show();
        return;
    }

    auto result = LevelShare::exportLevel(m_level, m_nameInput->getString());
    if (result) {
        this->keyBackClicked();
    }
    else {
        FLAlertLayer::create(this, "Error", "File with this name already exists!", "OK", "Replace")->show();
    }
}