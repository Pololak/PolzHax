#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class IconPreviewPopup : public FLAlertLayer {
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