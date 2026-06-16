#pragma once
#include "include.h"

namespace ModPatches {
    void loadPatches();

    // Bypass
    void onCharacterFilter();
    void onIcons();
    void onMainLevels();
    void onSliderLimit();
    void onTextLength();

    // Cosmetic
    void onNoDeathEffect();
}