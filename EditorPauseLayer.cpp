#include "EditorPauseLayer.hpp"
#include "EditorUI.hpp"
#include "utils.hpp"
#include "Setting.hpp"
#include "EditorOptionsLayer.hpp"
#include "LevelEditorLayer.hpp"
#include "RotateSaws.hpp"

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
	auto editorUI = editorLayer->m_uiLayer;

	auto objects = CCArray::create();
	for (auto section : CCArrayExt<CCArray*>(editorLayer->m_levelSections)) {
		if (section) {
			for (auto object : CCArrayExt<gd::GameObject*>(section)) {
				if (object && object->m_editorGroup == editorLayer->m_groupIDFilter || editorLayer->m_groupIDFilter == -1) {
					objects->addObject(object);
				}
			}
		}
	}

	editorUI->createUndoSelectObject(false);
	editorUI->selectObjects(objects);
	editorUI->updateButtons();
	editorUI->deactivateRotationControl();
}

void selectAllWithDirection(bool rightDir) {
	auto editorLayer = LevelEditorLayer::get();

	if (editorLayer) {
		auto editorUI = editorLayer->m_uiLayer;

		auto cameraPos = editorLayer->m_gameLayer->getPosition();
		auto cameraScale = editorLayer->m_gameLayer->getScale();
		int centerX = -(cameraPos.x) / cameraScale + CCDirector::sharedDirector()->getWinSize().width / 2;

		auto objects = CCArray::create();
		for (auto section : CCArrayExt<CCArray*>(editorLayer->m_levelSections)) {
			if (section) {
				for (auto object : CCArrayExt<gd::GameObject*>(section)) {
					if (object && (object->m_editorGroup == editorLayer->m_groupIDFilter) || editorLayer->m_groupIDFilter == -1) {
						if ((rightDir && object->getPositionX() >= centerX) || (!rightDir && object->getPositionX() <= centerX)) {
							objects->addObject(object);
						}
					}
				}
			}
		}

		editorUI->createUndoSelectObject(false);
		editorUI->selectObjects(objects);
		editorUI->updateButtons();
		editorUI->deactivateRotationControl();
	}
}

