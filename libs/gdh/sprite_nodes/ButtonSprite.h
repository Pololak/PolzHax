#ifndef __BUTTONSPRITE_H__
#define __BUTTONSPRITE_H__

#include "../include/gd.h"

class ButtonSprite : public cocos2d::CCSprite {
public:
    int m_mode;
    float m_width;
    float m_fUnknown1;
    float m_scale;
    float m_height;
    bool m_absolute;
    cocos2d::CCLabelBMFont* m_label;
    cocos2d::CCSprite* m_btnSprite;
    cocos2d::extension::CCScale9Sprite* m_bgSprite;
    cocos2d::CCPoint m_textOffset;
    cocos2d::CCPoint m_spritePosition;
    std::string m_caption;

    static ButtonSprite* create(cocos2d::CCSprite*, int, int, float, float, bool, char const*);
    static ButtonSprite* create(cocos2d::CCSprite*, int, int, float, float, bool);
    static ButtonSprite* create(cocos2d::CCSprite*);
    static ButtonSprite* create(char const*, int, int, float, bool, char const*, char const*, float);
    static ButtonSprite* create(char const*, int, int, float, bool, char const*, char const*);
    static ButtonSprite* create(char const*, int, int, float, bool);
    static ButtonSprite* create(char const*, float);
    static ButtonSprite* create(char const*);

    void setColor(cocos2d::_ccColor3B);
    void setString(char const*);
    void updateBGImage(char const*);
};

#endif