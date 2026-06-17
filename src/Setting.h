#pragma once
#include "include.h"
#include "schema.h"
#include "GameVariables.h"

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

    // Creator
    bool onHitboxBugFix;

    // Level
    bool onSmartStartPos;
    bool onStartPosSwitcher;

    // Universal
    bool onNoTransition;

    // Speehack
    bool onSpeedhack, onSpeedhackMusic;
    float speedhackValue = 1.f;

    // Editor options
    bool onDurationLines;
    bool onExperimentalLayering;
    bool onPreviewRotations;
    bool onShowClicks;
    bool onUnusedObjects;
    bool onNewColorSelectMenu;

    // Other menu things
    bool m_liveColorEnabled;
};

SettingStruct& setting();