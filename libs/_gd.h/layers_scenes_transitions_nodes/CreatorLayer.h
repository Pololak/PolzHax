#ifndef __CREATORLAYER_H__
#define __CREATORLAYER_H__

#include <gd.h>
#include "cocos2d.h"

namespace gd {

	class CreatorLayer : public cocos2d::CCLayer {
	public:

		auto getArray() {
			return from<cocos2d::CCArray*>(this, 0xA8);
		}
		//init base + 0x2cc30;
	};
}
#endif
