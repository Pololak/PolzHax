#pragma once
#include "../include.h"

class EditorOptionsLayer : public FLAlertLayer {
public:
    virtual bool init();
    virtual void keyBackClicked();
    void onClose(CCObject*);

    void onDurationLines(CCObject*);
    void onExperimentalLayering(CCObject*);
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