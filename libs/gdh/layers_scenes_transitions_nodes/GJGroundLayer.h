#ifndef __GJGROUNDLAYER_H__
#define __GJGROUNDLAYER_H__

#include "../include/gd.h"

class GJGroundLayer : public cocos2d::CCLayer {
public:
	cocos2d::CCSprite* m_groundSprite; // 0x118
	float m_groundWidth; // 0x11c
	cocos2d::CCSprite* m_line; // 0x120
	bool m_isActive; // 0x124

	void hideShadows() {
		this->getChildByType<cocos2d::CCSprite*>(2)->setVisible(false);
		this->getChildByType<cocos2d::CCSprite*>(3)->setVisible(false);
	}

	static GJGroundLayer* create(int groundID);
};

#endif
