#pragma once
#include "pch.h"

class CircleToolPopup : public gd::FLAlertLayer, gd::FLAlertLayerProtocol, gd::TextInputDelegate {
protected:
	CCLabelBMFont* m_label;
	gd::CCTextInputNode* m_angleInput;
	gd::CCTextInputNode* m_stepInput;
	
	virtual bool init();
	virtual void textChanged(gd::CCTextInputNode*);
	virtual void keyBackClicked();
	void onClose(CCObject*);
	void onApply(CCObject*);
	void perform();
	virtual void FLAlert_Clicked(gd::FLAlertLayer*, bool btn2) {
		if (btn2) {
			this->perform();
		}
	}
	void updateLabels();
	void onOriginalAuthor(CCObject*) {
		CCApplication::sharedApplication()->openURL("https://github.com/matcool/small-gd-mods/blob/main/src/circle-tool.cpp");
	}

public:
	static CircleToolPopup* create() {
		CircleToolPopup* ret = new CircleToolPopup();
		if (ret && ret->init()) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};