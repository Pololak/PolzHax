#ifndef __LOADINGCIRCLE_H__
#define __LOADINGCIRCLE_H__

#include "../include/gd.h"

class LoadingCircle : public cocos2d::CCLayerColor {
public:
    cocos2d::CCSprite* m_sprite; // m_circle
    cocos2d::CCLayer* m_parentLayer; // m_targetScene
    bool m_fade; // m_darkOverlay

    static LoadingCircle* create();

    void show();

    void fadeAndRemove();
};

#endif