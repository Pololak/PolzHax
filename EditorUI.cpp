#pragma once
#define NOMINMAX
#include "EditorUI.h"
#include "state.h"
#include "pch.h"
#include "utils.hpp"

gd::EditorPauseLayer* editorPauseLayer;
CCLayer* levelEditorLayer;
gd::EditorUI* editUI;
std::string savedClipboard;

bool hideUI = false;

CCSprite* hideui_toggle_texture(CCSprite* unhide, CCSprite* hide) {
    return (hideUI) ? unhide : hide;
}

void LevelEditorLayer::Callback::hideUI_btn(CCObject*) {
    hideUI = !hideUI;
    if (hideUI) {
        editUI->setVisible(0);
    }
    else {
        editUI->setVisible(1);
    }
}

bool __fastcall LevelEditorLayer::init_H(gd::LevelEditorLayer* self, void*, gd::GJGameLevel* level) {
    levelEditorLayer = self;
    if (!LevelEditorLayer::init(self, level)) return false;

    hideUI = false;

    auto director = CCDirector::sharedDirector();
    auto menuh = CCMenu::create();

    auto unhide = CCSprite::create("BE_eye-off-btn.png");
    auto hide = CCSprite::create("BE_eye-on-btn.png");

    unhide->setOpacity(100);

    auto hideui_btn = gd::CCMenuItemToggler::create(
        hideui_toggle_texture(unhide, hide),
        hideui_toggle_texture(hide, unhide),
        self,
        menu_selector(LevelEditorLayer::Callback::hideUI_btn));

    menuh->setZOrder(100);
    menuh->setScale(.5f);
    menuh->setPosition({ director->getScreenLeft() + 20, director->getScreenTop() - 100.5f });


    menuh->addChild(hideui_btn);
    self->addChild(menuh);
}

bool __fastcall EditorUI::init_H(gd::EditorUI* self, void*, CCLayer* editor) {
    editUI = self;

    bool result = EditorUI::init(self, editor);

    auto sliderArr = from<CCArray*>(self, 0xA8);
    auto Slider = reinterpret_cast<gd::Slider*>(sliderArr->objectAtIndex(0));
    Slider->setPositionX(Slider->getPositionX() + 15);

    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();

    if (setting().onPersClip)
    {
        if (!savedClipboard.empty()) {
            self->clipboard() = savedClipboard;
            self->updateButtons();
        }
    }

    return result;
}

bool __fastcall EditorUI::dtor_H(gd::EditorUI* self) {
    editUI = nullptr;
    if (setting().onPersClip) savedClipboard = self->clipboard();
    EditorUI::dtor(self);
}

void __fastcall EditorUI::scrollWheel_H(gd::EditorUI* _self, void* edx, float dy, float dx) {
    auto self = reinterpret_cast<gd::EditorUI*>(reinterpret_cast<uintptr_t>(_self) - 0xf8);
    auto layer = reinterpret_cast<gd::LevelEditorLayer*>(self->getParent())->gameLayer();
    auto zoom = layer->getScale();

    static_assert(offsetof(CCDirector, m_pKeyboardDispatcher) == 0x4c, "it wrong!");
    auto kb = CCDirector::sharedDirector()->m_pKeyboardDispatcher;
    if (kb->getControlKeyPressed()) {
        zoom = static_cast<float>(std::pow(2.71828182845904523536, std::log(std::max(zoom, 0.001f)) - dy * 0.01f));
        // zoom limit
        zoom = std::max(zoom, 0.01f);
        zoom = std::min(zoom, 1000000.f);
        self->updateZoom(zoom);
    }
    else if (kb->getShiftKeyPressed()) {
        layer->setPositionX(layer->getPositionX() - dy * 1.f);
    }
    else {
        EditorUI::scrollWheel(_self, dy, dx);
    }
}

void __fastcall EditorPauseLayer::customSetup_H(gd::CCBlockLayer* self) {
    //editorPauseLayer = self;
    EditorPauseLayer::customSetup(self);

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

void __fastcall EditorPauseLayer::onSaveAndPlay_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
    editUI = nullptr;

    EditorPauseLayer::onSaveAndPlay(self, sender);
}

void __fastcall EditorPauseLayer::onSaveAndExit_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
    editUI = nullptr;

    EditorPauseLayer::onSaveAndExit(self, sender);
}

void __fastcall EditorPauseLayer::onExitNoSave_H(gd::EditorPauseLayer* self, void*, CCObject* sender) {
    editUI = nullptr;

    EditorPauseLayer::onExitNoSave(self, sender);
}

void EditorUI::mem_init() {
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3fdc0),
        EditorUI::init_H,
        reinterpret_cast<void**>(&EditorUI::init));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3fb90),
        EditorUI::dtor_H,
        reinterpret_cast<void**>(&EditorUI::dtor));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x4ee90),
        EditorUI::scrollWheel_H,
        reinterpret_cast<void**>(&EditorUI::scrollWheel));
}

void LevelEditorLayer::mem_init() {
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x8c2c0),
        LevelEditorLayer::init_H,
        reinterpret_cast<void**>(&LevelEditorLayer::init));
}

void EditorPauseLayer::mem_init() {
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3e3d0),
        EditorPauseLayer::customSetup_H,
        reinterpret_cast<void**>(&EditorPauseLayer::customSetup));
}