#include "PulseBuilderPopup.hpp"
#include "LevelEditorLayer.hpp"

static float m_fadeIn = 0.f;
static float m_hold = 0.f;
static float m_fadeOut = 0.f;

bool PulseBuilderPopup::init() {
	m_fadeIn = 0.f;
	m_hold = 0.f;
	m_fadeOut = 0.f;

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
	bg->setContentSize({ 260.f, 160.f });
	bg->setPosition(winSize / 2.f);
	layer->addChild(bg, -1);

	auto title = CCLabelBMFont::create("Pulse Builder", "goldFont.fnt");
	title->setScale(.8f);
	title->setPosition(winSize.width / 2.f, winSize.height / 2.f + 65.f);
	layer->addChild(title);

	auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	auto onClose = gd::CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(PulseBuilderPopup::onClose));
	onClose->setPosition(-125.f, 75.f);
	menu->addChild(onClose);

	//

	auto fadeInLabel = CCLabelBMFont::create("Fade In:", "goldFont.fnt");
	fadeInLabel->setScale(.6f);
	fadeInLabel->setPosition(winSize.width / 2.f - 80.f, winSize.height / 2.f + 25.f);
	layer->addChild(fadeInLabel);

	auto fadeInBg = extension::CCScale9Sprite::create("square02_small.png");
	fadeInBg->setOpacity(100);
	fadeInBg->setPosition(winSize.width / 2.f - 80.f, winSize.height / 2.f);
	fadeInBg->setContentSize({ 50.f, 30.f });
	layer->addChild(fadeInBg, -1);

	m_fadeInInput = gd::CCTextInputNode::create(45.f, 30.f, "Num", this, "bigFont.fnt");
	m_fadeInInput->setDelegate(this);
	m_fadeInInput->setAllowedChars("0123456789.");
	m_fadeInInput->setPosition(winSize.width / 2.f - 80.f, winSize.height / 2.f);
	m_fadeInInput->setMaxLabelScale(.8f);
	m_fadeInInput->setLabelPlaceholderScale(.6f);
	m_fadeInInput->setLabelPlaceholderColor(ccGRAY);
	layer->addChild(m_fadeInInput);

	//

	auto holdLabel = CCLabelBMFont::create("Hold:", "goldFont.fnt");
	holdLabel->setScale(.6f);
	holdLabel->setPosition(winSize.width / 2.f, winSize.height / 2.f + 25.f);
	layer->addChild(holdLabel);

	auto holdBg = extension::CCScale9Sprite::create("square02_small.png");
	holdBg->setOpacity(100);
	holdBg->setPosition(winSize.width / 2.f, winSize.height / 2.f);
	holdBg->setContentSize({ 50.f, 30.f });
	layer->addChild(holdBg, -1);

	m_holdInput = gd::CCTextInputNode::create(45.f, 30.f, "Num", this, "bigFont.fnt");
	m_holdInput->setDelegate(this);
	m_holdInput->setAllowedChars("0123456789.");
	m_holdInput->setPosition(winSize.width / 2.f, winSize.height / 2.f);
	m_holdInput->setMaxLabelScale(.8f);
	m_holdInput->setLabelPlaceholderScale(.6f);
	m_holdInput->setLabelPlaceholderColor(ccGRAY);
	layer->addChild(m_holdInput);

	//

	auto fadeOutLabel = CCLabelBMFont::create("Fade Out:", "goldFont.fnt");
	fadeOutLabel->setScale(.6f);
	fadeOutLabel->setPosition(winSize.width / 2.f + 80.f, winSize.height / 2.f + 25.f);
	layer->addChild(fadeOutLabel);

	auto fadeOutBg = extension::CCScale9Sprite::create("square02_small.png");
	fadeOutBg->setOpacity(100);
	fadeOutBg->setPosition(winSize.width / 2.f + 80.f, winSize.height / 2.f);
	fadeOutBg->setContentSize({ 50.f, 30.f });
	layer->addChild(fadeOutBg, -1);

	m_fadeOutInput = gd::CCTextInputNode::create(45.f, 30.f, "Num", this, "bigFont.fnt");
	m_fadeOutInput->setDelegate(this);
	m_fadeOutInput->setAllowedChars("0123456789.");
	m_fadeOutInput->setPosition(winSize.width / 2.f + 80.f, winSize.height / 2.f);
	m_fadeOutInput->setMaxLabelScale(.8f);
	m_fadeOutInput->setLabelPlaceholderScale(.6f);
	m_fadeOutInput->setLabelPlaceholderColor(ccGRAY);
	layer->addChild(m_fadeOutInput);

	auto onCreateSpr = gd::ButtonSprite::create("Create", 0xdc, 0, .8f, false, "goldFont.fnt", "GJ_button_01.png", 30.f);
	auto onCreate = gd::CCMenuItemSpriteExtra::create(onCreateSpr, this, menu_selector(PulseBuilderPopup::onBuild));
	onCreate->setPositionY(-55.f);
	menu->addChild(onCreate);

	this->setKeypadEnabled(true);
	this->setTouchEnabled(true);

	return true;
}

