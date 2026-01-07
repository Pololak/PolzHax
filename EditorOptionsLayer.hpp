#pragma once
#include "pch.h"

class EditorOptionsLayer : public gd::FLAlertLayer {
protected:
	virtual bool init();
	virtual void keyBackClicked();
	void onClose(CCObject*);

	void onCopyString(CCObject*);
	void onDurationLines(CCObject*);
	void onExperimentalLayering(CCObject*);
	void onUnusedObjects(CCObject*);
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