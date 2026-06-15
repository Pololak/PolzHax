#ifndef __GJLISTLAYER_H__
#define __GJLISTLAYER_H__

#include "../include/gd.h"

#pragma runtime_checks("s", off)
class GJListLayer : public cocos2d::CCLayerColor {
protected:
    CCObject* m_pTarget;

public:
    static GJListLayer* create(CCObject* target, const char* title, cocos2d::ccColor4B color, float width, float height);
};
#pragma runtime_checks("s", restore)

#endif