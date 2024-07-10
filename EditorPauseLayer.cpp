#include "pch.h"
#include "EditorPauseLayer.h"
#include "utils.hpp"

gd::EditorPauseLayer* editorPauseLayer;

void __fastcall EditorPauseLayer::EditorPauseLayer_customSetup_H(gd::CCBlockLayer* self) {
	//editorPauseLayer = self;
	EditorPauseLayer::EditorPauseLayer_customSetup(self);

	auto director = CCDirector::sharedDirector();
	auto size = director->getWinSize();
	auto menu = CCMenu::create();

	constexpr auto handler = [](CCObject* self, CCObject*) {
		auto text = clipboard::read();
		auto editor = reinterpret_cast<gd::LevelEditorLayer*>(reinterpret_cast<CCNode*>(self)->getParent());
		editor->getEditorUI()->pasteObjects(text);
	};

	auto pssprite = gd::ButtonSprite::create("Paste\nString", 0x28, 0, 0.6f, true, "bigFont.fnt", "GJ_button_04.png", 30.0);
	auto psbutton = gd::CCMenuItemSpriteExtra::create(pssprite, nullptr, self, to_handler<SEL_MenuHandler, handler>);
	menu->setPosition({ director->getScreenRight(), director->getScreenTop() });
	psbutton->setPosition({ -50.f, -30.f });
	menu->addChild(psbutton);

	self->addChild(menu);
}

void __fastcall EditorPauseLayer::EditorPauseLayer_keyDown_H(gd::EditorPauseLayer* self, void* edx, enumKeyCodes key) {
	/*if (key == KEY_Space)
		editorPauseLayer->onResume(nullptr);
	else*/
	EditorPauseLayer::EditorPauseLayer_keyDown(self, key);
}