#ifndef __CCMENUITEMTOGGLER_H__
#define __CCMENUITEMTOGGLER_H__

#include "../include/gd.h"

class CCMenuItemSpriteExtra;

class CCMenuItemToggler : public cocos2d::CCMenuItem {
public:
	CCMenuItemSpriteExtra* m_offButton; // 0x108
	CCMenuItemSpriteExtra* m_onButton; // 0x10c
	bool m_toggled; // 0x110
	bool m_notClickable; // 0x114

	static CCMenuItemToggler* create(cocos2d::CCNode* off, cocos2d::CCNode* on, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);

	void setSizeMult(float mult);

	void toggle(bool on);
};

#endif