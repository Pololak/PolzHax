#pragma once
#include "pch.h"

class RemapTriggerWidget : public cocos2d::CCLayer {
protected:
	gd::ColorSelectPopup* m_parent;
	int m_type;
	CCLabelBMFont* m_label;

	virtual bool init(gd::ColorSelectPopup*);
	void updateLabel();
	void remapTrigger();
	void setType(int id);

	std::string intToColorString(int);

public:
	static RemapTriggerWidget* create(gd::ColorSelectPopup* parent) {
		RemapTriggerWidget* ret = new RemapTriggerWidget();
		if (ret && ret->init(parent)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	void onTypeUp(CCObject*);
	void onTypeDown(CCObject*);
};