#ifndef __FLALERTLAYER_H__
#define __FLALERTLAYER_H__

#include <gd.h>

namespace gd {
	class FLAlertLayerProtocol;
	class ButtonSprite;
	class ScrollingLayer;

	class FLAlertLayer : public cocos2d::CCLayerColor {
	public:
		cocos2d::CCMenu* m_buttonMenu; // 0x194
		int m_controlConnected; // 0x198
		int m_ZOrder; // 0x19c
		FLAlertLayerProtocol* m_alertProtocol; // 0x1a0
		cocos2d::CCNode* m_scene; // 0x1a4
		bool m_reverseKeyBack; // 0x1a8
		CCLayer* m_mainLayer; // 0x1ac
		ScrollingLayer* m_scrollingLayer; // 0x1b0
		int m_scrollAction; // 0x1b4
		bool m_containsBorder; // 0x1b8
		bool m_noAction; // 0x1bc

		virtual ~FLAlertLayer() {
			m_buttonMenu = nullptr;
			m_controlConnected = -1;
			m_ZOrder = 0;
			m_alertProtocol = nullptr;
			m_scene = nullptr;
			m_reverseKeyBack = false;
			m_mainLayer = nullptr;
			m_scrollingLayer = nullptr;
			m_scrollAction = -1;
			m_containsBorder = false;
			m_noAction = false;
		}

		virtual void registerWithTouchDispatcher() {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*)>(
				base + 0x161c0 //16110 //0x236F0
				)(this);
		}

		//reinterpret_cast<char*>(this) + 0xE8

		virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			return reinterpret_cast<bool(__thiscall*)(FLAlertLayer*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(
				base + 0x15f00 //15e50 //0x233C0
				)(reinterpret_cast<FLAlertLayer*>(reinterpret_cast<uintptr_t>(this) + 0xe8), pTouch, pEvent);
		}
		virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(
				base + 0x16050 //15fa0 //0x23510
				)(reinterpret_cast<FLAlertLayer*>(reinterpret_cast<uintptr_t>(this) + 0xe8), pTouch, pEvent);
		}
		virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(
				base + 0x15f90 //15ee0 //0x23450
				)(reinterpret_cast<FLAlertLayer*>(reinterpret_cast<uintptr_t>(this) + 0xe8), pTouch, pEvent);
		}
		virtual void ccTouchCancelled(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(
				base + 0x16000 //15f50 //0x234C0
				)(reinterpret_cast<FLAlertLayer*>(reinterpret_cast<uintptr_t>(this) + 0xe8), pTouch, pEvent);
		}

		virtual void keyDown(cocos2d::enumKeyCodes key) {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*, cocos2d::enumKeyCodes)>(
				base + 0x15d70 //15cd0 //0x23250 2.1
				)(this, key);
		}

		virtual void keyBackClicked() {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*)>(
				base + 0x15de0
				)(reinterpret_cast<FLAlertLayer*>(reinterpret_cast<uintptr_t>(this) + 0xf0));
		}

		virtual void show() {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*)>(
				base + 0x160a0
				)(this);
		}

		virtual void onBtn1(cocos2d::CCObject* btn) {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*, cocos2d::CCObject*)>(
				base + 0x15e60 //15bd0 //0x23340 2.1
				)(this, btn);
		}
		virtual void onBtn2(cocos2d::CCObject* btn) {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*, cocos2d::CCObject*)>(
				base + 0x15eb0 //15e00 //0x23380 2.1
				)(this, btn);
		}

		static auto create(FLAlertLayerProtocol* protocol, const char* title,
			const char* caption, const char* button1, const char* button2,
			float width, bool absolute, float height) {

			auto ret = reinterpret_cast<FLAlertLayer * (__fastcall*)(FLAlertLayerProtocol*, const char*, const char*,
				const char*, const char*, float, bool, float)>(base + 0x15360)(protocol, title,
					caption, button1, button2, width, absolute, height);
			__asm add esp, 0x18
			return ret;
		}
	};
}


#endif