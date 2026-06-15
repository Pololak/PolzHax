#pragma once
#include "../include.h"

namespace LevelSettingsLayerHook {
    LevelSettingsLayer* get();

    inline bool(*init)(LevelSettingsLayer*, LevelSettingsObject*);
    bool initH(LevelSettingsLayer*, LevelSettingsObject*);

    inline void(*colorSelectClosed)(LevelSettingsLayer*, ColorSelectPopup*);
    void colorSelectClosedH(LevelSettingsLayer*, ColorSelectPopup*);

    inline void(*selectArtClosed)(LevelSettingsLayer*, SelectArtLayer*);
    void selectArtClosedH(LevelSettingsLayer*, SelectArtLayer*);

    inline void(*destructor)(LevelSettingsLayer*);
    void destructorH(LevelSettingsLayer*);

    class Callback : public LevelSettingsLayer {
    public:
        void onPlaytestStartPos(CCObject*);
        void togglePlaytestStartPos(CCObject*);
    };

    void mem_init();
}