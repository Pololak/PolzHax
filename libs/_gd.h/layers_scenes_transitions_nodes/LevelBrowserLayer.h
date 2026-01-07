#ifndef __LEVELBROWSERLAYER_H__
#define __LEVELBROWSERLAYER_H__

#include <gd.h>
#include <cocos2d.h>

namespace gd
{
	class GJSearchObject;

	class LevelBrowserLayer : public cocos2d::CCLayer {
	public:
		static auto create(GJSearchObject* obj) {
			return reinterpret_cast<LevelBrowserLayer * (__fastcall*)(GJSearchObject*)>(base + 0x894f0)(obj);
		}

		virtual bool init(gd::GJSearchObject* search) {
			return reinterpret_cast<bool(__thiscall*)(LevelBrowserLayer*, gd::GJSearchObject*)>(base + 0x89590)(this, search);
		}

		//static auto create(GJSearchObject* obj) {
		//	return reinterpret_cast<LevelBrowserLayer * (__fastcall*)(gd::GJSearchObject*)>(base + 0x894f0)(obj); //0x894f0 GD 1.92
		//}

		void loadPage(GJSearchObject* search) {
			return reinterpret_cast<void(__thiscall*)(LevelBrowserLayer*, gd::GJSearchObject*)>(base + 0x8a000)(this, search);
		}
		

		static cocos2d::CCScene* scene(gd::GJSearchObject* obj) {
			auto scene = cocos2d::CCScene::create();
			auto lvlbrowser = LevelBrowserLayer::create(obj);
			scene->addChild(lvlbrowser);
			// AppDelegate::get()->m_runningScene = scene;
			return scene;
		}
	};
}

#endif