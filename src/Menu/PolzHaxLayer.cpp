#include "PolzHaxLayer.h"
#include "../utils.h"

bool PolzHaxLayer::init() {
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
    bg->setContentSize({ 420.f, 280.f });
    bg->setPosition(winSize / 2.f);
    layer->addChild(bg, -2);

    auto tabsBg = extension::CCScale9Sprite::create("square02_001.png");
    tabsBg->setContentSize({ 90.f, 250.f });
    tabsBg->setPosition(winSize.width / 2.f - 155.f, winSize.height / 2.f);
    tabsBg->setOpacity(100);
    layer->addChild(tabsBg, -1);

    auto contentBg = extension::CCScale9Sprite::create("square02_001.png");
    contentBg->setContentSize({ 300.f, 220.f });
    contentBg->setPosition(winSize.width / 2.f + 50.f, winSize.height / 2.f - 15.f);
    contentBg->setOpacity(100);
    layer->addChild(contentBg, -1);

    auto title = CCLabelBMFont::create("PolzHax: Mobile", "bigFont.fnt");
    title->setScale(.8f);
    title->setPosition(winSize.width / 2.f + 40, winSize.height / 2.f + 115.f);
    layer->addChild(title);

    auto version = CCLabelBMFont::create("v1.1.0", "goldFont.fnt");
    version->setScale(.5f);
    version->setPosition(winSize.width / 2.f + 135.f, winSize.height / 2.f + 101.5f);
    layer->addChild(version);

    auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    onCloseSpr->setScale(.75f);
    auto onClose = CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(PolzHaxLayer::onClose));
    onClose->setPosition(-206.5f, 134.5f);
    menu->addChild(onClose, 2);

    auto onSettingsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    onSettingsSpr->setScale(.65f);
    auto onSettings = CCMenuItemSpriteExtra::create(onSettingsSpr, this, 0);
    onSettings->setPosition(185.f, 115.f);
    menu->addChild(onSettings);

    auto gm = GameManager::sharedState();

    m_spritesArray = CCArray::create();
    m_spritesArray->retain();

    m_buttonsArray = CCArray::create();
    m_buttonsArray->retain();

    // Tabs
    auto onBypassSpr = PolzButtonSprite::create("Bypass");
    onBypassSpr->setScale(.85f);
    onBypassSpr->toggleBGImage(m_selectedTab == 0);
    auto onBypass = CCMenuItemSpriteExtra::create(onBypassSpr, this, menu_selector(PolzHaxLayer::onSelectTab));
    onBypass->setPosition(-155.f, 105.f);
    menu->addChild(onBypass, 0, 0);

    auto onCosmeticSpr = PolzButtonSprite::create("Cosmetic");
    onCosmeticSpr->setScale(.85f);
    onCosmeticSpr->toggleBGImage(m_selectedTab == 1);
    auto onCosmetic = CCMenuItemSpriteExtra::create(onCosmeticSpr, this, menu_selector(PolzHaxLayer::onSelectTab));
    onCosmetic->setPosition(-155.f, 75.f);
    menu->addChild(onCosmetic, 0, 1);

    auto onCreatorSpr = PolzButtonSprite::create("Creator");
    onCreatorSpr->setScale(.85f);
    onCreatorSpr->toggleBGImage(m_selectedTab == 2);
    auto onCreator = CCMenuItemSpriteExtra::create(onCreatorSpr, this, menu_selector(PolzHaxLayer::onSelectTab));
    onCreator->setPosition(-155.f, 45.f);
    menu->addChild(onCreator, 0, 2);

    auto onLevelSpr = PolzButtonSprite::create("Level");
    onLevelSpr->setScale(.85f);
    onLevelSpr->toggleBGImage(m_selectedTab == 3);
    auto onLevel = CCMenuItemSpriteExtra::create(onLevelSpr, this, menu_selector(PolzHaxLayer::onSelectTab));
    onLevel->setPosition(-155.f, 15.f);
    menu->addChild(onLevel, 0, 3);

    auto onUniversalSpr = PolzButtonSprite::create("Universal");
    onUniversalSpr->setScale(.85f);
    onUniversalSpr->toggleBGImage(m_selectedTab == 4);
    auto onUniversal = CCMenuItemSpriteExtra::create(onUniversalSpr, this, menu_selector(PolzHaxLayer::onSelectTab));
    onUniversal->setPosition(-155.f, -15.f);
    menu->addChild(onUniversal, 0, 4);

    auto onSpeedhackSpr = PolzButtonSprite::create("Speedhack");
    onSpeedhackSpr->setScale(.85f);
    onSpeedhackSpr->toggleBGImage(m_selectedTab == 5);
    auto onSpeedhack = CCMenuItemSpriteExtra::create(onSpeedhackSpr, this, menu_selector(PolzHaxLayer::onSelectTab));
    onSpeedhack->setPosition(-155.f, -45.f);
    menu->addChild(onSpeedhack, 0, 5);

    auto onStatusSpr = PolzButtonSprite::create("Status");
    onStatusSpr->setScale(.85f);
    onStatusSpr->toggleBGImage(m_selectedTab == 6);
    auto onStatus = CCMenuItemSpriteExtra::create(onStatusSpr, this, menu_selector(PolzHaxLayer::onSelectTab));
    onStatus->setPosition(-155.f, -75.f);
    menu->addChild(onStatus, 0, 6);

    auto onIconsSpr = PolzButtonSprite::create("Icons");
    onIconsSpr->setScale(.85f);
    onIconsSpr->toggleBGImage(m_selectedTab == 7);
    auto onIcons = CCMenuItemSpriteExtra::create(onIconsSpr, this, menu_selector(PolzHaxLayer::onSelectTab));
    onIcons->setPosition(-155.f, -105.f);
    menu->addChild(onIcons, 0, 7);

    m_spritesArray->addObject(onBypassSpr);
    m_spritesArray->addObject(onCosmeticSpr);
    m_spritesArray->addObject(onCreatorSpr);
    m_spritesArray->addObject(onLevelSpr);
    m_spritesArray->addObject(onUniversalSpr);
    m_spritesArray->addObject(onSpeedhackSpr);
    m_spritesArray->addObject(onStatusSpr);
    m_spritesArray->addObject(onIconsSpr);

    m_buttonsArray->addObject(onBypass);
    m_buttonsArray->addObject(onCosmetic);
    m_buttonsArray->addObject(onCreator);
    m_buttonsArray->addObject(onLevel);
    m_buttonsArray->addObject(onUniversal);
    m_buttonsArray->addObject(onSpeedhack);
    m_buttonsArray->addObject(onStatus);
    m_buttonsArray->addObject(onIcons);

    switch (m_selectedTab) {
    default:
    case 0:
        this->highlightSelected(onBypassSpr);
        onBypass->setEnabled(false);
        break;
    case 1:
        this->highlightSelected(onCosmeticSpr);
        onCosmetic->setEnabled(false);
        break;
    case 2:
        this->highlightSelected(onCreatorSpr);
        onCreator->setEnabled(false);
        break;
    case 3:
        this->highlightSelected(onLevelSpr);
        onLevel->setEnabled(false);
        break;
    case 4:
        this->highlightSelected(onUniversalSpr);
        onUniversal->setEnabled(false);
        break;
    case 5:
        this->highlightSelected(onSpeedhackSpr);
        onSpeedhack->setEnabled(false);
        break;
    case 6:
        this->highlightSelected(onStatusSpr);
        onStatus->setEnabled(false);
        break;
    case 7:
        this->highlightSelected(onIconsSpr);
        onIcons->setEnabled(false);
        break;
    }

    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);

    return true;
}

void PolzHaxLayer::keyBackClicked() {
    if (m_spritesArray) {
        m_spritesArray->release();
        m_spritesArray = nullptr;
    }

    if (m_buttonsArray) {
        m_buttonsArray->release();
        m_buttonsArray = nullptr;
    }

    this->setTouchEnabled(false);
    this->setKeypadEnabled(false);
    this->removeFromParentAndCleanup(true);
}

void PolzHaxLayer::onClose(CCObject*) {
    this->keyBackClicked();
}

void PolzHaxLayer::highlightSelected(PolzButtonSprite* sprite) {
    if (sprite) {
        CCARRAY_FOREACH_B_TYPE(m_spritesArray, tab, PolzButtonSprite) {
            tab->toggleBGImage(false);
        }
        sprite->toggleBGImage(true);
    }
}

void PolzHaxLayer::onSelectTab(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);

    m_selectedTab = btn->getTag();

    CCARRAY_FOREACH_B_TYPE(m_buttonsArray, button, CCMenuItemSpriteExtra) {
        button->setEnabled(true);
    }
    btn->setEnabled(false);

    this->highlightSelected(static_cast<PolzButtonSprite*>(btn->getChildren()->objectAtIndex(0)));
}