#pragma once
#include "pch.h"

class PulseHelperPopup : public gd::FLAlertLayer, gd::TextInputDelegate {
protected:
	gd::ColorSelectPopup* m_colorSelectPopup;
	gd::CCTextInputNode* m_fadeInInput;
	gd::CCTextInputNode* m_holdInput;
	gd::CCTextInputNode* m_fadeOutInput;

	bool init(gd::ColorSelectPopup*);
	virtual void keyBackClicked() override;
	virtual void textChanged(gd::CCTextInputNode*) override;
public:
	static PulseHelperPopup* create(gd::ColorSelectPopup* target) {
		PulseHelperPopup* ret = new PulseHelperPopup();
		if (ret && ret->init(target)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	void onClose(CCObject*);
	void onPulseHelper(CCObject*);
	void onCreate(CCObject*);
};