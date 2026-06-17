#pragma once
#include "include.h"

class ColorChannelSprite : public CCSprite {
protected:
	virtual bool init(const char*);

public:
	CCLabelBMFont* m_blendingDot;

	void updateBlending(bool);
	void updateValues(cocos2d::ccColor3B, bool blending = false);

	static ColorChannelSprite* create(const char* label = "") {
		ColorChannelSprite* ret = new ColorChannelSprite();
		if (ret && ret->init(label)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};