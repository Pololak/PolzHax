#include "GoToPagePopup.hpp"

bool GoToPagePopup::init(gd::LevelBrowserLayer* parent) {
	this->m_parent = parent;
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

	menu->setPosition({ winSize.width / 2.f, winSize.height / 2.f + 10.f });

	auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
	bg->setContentSize({ 220.f, 150.f });
	bg->setPosition({ winSize.width / 2.f, winSize.height / 2.f + 60.f });
	layer->addChild(bg, -2);

	auto title = CCLabelBMFont::create("Go To Page", "goldFont.fnt");
	title->setScale(.7f);
	title->setPosition({ winSize.width / 2.f, winSize.height / 2.f + 117.f });
	layer->addChild(title);

	auto inputBg = extension::CCScale9Sprite::create("square02_small.png");
	inputBg->setContentSize({ 50.f, 30.f });
	inputBg->setOpacity(100);
	inputBg->setPosition(winSize.width / 2.f, winSize.height / 2.f + 65.f);
	layer->addChild(inputBg);

	m_input = gd::CCTextInputNode::create(50.f, 40.f, "Num", this, "bigFont.fnt");
	m_input->setMaxLabelScale(.6f);
	m_input->setMaxLabelWidth(200.f);
	m_input->setAllowedChars("0123456789");
	m_input->setCharLimit(8);
	m_input->setLabelPlaceholderColor(ccc3(120, 170, 240));
	m_input->setLabelPlaceholderScale(.5f);
	m_input->setString(CCString::createWithFormat("%i", m_parent->m_searchObject->m_page + 1)->getCString());
	m_input->setPosition(inputBg->getPosition());
	layer->addChild(m_input, 20);

	auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	onCloseSpr->setScale(.8f);
	auto onClose = gd::CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(GoToPagePopup::onClose));
	onClose->setPosition(-100.f, 115.f);
	menu->addChild(onClose);

	auto onGoSpr = gd::ButtonSprite::create("OK", 0x28, 0, .8f, true, "goldFont.fnt", "GJ_button_01.png", 30.f);
	auto onGo = gd::CCMenuItemSpriteExtra::create(onGoSpr, this, menu_selector(GoToPagePopup::onGo));
	menu->addChild(onGo);

	auto onIncrementSpr = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
	onIncrementSpr->setScale(1.1f);
	auto onIncrement = gd::CCMenuItemSpriteExtra::create(onIncrementSpr, this, menu_selector(GoToPagePopup::onIncrement));
	onIncrement->setPosition(50.f, 56.f);
	menu->addChild(onIncrement);

	auto onDecrementSpr = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
	onDecrementSpr->setScale(1.1f);
	auto onDecrement = gd::CCMenuItemSpriteExtra::create(onDecrementSpr, this, menu_selector(GoToPagePopup::onDecrement));
	onDecrement->setPosition(-50.f, 56.f);
	menu->addChild(onDecrement);

	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);

	return true;
}

void GoToPagePopup::onIncrement(CCObject*) {
	m_input->setString(CCString::createWithFormat("%i", std::atoi(m_input->getString().c_str()) + 1)->getCString());
}

void GoToPagePopup::onDecrement(CCObject*) {
	m_input->setString(CCString::createWithFormat("%i", std::atoi(m_input->getString().c_str()) - 1)->getCString());
	if (std::atoi(m_input->getString().c_str()) < 1) {
		m_input->setString("1");
	}
}

void GoToPagePopup::keyBackClicked() {
	int inputPage = std::atoi(m_input->getString().c_str());
	int totalItems = m_parent->m_itemCount;
	int totalPages = (totalItems % 10 == 0) ? totalItems / 10 - 1 : totalItems / 10;

	m_parent->m_searchObject->m_page = inputPage - 1;

	if (inputPage < 1) {
		m_parent->m_searchObject->m_page = 0;
	}

	if (inputPage > totalPages) {
		m_parent->m_searchObject->m_page = totalPages;
	}

	m_parent->loadPage(m_parent->m_searchObject);

	this->setTouchEnabled(false);
	this->setKeypadEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void GoToPagePopup::onGo(CCObject*) {
	this->keyBackClicked();
}

void GoToPagePopup::onClose(CCObject*) {
	this->setTouchEnabled(false);
	this->setKeypadEnabled(false);
	this->removeFromParentAndCleanup(true);
}