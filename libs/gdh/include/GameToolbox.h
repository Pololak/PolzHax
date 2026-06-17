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

    static cocos2d::ccColor3B getMixedColor(cocos2d::ccColor3B color1, cocos2d::ccColor3B color2, float ratio) {
        auto r = color1.r * ratio + color2.r * (1.f - ratio);
        auto g = color1.g * ratio + color2.g * (1.f - ratio);
        auto b = color1.b * ratio + color2.b * (1.f - ratio);
        return { clamp<uint8_t>(r, 0, 255), clamp<uint8_t>(g, 0, 255), clamp<uint8_t>(b, 0, 255) };
    }
};

#endif