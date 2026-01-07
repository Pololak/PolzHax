//#include "pch.h"
//#include "EditorPauseLayer.h"
//#include "utils.hpp"
//
//
//gd::EditorUI* editUI;
//
//void __fastcall EditorPauseLayer::customSetup_H(gd::CCBlockLayer* self) {
//	//editorPauseLayer = self;
//	EditorPauseLayer::customSetup(self);
//
//	auto director = CCDirector::sharedDirector();
//	auto size = director->getWinSize();
//	auto menu = CCMenu::create();
//
//	constexpr auto handler = [](CCObject* self, CCObject*) {
//		auto text = clipboard::read();
//		auto editor = reinterpret_cast<gd::LevelEditorLayer*>(reinterpret_cast<CCNode*>(self)->getParent());
//		editor->getEditorUI()->pasteObjects(text);
//	};
//
//	auto pssprite = gd::ButtonSprite::create("Paste\nString", 0x28, 0, 0.6f, true, "bigFont.fnt", "GJ_button_04.png", 30.0);
//	auto psbutton = gd::CCMenuItemSpriteExtra::create(pssprite, nullptr, self, to_handler<SEL_MenuHandler, handler>);
//	menu->setPosition({ director->getScreenRight(), director->getScreenTop() });
//	psbutton->setPosition({ -50.f, -30.f });
//	menu->addChild(psbutton);
//
//	self->addChild(menu);
//}
//
//void __fastcall EditorPauseLayer::onSaveAndPlay_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
//	editUI = nullptr;
//
//	EditorPauseLayer::onSaveAndPlay(self, sender);
//}
//
//void __fastcall EditorPauseLayer::onSaveAndExit_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
//	editUI = nullptr;
//
//	EditorPauseLayer::onSaveAndExit(self, sender);
//}
//
//void __fastcall EditorPauseLayer::onExitNoSave_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
//	editUI = nullptr;
//
//	EditorPauseLayer::onExitNoSave(self, sender);
//}
//
//void EditorPauseLayer::mem_init() {
//	MH_CreateHook(
//		reinterpret_cast<void*>(gd::base + 0x3e3d0),
//		EditorPauseLayer::customSetup_H,
//		reinterpret_cast<void**>(&EditorPauseLayer::customSetup));
//
//	/*MH_CreateHook(
//		reinterpret_cast<void*>(gd::base + 0x3f570),
//		EditorPauseLayer::keyDown_H,
//		reinterpret_cast<void**>(&EditorPauseLayer::keyDown));*/
//}