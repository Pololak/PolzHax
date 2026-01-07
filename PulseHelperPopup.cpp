#include "PulseHelperPopup.h"
#include "utils.hpp"

void PulseHelperPopup::onPulseHelper(CCObject* obj) {
	gd::ColorSelectPopup* popup = static_cast<gd::ColorSelectPopup*>(static_cast<gd::CCMenuItemSpriteExtra*>(obj)->getParent()->getParent()->getParent());
	PulseHelperPopup::create(popup)->show();
}

void PulseHelperPopup::keyBackClicked() {
	this->setTouchEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void PulseHelperPopup::onClose(CCObject*) {
	this->keyBackClicked();
}

bool PulseHelperPopup::init(gd::ColorSelectPopup* popup) {
	m_colorSelectPopup = popup;
	if (!this->initWithColor({ 0, 0, 0, 75 })) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();
	director->m_pTouchDispatcher->incrementForcePrio();
	this->m_bTouchEnabled = true;
	this->m_bKeypadEnabled = true;
	this->registerWithTouchDispatcher();

	auto layer = CCLayer::create();
	auto menu = CCMenu::create();
	this->m_mainLayer = layer;
	this->m_buttonMenu = menu;

	layer->addChild(menu);
	this->addChild(layer);

	auto thathorriblecrashfixspr = CCSprite::create("GJ_button_01.png");
	thathorriblecrashfixspr->setScale(100.f);
	thathorriblecrashfixspr->setOpacity(0);
	auto thathorriblecrashfixbtn = gd::CCMenuItemSpriteExtra::create(thathorriblecrashfixspr, thathorriblecrashfixspr, this, nullptr);
	menu->addChild(thathorriblecrashfixbtn, 5);

	auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
	bg->setContentSize({ 260.f, 160.f });
	bg->setPosition(winSize / 2.f);
	layer->addChild(bg, -2);

	auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	auto onClose = gd::CCMenuItemSpriteExtra::create(onCloseSpr, nullptr, this, menu_selector(PulseHelperPopup::onClose));
	onClose->setPosition(-125.f, 75.f);
	menu->addChild(onClose);

	auto title = CCLabelBMFont::create("Pulse Helper", "goldFont.fnt");
	title->setScale(.8f);
	title->setPosition(winSize.width / 2.f, winSize.height / 2.f + 65.f);
	layer->addChild(title);

	auto fadeInLabel = CCLabelBMFont::create("Fade In:", "goldFont.fnt");
	fadeInLabel->setScale(.6f);
	fadeInLabel->setPosition(winSize.width / 2.f - 80.f, winSize.height / 2.f + 25.f);
	layer->addChild(fadeInLabel);

	auto fadeInBg = extension::CCScale9Sprite::create("square02_small.png");
	fadeInBg->setOpacity(100);
	fadeInBg->setPosition(winSize.width / 2.f - 80.f, winSize.height / 2.f);
	fadeInBg->setContentSize({ 50.f, 30.f });
	layer->addChild(fadeInBg, -1);

	m_fadeInInput = gd::CCTextInputNode::create("Num", this, "bigFont.fnt", 45.f, 30.f);
	m_fadeInInput->setDelegate(this);
	m_fadeInInput->setAllowedChars("0123456789.");
	m_fadeInInput->setPosition(winSize.width / 2.f - 80.f, winSize.height / 2.f);
	m_fadeInInput->setMaxLabelScale(.8f);
	m_fadeInInput->setLabelPlaceholderScale(.6f);
	m_fadeInInput->setLabelPlaceholderColor(cocos2d::ccGRAY);
	layer->addChild(m_fadeInInput);

	auto holdLabel = CCLabelBMFont::create("Hold:", "goldFont.fnt");
	holdLabel->setScale(.6f);
	holdLabel->setPosition(winSize.width / 2.f, winSize.height / 2.f + 25.f);
	layer->addChild(holdLabel);

	auto holdBg = extension::CCScale9Sprite::create("square02_small.png");
	holdBg->setOpacity(100);
	holdBg->setPosition(winSize.width / 2.f, winSize.height / 2.f);
	holdBg->setContentSize({ 50.f, 30.f });
	layer->addChild(holdBg, -1);

	m_holdInput = gd::CCTextInputNode::create("Num", this, "bigFont.fnt", 45.f, 30.f);
	m_holdInput->setDelegate(this);
	m_holdInput->setAllowedChars("0123456789.");
	m_holdInput->setPosition(winSize.width / 2.f, winSize.height / 2.f);
	m_holdInput->setMaxLabelScale(.8f);
	m_holdInput->setLabelPlaceholderScale(.6f);
	m_holdInput->setLabelPlaceholderColor(cocos2d::ccGRAY);
	layer->addChild(m_holdInput);

	auto fadeOutLabel = CCLabelBMFont::create("Fade Out:", "goldFont.fnt");
	fadeOutLabel->setScale(.6f);
	fadeOutLabel->setPosition(winSize.width / 2.f + 80.f, winSize.height / 2.f + 25.f);
	layer->addChild(fadeOutLabel);

	auto fadeOutBg = extension::CCScale9Sprite::create("square02_small.png");
	fadeOutBg->setOpacity(100);
	fadeOutBg->setPosition(winSize.width / 2.f + 80.f, winSize.height / 2.f);
	fadeOutBg->setContentSize({ 50.f, 30.f });
	layer->addChild(fadeOutBg, -1);

	m_fadeOutInput = gd::CCTextInputNode::create("Num", this, "bigFont.fnt", 45.f, 30.f);
	m_fadeOutInput->setDelegate(this);
	m_fadeOutInput->setAllowedChars("0123456789.");
	m_fadeOutInput->setPosition(winSize.width / 2.f + 80.f, winSize.height / 2.f);
	m_fadeOutInput->setMaxLabelScale(.8f);
	m_fadeOutInput->setLabelPlaceholderScale(.6f);
	m_fadeOutInput->setLabelPlaceholderColor(cocos2d::ccGRAY);
	layer->addChild(m_fadeOutInput);

	auto onCreateSpr = gd::ButtonSprite::create("Create", 0xdc, 0, .8f, false, "goldFont.fnt", "GJ_button_01.png", 30.f);
	auto onCreate = gd::CCMenuItemSpriteExtra::create(onCreateSpr, nullptr, this, menu_selector(PulseHelperPopup::onCreate));
	onCreate->setPositionY(-55.f);
	menu->addChild(onCreate);

	return true;
}

void PulseHelperPopup::textChanged(gd::CCTextInputNode* input) {
	if (input == m_fadeInInput || input == m_holdInput || input == m_fadeOutInput) {
		if (input == m_fadeInInput) std::cout << "Fade In: " << std::atof(input->getString()) << std::endl;
		if (input == m_holdInput) std::cout << "Hold: " << std::atof(input->getString()) << std::endl;
		if (input == m_fadeOutInput) std::cout << "Fade Out: " << std::atof(input->getString()) << std::endl;
	}
}

void PulseHelperPopup::onCreate(CCObject*) {
	gd::LevelEditorLayer* levelEditor = static_cast<gd::LevelEditorLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0));
	std::cout << levelEditor << std::endl;
	std::cout << m_colorSelectPopup << std::endl;

	float fadeIn = std::atof(m_fadeInInput->getString());
	float hold = std::atof(m_holdInput->getString());

	float newObjX;

	if ((fadeIn + hold) <= 0.f) {
		newObjX = m_colorSelectPopup->m_targetObject->getPositionX() + 2.f; // less than 2 just wouldn't work
	}
	else if (fadeIn > 0.f) {

	}

	CCPoint pos = ccp(newObjX, m_colorSelectPopup->m_targetObject->getPositionY());
	gd::GameObject* obj = levelEditor->createObject(m_colorSelectPopup->m_targetObject->m_objectID, pos);
	obj->m_triggerDuration = std::atof(m_fadeOutInput->getString());

	this->keyBackClicked();
}