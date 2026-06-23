#ifndef __CCCONTENTLAYER_H__
#define __CCCONTENTLAYER_H__

#include <gd.h>

namespace gd {
	class CCContentLayer : public cocos2d::CCLayerColor {
	public:
		static CCContentLayer* create(const cocos2d::ccColor4B& color, float width, float height) {
			__asm {
				movss xmm1, width
				movss xmm2, height
			}
			return reinterpret_cast<CCContentLayer* (__fastcall*)(const cocos2d::ccColor4B&)>(
				base + 0x172A0
				)(color);
		}

		virtual void setPosition(cocos2d::CCPoint const& position) {
			reinterpret_cast<void(__thiscall*)(CCContentLayer*, cocos2d::CCPoint const&)>(base + 0xb7b0)(this, position);
		}
	};
}

#endif