#ifndef __FLALERTLAYER_H__
#define __FLALERTLAYER_H__

#include <gd.h>

namespace gd {
	class FLAlertLayerProtocol;
	class ButtonSprite;
	class ScrollingLayer;

	#pragma runtime_checks("s", off)
	class FLAlertLayer : public cocos2d::CCLayerColor {
	public:
		cocos2d::CCMenu* m_buttonMenu; // 0x194
		int m_controlConnected; // 0x198
		int m_ZOrder; // 0x19c
		FLAlertLayerProtocol* m_alertProtocol; // 0x1a0
		cocos2d::CCNode* m_scene; // 0x1a4
		bool m_reverseKeyBack; // 0x1a8
		cocos2d::CCLayer* m_mainLayer; // 0x1ac
		ScrollingLayer* m_scrollingLayer; // 0x1b0
		int m_scrollAction; // 0x1b4
		bool m_containsBorder; // 0x1b8
		bool m_noAction; // 0x1bc

		virtual ~FLAlertLayer() {
			cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->decrementForcePrio();
		}

	public:
		virtual void registerWithTouchDispatcher() {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*)>(base + 0x161c0)(this);
		}

		virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			auto self = reinterpret_cast<FLAlertLayer*>(reinterpret_cast<uintptr_t>(this) + 0xe8);
			return reinterpret_cast<bool(__thiscall*)(FLAlertLayer*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(base + 0x15f00)(self, pTouch, pEvent);
		}
		virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			auto self = reinterpret_cast<FLAlertLayer*>(reinterpret_cast<uintptr_t>(this) + 0xe8);
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(base + 0x16050)(self, pTouch, pEvent);
		}
		virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			auto self = reinterpret_cast<FLAlertLayer*>(reinterpret_cast<uintptr_t>(this) + 0xe8);
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(base + 0x15f90)(self, pTouch, pEvent);
		}
		virtual void ccTouchCancelled(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			auto self = reinterpret_cast<FLAlertLayer*>(reinterpret_cast<uintptr_t>(this) + 0xe8);
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(base + 0x16000)(self, pTouch, pEvent);
		}

		virtual void keyDown(cocos2d::enumKeyCodes key) {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*, cocos2d::enumKeyCodes)>(base + 0x15d70)(this, key);
		}

		virtual void keyBackClicked() {
			auto self = reinterpret_cast<FLAlertLayer*>(reinterpret_cast<uintptr_t>(this) + 0xf0);
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*)>(base + 0x15de0)(self);
		}

		virtual void show() {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*)>(base + 0x160a0)(this);
		}
		FLAlertLayer() {
			reinterpret_cast<void(__thiscall*)(FLAlertLayer*)>(base + 0x15160)(this);
		}

		void onBtn1(cocos2d::CCObject* btn) {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*, cocos2d::CCObject*)>(base + 0x15e60)(this, btn);
		}
		void onBtn2(cocos2d::CCObject* btn) {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*, cocos2d::CCObject*)>(base + 0x15eb0)(this, btn);
		}

		static FLAlertLayer* create(FLAlertLayerProtocol* protocol, char const* title, char const* desc, char const* btn1, char const* btn2, float width, bool scroll, float height) {
			auto ret = reinterpret_cast<FLAlertLayer * (__fastcall*)(FLAlertLayerProtocol*, char const*, char const*, char const*, char const*, float, bool, float)>(base + 0x15360)(protocol, title, desc, btn1, btn2, width, scroll, height);

			__asm add esp, 0x18

			return ret;
		}

		static FLAlertLayer* create(FLAlertLayerProtocol* protocol, char const* title, char const* desc, char const* btn1, char const* btn2, float width) {
			return FLAlertLayer::create(protocol, title, desc, btn1, btn2, width, false, 0.f);
		}

		static FLAlertLayer* create(FLAlertLayerProtocol* delegate, char const* title, char const* desc, char const* btn1, char const* btn2) {
			return FLAlertLayer::create(delegate, title, desc, btn1, btn2, 300.0f);
		}

		static FLAlertLayer* create(char const* title, const char* desc, char const* btn) {
			return FLAlertLayer::create(nullptr, title, desc, btn, nullptr, 300.0f);
		}
	};
	#pragma runtime_checks("s", restore)
}


#endif