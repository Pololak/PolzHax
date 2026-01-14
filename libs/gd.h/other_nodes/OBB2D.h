#ifndef __OBB2D_H__
#define __OBB2D_H__

#include <gd.h>

namespace gd {
	class OBB2D : public cocos2d::CCNode {
	public:
		cocos2d::CCPoint m_obVertexBottomLeft; // 0xe8
		cocos2d::CCPoint m_obVertexBottomRight; // 0xf0
		cocos2d::CCPoint m_obVertexTopRight; // 0xf8
		cocos2d::CCPoint m_obVertexTopLeft; // 0x100
		cocos2d::CCPoint m_p2_1; // 0x108
		cocos2d::CCPoint m_p2_2; // 0x110
		cocos2d::CCPoint m_p2_3; // 0x118
		cocos2d::CCPoint m_p2_4; // 0x120
		cocos2d::CCPoint m_p3_1; // 0x128
		cocos2d::CCPoint m_p3_2; // 0x130
		cocos2d::CCPoint m_p3_3; // 0x138
		cocos2d::CCPoint m_p3_4; // 0x140
		double m_rot1; // 0x148
		double m_rot2; // 0x150
		cocos2d::CCPoint m_center; // 0x158
	};
}

#endif // !__OBB2D_H__
