#ifndef __CCMENUITEMSPRITEEXTRA_H__
#define __CCMENUITEMSPRITEEXTRA_H__

#include <gd.h>

namespace gd {
	#pragma runtime_checks("s", off)
	class CCMenuItemSpriteExtra_Original : public cocos2d::CCMenuItemSprite {
	protected:
		float m_fUnknown;
		float m_fUnknown2;

	public:
		static CCMenuItemSpriteExtra_Original* create(cocos2d::CCNode* sprite,
			cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback) {
			auto pRet = reinterpret_cast<CCMenuItemSpriteExtra_Original * (__thiscall*)(cocos2d::CCNode*,
				cocos2d::CCObject*, cocos2d::SEL_MenuHandler)>(
					base + 0x19020
					)(sprite, target, callback);
			//fix stack before returning
			__asm add esp, 0x8
			return pRet;
		}
		void setSizeMult(float mult) {
			__asm movss xmm1, mult
			return reinterpret_cast<void(__thiscall*)(CCMenuItemSpriteExtra_Original*)>(
				base + 0xd3b0
				)(this);
		}
	};
	class CCMenuItemSpriteExtra : public cocos2d::CCMenuItemSprite {
	public:
		static auto create(CCNode* activatedSprite, CCNode* deactivatedSprite, CCObject* target, cocos2d::SEL_MenuHandler callback) {
			auto ret = reinterpret_cast<CCMenuItemSpriteExtra * (__fastcall*)(CCNode*, CCNode*, CCObject*, cocos2d::SEL_MenuHandler)>
				(base + 0xd1e0)(activatedSprite, deactivatedSprite, target, callback); //base + 0xd1e0 GHS 1.92
			__asm add esp, 0x8;
			return ret;
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif