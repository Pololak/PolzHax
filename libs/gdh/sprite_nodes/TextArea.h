#ifndef __TEXTAREA_H__
#define __TEXTAREA_H__

#include "../include/gd.h"

class TextArea : public cocos2d::CCSprite {
public:
    void* m_label;
    float m_width;
    int m_iUnused1;
    std::string m_fontFile;
    float m_height;
    bool m_onTimer;
    cocos2d::CCPoint m_anchorPoint;
    bool m_allShown;
    float m_scale;
    int m_rectHeight;
    int m_rectWidth;
    float m_maxWidth;
    cocos2d::CCPoint m_textPosition;
};

#endif