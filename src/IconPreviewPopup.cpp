#include "IconPreviewPopup.hpp"
#include "Setting.hpp"

bool IconPreviewPopup::init() {
	if (!this->initWithColor({ 0, 0, 0, 75 })) return false;

	auto* director = CCDirector::sharedDirector();
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
	bg->setContentSize({ 285.f, 90.f });
	bg->setPosition(winSize / 2.f);
	layer->addChild(bg, -1);

	auto bgbg = extension::CCScale9Sprite::create("square02_small.png");
	bgbg->setContentSize({ 250.f, 50.f });
	bgbg->setPosition(winSize / 2.f);
	bgbg->setOpacity(75);
	layer->addChild(bgbg);

	auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	onCloseSpr->setScale(.8f);
	auto onClose = CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(IconPreviewPopup::onClose));
	onClose->setPosition(-140.f, 40.f);
	menu->addChild(onClose);

	auto gm = GameManager::sharedState();

	auto playerIDLabel = CCLabelBMFont::create("", "chatFont.fnt");
	playerIDLabel->setString(CCString::createWithFormat("ID: %i", gm->m_playerUserID)->getCString());
	playerIDLabel->setAnchorPoint({ 0.f, .5f });
	playerIDLabel->setColor(ccBLACK);
	playerIDLabel->setScale(.85f);
	playerIDLabel->setOpacity(75);
	playerIDLabel->setPosition(winSize.width / 2.f - 135.f, winSize.height / 2.f - 32.f);
	layer->addChild(playerIDLabel);

	auto playerIcon = SimplePlayer::create(gm->m_playerFrame);
	playerIcon->setPosition({ winSize.width / 2.f - 100.f, winSize.height / 2.f });
	playerIcon->setColor(gm->colorForIdx(gm->m_playerColor));
	playerIcon->setSecondColor(gm->colorForIdx(gm->m_playerColor2));
	playerIcon->m_special = gm->m_playerStreak || gm->getGameVariable("3097");
	playerIcon->updateColors();
	layer->addChild(playerIcon);

	auto shipIcon = SimplePlayer::create(gm->m_playerShip);
	shipIcon->updatePlayerFrame(gm->m_playerShip, IconType::Ship);
	shipIcon->setPosition({ winSize.width / 2.f - 50.f, winSize.height / 2.f });
	shipIcon->setColor(gm->colorForIdx(gm->m_playerColor));
	shipIcon->setSecondColor(gm->colorForIdx(gm->m_playerColor2));
	shipIcon->m_special = gm->m_playerStreak || gm->getGameVariable("3097");
	shipIcon->updateColors();
	layer->addChild(shipIcon);

	auto rollIcon = SimplePlayer::create(gm->m_playerBall);
	rollIcon->updatePlayerFrame(gm->m_playerBall, IconType::Ball);
	rollIcon->setPosition({ winSize / 2.f });
	rollIcon->setColor(gm->colorForIdx(gm->m_playerColor));
	rollIcon->setSecondColor(gm->colorForIdx(gm->m_playerColor2));
	rollIcon->m_special = gm->m_playerStreak || gm->getGameVariable("3097");
	rollIcon->updateColors();
	layer->addChild(rollIcon);

	auto birdIcon = SimplePlayer::create(gm->m_playerBird);
	birdIcon->updatePlayerFrame(gm->m_playerBird, IconType::Ufo);
	birdIcon->setPosition({ winSize.width / 2.f + 50.f, winSize.height / 2.f });
	birdIcon->setColor(gm->colorForIdx(gm->m_playerColor));
	birdIcon->setSecondColor(gm->colorForIdx(gm->m_playerColor2));
	birdIcon->m_special = gm->m_playerStreak || gm->getGameVariable("3097");
	birdIcon->updateColors();
	layer->addChild(birdIcon);

	auto dartIcon = SimplePlayer::create(1);
	dartIcon->updatePlayerFrame(1, IconType::Wave);
	dartIcon->setPosition({ winSize.width / 2.f + 100.f, winSize.height / 2.f });
	dartIcon->setColor(gm->colorForIdx(gm->m_playerColor));
	dartIcon->setSecondColor(gm->colorForIdx(gm->m_playerColor2));
	dartIcon->m_special = gm->m_playerStreak || gm->getGameVariable("3097");
	dartIcon->updateColors();
	layer->addChild(dartIcon);

	this->setKeypadEnabled(true);
	this->setTouchEnabled(true);

	return true;
}

void IconPreviewPopup::keyBackClicked() {
	this->setTouchEnabled(false);
	this->setKeypadEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void IconPreviewPopup::onClose(CCObject*) {
	this->keyBackClicked();
}