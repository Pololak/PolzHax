#pragma once
#include "../include.h"

namespace UILayerHook {
    inline bool(*init)(UILayer*);
    bool initH(UILayer*);

    inline void(*onPause)(UILayer*, CCObject*);
    void onPauseH(UILayer*, CCObject*);

    class Callback : public UILayer {
    public:
        void onPrevStartPos(CCObject*);
        void onNextStartPos(CCObject*);
    };

    void mem_init();
}