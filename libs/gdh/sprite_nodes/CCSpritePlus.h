#ifndef __CCSPRITEPLUS_H__
#define __CCSPRITEPLUS_H__

#include "../include/gd.h"

class CCSpritePlus : public cocos2d::CCSprite {
protected:
    cocos2d::CCArray* m_followers;
    CCSpritePlus* m_followingSprite;
    bool m_hasFollower;
};

#endif