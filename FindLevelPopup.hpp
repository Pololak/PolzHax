#pragma once
#include "pch.h"

class FindLevelPopup : public gd::FLAlertLayer, gd::TextInputDelegate {
protected:
	gd::LevelBrowserLayer* m_parent;
	std::string m_searchString;
	gd::CCTextInputNode* m_input;

	virtual bool init(gd::LevelBrowserLayer*);
	virtual void keyBackClicked();
	//virtual void textChanged(gd::CCTextInputNode*);
	void onSearch(CCObject*);
	void onClose(CCObject*);
public:
	static FindLevelPopup* create(gd::LevelBrowserLayer* parent) {
		FindLevelPopup* ret = new FindLevelPopup();
		if (ret && ret->init(parent)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};