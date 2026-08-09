#pragma once
#include "pch.h"

class EditorOptionsLayer : public gd::FLAlertLayer {
protected:
	virtual bool init();
	virtual void keyBackClicked();
	void onClose(CCObject*);

	void createToggleButton(std::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);

	void onCopyString(CCObject*);
	void onDurationLines(CCObject*);
	void onExperimentalLayering(CCObject*);
	void onLinkControls(CCObject*);
	void onHideUIOnPlaytest(CCObject*);
	void onUnusedObjects(CCObject*);
	void onPreviewRotations(CCObject*);
	void onNewColorSelect(CCObject*);
	void onShowClicks(CCObject*);
public:
	static EditorOptionsLayer* create() {
		EditorOptionsLayer* ret = new EditorOptionsLayer();
		if (ret && ret->init()) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};