#pragma once
#include "pch.h"

class PulseBuilderPopup : public gd::FLAlertLayer, gd::TextInputDelegate {
protected:
	gd::CCTextInputNode* m_fadeInInput;
	gd::CCTextInputNode* m_holdInput;
	gd::CCTextInputNode* m_fadeOutInput;

	virtual bool init();
	virtual void keyBackClicked();
	virtual void textChanged(gd::CCTextInputNode*);
	void onClose(CCObject*);
	void onBuild(CCObject*);
public:
	static PulseBuilderPopup* create() {
		PulseBuilderPopup* ret = new PulseBuilderPopup();
		if (ret && ret->init()) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};