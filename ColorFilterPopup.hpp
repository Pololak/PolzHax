#pragma once
#include "pch.h"

class ColorFilterPopup : public gd::FLAlertLayer {
protected:
	gd::EditorUI* m_parent;
	CCLabelBMFont* m_label;
	int m_color;

	virtual bool init(gd::EditorUI*);
	virtual void keyBackClicked();
	void onCancel(CCObject*);
	void onClose(CCObject*);
	void onNext(CCObject*);
	void onPrev(CCObject*);
	void onReset(CCObject*);
	void updateLabel();
	std::string colorToString(int);
public:
	static ColorFilterPopup* create(gd::EditorUI* parent) {
		ColorFilterPopup* ret = new ColorFilterPopup();
		if (ret && ret->init(parent)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};