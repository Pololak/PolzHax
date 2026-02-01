#pragma once
#include "pch.h"

class IconPreviewPopup : public gd::FLAlertLayer {
protected:
	virtual bool init();
	virtual void keyBackClicked();
	void onClose(CCObject*);
public:
	static IconPreviewPopup* create() {
		IconPreviewPopup* ret = new IconPreviewPopup();
		if (ret && ret->init()) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};