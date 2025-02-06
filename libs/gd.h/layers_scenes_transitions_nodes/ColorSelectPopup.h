#ifndef __COLORSELECTPOPUP_H__
#define __COLORSELECTPOPUP_H__

#include <gd.h>

namespace gd {
    class ColorSelectPopup : public FLAlertLayer {
    public:
		PAD(0x4)
		cocos2d::extension::CCControlColourPicker* m_colorPicker; // 0x1c0
		PAD(0x4)
		cocos2d::CCLabelBMFont* m_durationLabel; // 0x1c8
		Slider* m_durationSlider; // 0x1cc
		GameObject* m_targetObject; // 0x1d0
		CCMenuItemToggler* m_playerColorToggler; // 0x1d4
		CCMenuItemToggler* m_playerColor2Toggler; // 0x1d8
		cocos2d::ccColor3B m_loadedColor; // 0x1dc
		cocos2d::CCSprite* m_colorOldRef; // 0x1e0
		cocos2d::CCSprite* m_colorNewRef; // 0x1e4
		ColorSelectDelegate* m_delegate; // 0x1e8
		float m_duration; // 0x1ec
		int m_custom; // 0x1f0 // player color
		int m_custom2; // 0x1f4 // blending
    };
}

#endif