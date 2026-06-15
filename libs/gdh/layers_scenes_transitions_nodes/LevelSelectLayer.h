#ifndef __LEVELSELECTLAYER_H__
#define __LEVELSELECTLAYER_H__

#include "../include/gd.h"

class BoomScrollLayer;

class LevelSelectLayer : public cocos2d::CCLayer, BoomScrollLayerDelegate, DynamicScrollDelegate {
public:
    float m_width; // 0x120
	cocos2d::CCSprite* m_backgroundSprite; // 0x124
	cocos2d::CCSprite* m_groundSprite; // 0x128
	BoomScrollLayer* m_scrollLayer; // 0x12c
	int m_unknown; // 0x130
	int m_scrolls; // 0x134
	bool m_masterDetectiveUnlocked; // 0x138
};

#endif