#pragma once
#include "../include.h"

namespace LevelInfoLayerHook {
    inline bool(*init)(LevelInfoLayer*, GJGameLevel*);
    bool initH(LevelInfoLayer*, GJGameLevel*);

    inline void(*onClone)(LevelInfoLayer*, CCObject*);
    void onCloneH(LevelInfoLayer*, CCObject*);

    inline void(*FLAlert_Clicked)(LevelInfoLayer*, FLAlertLayer*, bool);
    void FLAlert_ClickedH(LevelInfoLayer*, FLAlertLayer*, bool);

    inline void(*levelUpdateFinished)(LevelInfoLayer*, GJGameLevel*, UpdateResponse);
    void levelUpdateFinishedH(LevelInfoLayer*, GJGameLevel*, UpdateResponse);

    inline void(*destructor)(LevelInfoLayer*);
    void destructorH(LevelInfoLayer*);

    class Callback : public LevelInfoLayer {
    public:
        void onPolzHax(CCObject*);

        void onExportLevel(CCObject*);
        void onMoveToTop(CCObject*);
    };

    void mem_init();
}