void alignObjects(bool alignY) {
	auto editorUI = EditorUI::get();

	if (editorUI) {
		auto selectedObjects = editorUI->m_selectedObjects;
		int objectCount = selectedObjects->count();
		auto randomKid = reinterpret_cast<gd::GameObject*>(selectedObjects->objectAtIndex(0));

		if (objectCount < 3) return;

		editorUI->m_editorLayer->addToUndoList(gd::UndoObject::createWithTransformObjects(selectedObjects, gd::UndoCommand::Transform), false);

		std::vector<gd::GameObject*> sortedArray;
		for (auto object : CCArrayExt<gd::GameObject*>(selectedObjects)) {
			if (object) {
				sortedArray.push_back(object);
			}
		}

		if (!alignY) {
			float minX = randomKid->getPositionX();
			float maxX = randomKid->getPositionX();

			for (auto object : CCArrayExt<gd::GameObject*>(selectedObjects)) {
				if (object) {
					float compareX = object->getPositionX();
					if (compareX < minX) {
						minX = compareX;
					}
					if (compareX > maxX) {
						maxX = compareX;
					}
				}
			}

			std::sort(sortedArray.begin(), sortedArray.end(), [](gd::GameObject* a, gd::GameObject* b) {
				return a->getPositionX() < b->getPositionX();
				});
			float step = (maxX - minX) / (objectCount - 1);
			float currentStep = 0.f;

			for (auto obj : sortedArray) {
				if (obj) {
					editorUI->moveObject(obj, ccp(minX + currentStep - obj->getPositionX(), 0));
					currentStep += step;
				}
			}
		}
		else {
			float minY = randomKid->getPositionY();
			float maxY = randomKid->getPositionY();

			for (auto object : CCArrayExt<gd::GameObject*>(selectedObjects)) {
				if (object) {
					float compareY = object->getPositionY();
					if (compareY < minY) {
						minY = compareY;
					}
					if (compareY > maxY) {
						maxY = compareY;
					}
				}
			}

			std::sort(sortedArray.begin(), sortedArray.end(), [](gd::GameObject* a, gd::GameObject* b) {
				return a->getPositionY() < b->getPositionY();
				});
			float step = (maxY - minY) / (objectCount - 1);
			float currentStep = 0.f;

			for (auto obj : sortedArray) {
				if (obj) {
					editorUI->moveObject(obj, ccp(0, minY + currentStep - obj->getPositionY()));
					currentStep += step;
				}
			}
		}
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

void EditorPauseLayer::Callback::onAlignY(CCObject*) {
	alignObjects(true);
}

void EditorPauseLayer::Callback::onAlignX(CCObject*) {
	alignObjects(false);
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

void EditorPauseLayer::Callback::onResetStartPos(CCObject*) {
	if (LevelEditorLayer::getPlaytestStartPos()) {
		LevelEditorLayer::setPlaytestStartPos(nullptr);
	}
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

	auto onAlignYSpr = gd::ButtonSprite::create("Align Y", 0x1e, 0, 1.f, true, "bigFont.fnt", "GJ_button_04.png", 30.f);
	auto onAlignY = gd::CCMenuItemSpriteExtra::create(onAlignYSpr, self, menu_selector(EditorPauseLayer::Callback::onAlignY));
	onAlignY->setPosition(bottom_menu->convertToNodeSpace({ winSize.width - 115.f, director->getScreenBottom() + 170.f }));
	bottom_menu->addChild(onAlignY);

	auto onAlignXSpr = gd::ButtonSprite::create("Align X", 0x1e, 0, 1.f, true, "bigFont.fnt", "GJ_button_04.png", 30.f);
	auto onAlignX = gd::CCMenuItemSpriteExtra::create(onAlignXSpr, self, menu_selector(EditorPauseLayer::Callback::onAlignX));
	onAlignX->setPosition(bottom_menu->convertToNodeSpace({ winSize.width - 115.f, director->getScreenBottom() + 205.f }));
	bottom_menu->addChild(onAlignX);

	auto onOptionsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");
	onOptionsSpr->setScale(.8f);
	auto onOptions = gd::CCMenuItemSpriteExtra::create(onOptionsSpr, self, menu_selector(EditorPauseLayer::Callback::onEditorOptions));
	onOptions->setPosition(bottom_menu->convertToNodeSpace({ winSize.width - 115.f, director->getScreenBottom() + 240.f }));
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

	auto onResetStartPosSpr = gd::ButtonSprite::create("Reset\nStartPos", 0x32, 0, .4f, true, "bigFont.fnt", "GJ_button_04.png", 30.f);
	auto onResetStartPos = gd::CCMenuItemSpriteExtra::create(onResetStartPosSpr, self, menu_selector(EditorPauseLayer::Callback::onResetStartPos));
	onResetStartPos->setPosition(bottom_menu->convertToNodeSpace({ director->getScreenRight() - 50.f, director->getScreenBottom() + 65.f }));
	bottom_menu->addChild(onResetStartPos);
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

void __fastcall EditorPauseLayer::saveLevelH(gd::EditorPauseLayer* self) {
	if (setting().onPreviewRotations) RotateSaws::stopRotations(self->m_levelEditorLayer);
	
	EditorPauseLayer::saveLevel(self);

	if (setting().onPreviewRotations) RotateSaws::beginRotations(self->m_levelEditorLayer);
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
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3eec0), EditorPauseLayer::saveLevelH, reinterpret_cast<void**>(&EditorPauseLayer::saveLevel));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3e280), EditorPauseLayer::destructorH, reinterpret_cast<void**>(&EditorPauseLayer::destructor));
}