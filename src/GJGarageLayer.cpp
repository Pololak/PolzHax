#include "GJGarageLayer.hpp"
#include "IconPreviewPopup.hpp"

void PolzGJGarageLayer::onIconPreview(CCObject*) {
    IconPreviewPopup::create()->show();
}

bool PolzGJGarageLayer::init() {
    if (!GJGarageLayer::init()) return false;
    
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto gm = GameManager::sharedState();

    auto demonIcon = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
    demonIcon->setPosition({ director->getScreenRight() - 40.f, director->getScreenTop() - 96.f });
    this->addChild(demonIcon);

    auto demonCountLabel = CCLabelBMFont::create("", "bigFont.fnt");
    demonCountLabel->setString(CCString::createWithFormat("%i", GameStatsManager::sharedState()->getStat("5"))->getCString());
    demonCountLabel->setScale(.5f);
    demonCountLabel->setAnchorPoint({ 1.f, .5f });
    demonCountLabel->setPosition(director->getScreenRight() - 58.f, director->getScreenTop() - 96.f);
    this->addChild(demonCountLabel);

    auto iconPreviewMenu = CCMenu::create();
    iconPreviewMenu->setPosition(this->m_playerPreview->getPositionX() + 65.f, this->m_playerPreview->getPositionY());
    this->addChild(iconPreviewMenu);

    auto onIconPreviewSpr00 = CCSprite::create("GJ_button_01.png");
    onIconPreviewSpr00->setScale(.8f);
    auto onIconPreviewSpr01 = SimplePlayer::create(gm->m_playerFrame);
    onIconPreviewSpr01->setScale(.9f);
    onIconPreviewSpr01->setPosition({ 20.f, 20.f });
    onIconPreviewSpr00->addChild(onIconPreviewSpr01);
    auto onIconPreview = CCMenuItemSpriteExtra::create(onIconPreviewSpr00, this, menu_selector(PolzGJGarageLayer::onIconPreview));
    iconPreviewMenu->addChild(onIconPreview);

    return true;
}