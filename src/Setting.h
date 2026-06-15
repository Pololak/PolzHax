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
};

SettingStruct& setting();

DEF_SCHEMA(
    SettingStruct,
    onCharacterFilter,
    onIcons,
    onMainLevels,
    onSliderLimit,
    onTextLength
)