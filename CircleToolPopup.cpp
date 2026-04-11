//#include "CircleToolPopup.hpp"
//
//static float m_angle = 180.f;
//static float m_step = 5.f;
//
//bool CircleToolPopup::init() {
//	if (!this->initWithColor({ 0, 0, 0, 75 })) return false;
//
//	auto* director = CCDirector::sharedDirector();
//	auto winSize = director->getWinSize();
//	director->getTouchDispatcher()->incrementForcePrio();
//	this->registerWithTouchDispatcher();
//
//	auto layer = CCLayer::create();
//	auto menu = CCMenu::create();
//	this->m_mainLayer = layer;
//	this->m_buttonMenu = menu;
//
//	layer->addChild(menu);
//	this->addChild(layer);
//
//	auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
//	bg->setContentSize({ 300.f, 220.f });
//	bg->setPosition(winSize / 2.f);
//	layer->addChild(bg, -2);
//
//	auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
//	onCloseSpr->setScale(.8f);
//	auto onClose = gd::CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(CircleToolPopup::onClose));
//	onClose->setPosition(-145.f, 105.f);
//	menu->addChild(onClose);
//
//	auto title = CCLabelBMFont::create("Circle Tool", "goldFont.fnt");
//	title->setPosition(winSize.width / 2.f, winSize.height / 2.f + 95.f);
//	title->setScale(.75f);
//	layer->addChild(title);
//
//	auto arcLabel = CCLabelBMFont::create("Arc", "goldFont.fnt");
//	arcLabel->setPosition(winSize.width / 2.f - 60.f, winSize.height / 2.f + 64.f);
//	arcLabel->setScale(.75f);
//	layer->addChild(arcLabel);
//
//	m_angleInput = gd::CCTextInputNode::create(60, 30, "Val", this, "bigFont.fnt");
//	m_angleInput->setString(CCString::createWithFormat("%.0f", m_angle)->getCString());
//	m_angleInput->m_delegate = this;
//	m_angleInput->setPosition(winSize.width / 2.f - 60.f, winSize.height / 2.f + 38.f);
//	m_angleInput->setAllowedChars("0123456789.");
//	m_angleInput->setMaxLabelScale(.8f);
//	m_angleInput->setLabelPlaceholderScale(.6f);
//	m_angleInput->setLabelPlaceholderColor(ccGRAY);
//	layer->addChild(m_angleInput);
//
//	this->setKeypadEnabled(true);
//	this->setTouchEnabled(true);
//
//	return true;
//}