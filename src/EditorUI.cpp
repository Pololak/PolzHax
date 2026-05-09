#include "EditorUI.hpp"
#include "Setting.hpp"

void PolzEditorUI::keyDown(enumKeyCodes key) {
    if (key == setting().m_p1click) {
        if (this->m_editorLayer->m_playerState == 1) {
            this->m_editorLayer->pushButton(1, true);
        }
    }
    else if (key == setting().m_p2click) {
        if (this->m_editorLayer->m_playerState == 1) {
            this->m_editorLayer->pushButton(1, false);
        }
    }
    else {
        EditorUI::keyDown(key);
    }
}

void PolzEditorUI::keyUp(enumKeyCodes key) {
    if (key == setting().m_p1click) {
        if (this->m_editorLayer->m_playerState == 1) {
            this->m_editorLayer->releaseButton(1, true);
        }
    }
    else if (key == setting().m_p2click) {
        if (this->m_editorLayer->m_playerState == 1) {
            this->m_editorLayer->releaseButton(1, false);
        }
    }
    else {
        EditorUI::keyUp(key);
    }
}