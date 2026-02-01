#include "EditorPauseLayer.hpp"
#include "EditorUI.hpp"
#include "utils.hpp"
#include "Setting.hpp"
#include "EditorOptionsLayer.hpp"
#include "LevelEditorLayer.hpp"

std::string lengthToString(float time) { // Math in GD is fucking ass, what's wrong with it??? // Also I guess timeForXPos is horrible func...
	if (time <= 8.88696f) return "Tiny";
	if (time < 28.8561f) return "Short";
	if (time < 58.8f) return "Medium";
	if (time <= 118.683f) return "Long";
	else return "Extra-Long";
}

bool __fastcall EditorPauseLayer::initH(gd::EditorPauseLayer* self, void*, gd::LevelEditorLayer* editorLayer) {
	if (!EditorPauseLayer::init(self, editorLayer)) return false;
	LevelEditorLayer::setIsEditorPaused(true);

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto levelTimeLabel = CCLabelBMFont::create("", "goldFont.fnt");
	levelTimeLabel->setScale(.5f);
	levelTimeLabel->setPosition(director->getScreenLeft() + 10.f, director->getScreenTop() - 25.f);
	levelTimeLabel->setAnchorPoint({ 0.f, 1.f });
	self->addChild(levelTimeLabel);

	// Time logic
	float time = floorf(editorLayer->m_gridLayer->timeForXPos(editorLayer->getLastObjectX()));
	int hours = time / 3600.f; // why not
	int minutes = (time - (hours * 3600)) / 60.f;
	int seconds = (time - (minutes * 60)) - (hours * 3600);

	if (time > 3599) {
		levelTimeLabel->setString(CCString::createWithFormat("%ih %im %is", hours, minutes, seconds)->getCString());
	}
	else if (time > 59) {
		levelTimeLabel->setString(CCString::createWithFormat("%imin %is", minutes, seconds)->getCString());
	}
	else {
		levelTimeLabel->setString(CCString::createWithFormat("%is", seconds)->getCString());
	}
	//

	//auto levelLengthLabel = CCLabelBMFont::create("", "goldFont.fnt");
	//levelLengthLabel->setScale(.5f);
	//levelLengthLabel->setPosition(director->getScreenLeft() + 10.f, director->getScreenTop() - 45.f);
	//levelLengthLabel->setAnchorPoint({ 0.f, 1.f });
	//self->addChild(levelLengthLabel);

	//levelLengthLabel->setString(lengthToString(editorLayer->m_gridLayer->timeForXPos(editorLayer->getLastObjectX())).c_str());

	return true;
}

void EditorPauseLayer::Callback::onSave(CCObject*) {
	auto layer = gd::FLAlertLayer::create(this, "Save", "<cy>Save</c> the level?", "NO", "YES", 300.f);
	layer->setTag(2);
	layer->show();
}

void EditorPauseLayer::Callback::onSelectAll(CCObject*) {
	auto editorLayer = this->m_levelEditorLayer;

	if (editorLayer) {
		auto editorUI = editorLayer->m_uiLayer;

		CCArray* arr = CCArray::create();

		CCARRAY_FOREACH_B_TYPE(editorLayer->getAllObjects(), object, gd::GameObject) {
			if (object && (object->m_editorGroup == editorLayer->m_groupIDFilter) || editorLayer->m_groupIDFilter == -1) {
				arr->addObject(object);
				editorUI->selectObjects(arr);
			}
		}

		editorUI->updateButtons();
		editorUI->deactivateRotationControl();
	}
}

void selectAllWithDirection(bool rightDir) {
	auto editorLayer = LevelEditorLayer::get();

	if (editorLayer) {
		auto editorUI = editorLayer->m_uiLayer;

		auto cameraPos = editorLayer->m_gameLayer->getPosition();
		auto cameraScale = editorLayer->m_gameLayer->getScale();
		int centerX = -(cameraPos.x) / cameraScale + CCDirector::sharedDirector()->getWinSize().width / 2;

		CCArray* arr = CCArray::create();

		CCARRAY_FOREACH_B_TYPE(editorLayer->getAllObjects(), object, gd::GameObject) {
			if (object && (object->m_editorGroup == editorLayer->m_groupIDFilter) || editorLayer->m_groupIDFilter == -1) {
				if ((rightDir && object->getPositionX() >= centerX) || (!rightDir && object->getPositionX() <= centerX)) {
					arr->addObject(object);
					editorUI->selectObjects(arr);
				}
			}
		}

		editorUI->updateButtons();
		editorUI->deactivateRotationControl();
	}
}

