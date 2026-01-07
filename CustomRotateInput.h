#pragma once
#include "pch.h"

class CustomRotateInput : public CCLayer, gd::TextInputDelegate {
protected:
	gd::GJRotationControl* m_parent;
	bool init(gd::GJRotationControl*);
	virtual void textChanged(gd::CCTextInputNode*) override;
public:
	gd::CCTextInputNode* m_rotateInput;
	static CustomRotateInput* create(gd::GJRotationControl* rotationControl) {
		CustomRotateInput* ret = new CustomRotateInput();
		if (ret && ret->init(rotationControl)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};