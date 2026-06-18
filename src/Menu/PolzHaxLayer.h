#pragma once
#include "../include.h"

class PolzButtonSprite : public cocos2d::CCSprite {
protected:
    virtual bool init(const char* caption) {
        auto label = CCLabelBMFont::create(caption, "bigFont.fnt");
        label->limitLabelWidth(75.f, .6f, .1f);
        label->setPosition({45.875f, 15.25f});
        this->addChild(label);

        return this->initWithSpriteFrameName("GJ_longBtn01_001.png");
    }
public:
    void toggleBGImage(bool toggle) {
        this->initWithSpriteFrameName(toggle ? "GJ_longBtn02_001.png" : "GJ_longBtn01_001.png");
    }

    static PolzButtonSprite* create(const char* caption) {
		PolzButtonSprite* ret = new PolzButtonSprite();
		if (ret && ret->init(caption)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};

class PolzHaxLayer : public FLAlertLayer {
protected:
    int m_selectedTab;
    CCArray* m_spritesArray;
    CCArray* m_buttonsArray;

    virtual bool init();
    virtual void keyBackClicked();
    void onClose(CCObject*);
    void highlightSelected(PolzButtonSprite*);
    void onSelectTab(CCObject*);
public:
    static PolzHaxLayer* create() {
        PolzHaxLayer* ret = new PolzHaxLayer();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};