void EditorPauseLayer::Callback::onSelectAllLeft(CCObject*) {
	selectAllWithDirection(false);
}

void EditorPauseLayer::Callback::onSelectAllRight(CCObject*) {
	selectAllWithDirection(true);
}

void EditorPauseLayer::Callback::onPasteString(CCObject*) {
	auto editorUI = this->m_levelEditorLayer->m_uiLayer;
	if (editorUI) {
		editorUI->pasteObjects(clipboard::read());
		editorUI->updateButtons();
	}
}

void EditorPauseLayer::Callback::onEditorOptions(CCObject*) {
	EditorOptionsLayer::create()->show();
}

void EditorPauseLayer::Callback::onSelectFilter(CCObject*) {
	setting().onSelectFilter = !setting().onSelectFilter;
}

void EditorPauseLayer::Callback::onShowObjectInfo(CCObject*) {
	setting().onShowObjectInfo = !setting().onShowObjectInfo;
	auto editorUI = this->m_levelEditorLayer->m_uiLayer;
	if (editorUI) {
		auto objectInfoLabel = static_cast<CCLabelBMFont*>(editorUI->getChildByTag(2701));
		if (objectInfoLabel) {
			objectInfoLabel->setVisible(setting().onShowObjectInfo);
			EditorUI::updateObjectInfoLabel(editorUI);
		}
	}
}

void EditorPauseLayer::Callback::onPreviewMode(CCObject*) {
	setting().onPreviewMode = !setting().onPreviewMode;
	if (setting().onPreviewMode) {
		LevelEditorLayer::updatePreviewMode();
	}
	else {
		LevelEditorLayer::resetColors();
	}
}

void EditorPauseLayer::Callback::onSmallEditorStep(CCObject*) {
	gd::GameManager::sharedState()->toggleGameVariable("0035");
}

