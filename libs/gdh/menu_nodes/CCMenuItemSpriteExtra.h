#ifndef __CCMENUITEMSPRITEEXTRA_H__
#define __CCMENUITEMSPRITEEXTRA_H__

#include "../include/gd.h"

class CCMenuItemSpriteExtra : public cocos2d::CCMenuItemSprite {
public:
	float m_scaleMultiplier;
	float m_baseScale;
	bool m_animationEnabled;
	bool m_colorEnabled;
	float m_volume;
	std::string m_clickSound;

	static CCMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCNode* disabledSprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);

	static CCMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback) {
		return CCMenuItemSpriteExtra::create(sprite, nullptr, target, callback);
	}

	void setSizeMult(float mult);
};

class CreateMenuItem : public CCMenuItemSpriteExtra {
public:

};

#endif