#ifndef __CUSTOMLISTVIEW_H__
#define __CUSTOMLISTVIEW_H__

#include <gd.h>

namespace gd {
	class BoomListView;

	#pragma runtime_checks("s", off)
	class CustomListView : public BoomListView {
	public:
		CustomListView() {
			reinterpret_cast<void(__thiscall*)(CustomListView*)>(base + 0x2e890)(this);
		}

		static CustomListView* create(cocos2d::CCArray* entries, float width, float height, BoomListType type) {
			__asm {
				movss xmm1, width
				movss xmm2, height
			}
			auto ret = reinterpret_cast<CustomListView * (__thiscall*)(cocos2d::CCArray*, BoomListType)>(base + 0x2e970)(entries, type);

			__asm add esp, 0x4

			return ret;
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif