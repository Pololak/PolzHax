#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>
#include <gd.h>

struct LayersStruct {
    cocos2d::CCLayer* PauseLayerObject;
};


struct SettingStruct {
    // PolzHax
    bool onAutoSave = false;
    bool onExplorer = false;
    cocos2d::extension::CCControlColourPicker* onColorPicker = nullptr;
    
    // Bypass
    bool onCharFilter = false;
    bool onIcons = false;
    bool onMainLevels = false;
    bool onSliderLimit = false;
    bool onTextLength = false;

    // Cosmetic
    bool onSCoinUncoll = false;
    bool onCoinsPractice = false;
    bool onDontEnter = false;
    bool onFObjectInvisible = false;
    bool onHideAttempts = false;
    bool onHidePauseMenu = false;
    bool onHidePlayer = false;
    bool onHidePracticeBtn = false;
    bool onInstantMirror = false;
    bool onMaxParticles = false;
    bool onMiniCube = false;
    bool onNoAnimations = false;
    bool onNoBGFlash = false;
    bool onNoCubeRotation = false;
    bool onNoDeathEffect = false;
    bool onNoForceGlow = false;
    bool onNoGhostTrail = false;
    bool onNoGlowObject = false;
    bool onNoGravityEffect = false;
    bool onNoMirror = false;
    bool onNoNewBest = false;
    bool onNoOrbRing = false;
    bool onNoParticles = false;
    bool onNoLightning = false;
    bool onNoPulse = false;
    bool onNoRespawnFlash = false;
    bool onNoShadeEffect = false;
    //bool onNoWavePulse = false;
    bool onNoWaveTrail = false;
    bool onPracticePulse = false;
    bool onSolidPlayerTrail = false;
    bool onSolidWaveTrail = false;
    bool onTrailAlwaysOff = false;
    bool onTrailAlwaysOn = false;
    bool onTrailBugFix = false;
    bool onWaveTrailDeath = false;

    // Creator
    bool onAbsolutePosition = false;
    bool onCopyHack = false;
    bool onDefSongBypass = false;
    bool onEditorExtension = false;
    bool onEditorPreview = false;
    bool onFreeScroll = false;
    bool onPersClip = false;
    bool onHideGrid = false;
    bool onHideTrigLine = false;
    bool onLevelEdit = false;
    bool onNoCMark = false;
    bool onNoDeathX = false;
    bool onNoEditorTrail = false;
    bool onObjBypass = false;
    bool onPlaceOver = false;
    bool onSmoothEditTrail = false;
    bool onVerifyHack = false;
    bool onZoomBypass = false;

    // Level
    bool onBallRotationFix = false;
    bool onConfirmExit = false;
    bool onMusicSync = false;
    bool onHiFPSRotation = false;
    bool onHitboxes = false;
    bool onIgnoreEsc = false;
    bool onInstantComplete = false;
    bool onInstantTriggers = false;
    bool onJumpHack = false;
    bool onNoclip = false;
    bool onPauseDurComp = false;
    bool onPracticeMusic = false;
    bool onSuicide = false;
    
    // Universal
    bool onFPSBypass = false;
    bool onPitch = false;
    bool onAllowLowVolume = false;
    bool onFastAlt = false;
    bool onForceVis = false;
    bool onFreeWinReSize = false;
    bool onIgnoreSlider = false;
    bool onImmortalIcons = false;
    bool onInstantGameWork = false;
    bool onKrmalMode = false;
    bool onNoRotation = false;
    bool onNoTransition = false;
    bool onRestartButton = false;
    bool onRetryBind = false;
    bool onSafeMode = false;
    bool onShowPercentage = false;
    bool onTransparentBG = false;
    bool onTransparentLists = false;
    bool onTransparentTextLabels = false;
    bool onShouldHue = false;
    bool onAlwaysHue = false;

    // Icons
    bool onSameDualColor = false;

    // SpeedHack

    bool onSpeedhack = false;
    bool onSpeedhackMusic = false;

    float grid = 30;
    float gridDefault = 30;
    float fps = 60.f;
    float speedhack = 1.f;
    float pitchshift = 1.f;
    float UISize = 1.0f;

    float Overlaycolor[4] = { 0.235, 0.235, 0.235, 1 };
    float BGcolor[4] = { 0, 0, 0, 0.902 };
};

SettingStruct& setting();
LayersStruct& layers();