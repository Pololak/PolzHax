#pragma once
#include "../include.h"

class ExportLevelLayer : public FLAlertLayer, FLAlertLayerProtocol {
protected:
    GJGameLevel* m_level;
    CCTextInputNode* m_nameInput;

    virtual bool init(GJGameLevel*);
    virtual void keyBackClicked();
    virtual void FLAlert_Clicked(FLAlertLayer*, bool) override;
    void onClose(CCObject*);
    void onExport(CCObject*);
public:
    static ExportLevelLayer* create(GJGameLevel* level) {
        ExportLevelLayer* ret = new ExportLevelLayer();
        if (ret && ret->init(level)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return ret;
    }
};