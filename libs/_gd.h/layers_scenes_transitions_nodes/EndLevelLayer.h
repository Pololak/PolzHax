#ifndef __ENDLEVELLAYER_H__
#define __ENDLEVELLAYER_H__

#include <gd.h>

namespace gd {

	class EndLevelLayer : public cocos2d::CCLayer {
	public:
		static EndLevelLayer* create() {
			return reinterpret_cast<EndLevelLayer * (__stdcall*)()>(
				base + 0x502C0
				)();
		}
	};
}

#endif