#pragma once
#include "pch.h"

class RGBColorInputWidget : public CCLayer, public gd::TextInputDelegate {
protected:
	gd::ColorSelectPopup* parent;
	gd::CCTextInputNode* red_input;
	gd::CCTextInputNode* green_input;
	gd::CCTextInputNode* blue_input;
	gd::CCTextInputNode* hex_input;

	bool init(gd::ColorSelectPopup* parent);

	bool ignore = false;

	virtual void textChanged(gd::CCTextInputNode* input) override;

public:
	void update_labels(bool hex, bool rgb);

	static RGBColorInputWidget* create(gd::ColorSelectPopup* parent);
};