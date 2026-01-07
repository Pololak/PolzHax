#ifndef __STARTPOSOBJECT_H__
#define __STARTPOSOBJECT_H__

#include <gd.h>
#include "cocos2d.h"

namespace gd {

	class LevelSettingsObject;

	class StartPosObject : public GameObject {
	public:
			
		float getAudioScale() {
			return from<float>(this, 0x1AC);
		}
		static StartPosObject* create() {
			return reinterpret_cast<StartPosObject * (__fastcall*)()>(base + 0x75bf0)();
		}
		LevelSettingsObject* levelSettingsObject() {
			return from<LevelSettingsObject*>(this, 0x328);
		}
	};
}
#endif
