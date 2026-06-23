#ifndef __SCROLLINGLAYER_H__
#define __SCROLLINGLAYER_H__

#include <gd.h>

namespace gd {
	class ScrollingLayer : public cocos2d::CCLayerColor { // I do care.
    public:
        cocos2d::CCPoint m_position; // 0x194
        cocos2d::CCSize m_size; // 0x19c
        cocos2d::CCPoint m_touchStartPosition; // 0x1a4
        cocos2d::CCPoint m_touchPosition; // 0x1ac
        cocos2d::CCPoint m_startOffset; // 0x1b4
        float m_scrollFactor; // 0x1bc
        int m_touchID; // 0x1c0
        cocos2d::CCLayer* m_contentLayer; // 0x1c4
        cocos2d::CCLayer* m_parentLayer; // 0x1c8

        static ScrollingLayer* create(cocos2d::CCSize size, cocos2d::CCPoint position, float factor) {
            auto ret = reinterpret_cast<ScrollingLayer * (__stdcall*)(cocos2d::CCSize, cocos2d::CCPoint, float)>(base + 0x1c4a0)(size, position, factor);

            __asm add esp, 0x14

            return ret;
        }
	};
}

#endif