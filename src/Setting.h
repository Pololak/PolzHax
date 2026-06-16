#pragma once
#include "include.h"
#include "schema.h"

struct SettingStruct {
    void load();
    void save();

    bool onCharacterFilter;
    bool onIcons;
    bool onMainLevels;
    bool onSliderLimit;
    bool onTextLength;

    bool onNoDeathEffect;

    bool onSpeedhack, onSpeedhackMusic;
    float speedhackValue = 1.f;
};

SettingStruct& setting();