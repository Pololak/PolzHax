#pragma once
#include "include.h"
#include "schema.h"

struct SettingStruct {
    void load();
    void save();

    // Bypass
    bool onCharacterFilter;
    bool onIcons;
    bool onMainLevels;
    bool onSliderLimit;
    bool onTextLength;

    // Cosmetic
    bool onNoDeathEffect;

    // Level
    bool onSmartStartPos;
    bool onStartPosSwitcher;

    bool onSpeedhack, onSpeedhackMusic;
    float speedhackValue = 1.f;
};

SettingStruct& setting();