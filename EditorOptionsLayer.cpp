#include "EditorOptionsLayer.hpp"
#include "Setting.hpp"
#include "LevelEditorLayer.hpp"
#include "RotateSaws.hpp"

void EditorOptionsLayer::onCopyString(CCObject*) {
	setting().onCopyString = !setting().onCopyString;
}

void EditorOptionsLayer::onDurationLines(CCObject*) {
	setting().onDurationLines = !setting().onDurationLines;
}

void EditorOptionsLayer::onExperimentalLayering(CCObject*) {
	setting().onExperimentalLayering = !setting().onExperimentalLayering;
}

void EditorOptionsLayer::onLinkControls(CCObject*) {
	setting().onLinkControls = !setting().onLinkControls;
	if (LevelEditorLayer::get()) {
		LevelEditorLayer::get()->m_uiLayer->updateButtons();
	}
}

void EditorOptionsLayer::onUnusedObjects(CCObject*) {
	setting().onUnusedObjects = !setting().onUnusedObjects;
}

void EditorOptionsLayer::onPreviewRotations(CCObject*) {
	setting().onPreviewRotations = !setting().onPreviewRotations;
	if (setting().onPreviewRotations) {
		RotateSaws::beginRotations(LevelEditorLayer::get());
	}
	else {
		RotateSaws::stopRotations(LevelEditorLayer::get());
	}
}

void EditorOptionsLayer::onNewColorSelect(CCObject*) {
	setting().onNewColorSelectMenu = !setting().onNewColorSelectMenu;
}

void EditorOptionsLayer::onShowClicks(CCObject*) {
	setting().onShowClicks = !setting().onShowClicks;
}

void EditorOptionsLayer::createToggleButton(std::string caption, cocos2d::SEL_MenuHandler callback, bool toggle, cocos2d::CCMenu* menu, cocos2d::CCPoint position) {
	CCSprite* toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
	toggleOff->setScale(.8f);
	CCSprite* toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
	toggleOn->setScale(.8f);

	auto toggler = gd::CCMenuItemToggler::create(toggleOff, toggleOn, this, callback);
	toggler->toggle(toggle);
	toggler->setPosition(position);
	menu->addChild(toggler);
	auto label = CCLabelBMFont::create(caption.c_str(), "bigFont.fnt");
	label->limitLabelWidth(130.f, .5f, 0.f);
	label->setAnchorPoint({ 0.f, .5f });
	auto convertedPos = menu->convertToWorldSpace(toggler->getPosition());
	label->setPosition(convertedPos.x + 18.f, convertedPos.y);
	this->m_mainLayer->addChild(label);
}

bool EditorOptionsLayer::init() {
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
	bg->setContentSize({ 400.f, 280.f });
	bg->setPosition(winSize / 2.f);
	layer->addChild(bg, -1);

	auto title = CCLabelBMFont::create("Options", "bigFont.fnt");
	title->setScale(.8f);
	title->setPosition(winSize.width / 2.f, winSize.height / 2.f + 124.f);
	layer->addChild(title);

	auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	auto onClose = gd::CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(EditorOptionsLayer::onClose));
	onClose->setPosition(-195.f, 135.f);
	menu->addChild(onClose);

	CCSprite* toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
	toggleOff->setScale(.8f);
	CCSprite* toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
	toggleOn->setScale(.8f);

	this->createToggleButton(
		"Copy String",
		menu_selector(EditorOptionsLayer::onCopyString),
		setting().onCopyString,
		menu,
		ccp(-160.f, 80.f)
	);

	this->createToggleButton(
		"Duration Lines",
		menu_selector(EditorOptionsLayer::onDurationLines),
		setting().onDurationLines,
		menu,
		ccp(32.f, 80.f)
	);

	this->createToggleButton(
		"Experimental Layering",
		menu_selector(EditorOptionsLayer::onExperimentalLayering),
		setting().onExperimentalLayering,
		menu,
		ccp(-160.f, 40.f)
	);

	this->createToggleButton(
		"Link Controls (Exp)",
		menu_selector(EditorOptionsLayer::onLinkControls),
		setting().onLinkControls,
		menu,
		ccp(32.f, 40.f)
	);

	this->createToggleButton(
		"New Color Select",
		menu_selector(EditorOptionsLayer::onNewColorSelect),
		setting().onNewColorSelectMenu,
		menu,
		ccp(-160.f, 0.f)
	);

	this->createToggleButton(
		"Preview Rotations",
		menu_selector(EditorOptionsLayer::onPreviewRotations),
		setting().onPreviewRotations,
		menu,
		ccp(32.f, 0.f)
	);

	this->createToggleButton(
		"Show Clicks",
		menu_selector(EditorOptionsLayer::onShowClicks),
		setting().onShowClicks,
		menu,
		ccp(-160.f, -40.f)
	);

	this->createToggleButton(
		"Unused Objects",
		menu_selector(EditorOptionsLayer::onUnusedObjects),
		setting().onUnusedObjects,
		menu,
		ccp(32.f, -40.f)
	);

	this->setKeypadEnabled(true);
	this->setTouchEnabled(true);

	return true;
}

void EditorOptionsLayer::keyBackClicked() {
	this->setTouchEnabled(false);
	this->setKeypadEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void EditorOptionsLayer::onClose(CCObject*) {
	this->keyBackClicked();
}