#ifndef __COLORACTION_H__
#define __COLORACTION_H__

#include <gd.h>

namespace gd {
	class ColorAction : public cocos2d::CCNode {
	public:
		cocos2d::ccColor3B m_fromColor;
		cocos2d::ccColor3B m_toColor;
		float m_duration;
		double m_timeStamp;
		bool m_blend;
	};
}

#endif // !__COLORACTION_H__
