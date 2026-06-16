#pragma once
#include "../include.h"

class SpeedHackLayer : public FLAlertLayer, TextInputDelegate {
protected:
    CCTextInputNode* m_input;

    virtual bool init();
    virtual void keyBackClicked();
    virtual void textChanged(CCTextInputNode*);
    void onClose(CCObject*);
    void onSpeedhackMusic(CCObject*);

public:
    static SpeedHackLayer* create() {
        SpeedHackLayer* ret = new SpeedHackLayer();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};