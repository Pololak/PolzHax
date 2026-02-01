#ifndef __CCMENUITEMTOGGLER_H__
#define __CCMENUITEMTOGGLER_H__

#include <gd.h>

namespace gd {

	class CCMenuItemSpriteExtra;

	#pragma runtime_checks("s", off)
	class CCMenuItemToggler : public cocos2d::CCMenuItem {
	public:
		CCMenuItemSpriteExtra* m_offButton; // 0x108
		CCMenuItemSpriteExtra* m_onButton; // 0x10c
		bool m_toggled; // 0x110
		bool m_notClickable; // 0x114

		static CCMenuItemToggler* create(cocos2d::CCNode* off, cocos2d::CCNode* on,
			cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback) {
			auto pRet = reinterpret_cast<CCMenuItemToggler* (__fastcall*)(cocos2d::CCNode*,
				cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler)>(
					base + 0xd720
					)(off, on, target, callback);
			__asm add esp, 0x8
			return pRet;
		}

		/*void setSizeMult(float mult) {
			this->m_offButton->setSizeMult(mult);
			this->m_onButton->setSizeMult(mult);
			this->toggle(this->m_toggled);
		}*/

		void setSizeMult(float mult) { // CCMenuItemSpriteExtra func actually, but it works fine lol.
			__asm movss xmm1, mult
			reinterpret_cast<void(__thiscall*)(CCMenuItemToggler*)>(base + 0xd3b0)(this);
		}

		void toggle(bool on) {
			reinterpret_cast<void(__thiscall*)(CCMenuItemToggler*, bool)>(base + 0xda70)(this, on);
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif