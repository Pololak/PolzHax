#pragma once
#include "pch.h"

class RGBColorInputWidget : public CCLayer, gd::TextInputDelegate {
protected:
	gd::ColorSelectPopup* m_parent;
	gd::CCTextInputNode* red_input;
	gd::CCTextInputNode* green_input;
	gd::CCTextInputNode* blue_input;
	gd::CCTextInputNode* hex_input;

	bool init(gd::ColorSelectPopup*);
	bool ignore = false;
	virtual void textChanged(gd::CCTextInputNode*) override;

public:
	void update_labels(bool, bool);

	static RGBColorInputWidget* create(gd::ColorSelectPopup* parent) {
		RGBColorInputWidget* ret = new RGBColorInputWidget();
		if (ret && ret->init(parent)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};