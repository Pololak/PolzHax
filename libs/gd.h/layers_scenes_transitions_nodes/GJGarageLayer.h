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
		PAD(16)
		CCTextInputNode* m_nameInput; // 0x128
		SimplePlayer* m_playerPreview; // 0x12c
		PAD(8)
		cocos2d::CCSprite* m_colorSelector1; // 0x138
		cocos2d::CCSprite* m_colorSelector2; // 0x13c
		CCMenuItemSpriteExtra* m_selectedColor1; // 0x140
		CCMenuItemSpriteExtra* m_selectedColor2; // 0x144
		PAD(8)
		cocos2d::CCArray* m_pagesArray; // 0x150
		float m_colorMinX; // 0x154
		float m_colorMaxX; // 0x158
		CCMenuItemToggler* m_tabToggleCube; // 0x15c
		CCMenuItemToggler* m_tabToggleShip; // 0x160
		CCMenuItemToggler* m_tabToggleBall; // 0x164
		CCMenuItemToggler* m_tabToggleUfo; // 0x168
		CCMenuItemToggler* m_tabToggleSpecial; // 0x16c


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
		static cocos2d::CCScene* scene() {
			return reinterpret_cast<cocos2d::CCScene*(__fastcall*)()>(base + 0x7c240)();
		}
	};

	class GaragePage : public cocos2d::CCNode {
	public:
		PAD(0x34)
		GJGarageLayer* m_garageLayer; // 0x11c
		cocos2d::SEL_MenuHandler m_callback; // 0x120
		cocos2d::CCSprite* m_selectSprite; // 0x124
		CCMenuItemSpriteExtra* m_selectedSprite; // 0x128
		IconType m_iconType; // 0x12c


		static auto create(IconType p0, GJGarageLayer* p1, cocos2d::SEL_MenuHandler p2) {
			return reinterpret_cast<GaragePage * (__stdcall*)(IconType, GJGarageLayer*, cocos2d::SEL_MenuHandler)>(base + 0x7fc90)(p0, p1, p2);
		}
	};
}

#endif // ! __GJGARAGELAYER_H__