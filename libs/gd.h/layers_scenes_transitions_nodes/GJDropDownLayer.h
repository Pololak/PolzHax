#ifndef __GJDROPDOWNLAYER_H__
#define __GJDROPDOWNLAYER_H__

#include <gd.h>

namespace gd {

	class GJListLayer;

	#pragma runtime_checks("s", off)
	class GJDropDownLayer : public cocos2d::CCLayerColor {
	public:
		void showLayer(bool transition) {
			reinterpret_cast<void(__thiscall*)(GJDropDownLayer*, bool)>(base + 0x7bf20)(this, transition);
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif