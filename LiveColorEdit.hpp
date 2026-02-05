#pragma once
#include "pch.h"
#include "hsv.hpp"

class LiveColorEdit : public cocos2d::CCLayer {
protected:
	gd::GameObject* m_targetObject;
	int m_colorTag;
	gd::LevelSettingsObject* m_levelSettings;

	gd::Slider* m_hueSlider;
	CCSprite* m_saturationBar;
	gd::Slider* m_saturationSlider;
	CCSprite* m_valueBar;
	gd::Slider* m_valueSlider;

	color_utils::HSV m_hsv;

	CCSprite* m_newColorPreview;

	CCLabelBMFont* m_infoLabel;

	std::string defineLabel();

	virtual bool init(gd::GameObject*, int);
	void sliderChanged(CCObject*);

	gd::SettingsColorObject* tagToColor(gd::LevelSettingsObject*);

	void updateDevLabel();

public:
	static LiveColorEdit* create(gd::GameObject* obj, int tag) {
		LiveColorEdit* ret = new LiveColorEdit();
		if (ret && ret->init(obj, tag)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	void closeColorEdit(CCObject*);
};