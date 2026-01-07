#ifndef __CCMENUITEMSPRITEEXTRA_H__
#define __CCMENUITEMSPRITEEXTRA_H__

#include <gd.h>

namespace gd {
	class CCMenuItemSpriteExtra : public cocos2d::CCMenuItemSprite {
	public:
		float m_scaleMultiplier;
		float m_baseScale;
		bool m_animationEnabled;
		bool m_colorEnabled;
		float m_volume;
		std::string m_clickSound;

		static CCMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCNode* disabledSprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback) {
			auto ret = reinterpret_cast<CCMenuItemSpriteExtra * (__fastcall*)(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler)>(base + 0xd1e0)(sprite, disabledSprite, target, callback);
			
			__asm add esp, 0x8

			return ret;
		}

		static CCMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback) {
			return CCMenuItemSpriteExtra::create(sprite, nullptr, target, callback);
		}

		void setSizeMult(float mult) {
			__asm movss xmm1, mult
			reinterpret_cast<void(__thiscall*)(CCMenuItemSpriteExtra*)>(base + 0xd3b0)(this);
		}
	};

	class CreateMenuItem : public CCMenuItemSpriteExtra {
	public:

	};
}

#endif