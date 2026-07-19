#ifndef __GJLISTLAYER_H__
#define __GJLISTLAYER_H__

#include <gd.h>

namespace gd {
	class BoomListView;

	class GJListLayer : public cocos2d::CCLayerColor {
	public:
		BoomListView* m_listView;

		static GJListLayer* create(BoomListView* target, char const* title, cocos2d::ccColor4B color, float width, float height) {
			__asm movss xmm3, width
			
			auto ret = reinterpret_cast<GJListLayer * (__fastcall*)(BoomListView*, char const*, cocos2d::ccColor4B, float)>(base + 0x81720)(target, title, color, height);

			__asm add esp, 0x8

			return ret;
		}
	};
}

#endif