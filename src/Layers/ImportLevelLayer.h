#pragma once
#include "../include.h"

class ImportLevelCell : public TableViewCell {
protected:

public:
    ImportLevelCell();
};

class ManualImportLevelLayer : public FLAlertLayer, public TextInputDelegate {
protected:
    CCTextInputNode* m_pathInput;
    CCSprite* m_fileCheckSprite;

    virtual bool init();
    virtual void keyBackClicked();
    virtual void textChanged(CCTextInputNode*);
    void updateSprite();
    void onClose(CCObject*);
    void onImport(CCObject*);
public:
    static ManualImportLevelLayer* create() {
        ManualImportLevelLayer* ret = new ManualImportLevelLayer();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return ret;
    }
};