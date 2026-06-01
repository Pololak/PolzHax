#ifndef __GJGARAGELAYER_H__
#define __GJGARAGELAYER_H__

#include <gd.h>

namespace gd {
	class CCTextInputNode;
	class SimplePlayer;
	class CCMenuItemSpriteExtra;
	class CCMenuItemToggler;

	class GJGarageLayer : public cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, GameRateDelegate, ListButtonBarDelegate {
	public:
		CCTextInputNode* m_nameInput; // 0x128
		SimplePlayer* m_playerPreview; // 0x12c
		cocos2d::CCNode* m_pUnknown1; // 0x130
		cocos2d::CCNode* m_pUnknown2; // 0x134
		cocos2d::CCSprite* m_colorSelector1; // 0x138
		cocos2d::CCSprite* m_colorSelector2; // 0x13c
		CCMenuItemSpriteExtra* m_selectedColor; // 0x140
		CCMenuItemSpriteExtra* m_selectedColor2; // 0x144
		void* m_pUnused1; // 0x148
		void* m_pUnused2; // 0x14c
		cocos2d::CCArray* m_pagesArray; // 0x150
		float m_colorMinX; // 0x154
		float m_colorMaxX; // 0x158
		CCMenuItemToggler* m_tabToggleCube; // 0x15c
		CCMenuItemToggler* m_tabToggleShip; // 0x160
		CCMenuItemToggler* m_tabToggleBall; // 0x164
		CCMenuItemToggler* m_tabToggleUfo; // 0x168
		CCMenuItemToggler* m_tabToggleSpecial; // 0x16c
		cocos2d::CCSprite* m_rateSprite; // 0x170
		bool m_doSelect; // 0x174

		void onSelectTab(CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(GJGarageLayer*, cocos2d::CCObject*)>(base + 0x7de50)(this, sender);
		}

		void selectPage(IconType iconType) {
			reinterpret_cast<void(__thiscall*)(GJGarageLayer*, IconType)>(base + 0x7de70)(this, iconType);
		}

		static GJGarageLayer* node() {
			return reinterpret_cast<GJGarageLayer * (__fastcall*)()>(base + 0x7c240)();
		}
	};
}

#endif // !__GJGARAGELAYER_H__