void __fastcall EditorPauseLayer::customSetupH(gd::EditorPauseLayer* self) {
	EditorPauseLayer::customSetup(self);

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto mainMenu = static_cast<CCMenu*>(self->getChildren()->objectAtIndex(0));
	mainMenu->setAnchorPoint({ .0f, .0f });
	mainMenu->setScale(.9f);

	auto onSaveSpr = gd::ButtonSprite::create("Save", 0xdc, 0, 1.f, true, "goldFont.fnt", "GJ_button_01.png", 30.f);
	auto onSave = gd::CCMenuItemSpriteExtra::create(onSaveSpr, self, menu_selector(EditorPauseLayer::Callback::onSave));

	static_cast<gd::CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(0))->setPositionY(90.f);
	static_cast<gd::CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(1))->setPositionY(45.f);
	static_cast<gd::CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(2))->setPositionY(0.f);
	onSave->setPositionY(-45.f);
	static_cast<gd::CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(3))->setPositionY(-90.f);

	mainMenu->addChild(onSave);

	auto bottom_menu = static_cast<CCMenu*>(self->m_audioOnBtn->getParent());

	auto onSelectAllRightSpr = gd::ButtonSprite::create("Select\nAll Right", 0x1e, 0, 1.f, true, "bigFont.fnt", "GJ_button_04.png", 30.f);
	auto onSelectAllRight = gd::CCMenuItemSpriteExtra::create(onSelectAllRightSpr, self, menu_selector(EditorPauseLayer::Callback::onSelectAllRight));
	onSelectAllRight->setPosition(bottom_menu->convertToNodeSpace({winSize.width - 115.f, director->getScreenBottom() + 30.f}));
	bottom_menu->addChild(onSelectAllRight);

	auto onSelectAllLeftSpr = gd::ButtonSprite::create("Select\nAll Left", 0x1e, 0, 1.f, true, "bigFont.fnt", "GJ_button_04.png", 30.f);
	auto onSelectAllLeft = gd::CCMenuItemSpriteExtra::create(onSelectAllLeftSpr, self, menu_selector(EditorPauseLayer::Callback::onSelectAllLeft));
	onSelectAllLeft->setPosition(bottom_menu->convertToNodeSpace({ winSize.width - 115.f, director->getScreenBottom() + 65.f }));
	bottom_menu->addChild(onSelectAllLeft);

	auto onSelectAllSpr = gd::ButtonSprite::create("Select\nAll", 0x1e, 0, 1.f, true, "bigFont.fnt", "GJ_button_04.png", 30.f);
	auto onSelectAll = gd::CCMenuItemSpriteExtra::create(onSelectAllSpr, self, menu_selector(EditorPauseLayer::Callback::onSelectAll));
	onSelectAll->setPosition(bottom_menu->convertToNodeSpace({ winSize.width - 115.f, director->getScreenBottom() + 100.f }));
	bottom_menu->addChild(onSelectAll);

	auto onPasteStringSpr = gd::ButtonSprite::create("Paste\nString", 0x1e, 0, 1.f, true, "bigFont.fnt", "GJ_button_04.png", 30.f);
	auto onPasteString = gd::CCMenuItemSpriteExtra::create(onPasteStringSpr, self, menu_selector(EditorPauseLayer::Callback::onPasteString));
	onPasteString->setPosition(bottom_menu->convertToNodeSpace({ winSize.width - 115.f, director->getScreenBottom() + 135.f }));
	bottom_menu->addChild(onPasteString);

	auto onOptionsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");
	onOptionsSpr->setScale(.8f);
	auto onOptions = gd::CCMenuItemSpriteExtra::create(onOptionsSpr, self, menu_selector(EditorPauseLayer::Callback::onEditorOptions));
	onOptions->setPosition(bottom_menu->convertToNodeSpace({ winSize.width - 115.f, director->getScreenBottom() + 170.f }));
	bottom_menu->addChild(onOptions);

	gd::GameToolbox::createToggleButton(
		menu_selector(EditorPauseLayer::Callback::onSmallEditorStep),
		gd::GameManager::sharedState()->getGameVariable("0035"),
		bottom_menu,
		self, self,
		.7f, .4f, 80.f,
		"bigFont.fnt",
		false,
		"Small Editor Step",
		ccp(30.f, director->getScreenBottom() + 120.f),
		ccp(8.f, 0.f)
	);

	gd::GameToolbox::createToggleButton(
		menu_selector(EditorPauseLayer::Callback::onSelectFilter),
		setting().onSelectFilter,
		bottom_menu,
		self, self,
		.7f, .4f, 80.f,
		"bigFont.fnt",
		false,
		"Select Filter",
		ccp(30.f, director->getScreenBottom() + 150.f),
		ccp(8.f, 0.f)
	);

	gd::GameToolbox::createToggleButton(
		menu_selector(EditorPauseLayer::Callback::onShowObjectInfo),
		setting().onShowObjectInfo,
		bottom_menu,
		self, self,
		.7f, .4f, 80.f,
		"bigFont.fnt",
		false,
		"Show Object Info",
		ccp(30.f, director->getScreenBottom() + 180.f),
		ccp(8.f, 0.f)
	);

	gd::GameToolbox::createToggleButton(
		menu_selector(EditorPauseLayer::Callback::onPreviewMode),
		setting().onPreviewMode,
		bottom_menu,
		self, self,
		.7f, .4f, 80.f,
		"bigFont.fnt",
		false,
		"Preview Mode",
		ccp(30.f, director->getScreenBottom() + 210.f),
		ccp(8.f, 0.f)
	);
}

void __fastcall EditorPauseLayer::FLAlert_ClickedH(gd::EditorPauseLayer* _self, void*, gd::FLAlertLayer* layer, bool btn2) {
	auto self = reinterpret_cast<gd::EditorPauseLayer*>(reinterpret_cast<uintptr_t>(_self) - 0x198);
	if ((layer->getTag() == 2) && btn2) {
		self->saveLevel();
	}
	EditorPauseLayer::FLAlert_Clicked(_self, layer, btn2);
}

void __fastcall EditorPauseLayer::keyDownH(gd::EditorPauseLayer* _self, void*, enumKeyCodes key) {
	auto self = reinterpret_cast<gd::EditorPauseLayer*>(reinterpret_cast<uintptr_t>(_self) - 0xf4);
	if (key == KEY_Escape)
		self->onResume(nullptr);
	else 
		EditorPauseLayer::keyDown(_self, key);
}

void __fastcall EditorPauseLayer::destructorH(gd::EditorPauseLayer* self) {
	LevelEditorLayer::setIsEditorPaused(false);
	EditorPauseLayer::destructor(self);
}

void EditorPauseLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3e2e0), EditorPauseLayer::initH, reinterpret_cast<void**>(&EditorPauseLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3e3d0), EditorPauseLayer::customSetupH, reinterpret_cast<void**>(&EditorPauseLayer::customSetup));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3f480), EditorPauseLayer::FLAlert_ClickedH, reinterpret_cast<void**>(&EditorPauseLayer::FLAlert_Clicked));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3f570), EditorPauseLayer::keyDownH, reinterpret_cast<void**>(&EditorPauseLayer::keyDown));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3e280), EditorPauseLayer::destructorH, reinterpret_cast<void**>(&EditorPauseLayer::destructor));
}