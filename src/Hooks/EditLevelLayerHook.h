#pragma once
#include "../include.h"

namespace EditLevelLayerHook {
    inline bool(*init)(EditLevelLayer*, GJGameLevel*);
    bool initH(EditLevelLayer*, GJGameLevel*);

    inline void(*FLAlert_Clicked)(EditLevelLayer*, FLAlertLayer*, bool);
    void FLAlert_ClickedH(EditLevelLayer*, FLAlertLayer*, bool);

    inline void(*onClone)(EditLevelLayer*);
    void onCloneH(EditLevelLayer*);

    void updateLevelOrderLabel(EditLevelLayer*);

    class Callback : public EditLevelLayer {
    public:
        void onMoveToTop(CCObject*);
        void onExportLevel(CCObject*);
        void onLevelOrderUp(CCObject*);
        void onLevelOrderDown(CCObject*);
        void onLevelID(CCObject*);
    };

    void mem_init();
}