#ifndef __OBB2D_H__
#define __OBB2D_H__

#include <gd.h>

namespace gd {
	class OBB2D : public cocos2d::CCNode {
	public:
		cocos2d::CCPoint m_obVertexBottomLeft;
		cocos2d::CCPoint m_obVertexBottomRight;
		cocos2d::CCPoint m_obVertexTopRight;
		cocos2d::CCPoint m_obVertexTopLeft;
		cocos2d::CCPoint m_p2_1;
		cocos2d::CCPoint m_p2_2;
		cocos2d::CCPoint m_p2_3;
		cocos2d::CCPoint m_p2_4;
		cocos2d::CCPoint m_p3_1;
		cocos2d::CCPoint m_p3_2;
		cocos2d::CCPoint m_p3_3;
		cocos2d::CCPoint m_p3_4;
		double m_rot1;
		double m_rot2;
		cocos2d::CCPoint m_center;
	};
}

#endif // !__OBB2D_H__
