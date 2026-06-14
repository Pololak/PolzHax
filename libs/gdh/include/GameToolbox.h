#ifndef __GAMETOOLBOX_H__
#define __GAMETOOLBOX_H__

#include "gd.h"

class CCMenuItemTogler;

class GameToolbox {
public:
    static CCMenuItemToggler* createToggleButton(
        std::string label,
        cocos2d::SEL_MenuHandler callback,
        bool isActive,
        cocos2d::CCMenu* targetMenu,
        cocos2d::CCPoint position,
        cocos2d::CCNode* callbackObject,
        cocos2d::CCNode* targetNode,
        float toggleScale,
        float labelMaxScale,
        float labelMaxWidth,
        cocos2d::CCPoint labelOffset,
        char const* font,
        bool verticalLayout
    );
};

#endif