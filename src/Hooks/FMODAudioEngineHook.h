#pragma once
#include "../include.h"

namespace FMODAudioEngineHook {
    inline void(*playBackgroundMusic)(FMODAudioEngine*, std::string, bool, bool);
    void playBackgroundMusicH(FMODAudioEngine*, std::string, bool, bool);

    void mem_init();
}