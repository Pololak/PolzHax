#ifndef __ENDPORTALOBJECT_H__
#define __ENDPORTALOBJECT_H__

#include <gd.h>

namespace gd {
	class GameObject;

	class EndPortalObject : public GameObject {
	public:
		cocos2d::CCSprite* m_gradientBar; // 0x328
	};
}

#endif // !__ENDPORTALOBJECT_H__
