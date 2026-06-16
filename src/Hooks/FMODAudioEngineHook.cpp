#include "FMODAudioEngineHook.h"
#include "../Setting.h"
#include "../Speedhack.h"

void FMODAudioEngineHook::playBackgroundMusicH(FMODAudioEngine* self, std::string path, bool p0, bool p1) {
    FMODAudioEngineHook::playBackgroundMusic(self, path, p0, p1);

    if (setting().onSpeedhack && setting().onSpeedhackMusic) {
        Speedhack::updateSpeedhackMusic();
    }
}

void FMODAudioEngineHook::mem_init() {
    HOOK("_ZN15FMODAudioEngine19playBackgroundMusicESsbb", FMODAudioEngineHook::playBackgroundMusicH, FMODAudioEngineHook::playBackgroundMusic);
}