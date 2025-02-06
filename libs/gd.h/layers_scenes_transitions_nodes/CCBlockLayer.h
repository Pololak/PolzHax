#ifndef __CCBLOCKLAYER_H__
#define __CCBLOCKLAYER_H__

#include <gd.h>

namespace gd {
	class CCBlockLayer : public cocos2d::CCLayerColor {
	public:
		bool m_removeOnExit;
		bool m_incrementedTouchPriority;
	};
}

#endif