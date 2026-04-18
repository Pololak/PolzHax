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

void EditorOptionsLayer::onShowClicks(CCObject*) {
	setting().onShowClicks = !setting().onShowClicks;
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

	auto onCopyString = gd::CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(EditorOptionsLayer::onCopyString));
	onCopyString->toggle(setting().onCopyString);
	onCopyString->setPosition(-160.f, 80.f);
	menu->addChild(onCopyString);
	auto onCopyStringLabel = CCLabelBMFont::create("Copy String", "bigFont.fnt");
	onCopyStringLabel->limitLabelWidth(130.f, .5f, 0.f);
	onCopyStringLabel->setAnchorPoint({ 0.f, .5f });
	onCopyStringLabel->setPosition(winSize.width / 2.f - 138.f, winSize.height / 2.f + 80.f);
	layer->addChild(onCopyStringLabel);

	auto onDurationLines = gd::CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(EditorOptionsLayer::onDurationLines));
	onDurationLines->toggle(setting().onDurationLines);
	onDurationLines->setPosition(32.f, 80.f);
	menu->addChild(onDurationLines);
	auto onDurationLinesLabel = CCLabelBMFont::create("Duration Lines", "bigFont.fnt");
	onDurationLinesLabel->limitLabelWidth(130.f, .5f, 0.f);
	onDurationLinesLabel->setAnchorPoint({ 0.f, .5f });
	onDurationLinesLabel->setPosition(winSize.width / 2.f + 54.f, winSize.height / 2.f + 80.f);
	layer->addChild(onDurationLinesLabel);

	auto onExperimentalLayering = gd::CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(EditorOptionsLayer::onExperimentalLayering));
	onExperimentalLayering->toggle(setting().onExperimentalLayering);
	onExperimentalLayering->setPosition(-160.f, 40.f);
	menu->addChild(onExperimentalLayering);
	auto onExperimentalLayeringLabel = CCLabelBMFont::create("Experimental Layering", "bigFont.fnt");
	onExperimentalLayeringLabel->limitLabelWidth(130.f, .5f, 0.f);
	onExperimentalLayeringLabel->setAnchorPoint({ 0.f, .5f });
	onExperimentalLayeringLabel->setPosition(winSize.width / 2.f - 138.f, winSize.height / 2.f + 40.f);
	layer->addChild(onExperimentalLayeringLabel);

	auto onPreviewRotations = gd::CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(EditorOptionsLayer::onPreviewRotations));
	onPreviewRotations->toggle(setting().onPreviewRotations);
	onPreviewRotations->setPosition(32.f, 40.f);
	menu->addChild(onPreviewRotations);
	auto onPreviewRotationsLabel = CCLabelBMFont::create("Preview Rotations", "bigFont.fnt");
	onPreviewRotationsLabel->limitLabelWidth(130.f, .5f, 0.f);
	onPreviewRotationsLabel->setAnchorPoint({ 0.f, .5f });
	onPreviewRotationsLabel->setPosition(winSize.width / 2.f + 54.f, winSize.height / 2.f + 40.f);
	layer->addChild(onPreviewRotationsLabel);

	auto onShowClicks = gd::CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(EditorOptionsLayer::onShowClicks));
	onShowClicks->toggle(setting().onShowClicks);
	onShowClicks->setPosition(-160.f, 0.f);
	menu->addChild(onShowClicks);
	auto onShowClicksLabel = CCLabelBMFont::create("Show Clicks", "bigFont.fnt");
	onShowClicksLabel->limitLabelWidth(130.f, .5f, 0.f);
	onShowClicksLabel->setAnchorPoint({ 0.f, .5f });
	onShowClicksLabel->setPosition(winSize.width / 2.f - 138.f, winSize.height / 2.f);
	layer->addChild(onShowClicksLabel);

	auto onUnusedObjects = gd::CCMenuItemToggler::create(toggleOff, toggleOn, this, menu_selector(EditorOptionsLayer::onUnusedObjects));
	onUnusedObjects->toggle(setting().onUnusedObjects);
	onUnusedObjects->setPosition(32.f, 0.f);
	menu->addChild(onUnusedObjects);
	auto onUnusedObjectsLabel = CCLabelBMFont::create("Show Unused Objects", "bigFont.fnt");
	onUnusedObjectsLabel->limitLabelWidth(130.f, .5f, 0.f);
	onUnusedObjectsLabel->setAnchorPoint({ 0.f, .5f });
	onUnusedObjectsLabel->setPosition(winSize.width / 2.f + 54.f, winSize.height / 2.f);
	layer->addChild(onUnusedObjectsLabel);

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