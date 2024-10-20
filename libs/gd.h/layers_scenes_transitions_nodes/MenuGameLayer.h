#ifndef __MENUGAMELAYER_H__
#define __MENUGAMELAYER_H__

#include <gd.h>

namespace gd {

	class PlayerObject;

	class MenuGameLayer : public cocos2d::CCLayer {
	public:
		void tryJump(float param_1) {
			return reinterpret_cast<void(__fastcall*)(
				MenuGameLayer*, float
				)>(
					base + 0xADFF0
				)(
					this, param_1
				);
		}
		static MenuGameLayer* create() {
			return reinterpret_cast<MenuGameLayer * (__stdcall*)()>(
				base + 0xAD430
				)();
		}
		void update(float idk) {
			return reinterpret_cast<void(__thiscall*)(
				MenuGameLayer*, float
				)>(
					base + 0xAE210
				)(
					this, idk
				);
		}

		void destroyPlayer() {
			return reinterpret_cast<void(__fastcall*)(MenuGameLayer*)>(base + 0xaec50)(this);
		}

		PlayerObject* getPlayerObject() {
			return from<PlayerObject*>(this, 0x128);
		}
		/*gd::MenuGameLayer* create() {
			return reinterpret_cast<void(__thiscall*)(MenuGameLayer*)(
			)>(
				base + 0xAD430
			)(
				this
			);
		}*/
	};
}

// https://www.youtube.com/watch?v=zpU3b6H2PbU

#endif

