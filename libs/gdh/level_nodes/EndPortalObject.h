#ifndef __ENDPORTALOBJECT_H__
#define __ENDPORTALOBJECT_H__

#include "../include/gd.h"

class GameObject;

class EndPortalObject : public GameObject {
public:
	cocos2d::CCSprite* m_gradientBar;
};

#endif
