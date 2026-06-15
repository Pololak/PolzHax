#pragma once
#include "../include.h"

class EditorOptionsLayer : public FLAlertLayer {
public:
    virtual bool init();
    virtual void keyBackClicked();
    void onClose(CCObject*);

    void onCopyString(CCObject*);
};