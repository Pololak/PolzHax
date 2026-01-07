#pragma once
#include "pch.h"

class EditorLayerInput : public CCLayer, gd::TextInputDelegate {
protected:
    gd::EditorUI* m_editorUI;

    bool init(gd::EditorUI*);
    virtual void textChanged(gd::CCTextInputNode*);
public:
    gd::CCTextInputNode* m_input;

    static EditorLayerInput* create(gd::EditorUI*);

    void updateInputNode();
};