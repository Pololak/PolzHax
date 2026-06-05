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

		static CustomListView* create(cocos2d::CCArray* entries, int type, float width, float height) {
			__asm {
				movss xmm1, height
				movss xmm2, width
			}

			auto pRet = reinterpret_cast<CustomListView* (__thiscall*)(cocos2d::CCArray*, int)>(base + 0x2e970)(entries, type);

			__asm add esp, 0x4

			return pRet;
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif