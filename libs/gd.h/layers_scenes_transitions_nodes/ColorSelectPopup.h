#ifndef __COLORSELECTPOPUP_H__
#define __COLORSELECTPOPUP_H__

#include <gd.h>

namespace gd {
	class Slider;
	class GameObject;
	class CCMenuItemToggler;
	class ColorSelectDelegate;

	class ColorSelectPopup : public FLAlertLayer, cocos2d::extension::ColorPickerDelegate {
	public:
		cocos2d::extension::CCControlColourPicker* m_colorPicker; // 0x1c0
		bool m_bUnknown1; // 0x1c4
		cocos2d::CCLabelBMFont* m_durationLabel; // 0x1c8
		Slider* m_durationSlider; // 0x1cc
		GameObject* m_targetObject; // 0x1d0
		CCMenuItemToggler* m_playerColorToggle; // 0x1d4
		CCMenuItemToggler* m_playerColor2Toggle; // 0x1d8
		cocos2d::ccColor3B m_loadedColor; // 0x1dc
		cocos2d::CCSprite* m_colorOldRef; // 0x1e0
		cocos2d::CCSprite* m_colorNewRef; // 0x1e4
		ColorSelectDelegate* m_delegate; // 0x1e8
		float m_duration; // 0x1ec
		int m_custom; // 0x1f0
		int m_custom2; // 0x1f4

		static ColorSelectPopup* create(GameObject* object, int p0, int p1, int p2) {
			return reinterpret_cast<ColorSelectPopup*(__fastcall*)(gd::GameObject*, int, int, int)>(base + 0x29cf0)(object, p0, p1, p2);
		}

		static ColorSelectPopup* create(GameObject* object) {
			return ColorSelectPopup::create(object, 0, 0, 0);
		}

		void closeColorSelect(cocos2d::CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(ColorSelectPopup*, cocos2d::CCObject*)>(base + 0x2aec0)(this, sender);
		}

		cocos2d::ccColor3B getColorValue() {
			return this->m_colorPicker->getColorValue();
		}

		void selectColor(cocos2d::ccColor3B color) {
			this->m_colorPicker->setColorValue(color);
		}
	};
}

#endif
