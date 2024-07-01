#ifndef __FLALERTLAYER_H__
#define __FLALERTLAYER_H__

#include <gd.h>

namespace gd {

	class FLAlertLayerProtocol;
	class ButtonSprite;
	class ScrollingLayer;

	#pragma runtime_checks("s", off)
	class FLAlertLayer : public cocos2d::CCLayerColor {
	protected:
		cocos2d::CCMenu* m_pButtonMenu;
		int m_nControlConnected; //?
		cocos2d::CCObject* m_pTarget;
		PAD(12);
		cocos2d::CCLayer* m_pLayer; //not working bruh
		int m_nZOrder2;
		bool m_bNoElasticity;
		cocos2d::ccColor3B m_cColor2; //?
		ButtonSprite* m_pButton1;
		ButtonSprite* m_pButton2;
		ScrollingLayer* m_pScrollingLayer;
		int m_nJoystickConnected;
		bool m_bBorder; //?
		bool m_bNoAction; //?

	public:
		CCNode vtable;
		auto getMenu()
		{
			return m_pButtonMenu;
		}

		auto menu()
		{
			return from<cocos2d::CCMenu*>(this, 0x194);
		}

		auto getLayer()
		{
			return from<cocos2d::CCLayer*>(this, 0x1AC);
		}

		virtual void onEnter() {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*)>(
				base + 0x23750
				)(this);
		}
		virtual void registerWithTouchDispatcher() {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*)>(
				base + 0x161c0 //16110 //0x236F0
				)(this);
		}

		//CCTouchDelegate vtable
		virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) { 
			return reinterpret_cast<bool(__thiscall*)(char*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(
				base + 0x15f00 //15e50 //0x233C0
				)(reinterpret_cast<char*>(this) + 0xEC, pTouch, pEvent);
		}
		virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			return reinterpret_cast<void(__thiscall*)(char*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(
				base + 0x16050 //15fa0 //0x23510
				)(reinterpret_cast<char*>(this) + 0xEC, pTouch, pEvent);
		}
		virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			return reinterpret_cast<void(__thiscall*)(char*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(
				base + 0x15f90 //15ee0 //0x23450
				)(reinterpret_cast<char*>(this) + 0xEC, pTouch, pEvent);
		}
		virtual void ccTouchCancelled(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
			return reinterpret_cast<void(__thiscall*)(char*, cocos2d::CCTouch*, cocos2d::CCEvent*)>(
				base + 0x16000 //15f50 //0x234C0
				)(reinterpret_cast<char*>(this) + 0xEC, pTouch, pEvent);
		}

		//CCKeyboardDelegate vtable
		virtual void keyDown(cocos2d::enumKeyCodes key) {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*, cocos2d::enumKeyCodes)>(
				base + 0x15d70 //15cd0 //0x23250 2.1
				)(this, key);
		}

		//CCKeypadDelegate vtable
		virtual void keyBackClicked() {
			return reinterpret_cast<void(__thiscall*)(char*)>(
				base + 0x15de0 //15d30 //0x232C0 2.1
				)(reinterpret_cast<char*>(this) + 0xF4);
		}

		//vtable
		virtual void show() {
			return reinterpret_cast<void(__thiscall*)(FLAlertLayer*)>(
				base + 0x160a0
				)(this);
			//base + 0x160a0 GHS 1.92
			//base + 0x23560 GD 2.13
			//base + 0x15ff0 GHS 1.91
		}

		FLAlertLayer() {
			reinterpret_cast<void(__thiscall*)(FLAlertLayer*)>(
				base + 0x15160 //150d0 //0x224B0 2.1
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
		/*FLAlertLayer supports colors of text for the caption. wrap desired text in "<cx></c>"
			* where x is the color desired. colors are:
			* r - red
			* l - cyan
			* g - green
			* y - yellow
			* o - orange
			* c - REDDD
			* p - purple
			* b - blue
			* and more that i'm too lazy to find.
			* MAKE SURE YOU FOLLOW THIS FORMAT. ROB'S PARSING CAN AND WILL MESS UP OTHERWISE.
			*/
		//static FLAlertLayer* create_original(FLAlertLayerProtocol* target, const char* title,
		//	const char* btn1, const char* btn2, std::string caption) {
		//	auto pRet = reinterpret_cast<FLAlertLayer* (__fastcall*)(FLAlertLayerProtocol*, const char*,
		//		const char*, const char*, std::string)>(
		//			base + 0x22680
		//			)(target, title, btn1, btn2, caption);
		//	//clean stack.
		//	__asm add esp, 0x20
		//	return pRet;
		//}
		//static FLAlertLayer* create_original(FLAlertLayerProtocol* target, const char* title,
		//	const char* btn1, const char* btn2, float width, std::string caption) {
		//	auto pRet = reinterpret_cast<FLAlertLayer* (__fastcall*)(FLAlertLayerProtocol*, const char*,
		//		const char*, const char*, float, std::string)>(
		//			base + 0x22730
		//			)(target, title, btn1, btn2, width, caption);
		//	__asm add esp, 0x24
		//	return pRet;
		//}
		
		/*
		auto menu() {
			return from<CCMenu*>(this, 0x194);
		}
		*/

		static auto create(FLAlertLayerProtocol* protocol, const char* title,
			const char* caption, const char* button1, const char* button2,
			float width, bool absolute, float height) {

			auto ret = reinterpret_cast<FLAlertLayer * (__fastcall*)(FLAlertLayerProtocol*, const char*, const char*,
				const char*, const char*, float, bool, float)>(base + 0x15360)(protocol, title,
					caption, button1, button2, width, absolute, height);
			__asm add esp, 24
			return ret;


		}
		//base + 0x15360 - GHS 1.92
		//base + 0x22680 and //base + 0x22730 - GD 2.13
		//base + 0x152d0 - GHS 1.91

		
		
	};
	#pragma runtime_checks("s", restore)
}


#endif