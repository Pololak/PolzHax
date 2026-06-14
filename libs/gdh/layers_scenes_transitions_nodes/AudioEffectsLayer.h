#ifndef __AUDIOEFFECTSLAYER_H__
#define __AUDIOEFFECTSLAYER_H__

#include "../include/gd.h"

class AudioEffectsLayer : public cocos2d::CCLayerColor {
public:
	cocos2d::CCSpriteBatchNode* m_batchNode;
	cocos2d::CCArray* m_audioSteps;
	cocos2d::CCArray* m_remainingSteps;
	float m_currentTime;
	float m_nextEffect;
	bool m_goingDown;
	float m_baseAudioScale;
	bool m_rainActive;
};

#endif
