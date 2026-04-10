#pragma once
#include "pch.h"

class GoToPagePopup : public gd::FLAlertLayer {
protected:
	gd::LevelBrowserLayer* m_parent;
	gd::CCTextInputNode* m_input;

	virtual bool init(gd::LevelBrowserLayer*);
	virtual void keyBackClicked();
	void onGo(CCObject*);
	void onClose(CCObject*);
	void onIncrement(CCObject*);
	void onDecrement(CCObject*);
public:
	static GoToPagePopup* create(gd::LevelBrowserLayer* parent) {
		GoToPagePopup* ret = new GoToPagePopup();
		if (ret && ret->init(parent)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};