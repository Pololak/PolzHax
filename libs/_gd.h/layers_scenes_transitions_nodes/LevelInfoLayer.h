#ifndef __LEVELINFOLAYER_H__
#define __LEVELINFOLAYER_H__

#include <gd.h>

namespace gd {

	class GJGameLevel;

	class LevelInfoLayer : public cocos2d::CCLayer {
	public:
		auto level() {
			return from<GJGameLevel*>(this, 0x13c);
		}

		bool shouldDownloadLevel() {
			return reinterpret_cast<bool(__thiscall*)(LevelInfoLayer*)>(base + 0x9cc40)(this);
		}
	};
}

#endif
