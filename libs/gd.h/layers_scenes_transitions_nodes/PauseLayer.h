#ifndef __PAUSELAYER_H__
#define __PAUSELAYER_H__

#include <gd.h>

namespace gd {
	class CCBlockLayer;
	class CCMenuItemToggler;

	class PauseLayer : public CCBlockLayer {
	public:
		bool m_unfocused; // 0x198

		void createToggleButton(std::string label, cocos2d::SEL_MenuHandler callback, bool value, cocos2d::CCMenu* menu, cocos2d::CCPoint pos) {
			return reinterpret_cast<void(__thiscall*)(PauseLayer*, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, std::string, cocos2d::CCPoint)>(base + 0xd6eb0)(this, callback, value, menu, label, pos);
		}

		void onResume(cocos2d::CCObject* sender) {
			reinterpret_cast<void(__thiscall*)(PauseLayer*, cocos2d::CCObject*)>(base + 0xd7b20)(this, sender);
		}
	};
}

#endif