#include "EditorSettingsLayer.hpp"
#include "GameVariables.hpp"
#include "RotatingSaws.hpp"
#include "preview-mode.hpp"

CCLayer* editorSettingsLayer;

EditorSettingsLayer* EditorSettingsLayer::create() {
	EditorSettingsLayer* pRet = new EditorSettingsLayer();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

auto copyStringToggle(CCSprite* toggleOn, CCSprite* toggleOff) {
	bool copyString_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::COPY_STRING);
	return (copyString_enabled) ? toggleOn : toggleOff;
}

void EditorSettingsLayer::onCopyString(CCObject*) {
	gd::GameManager::sharedState()->toggleGameVariable(GameVariable::COPY_STRING);
}

void EditorSettingsLayer::onCopyStringInfo(CCObject*) {
	gd::FLAlertLayer::create(nullptr, "Copy Objects String", "When you copy objects, the object string\nwill be copied on your <cr>clipboard</c>.", "OK", nullptr, 320.f, false, 160.f)->show();
}

auto selectFilterToggle(CCSprite* toggleOn, CCSprite* toggleOff) {
	bool selectFilter_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::SELECT_FILTER);
	return (selectFilter_enabled) ? toggleOn : toggleOff;
}

void EditorSettingsLayer::onSelectFilter(CCObject*) {
	gd::GameManager::sharedState()->toggleGameVariable(GameVariable::SELECT_FILTER);
}

void EditorSettingsLayer::onSelectFilterInfo(CCObject*) {
	gd::FLAlertLayer::create(nullptr, "Select Filter", "Allows you to use filters to select objects.", "OK", nullptr, 320.f, false, 160.f)->show();
}

auto globalClipboardToggle(CCSprite* toggleOn, CCSprite* toggleOff) {
	bool globalClipboard_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::GLOBAL_CLIPBOARD);
	return (globalClipboard_enabled) ? toggleOn : toggleOff;
}

void EditorSettingsLayer::onGlobalClipboard(CCObject*) {
	gd::GameManager::sharedState()->toggleGameVariable(GameVariable::GLOBAL_CLIPBOARD);
}

void EditorSettingsLayer::onGlobalClipboardInfo(CCObject*) {
	gd::FLAlertLayer::create(nullptr, "Global Clipboard", "Allows you to copy objects between levels.", "OK", nullptr, 320.f, false, 160.f)->show();
}

auto showObjectInfoToggle(CCSprite* toggleOn, CCSprite* toggleOff) {
	bool showObjInfo_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::SHOW_OBJ_INFO);
	return (showObjInfo_enabled) ? toggleOn : toggleOff;
}

void EditorSettingsLayer::onShowObjInfo(CCObject*) {
	gd::GameManager::sharedState()->toggleGameVariable(GameVariable::SHOW_OBJ_INFO);
	EditorUI::updateObjectInfo();
}

//void EditorSettingsLayer::onShowObjInfoInfo(CCObject*) {
//	gd::FLAlertLayer::create(nullptr, "Show Object Info", "Enables object info (like in 2.0).", "OK", nullptr, 320.f, false, 160.f)->show();
//}

auto previewAnimationsInfoToggle(CCSprite* toggleOn, CCSprite* toggleOff) {
	bool previewAnims_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::PREV_ANIMS);
	return (previewAnims_enabled) ? toggleOn : toggleOff;
}

void EditorSettingsLayer::onPrevAnimations(CCObject*) {
	gd::GameManager::sharedState()->toggleGameVariable(GameVariable::PREV_ANIMS);
	auto levelEditorLayer = reinterpret_cast<gd::LevelEditorLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0));
	bool previewAnims_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::PREV_ANIMS);
	if (previewAnims_enabled) {
		RotateSaws::beginRotations(levelEditorLayer);
	}
	else {
		RotateSaws::stopRotations(levelEditorLayer);
	}
}

//void EditorSettingsLayer::onPrevAnimationsInfo(CCObject*) {
//	gd::FLAlertLayer::create(nullptr, "Preview Animations", "Enables rotations on objects (saws).\n<cr>(Buggy because of Preview Mode)\n(also don't copypaste rotating obj)</c>", "OK", nullptr, 320.f, false, 160.f)->show();
//}

