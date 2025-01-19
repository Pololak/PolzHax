#ifndef __LEVELBROWSERLAYER_H__
#define __LEVELBROWSERLAYER_H__

#include <gd.h>
#include <cocos2d.h>

namespace gd
{
	class GJSearchObject;

	class LevelBrowserLayer : public cocos2d::CCLayer {
	public:
		static LevelBrowserLayer* create(gd::GJSearchObject* pSearch) {
			return reinterpret_cast<LevelBrowserLayer * (__fastcall*)(
				gd::GJSearchObject*
				)>(
					gd::base + 0x894f0
					)(
						pSearch
						);
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
		
		static void scene(gd::GJSearchObject* pSearch) {
			auto pScene = cocos2d::CCScene::create();

			pScene->addChild(gd::LevelBrowserLayer::create(pSearch));

			cocos2d::CCDirector::sharedDirector()->replaceScene(
				cocos2d::CCTransitionFade::create(.5f, pScene)
			);
		}
	};
}

#endif