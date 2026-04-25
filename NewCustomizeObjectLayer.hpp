#pragma once
#include "pch.h"
#include "ColorChannelSprite.hpp"

class NewCustomizeObjectLayer : public gd::FLAlertLayer, gd::ColorSelectDelegate {
protected:
	gd::GameObject* m_selectedObject;
	CCArray* m_selectedObjects;
	CCArray* m_buttonsArray;
	CCArray* m_colorSprites;
	CCSprite* m_selectSprite;
	int m_selectedColorIdx;
	bool m_liveColorEnabled;

	virtual bool init(gd::GameObject*, CCArray*);
	virtual void colorSelectClosed(gd::ColorSelectPopup*);
	virtual void keyBackClicked();
	const char* colorToString(int);
	gd::SettingsColorObject* intToSettings(int);

	void onSelectColor(CCObject*);
	void editColor(CCObject*);
	void updateSelectedColorLabel();
	void updateSelectPosition();
	void updateSelectedColorSprite();
	void updateColorSprites();
	void toggleLiveColor(CCObject*);

public:
	ColorChannelSprite* m_selectedColorSprite;
	CCLabelBMFont* m_selectedColorLabel;

	static NewCustomizeObjectLayer* get();

	void onClose(CCObject*);

	static NewCustomizeObjectLayer* create(gd::GameObject* object, CCArray* objects) {
		NewCustomizeObjectLayer* ret = new NewCustomizeObjectLayer();
		if (ret && ret->init(object, objects)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};