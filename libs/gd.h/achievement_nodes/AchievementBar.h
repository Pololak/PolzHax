#ifndef __ACHIEVEMENTBAR_H__
#define __ACHIEVEMENTBAR_H__

#include <gd.h>

namespace gd {
	#pragma runtime_checks("s", off)
	class AchievementBar : public cocos2d::CCNode {
	public:
		cocos2d::CCLayerColor* m_layerColor; // 0xe8
		float m_screenOffset; // 0xec
		cocos2d::CCNode* m_targetScene; // 0xf0

		static AchievementBar* create(char const* title, char const* desc, char const* icon) {
			auto ret = reinterpret_cast<AchievementBar * (__fastcall*)(char const*, char const*, char const*)>(base + 0x273d0)(title, desc, icon);

			__asm add esp, 0x4

			return ret;
		}

		void show() {
			reinterpret_cast<void(__fastcall*)(AchievementBar*)>(base + 0x27d80)(this);
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif