#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class PolzEditorUI : public geode::Modify<PolzEditorUI, EditorUI> {
public:
    void keyDown(enumKeyCodes);
    void keyUp(enumKeyCodes);
};