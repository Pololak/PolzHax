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

    // Editor
    void onEditorExtension();
    void onObjectBypass();

    // Cosmetic
    void onNoDeathEffect();

    // Level
    void onNoclip();
    void onPracticeMusic();
}