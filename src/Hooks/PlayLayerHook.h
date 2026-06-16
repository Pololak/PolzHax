#pragma once
#include "../include.h"

namespace PlayLayerHook {
    inline bool(*init)(PlayLayer*, GJGameLevel*);
    bool initH(PlayLayer*, GJGameLevel*);

    inline void(*update)(PlayLayer*, float);
    void updateH(PlayLayer*, float);

    inline void(*resetLevel)(PlayLayer*);
    void resetLevelH(PlayLayer*);

    inline void(*addToSection)(PlayLayer*, GameObject*);
    void addToSectionH(PlayLayer*, GameObject*);

    inline void(*createObjectsFromSetup)(PlayLayer*, std::string);
    void createObjectsFromSetupH(PlayLayer*, std::string);

    inline void(*updateVisibility)(PlayLayer*, float);
    void updateVisibilityH(PlayLayer*, float);

    inline void(*updateAttempts)(PlayLayer*);
    void updateAttemptsH(PlayLayer*);

    inline void(*processItems)(PlayLayer*);
    void processItemsH(PlayLayer*);

    inline void(*destroyPlayer)(PlayLayer*, PlayerObject*);
    void destroyPlayerH(PlayLayer*, PlayerObject*);

    inline void(*levelComplete)(PlayLayer*);
    void levelCompleteH(PlayLayer*);

    inline void(*loadLastCheckpoint)(PlayLayer*);
    void loadLastCheckpointH(PlayLayer*);

    inline void(*destructor)(PlayLayer*);
    void destructorH(PlayLayer*);

    void updateStartPosSwitcherLabel();
    void nextStartPos();
    void prevStartPos();

    void mem_init();
}