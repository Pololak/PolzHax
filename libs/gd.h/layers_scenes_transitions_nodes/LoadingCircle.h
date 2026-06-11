#ifndef __LOADINGCIRCLE_H__
#define __LOADINGCIRCLE_H__

#include <gd.h>

namespace gd {
	class LoadingCircle : public cocos2d::CCLayerColor {
	public:
		cocos2d::CCSprite* m_sprite; // m_circle
		cocos2d::CCLayer* m_parentLayer; // m_targetScene
		bool m_fade; // m_darkOverlay

		static LoadingCircle* create() {
			return reinterpret_cast<LoadingCircle * (__stdcall*)()>(base + 0x1a4f0)();
		}

		void show() {
			reinterpret_cast<void(__thiscall*)(LoadingCircle*)>(base + 0x1a6b0)(this);
		}

		void fadeAndRemove() {
			reinterpret_cast<void(__thiscall*)(LoadingCircle*)>(base + 0x1a780)(this);
		}
	};
}

#endif