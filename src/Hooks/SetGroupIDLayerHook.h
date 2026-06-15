#pragma once
#include "../include.h"

namespace SetGroupIDLayerHook {
    inline bool(*init)(SetGroupIDLayer*, GameObject*, CCArray*);
    bool initH(SetGroupIDLayer*, GameObject*, CCArray*);

    inline void(*updateGroupID)(SetGroupIDLayer*);
    void updateGroupIDH(SetGroupIDLayer*);

    class Callback : public SetGroupIDLayer {
    public:
        void onCurrentGroup(CCObject*);
        void onNextFree(CCObject*);
    };

    void mem_init();
}