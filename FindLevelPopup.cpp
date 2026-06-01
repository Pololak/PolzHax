#include "FindLevelPopup.hpp"
#include "LevelBrowserLayer.hpp"

bool FindLevelPopup::init(gd::LevelBrowserLayer* parent) {
	m_parent = parent;
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
	bg->setContentSize({ 300.f, 150.f });
	bg->setPosition({ winSize.width / 2.f, winSize.height / 2.f + 60.f });
	layer->addChild(bg, -2);

	auto title = CCLabelBMFont::create("Find Level", "goldFont.fnt");
	title->setScale(.7f);
	title->setPosition({ winSize.width / 2.f, winSize.height / 2.f + 120.f });
	layer->addChild(title);

	auto inputBg = extension::CCScale9Sprite::create("square02_small.png");
	inputBg->setContentSize({ 200.f, 30.f });
	inputBg->setOpacity(100);
	inputBg->setColor({ 0, 0, 0 });
	inputBg->setPosition(bg->getPosition());
	layer->addChild(inputBg, -1);

	m_input = gd::CCTextInputNode::create(200.f, 40.f, "Search", this, "bigFont.fnt");
	m_input->setMaxLabelScale(.6f);
	m_input->setMaxLabelWidth(190.f);
	m_input->refreshLabel();
	m_input->setAllowedChars(" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,-!?:;)(/\\\"'`*=+-_%[]<>|@&^#{}%$~");
	m_input->setLabelPlaceholderColor({ 120, 170, 240 });
	m_input->setLabelPlaceholderScale(.5f);
	m_input->setPosition(bg->getPosition());
	m_input->setDelegate(this);
	layer->addChild(m_input, 20);

	auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	onCloseSpr->setScale(.8f);
	auto onClose = gd::CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(FindLevelPopup::onClose));
	onClose->setPosition({ -140.f, 115.f });
	menu->addChild(onClose);

	auto onSearchSpr = gd::ButtonSprite::create("OK", 0x28, 0, .8f, true, "goldFont.fnt", "GJ_button_01.png", 30.f);
	auto onSearch = gd::CCMenuItemSpriteExtra::create(onSearchSpr, this, menu_selector(FindLevelPopup::onSearch));
	menu->addChild(onSearch);

	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);

	return true;
}

void FindLevelPopup::onSearch(CCObject*) {
	if (m_parent && !this->m_input->getString().empty()) {
		m_parent->m_searchObject->m_searchQuery = this->m_input->getString();
		m_parent->m_searchObject->m_page = 0;
		m_parent->loadPage(m_parent->m_searchObject);
	}

	this->keyBackClicked();
}

void FindLevelPopup::keyBackClicked() {
	this->setTouchEnabled(false);
	this->setKeypadEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void FindLevelPopup::onClose(CCObject*) {
	this->keyBackClicked();
}

