#pragma once
#include "pch.h"

class ObjectIDEnterPopup : public gd::FLAlertLayer, gd::TextInputDelegate {
protected:
	gd::CCTextInputNode* m_inputNode;
	gd::EditorUI* m_parent;
	bool init(gd::EditorUI*);
	//virtual void textChanged(gd::CCTextInputNode*);
	virtual void keyBackClicked();
	void onClose(CCObject*);
public:
	static ObjectIDEnterPopup* create(gd::EditorUI*);
};