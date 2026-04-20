#include "RemapTriggerWidget.hpp"
#include "LevelEditorLayer.hpp"

bool RemapTriggerWidget::init(gd::ColorSelectPopup* parent) {
	m_parent = parent;
	if (!CCLayer::init()) return false;

	auto labelBg = extension::CCScale9Sprite::create("square02_small.png");
	labelBg->setContentSize({ 40.f, 30.f });
	labelBg->setOpacity(100);
	this->addChild(labelBg);

	m_label = CCLabelBMFont::create("", "bigFont.fnt");
	this->addChild(m_label);

	auto menu = CCMenu::create();
	menu->setPosition(0.f, 0.f);
	this->addChild(menu);

	auto onTypeUpSpr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
	auto onTypeUp = gd::CCMenuItemSpriteExtra::create(onTypeUpSpr, this, menu_selector(RemapTriggerWidget::onTypeUp));
	onTypeUp->setPositionY(30.f);
	menu->addChild(onTypeUp);

	auto onTypeDownSpr = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
	auto onTypeDown = gd::CCMenuItemSpriteExtra::create(onTypeDownSpr, this, menu_selector(RemapTriggerWidget::onTypeDown));
	onTypeDown->setPositionY(-30.f);
	menu->addChild(onTypeDown);

	this->updateLabel();

	return true;
}

void RemapTriggerWidget::updateLabel() {
	if (m_label) {
		m_label->limitLabelWidth(36.f, .8f, .1f);
	}
}

void RemapTriggerWidget::remapTrigger() {
	auto editorLayer = LevelEditorLayer::get();

	if (editorLayer) {

	}
}

void RemapTriggerWidget::onTypeUp(CCObject*) {
	this->updateLabel();
}

void RemapTriggerWidget::onTypeDown(CCObject*) {
	this->updateLabel();
}