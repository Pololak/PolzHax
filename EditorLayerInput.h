#pragma once
#include "pch.h"
#include "utils.hpp"

class EditorLayerInput : public CCLayer, gd::TextInputDelegate {
protected:
	gd::EditorUI* m_parent;
	bool init(gd::EditorUI*);
	virtual void textChanged(gd::CCTextInputNode* input) override {
		if (input->getString() && strlen(input->getString())) {
			m_parent->m_editorLayer->m_groupIDFilter = strToInt(input->getString());
		}
		else {
			m_parent->m_editorLayer->m_groupIDFilter = -1;
		}
	}
public:
	gd::CCTextInputNode* m_layerInput;
	static EditorLayerInput* create(gd::EditorUI* editor) {
		EditorLayerInput* ret = new EditorLayerInput();
		if (ret && ret->init(editor)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};

class ObjectGroupInput : public CCLayer, gd::TextInputDelegate {
protected:
	gd::SetGroupIDLayer* m_parent;
	bool init(gd::SetGroupIDLayer*);
	virtual void textChanged(gd::CCTextInputNode* input) override {
		if (input->getString() && strlen(input->getString())) {
			for (auto obj : CCArrayExt<gd::GameObject*>(static_cast<gd::LevelEditorLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->m_uiLayer->getSelectedObjectsOfCCArray())) {
				if (obj)
					obj->m_editorGroup = strToInt(input->getString());
			}
		}
		else {
			for (auto obj : CCArrayExt<gd::GameObject*>(static_cast<gd::LevelEditorLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->m_uiLayer->getSelectedObjectsOfCCArray())) {
				if (obj)
					obj->m_editorGroup = 0;
			}
		}
	}
public:
	gd::CCTextInputNode* m_layerInput;
	static ObjectGroupInput* create(gd::SetGroupIDLayer* groupIDLayer) {
		ObjectGroupInput* ret = new ObjectGroupInput();
		if (ret && ret->init(groupIDLayer)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};