void PulseBuilderPopup::textChanged(gd::CCTextInputNode* input) {
	if (input == m_fadeInInput) {
		m_fadeIn = std::atof(input->getString().c_str());
		std::cout << "Fade In: " << m_fadeIn << std::endl;
	}
	else if (input == m_holdInput) {
		m_hold = std::atof(input->getString().c_str());
		std::cout << "Hold: " << m_hold << std::endl;
	}
	else if (input == m_fadeOutInput) {
		m_fadeOut = std::atof(input->getString().c_str());
		std::cout << "Fade Out: " << m_fadeOut << std::endl;
	}
}

void PulseBuilderPopup::onBuild(CCObject*) {
	auto editorLayer = LevelEditorLayer::get();
	
	if (editorLayer == nullptr) return;

	auto editorUI = editorLayer->m_uiLayer;

	if (editorUI == nullptr) return;

	this->keyBackClicked();
	if (auto colorSelect = static_cast<gd::ColorSelectPopup*>(this->getUserData())) {
		colorSelect->closeColorSelect(nullptr);
	}

	bool fadeInTriggerUsed = false;

	if (m_fadeIn > 0.f) {
		editorUI->m_selectedObject->m_triggerDuration = m_fadeIn;
		fadeInTriggerUsed = true;
	}
	else {
		editorUI->m_selectedObject->m_triggerDuration = 0.f;
		fadeInTriggerUsed = false;
	}

	//if (m_hold > 0.f) {
	//	if (m_fadeIn > 0.f) {
	//		editorUI->onDuplicate(nullptr);
	//	}
	//	
	//	if (m_fadeIn > 0.f) {
	//		editorUI->moveObject(editorUI->m_selectedObject, { editorLayer->m_gridLayer->xPosForTime(m_fadeIn), 0.f });
	//	}
	//	else if (!fadeInTriggerUsed) {
	//		editorUI->moveObject(editorUI->m_selectedObject, { 0.f, 0.f });
	//	}
	//	else {
	//		editorUI->moveObject(editorUI->m_selectedObject, { 2.f, 0.f });
	//	}
	//}
	//else {
	//	if (m_fadeIn <= 0.f) {
	//		editorUI->m_selectedObject->m_triggerDuration = 0.f;
	//	}
	//}

	if (m_fadeOut > 0.f) {
		editorUI->onDuplicate(nullptr);
		editorUI->m_selectedObject->m_triggerDuration = m_fadeOut;
		if ((m_fadeIn > 0.f) || (m_hold > 0.f)) {
			editorUI->moveObject(editorUI->m_selectedObject, { editorLayer->m_gridLayer->xPosForTime(m_fadeIn + m_hold), 0.f });
		}
		else {
			editorUI->moveObject(editorUI->m_selectedObject, { 2.f, 0.f });
		}
	}
}

void PulseBuilderPopup::keyBackClicked() {
	this->setTouchEnabled(false);
	this->setKeypadEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void PulseBuilderPopup::onClose(CCObject*) {
	this->keyBackClicked();
}