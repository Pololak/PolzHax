#include "pch.h"
#include "EditorPauseLayer.h"

gd::CCBlockLayer* editorPauseLayer;
CCMenu* editorPauseLayer_centerMenu = nullptr;
CCMenu* editorPauseLayerBottomMenu = nullptr;

bool HTL = false;

CCSprite* HTL_togtex(CCSprite* toggleOn, CCSprite* toggleOff) {
	return (HTL) ? toggleOn : toggleOff;
}

void EditorPauseLayer::Callback::HTLToggler(CCObject*) {
	HTL = !HTL;
	if (HTL) {
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493E08), "\xE9\xCE\x00\x00\x00\x90", 6, NULL);
	}
	else {
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x493E08), "\x0F\x84\xCD\x00\x00\x00", 6, NULL);
	}
}

bool __fastcall EditorPauseLayer::EditorPauseLayer_customSetup_H(gd::CCBlockLayer* self) {
	editorPauseLayer = self;
	editorPauseLayer_centerMenu = nullptr;
	editorPauseLayerBottomMenu = nullptr;

	bool result = EditorPauseLayer::EditorPauseLayer_customSetup(self);
	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();
	auto menu = CCMenu::create();
	menu->setPosition({ director->getScreenLeft(), director->getScreenBottom() });

	auto btnArray = from<CCArray*>(editorPauseLayer_centerMenu, 0xA8);
	auto onExitNoSaveBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(btnArray->objectAtIndex(3));

	auto btnArrayBottom = from<CCArray*>(editorPauseLayerBottomMenu, 0xA8);
	//auto onSongBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(btnArrayBottom->objectAtIndex(2));
	//onSongBtn->setPositionY(onSongBtn->getPositionY() - 12);
	//auto openEditorGuide = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(btnArrayBottom->objectAtIndex(1));
	//openEditorGuide->setPositionY(openEditorGuide->getPositionY() - 12);

	//onExitNoSaveBtn->setPositionY(onExitNoSaveBtn->getPositionY() - 45);

	auto onSaveLevelSpr = gd::ButtonSprite::create("Save", 0, 220, 1.f, false, "goldFont.fnt", "GJ_button_01.png", 30.f);
	auto onSaveLevelBtn = gd::CCMenuItemSpriteExtra::create(onSaveLevelSpr, nullptr, self, menu_selector(gd::EditorPauseLayer::saveLevel));
	//onSaveLevelBtn->setPositionY(-67.5f);
	//editorPauseLayer_centerMenu->setPositionY(editorPauseLayer_centerMenu->getPositionY() + 22);

	auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
	auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

	auto HTLButton = gd::CCMenuItemToggler::create(HTL_togtex(toggleOn, toggleOff), HTL_togtex(toggleOff, toggleOn), self, menu_selector(EditorPauseLayer::Callback::HTLToggler));
	auto HTLLabel = CCLabelBMFont::create("Hide Trigger Lines", "bigFont.fnt");
	HTLButton->setScale(.7f);
	HTLButton->setPosition({ 30, 120 });
	HTLLabel->setScale(.33f);
	HTLLabel->setPosition({ 103, 120 });
	menu->addChild(HTLButton);
	menu->addChild(HTLLabel);

	//auto smth = CCSprite::create("GJ_button_01.png");
	//smth->setPosition({ size.width / 2,size.height / 2 });
	//self->addChild(smth);

	//editorPauseLayer_centerMenu->addChild(onSaveLevelBtn);
	self->addChild(menu);

	return result;
}

void __fastcall EditorPauseLayer::EditorPauseLayer_centerMenu_H() {
	__asm {
		mov editorPauseLayer_centerMenu, eax
	}
	EditorPauseLayer::EditorPauseLayer_centerMenu();
}

void __fastcall EditorPauseLayer::EditorPauseLayer_bottomMenu_H() {
	__asm {
		mov editorPauseLayerBottomMenu, eax
	}
	EditorPauseLayer::EditorPauseLayer_bottomMenu();
}