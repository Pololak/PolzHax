#ifndef __SETTINGSCOLOROBJECT_H__
#define __SETTINGSCOLOROBJECT_H__

#include "../include/gd.h"

class SettingsColorObject : public cocos2d::CCNode {
public:
    cocos2d::ccColor3B m_color;
    bool m_blending;
    int m_playerColor;
};

#endif
