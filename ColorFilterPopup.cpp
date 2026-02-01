#include "ColorFilterPopup.hpp"
#include "Setting.hpp"
#include <unordered_set>
#include "EditorUI.hpp"

std::string ColorFilterPopup::colorToString(int id) {
	switch (id) {
	case 0: return "D"; break;
	case 1: return "P1"; break;
	case 2: return "P2"; break;
	case 3: return "Col1"; break;
	case 4: return "Col2"; break;
	case 5: return "LBG"; break;
	case 6: return "Col3"; break;
	case 7: return "Col4"; break;
	case 8: return "3DL"; break;
	case 9: return "W"; break;
	default: return "Unk"; break;
	}
}

const int colors[] = {
	0, 1, 2, 5, 3, 4, 6, 7, 8, 9
};

bool ColorFilterPopup::init(gd::EditorUI* parent) {
	m_parent = parent;
	m_color = setting().colorFilterOrder;
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
	bg->setContentSize({ 220.f, 150.f });
	bg->setPosition(winSize / 2.f);
	layer->addChild(bg, -1);

	auto title = CCLabelBMFont::create("Color Filter", "goldFont.fnt");
	title->setScale(.7f);
	title->setPosition(winSize.width / 2.f, winSize.height / 2.f + 57.f);
	layer->addChild(title);

	auto onCancelSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	onCancelSpr->setScale(.8f);
	auto onCancel = gd::CCMenuItemSpriteExtra::create(onCancelSpr, this, menu_selector(ColorFilterPopup::onCancel));
	onCancel->setPosition(-100.f, 65.f);
	menu->addChild(onCancel);

	auto onCloseSpr = gd::ButtonSprite::create("OK", 40, 0, .8f, true, "goldFont.fnt", "GJ_button_01.png", 30.f);
	auto onClose = gd::CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(ColorFilterPopup::onClose));
	onClose->setPositionY(-50.f);
	menu->addChild(onClose);

	auto textBg = extension::CCScale9Sprite::create("square02_small.png");
	textBg->setContentSize({ 50.f, 30.f });
	textBg->setPosition(winSize.width / 2.f, winSize.height / 2.f + 5.f);
	textBg->setOpacity(100);
	layer->addChild(textBg);

	m_label = CCLabelBMFont::create("", "bigFont.fnt");
	m_label->setPosition(winSize.width / 2.f, winSize.height / 2.f + 5.f);
	layer->addChild(m_label);
	this->updateLabel();

	auto onNextSpr = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
	auto onNext = gd::CCMenuItemSpriteExtra::create(onNextSpr, this, menu_selector(ColorFilterPopup::onNext));
	onNext->setPosition(45.f, 5.f);
	menu->addChild(onNext);

	auto onPrevSpr = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
	auto onPrev = gd::CCMenuItemSpriteExtra::create(onPrevSpr, this, menu_selector(ColorFilterPopup::onPrev));
	onPrev->setPosition(-45.f, 5.f);
	menu->addChild(onPrev);

	auto onResetSpr = CCSprite::create("GJ_trashBtn_001.png");
	onResetSpr->setScale(.75f);
	auto onReset = gd::CCMenuItemSpriteExtra::create(onResetSpr, this, menu_selector(ColorFilterPopup::onReset));
	onReset->setPosition(85.f, 50.f);
	menu->addChild(onReset);

	this->setKeypadEnabled(true);
	this->setTouchEnabled(true);

	return true;
}

void ColorFilterPopup::onReset(CCObject*) {
	m_color = 0;
	this->updateLabel();
}

void ColorFilterPopup::onNext(CCObject*) {
	m_color++;
	if (m_color > 9) m_color = 9;

	this->updateLabel();
}

void ColorFilterPopup::onPrev(CCObject*) {
	m_color--;
	if (m_color < 0) m_color = 0;

	this->updateLabel();
}

void ColorFilterPopup::updateLabel() {
	if (m_label) {
		m_label->setString(this->colorToString(colors[m_color]).c_str());
		m_label->limitLabelWidth(45.f, .6f, .1f);
	}

	std::cout << "Colors[]: " << colors[m_color] << "\n";
	std::cout << "m_color: " << m_color << "\n";
	std::cout << "setting().colorFilter: " << setting().colorFilter << "\n";
}

void ColorFilterPopup::keyBackClicked() {
	setting().colorFilter = colors[m_color];
	setting().colorFilterOrder = m_color;

	if (EditorUI::get()) {
		auto button = static_cast<gd::CCMenuItemSpriteExtra*>(EditorUI::get()->m_deleteMenu->getChildByTag(23));
		if (button) {
			auto buttonSprite = static_cast<gd::ButtonSprite*>(button->getChildren()->objectAtIndex(0));
			if (buttonSprite) {
				buttonSprite->m_label->setString(EditorUI::colorToShortString(colors[m_color]).c_str());
				buttonSprite->m_label->limitLabelWidth(32.f, .75f, .1f);
			}
		}
	}

	std::cout << "Colors[]: " << colors[m_color] << "\n";
	std::cout << "m_color: " << m_color << "\n";
	std::cout << "setting().colorFilter: " << setting().colorFilter << "\n";

	this->setTouchEnabled(false);
	this->setKeypadEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void ColorFilterPopup::onCancel(CCObject*) {
	this->setTouchEnabled(false);
	this->setKeypadEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void ColorFilterPopup::onClose(CCObject*) {
	this->keyBackClicked();
}