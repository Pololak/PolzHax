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
	};
}

#endif // !__VIDEOOPTIONSLAYER_H__
