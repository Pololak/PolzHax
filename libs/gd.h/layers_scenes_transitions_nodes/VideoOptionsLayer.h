#ifndef __VIDEOOPTIONSLAYER_H__
#define __VIDEOOPTIONSLAYER_H__

#include <gd.h>

namespace gd {
	class CCMenuItemSpriteExtra;

	class VideoOptionsLayer : public FLAlertLayer {
	public:
		CCMenuItemSpriteExtra* m_prevResolutionBtn; // 0x1bc
		CCMenuItemSpriteExtra* m_nextResolutionBtn; // 0x1c0
		cocos2d::CCLabelBMFont* m_windowedResolutionLabel; // 0x1c4
		cocos2d::CCLabelBMFont* m_selectedResolutionLabel; // 0x1c8
		cocos2d::CCLabelBMFont* m_qualityLabel; // 0x1cc
		int m_textureQuality; // 0x1d0
		cocos2d::CCArray* m_availableResolutions; // 0x1d4
		bool m_fullscreen; // 0x1d8
		int m_currentResolution; // 0x1dc

		void createToggleButton(cocos2d::SEL_MenuHandler callback, bool toggle, cocos2d::CCMenu* menu, float toggleScale, float labelMaxWidth, bool center, std::string caption, cocos2d::CCPoint position) {
			return reinterpret_cast<void(__thiscall*)(VideoOptionsLayer*, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, float, float, bool, std::string, cocos2d::CCPoint)>(base + 0xd4810)(this, callback, toggle, menu, toggleScale, labelMaxWidth, center, caption, position);
		}
	};
}

#endif // !__VIDEOOPTIONSLAYER_H__
