#ifndef __CUSTOMIZEOBJECTLAYER_H__
#define __CUSTOMIZEOBJECTLAYER_H__

#include "../include/gd.h"

class GameObject;
class ButtonSprite;

class CustomizeObjectLayer : public FLAlertLayer {
public:
    GameObject* m_targetObject; // 0x1bc
    cocos2d::CCArray* m_targetObjects; // 0x1c0
    cocos2d::CCArray* m_buttonsArray; // 0x1c4

    void highlightSelected(ButtonSprite*);

    void onSelectColor(cocos2d::CCObject*);
};

#endif
