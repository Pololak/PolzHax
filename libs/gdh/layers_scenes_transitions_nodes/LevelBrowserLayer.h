#ifndef __LEVELBROWSERLAYER_H__
#define __LEVELBROWSERLAYER_H__

#include "../include/gd.h"

class TextArea;
class GJListLayer;
class CCMenuItemSpriteExtra;
class GJSearchObject;
class LoadingCircle;

class LevelBrowserLayer : public cocos2d::CCLayer, LevelManagerDelegate, public FLAlertLayerProtocol {
public:
    TextArea* m_noInternet; // 0x120
    GJListLayer* m_list; // 0x124
    CCMenuItemSpriteExtra* m_rightArrow; // 0x128
    CCMenuItemSpriteExtra* m_leftArrow; // 0x12c
    cocos2d::CCArray* m_array; // 0x130
    GJSearchObject* m_searchObject; // 0x134
    cocos2d::CCLabelBMFont* m_countText; // 0x138
    int m_itemCount; // 0x13c
    int m_pageStartIdx; // 0x140
    int m_pageEndIdx; // 0x144
    LoadingCircle* m_circle; // 0x148

    static cocos2d::CCScene* scene(GJSearchObject* searchObject);

    void loadPage(GJSearchObject* search);

    void setupLevelBrowser(cocos2d::CCArray* levels);

    void updateLevelsLabel();
};

#endif // !__LEVELBROWSERLAYER_H__
