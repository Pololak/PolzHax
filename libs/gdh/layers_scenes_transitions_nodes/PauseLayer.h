#ifndef __PAUSELAYER_H__
#define __PAUSELAYER_H__

#include "../include/gd.h"

class CCBlockLayer;

class PauseLayer : public CCBlockLayer {
public:
    bool m_unfocused;

    void createToggleButton(std::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
};

#endif