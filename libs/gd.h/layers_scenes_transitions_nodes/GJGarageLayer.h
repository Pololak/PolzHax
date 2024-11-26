#ifndef __GJGARAGELAYER_H__
#define __GJGARAGELAYER_H__

#include <gd.h>
#include "cocos2d.h"

namespace gd {

	class CCMenuItemToggler;
	class SimplePlayer;
	enum IconType;

	class GJGarageLayer : public cocos2d::CCLayer {
	public:
		CCMenuItemToggler* getCubeBtn() {
			return from<CCMenuItemToggler*>(this, 0x15c);
		}

		CCMenuItemToggler* getShipBtn() {
			return from<CCMenuItemToggler*>(this, 0x160);
		}

		CCMenuItemToggler* getBallBtn() {
			return from<CCMenuItemToggler*>(this, 0x164);
		}

		CCMenuItemToggler* getBirdBtn() {
			return from<CCMenuItemToggler*>(this, 0x168);
		}

		CCMenuItemToggler* getSpecialBtn() {
			return from<CCMenuItemToggler*>(this, 0x16c);
		}

		SimplePlayer* getPlayerIcon() {
			return from<SimplePlayer*>(this, 0x12c);
		}

		cocos2d::CCArray* getGaragePageArray() {
			return from<cocos2d::CCArray*>(this, 0x150);
		}
		void onSelectTab(CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(GJGarageLayer*, cocos2d::CCObject*)>(base + 0x7de50)(this, sender);
		}
		void onBallIcon(CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(GJGarageLayer*, cocos2d::CCObject*)>(base + 0x7e190)(this, sender);
		}
	};

	class GaragePage : public cocos2d::CCNode {
	public:
		static auto create(IconType p0, GJGarageLayer* p1, cocos2d::SEL_MenuHandler p2) {
			return reinterpret_cast<GaragePage * (__stdcall*)(IconType, GJGarageLayer*, cocos2d::SEL_MenuHandler)>(base + 0x7fc90)(p0, p1, p2);
		}
	};
}

#endif // ! __GJGARAGELAYER_H__