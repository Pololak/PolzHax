#ifndef __MENULAYER_H__
#define __MENULAYER_H__

#include <gd.h>

namespace gd {

	class FLAlertLayerProtocol;

	class MenuLayer : public cocos2d::CCLayer, public gd::FLAlertLayerProtocol {
	public:
		static MenuLayer* init() {
			return reinterpret_cast<MenuLayer * (__stdcall*)()>(
				base + 0xaf210
				)();
		}
		static MenuLayer* node() {
			return reinterpret_cast<MenuLayer* (__stdcall*)()>(
				base + 0xaf100 //190550
				)();
		}
		static cocos2d::CCScene* scene(bool options) {
			return reinterpret_cast<cocos2d::CCScene* (__fastcall*)(bool)>(
				base + 0xaf1a0 //190720
				)(options);
		}
		void onGarage(CCObject* sender) {
			return reinterpret_cast<void(__thiscall*)(MenuLayer*, CCObject*)>(base + 0xb0460)(this, sender);
		}
		void onOptions(CCObject* sender) {
			return reinterpret_cast<void(__thiscall*)(MenuLayer*, CCObject*)>(base + 0xb02e0)(this, sender);
		}
		void onOptionsInstant(CCObject* sender) {
			return reinterpret_cast<void(__thiscall*)(MenuLayer*, CCObject*)>(base + 0xb0310)(this, sender);
		}
		void onPlay(CCObject* sender) {
			return reinterpret_cast<void(__thiscall*)(MenuLayer*, CCObject*)>(base + 0xb0200)(this, sender);
		}
	};

	class GJMoreGamesLayer : public GJDropDownLayer {
	public:
		cocos2d::CCArray* getMoreGamesList();
	};
}

#endif