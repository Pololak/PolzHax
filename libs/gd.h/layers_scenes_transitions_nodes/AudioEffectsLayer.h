#ifndef __AUDIOEFFECTSLAYER_H__
#define __AUDIOEFFECTSLAYER_H__

#include <gd.h>

namespace gd {
	class AudioEffectsLayer : public cocos2d::CCLayerColor {
	public:
		cocos2d::CCSpriteBatchNode* m_batchNode; // 0x194
		cocos2d::CCArray* m_audioSteps; // 0x198
		cocos2d::CCArray* m_remainingSteps; // 0x19c
		float m_currentTime; // 0x1a0
		float m_nextEffect; // 0x1a4
		bool m_goingDown; // 0x1a8
		float m_baseAudioScale; // 0x1ac
		bool m_rainActive; // 0x1b0
	};
}

#endif // !__AUDIOEFFECTSLAYER_H__
