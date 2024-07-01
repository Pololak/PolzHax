#ifndef __STARTPOSOBJECT_H__
#define __STARTPOSOBJECT_H__

#include <gd.h>
#include "cocos2d.h"

namespace gd {

	class StartPosObject : public GameObject {
	public:
			
		static StartPosObject* create() {
			return reinterpret_cast<StartPosObject * (__fastcall*)()>(base + 0x754a0)();
		}
	};
}
#endif
