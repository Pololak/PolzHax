#pragma once
#include "../include.h"

namespace CustomizeObjectLayerHook {
    inline bool(*init)(CustomizeObjectLayer*, GameObject*, CCArray*);
    bool initH(CustomizeObjectLayer*, GameObject*, CCArray*);

    inline void(*onSelectColor)(CustomizeObjectLayer*, CCObject*);
    void onSelectColorH(CustomizeObjectLayer*, CCObject*);

    void mem_init();
}