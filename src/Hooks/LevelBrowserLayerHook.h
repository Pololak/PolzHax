#pragma once
#include "../include.h"

namespace LevelBrowserLayerHook {
    inline bool(*init)(LevelBrowserLayer*, GJSearchObject*);
    bool initH(LevelBrowserLayer*, GJSearchObject*);

    inline void(*loadPage)(LevelBrowserLayer*, GJSearchObject*);
    void loadPageH(LevelBrowserLayer*, GJSearchObject*);

    inline void(*FLAlert_Clicked)(LevelBrowserLayer*, FLAlertLayer*, bool);
    void FLAlert_ClickedH(LevelBrowserLayer*, FLAlertLayer*, bool);

    void updatePageButton(LevelBrowserLayer*);

    class Callback : public LevelBrowserLayer {
    public:
        void onImportLevel(CCObject*);
        void onRefresh(CCObject*);

        void onFirstPage(CCObject*);
        void onLastPage(CCObject*);
        void onGoToPage(CCObject*);

        void onDeleteSelected(CCObject*);
    };

    void mem_init();
}