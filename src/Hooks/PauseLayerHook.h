#pragma once
#include "../include.h"

namespace PauseLayerHook {
    inline void(*customSetup)(PauseLayer*);
    void customSetupH(PauseLayer*);

    inline void(*onProgressBar)(PauseLayer*, CCObject*);
    void onProgressBarH(PauseLayer*, CCObject*);

    class Callback : public PauseLayer {
    public:
        void onPolzHax(CCObject*);

        void onShowPercentage(CCObject*);
        void onHide(CCObject*);
    };

    void mem_init();
}