auto hidePreviewLineToggle(CCSprite* toggleOn, CCSprite* toggleOff) {
	bool hidePreviewLine_enabled = gd::GameManager::sharedState()->getGameVariable(GameVariable::HIDE_PREVIEW_LINE);
	return (hidePreviewLine_enabled) ? toggleOn : toggleOff;
}

void EditorSettingsLayer::onHidePreviewLine(CCObject* obj) {
	gd::GameManager::sharedState()->toggleGameVariable(GameVariable::HIDE_PREVIEW_LINE);
	auto lel = reinterpret_cast<gd::LevelEditorLayer*>(from<CCLayer*>(obj, 0xFC)->getParent()->getChildren()->objectAtIndex(0));
	auto previewLine = reinterpret_cast<CCSprite*>(lel->getChildByTag(87601));
	if (gd::GameManager::sharedState()->getGameVariable(GameVariable::HIDE_PREVIEW_LINE)) {
		previewLine->setVisible(0);
	}
	else {
		previewLine->setVisible(1);
	}
}

auto eltog(CCSprite* toggleOn, CCSprite* toggleOff) {
	bool el = gd::GameManager::sharedState()->getGameVariable(GameVariable::EXPERIMENTAL_LAYERING);
	return (el) ? toggleOn : toggleOff;
}

void EditorSettingsLayer::onExperimentalLayering(CCObject*) {
	gd::GameManager::sharedState()->toggleGameVariable(GameVariable::EXPERIMENTAL_LAYERING);
}

//void EditorSettingsLayer::onHidePreviewLineInfo(CCObject*) {
//	gd::FLAlertLayer::create(nullptr, "Link Controls", "Enables option to link objects together in the editor. Linked objects are always selected and deleted together.", "OK", nullptr, 320.f, false, 160.f)->show();
//}

bool EditorSettingsLayer::init() {
	if (!CCLayer::init()) return false;
	const float width = 360, height = 240;
	CCLayerColor* CCLayerCol = CCLayerColor::create(ccc4(0, 0, 0, 0));
	CCLayerCol->setZOrder(1);
	CCLayerCol->setScale(10.f);
	this->addChild(CCLayerCol);
	auto actionColor = CCFadeTo::create(0.1f, 75);
	CCLayerCol->runAction(actionColor);

	auto touchDispatcher = CCDirector::sharedDirector()->m_pTouchDispatcher;
	touchDispatcher->incrementForcePrio();
	this->registerWithTouchDispatcher();
	setTouchEnabled(true);
	setKeypadEnabled(true);
	setMouseEnabled(true);

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto bgSprite = CCSprite::create("GJ_button_03.png");
	bgSprite->setScale(100.f);
	bgSprite->setOpacity(0);
	auto bgButton = gd::CCMenuItemSpriteExtra::create(bgSprite, nullptr, this, nullptr);
	auto bgMenu = CCMenu::create();
	bgMenu->addChild(bgButton);
	bgMenu->setZOrder(0);
	bgMenu->setPosition((CCDirector::sharedDirector()->getScreenRight()) - 25, (CCDirector::sharedDirector()->getScreenTop()) - 25);
	this->addChild(bgMenu); // DONT EVEN ASK WHAT IS THIS

	auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
	bg->setContentSize({ width, height });
	bg->setPosition(director->getWinSize().width / 2, director->getWinSize().height / 2);
	bg->setZOrder(2);
	this->addChild(bg);

	auto label = CCLabelBMFont::create("Editor Settings", "bigFont.fnt");
	label->setPosition({ winSize.width / 2, (winSize.height / 2.f) - -(bg->getContentSize().height / 2) - 15});
	label->setScale(0.8f);
	label->setZOrder(2);
	this->addChild(label);

	auto appearAction = CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f), .6f);

	auto menu = CCMenu::create();
	menu->setZOrder(3);
	this->addChild(menu);

	auto closeBtn = gd::CCMenuItemSpriteExtra::create(
		(CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png")),
		nullptr,
		this,
		menu_selector(EditorSettingsLayer::onClose));
	closeBtn->setPosition(-((width / 2) - 5), (height / 2) - 5);

	menu->addChild(closeBtn);

	auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
	auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
	auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
	infoSprite->setScale(0.5f);

	auto COSToggler = gd::CCMenuItemToggler::create(copyStringToggle(toggleOn, toggleOff), copyStringToggle(toggleOff, toggleOn), this, menu_selector(EditorSettingsLayer::onCopyString));
	auto COSLabel = CCLabelBMFont::create("Copy String", "bigFont.fnt");
	auto COSInfo = gd::CCMenuItemSpriteExtra::create(infoSprite, nullptr, this, menu_selector(EditorSettingsLayer::onCopyStringInfo));
	COSToggler->setScale(0.8f);
	COSToggler->setPosition({ -150, 60 });
	COSLabel->setAnchorPoint({ 0.f, 0.5f });
	COSLabel->setScale(0.4f);
	COSLabel->setPosition({ COSToggler->getPosition().x + 20.f, COSToggler->getPosition().y });
	COSInfo->setPosition({ COSToggler->getPosition().x - 17.5f, COSToggler->getPosition().y + 15.f });
	menu->addChild(COSToggler);
	menu->addChild(COSLabel);
	menu->addChild(COSInfo);

	auto SFToggler = gd::CCMenuItemToggler::create(selectFilterToggle(toggleOn, toggleOff), selectFilterToggle(toggleOff, toggleOn), this, menu_selector(EditorSettingsLayer::onSelectFilter));
	auto SFLabel = CCLabelBMFont::create("Select Filter", "bigFont.fnt");
	auto SFInfo = gd::CCMenuItemSpriteExtra::create(infoSprite, nullptr, this, menu_selector(EditorSettingsLayer::onSelectFilterInfo));
	SFToggler->setScale(0.8f);
	SFToggler->setPosition({ 20, 60 });
	SFLabel->setAnchorPoint({ 0.f, 0.5f });
	SFLabel->setScale(0.4f);
	SFLabel->setPosition({ SFToggler->getPosition().x + 20.f, SFToggler->getPosition().y });
	SFInfo->setPosition({ SFToggler->getPosition().x - 17.5f, SFToggler->getPosition().y + 15.f });
	menu->addChild(SFToggler);
	menu->addChild(SFLabel);
	menu->addChild(SFInfo);

	auto GCToggler = gd::CCMenuItemToggler::create(globalClipboardToggle(toggleOn, toggleOff), globalClipboardToggle(toggleOff, toggleOn), this, menu_selector(EditorSettingsLayer::onGlobalClipboard));
	auto GCLabel = CCLabelBMFont::create("Global Clipboard", "bigFont.fnt");
	auto GCInfo = gd::CCMenuItemSpriteExtra::create(infoSprite, nullptr, this, menu_selector(EditorSettingsLayer::onGlobalClipboardInfo));
	GCToggler->setScale(0.8f);
	GCToggler->setPosition({ -150, 20 });
	GCLabel->setAnchorPoint({ 0.f, 0.5f });
	GCLabel->setScale(0.4f);
	GCLabel->setPosition({ GCToggler->getPosition().x + 20.f, GCToggler->getPosition().y });
	GCInfo->setPosition({ GCToggler->getPosition().x - 17.5f, GCToggler->getPosition().y + 15.f });
	menu->addChild(GCToggler);
	menu->addChild(GCLabel);
	menu->addChild(GCInfo);

	//auto PAToggler = gd::CCMenuItemToggler::create(previewAnimationsInfoToggle(toggleOn, toggleOff), previewAnimationsInfoToggle(toggleOff, toggleOn), this, menu_selector(EditorSettingsLayer::onPrevAnimations));
	//auto PALabel = CCLabelBMFont::create("Preview Rotations", "bigFont.fnt");
	////auto PAInfo = gd::CCMenuItemSpriteExtra::create(infoSprite, nullptr, this, menu_selector(EditorSettingsLayer::onPrevAnimationsInfo));
	//PAToggler->setScale(0.8f);
	//PAToggler->setPosition({ -150, -20 });
	//PALabel->setAnchorPoint({ 0.f, 0.5f });
	//PALabel->setScale(0.35f);
	//PALabel->setPosition({ PAToggler->getPosition().x + 20.f, PAToggler->getPosition().y });
	////PAInfo->setPosition({ PAToggler->getPosition().x - 17.5f, PAToggler->getPosition().y + 15.f });
	//menu->addChild(PAToggler);
	//menu->addChild(PALabel);
	////menu->addChild(PAInfo);

	auto ELToggler = gd::CCMenuItemToggler::create(eltog(toggleOn, toggleOff), eltog(toggleOff, toggleOn), this, menu_selector(EditorSettingsLayer::onExperimentalLayering));
	auto ELLabel = CCLabelBMFont::create("Experimental Layering", "bigFont.fnt");
	//auto ELInfo = gd::CCMenuItemSpriteExtra::create(infoSprite, nullptr, this, menu_selector(EditorSettingsLayer::onPrevAnimationsInfo));
	ELToggler->setScale(0.8f);
	ELToggler->setPosition({ -150, -20 });
	ELLabel->setAnchorPoint({ 0.f, 0.5f });
	ELLabel->setScale(0.35f);
	ELLabel->setPosition({ ELToggler->getPosition().x + 20.f, ELToggler->getPosition().y });
	//ELInfo->setPosition({ ELToggler->getPosition().x - 17.5f, ELToggler->getPosition().y + 15.f });
	menu->addChild(ELToggler);
	menu->addChild(ELLabel);
	//menu->addChild(ELInfo);

	auto SOIToggler = gd::CCMenuItemToggler::create(showObjectInfoToggle(toggleOn, toggleOff), showObjectInfoToggle(toggleOff, toggleOn), this, menu_selector(EditorSettingsLayer::onShowObjInfo));
	auto SOILabel = CCLabelBMFont::create("Show Object Info", "bigFont.fnt");
	//auto SOIInfo = gd::CCMenuItemSpriteExtra::create(infoSprite, nullptr, this, menu_selector(EditorSettingsLayer::onShowObjInfoInfo));
	SOIToggler->setScale(0.8f);
	SOIToggler->setPosition({ 20, 20 });
	SOILabel->setAnchorPoint({ 0.f, 0.5f });
	SOILabel->setScale(0.4f);
	SOILabel->setPosition({ SOIToggler->getPosition().x + 20.f, SOIToggler->getPosition().y });
	//SOIInfo->setPosition({ SOIToggler->getPosition().x - 17.5f, SOIToggler->getPosition().y + 15.f });
	menu->addChild(SOIToggler);
	menu->addChild(SOILabel);
	//menu->addChild(SOIInfo);

	//auto HPLToggler = gd::CCMenuItemToggler::create(hidePreviewLineToggle(toggleOn, toggleOff), hidePreviewLineToggle(toggleOff, toggleOn), this, menu_selector(EditorSettingsLayer::onHidePreviewLine));
	//auto HPLLabel = CCLabelBMFont::create("Hide Preview Line", "bigFont.fnt");
	////auto HPLInfo = gd::CCMenuItemSpriteExtra::create(infoSprite, nullptr, this, menu_selector(EditorSettingsLayer::onHidePreviewLineInfo));
	//HPLToggler->setScale(0.8f);
	//HPLToggler->setPosition({ 20, -20 });
	//HPLLabel->setAnchorPoint({ 0.f, 0.5f });
	//HPLLabel->setScale(0.4f);
	//HPLLabel->setPosition({ HPLToggler->getPosition().x + 20.f, HPLToggler->getPosition().y });
	////HPLInfo->setPosition({ HPLToggler->getPosition().x - 17.5f, HPLToggler->getPosition().y + 15.f });
	//menu->addChild(HPLToggler);
	//menu->addChild(HPLLabel);
	////menu->addChild(HPLInfo);

	this->setScale(0.1f);
	this->runAction(appearAction);

	return true;
}

void EditorSettingsLayer::keyBackClicked() {
	this->setTouchEnabled(false);
	this->removeFromParentAndCleanup(true);
	editorSettingsLayer = nullptr;
}

void EditorSettingsLayer::onClose(CCObject*) {
	this->keyBackClicked();
}

void EditorSettingsLayer::showCallback(CCObject*) {
	auto myLayer = EditorSettingsLayer::create();
	myLayer->setZOrder(105);
	CCDirector::sharedDirector()->getRunningScene()->addChild(myLayer);
	editorSettingsLayer = myLayer;
}