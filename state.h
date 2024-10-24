#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>
#include <gd.h>

struct LayersStruct {
    cocos2d::CCLayer* PauseLayerObject;
};

struct SettingStruct {
    int cheatsCount = 0;
    int beforeRestartCheatsCount = 0;

    int NoclipByte;
    int CurrentNoclipByte;

    int hitboxFillOpacity = 255;
    int labelsOpacity = 255;
    int hitboxOpacity = 255;

    bool show = false;
    bool onAutoSave = false;
    bool onExplorer = false;

    bool onCharFilter = false;
    bool onIcons = false;
    bool onMainLevels = false;
    bool onSliderLimit = false;
    bool onTextLength = false;
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
    bool onNoWavePulse = false;
    bool onNoWaveTrail = false;
    bool onPracticePulse = false;
    bool onSolidPlayerTrail = false;
    bool onSolidWaveTrail = false;
    bool onTrailAlwaysOff = false;
    bool onTrailAlwaysOn = false;
    bool onTrailBugFix = false;
    bool onWaveTrailDeath = false;
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
    bool onBallRotationFix = false;
    bool onConfirmExit = false;
    bool onMusicSync = false;
    bool onHiFPSRotation = false;
    bool onHitboxes = false;
    bool onHitboxesOnDeath = false;
    bool onPlayerHitbox = false;
    bool onSolidsHitbox = false;
    bool onHazardsHitbox = false;
    bool onSpecialsHitbox = false;
    bool onIgnoreEsc = false;
    bool onInstantComplete = false;
    bool onInstantTriggers = false;
    bool onJumpHack = false;
    bool onNoclip = false;
    bool onPauseDurComp = false;
    bool onPracticeFix = false;
    bool onPracticeMusic = false;
    bool onSPSwitcher = false;
    bool onSuicide = false;
    bool onFPSBypass = false;
    bool onAllowLowVolume = false;
    bool onAutoSafe = false;
    bool onFastAlt = false;
    bool onForceVis = false;
    bool onFreeWinReSize = false;
    bool onIgnoreSlider = false;
    bool onImmortalIcons = false;
    bool onInstantGameWork = false;
    bool onKrmalMode = false;
    bool onLockCursor = false; // CURSOS
    bool onNoRotation = false;
    bool onNoTransition = false;
    bool onRestartButton = false;
    bool onRetryBind = false;
    bool onSafeMode = false;
    bool onShowPercentage = false;
    bool onTransparentBG = false;
    bool onTransparentLists = false;
    bool onTransparentTextLabels = false;
    bool onTransparentPause = false;
    bool onShouldHue = false;
    bool onAlwaysHue = false;
    bool onSameDualColor = false;
    bool onSwapWaveColors = false;
    bool onGlowPulse = false;
    bool onSecondaryPulse = false;
    bool onPrimaryPulse = false;
    bool onWaveTrailPulse = false;
    bool onSpeedhack = false;
    bool onSpeedhackMusic = false;
    bool onCheatIndicator = false;
    bool onClock = false;
    bool onFPSLabel = false;
    bool onAttemptsLabel = false;
    bool onCPSLabel = false;
    bool onBestRunLabel = false;
    bool onMessageLabel = false;
    bool onNoclipAccuracy = false;
    bool onNoclipDeaths = false;
    bool onMeta = false;
    bool onNoclipOutOfMe = false;
    bool onSpeedhackCounted = false;
    bool onHideLabels = false;
    bool onZeroDelay = false;
    bool isSafeMode;
    bool onSessionTime = false;
    bool onNoMiniIcon = false;
    bool onDontFade = false;
    bool extraFeatures = false;
    bool onSwitchToUpArrow = false;
    bool onNoEffectCircle = false;
    bool onShowLayout = false;
    bool onGridSize = false;
    bool onBetterLevelUpdate = false;

    float labelsScale = 1.f;
    float solidsColor[3] = { 0, 0, 1 };
    float hazardsColor[3] = { 1, 0, 0 };
    float specialsColor[3] = { 0, 1, 0 };
    float fps = 60.f;
    float speedhack = 1.f;
    float UISize = 1.0f;
    float wavePulseSize = 2.1f;
    float Overlaycolor[4] = { 0.235, 0.235, 0.235, 1 };
    float BGcolor[4] = { 0, 0, 0, 0.902 };
    float gridSize = 30.f;

    cocos2d::extension::CCControlColourPicker* onColorPicker = nullptr;
    char message[50] = "Your Text";

    int selected_dart = 1;
};

enum LabelPosition {
    TL,
    TR,
    BL,
    BR,
};

SettingStruct& setting();
LayersStruct& layers();