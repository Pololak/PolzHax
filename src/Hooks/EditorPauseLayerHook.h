#pragma once
#include "../include.h"

namespace EditorPauseLayerHook {
    inline bool(*init)(EditorPauseLayer*, LevelEditorLayer*);
    bool initH(EditorPauseLayer*, LevelEditorLayer*);

    inline void(*customSetup)(EditorPauseLayer*);
    void customSetupH(EditorPauseLayer*);

    inline void(*FLAlert_Clicked)(EditorPauseLayer*, FLAlertLayer*, bool);
    void FLAlert_ClickedH(EditorPauseLayer*, FLAlertLayer*, bool);

    inline void(*destructor)(EditorPauseLayer*);
    void destructorH(EditorPauseLayer*);

    class Callback : public EditorPauseLayer {
    public:
        void onSave(CCObject*);

        void onSelectAll(CCObject*);
        void onSelectAllLeft(CCObject*);
        void onSelectAllRight(CCObject*);
        void onAlignY(CCObject*);
        void onAlignX(CCObject*);

        void onSmallEditorStep(CCObject*);
        void onSelectFilter(CCObject*);
        void onShowObjectInfo(CCObject*);
        void onPreviewMode(CCObject*);
        void onShowGround(CCObject*);

        void onEditorOptions(CCObject*);

        void onResetStartPos(CCObject*);
    };

    void mem_init();
}