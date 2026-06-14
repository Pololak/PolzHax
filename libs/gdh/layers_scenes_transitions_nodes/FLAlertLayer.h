#ifndef __FLALERTLAYER_H__
#define __FLALERTLAYER_H__

#include "../include/gd.h"

class FLAlertLayerProtocol;
class ScrollingLayer;

class FLAlertLayer : public cocos2d::CCLayerColor {
public:
    cocos2d::CCMenu* m_buttonMenu;
    int m_controlConnected;
    int m_ZOrder;
    FLAlertLayerProtocol* m_alertProtocol;
    cocos2d::CCNode* m_scene;
    bool m_reverseKeyBack;
    cocos2d::CCLayer* m_mainLayer;
    ScrollingLayer* m_scrollingLayer;
    int m_scrollAction;
    bool m_containsBorder;
    bool m_noAction;

    virtual ~FLAlertLayer() {
        cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->decrementForcePrio();
    }

public:
    virtual void registerWithTouchDispatcher();

    virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
    virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
    virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
    virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);

    virtual void keyDown(cocos2d::enumKeyCodes);

    virtual void keyBackClicked();

    virtual void show();

    FLAlertLayer();

    void onBtn1(cocos2d::CCObject*);
    void onBtn2(cocos2d::CCObject*);

    static FLAlertLayer* create(FLAlertLayerProtocol* protocol, char const* title, char const* desc, char const* btn1, char const* btn2, float width, bool scroll, float height);

    static FLAlertLayer* create(FLAlertLayerProtocol* protocol, char const* title, char const* desc, char const* btn1, char const* btn2, float width) {
        return FLAlertLayer::create(protocol, title, desc, btn1, btn2, width, false, 0.f);
    }

    static FLAlertLayer* create(FLAlertLayerProtocol* protocol, char const* title, char const* desc, char const* btn1, char const* btn2) {
        return FLAlertLayer::create(protocol, title, desc, btn1, btn2, 300.f);
    }

    static FLAlertLayer* create(char const* title, char const* desc, char const* btn1) {
        return FLAlertLayer::create(nullptr, title, desc, btn1, nullptr, 300.f);
    }
};

#endif