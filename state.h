#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>
#include <gd.h>
#include <string>
#include <unordered_map>
#include "schema.hpp"
#include <fstream>

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

    bool show;
    bool onAutoSave;
    bool onExplorer;

    bool onCharFilter;
    bool onIcons;
    bool onMainLevels;
    bool onSliderLimit;
    bool onTextLength;
    bool onSCoinUncoll;
    bool onCoinsPractice;
    bool onDontEnter;
    bool onFObjectInvisible;
    bool onHideAttempts;
    bool onHidePauseMenu;
    bool onHidePlayer;
    bool onHidePracticeBtn;
    bool onInstantMirror;
    bool onMaxParticles;
    bool onMiniCube;
    bool onNoAnimations;
    bool onNoBGFlash;
    bool onNoDeathEffect;
    bool onNoGhostTrail;
    bool onNoGlowObject;
    bool onNoGravityEffect;
    bool onNoMirror;
    bool onNoNewBest;
    bool onNoOrbRing;
    bool onNoParticles;
    bool onNoLightning;
    bool onNoPulse;
    bool onNoRespawnFlash;
    bool onNoShadeEffect;
    bool onNoWavePulse;
    bool onNoWaveTrail;
    bool onPracticePulse;
    bool onSolidPlayerTrail;
    bool onSolidWaveTrail;
    bool onTrailAlwaysOff;
    bool onTrailAlwaysOn;
    bool onTrailBugFix;
    bool onWaveTrailDeath;
    bool onAbsolutePosition;
    bool onCopyHack;
    bool onDefSongBypass;
    bool onEditorExtension;
    bool onEditorPreview;
    bool onFreeScroll;
    bool onHideGrid;
    bool onHideTrigLine;
    bool onLevelEdit;
    bool onNoCMark;
    bool onNoDeathX;
    bool onNoEditorTrail;
    bool onObjBypass;
    bool onPlaceOver;
    bool onSmoothEditTrail;
    bool onVerifyHack;
    bool onZoomBypass;
    bool onBallRotationFix;
    bool onConfirmExit;
    bool onMusicSync;
    bool onHiFPSRotation;
    bool onHitboxes;
    bool onHitboxesOnDeath;
    bool onPlayerHitbox = true;
    bool onSolidsHitbox = true;
    bool onHazardsHitbox = true;
    bool onSpecialsHitbox = true;
    bool onIgnoreEsc;
    bool onInstantComplete;
    bool onInstantTriggers;
    bool onJumpHack;
    bool onNoclip;
    bool onNoclipUnstuck;
    bool onPauseDurComp;
    bool onPracticeFix; // YES
    bool onPracticeMusic;
    bool onSPSwitcher;
    bool onSuicide;
    bool onFPSBypass;
    bool onAllowLowVolume;
    bool onAutoSafe;
    bool onFastAlt;
    bool onForceVis;
    bool onFreeWinReSize;
    bool onIgnoreSlider;
    bool onImmortalIcons;
    bool onInstantGameWork;
    bool onKrmalMode;
    bool onLockCursor; // CURSOS
    bool onNoRotation;
    bool onNoTransition;
    bool onRestartButton;
    bool onRetryBind;
    bool onSafeMode;
    bool onShowPercentage;
    bool onTransparentBG;
    bool onTransparentLists;
    bool onTransparentTextLabels;
    bool onShouldHue;
    bool onAlwaysHue;
    bool onSameDualColor;
    bool onRespawnTime;
    bool onResetPercentage; //
    bool onSpeedhack;
    bool onSpeedhackMusic;
    bool onCheatIndicator;
    bool onClock;
    bool onFPSLabel;
    bool onAttemptsLabel;
    bool onCPSLabel;
    bool onBestRunLabel;
    bool onMessageLabel;
    bool onNoclipAccuracy;
    bool onNoclipDeaths;
    bool onMeta;
    bool onNoclipOutOfMe;
    bool onSpeedhackCounted;
    bool onHideLabels;
    bool onZeroDelay; //
    bool isSafeMode;
    bool onSessionTime;
    bool onNoMiniIcon;
    bool onDontFade;
    bool onMenuGameplay;
    bool onSwitchToUpArrow;
    bool onNoEffectCircle;
    bool onShowLayout;
    bool onSmartStartPos; //
    bool onRainbowIcon;
    bool onNoWaveTrailBehind;
    bool onNoForceGlow;

    float respawnTime = 1.f;
    float LevelGColor[3] = { 1, 1, 1 };
    float LevelBGColor[3] = { 0.156f, 0.490f, 1.f };
    float labelsScale = 1.f;
    float solidsColor[3] = { 0, 0, 1 };
    float hazardsColor[3] = { 1, 0, 0 };
    float specialsColor[3] = { 0, 1, 0 };
    float fps = 60.f;
    float speedhack = 1.f;
    float UISize = 1.0f;
    float wavePulseSize = 2.1f;
    float Overlaycolor[4] = { 0.235f, 0.235f, 0.235f, 1.f };
    float BGcolor[4] = { 0.f, 0.f, 0.f, 0.902f };
    float gridSize = 30.f;
    float rgbSpeed = 1.f;

    int selected_dart = 1;

    std::string message = "Your Text";

    bool managerView;
    int priority = 2;
    bool onForceBlockType;
    bool onLastCheckpoint;
    bool onAlwaysNewBest;
    bool onFastAnimComp;
    bool onFastShowComp;
    bool onAutoDownloadSong;
    bool onClassicMode; // Speedhack
    bool onResetEditPercentage;
    bool onAutoPractice;
    bool onZeroPracticeComplete;
    bool onInversedTrail;
    bool onSwapWaveTrailColors;
    bool onFastMenu;

    bool onAutoKill;
    int killPercent = 50;

    bool onVSync;
    bool onHidePauseButton;

    bool onAutoDeafen;
    int deafenPercent = 50;
    int deafenKey;

    int accuratePercentage = 4;

    bool onHitboxBugFix;
    int retryKeybind = 0x52;

    int spsPrevKey = 37;
    int spsNextKey = 39;
};

enum LabelPosition {
    TL,
    TR,
    BL,
    BR,
};

SettingStruct& setting();
LayersStruct& layers();

//DEF_SCHEMA(
//    SettingStruct,
//    cheatsCount, beforeRestartCheatsCount, NoclipByte, CurrentNoclipByte,
//    onAutoSave,
//    UISize,
//    priority,
//
//    onCharFilter,
//    onIcons,
//    onMainLevels,
//    onSliderLimit,
//    onTextLength,
//
//    accuratePercentage,
//    onAlwaysNewBest,
//    onSCoinUncoll,
//    onCoinsPractice,
//    onDontEnter,
//    onDontFade,
//    onFastAnimComp,
//    onFastShowComp,
//    onFObjectInvisible,
//    onHideAttempts,
//    onHidePauseButton,
//    onHidePauseMenu,
//    onHidePlayer,
//    onHidePracticeBtn,
//    onInstantMirror,
//    onInversedTrail,
//    onMaxParticles,
//    onMiniCube,
//    onNoAnimations,
//    onNoBGFlash,
//    onNoDeathEffect,
//    onNoEffectCircle,
//    onNoForceGlow,
//    onNoGhostTrail,
//    onNoGlowObject,
//    onNoGravityEffect,
//    onNoMiniIcon,
//    onNoMirror,
//    onNoNewBest,
//    onNoOrbRing,
//    onNoParticles,
//    onNoLightning,
//    onNoPulse,
//    onNoRespawnFlash,
//    onNoShadeEffect,
//    onNoWavePulse, wavePulseSize,
//    onNoWaveTrail,
//    onNoWaveTrailBehind,
//    onPracticePulse,
//    onRespawnTime, respawnTime,
//    onSolidPlayerTrail,
//    onSolidWaveTrail,
//    onTrailAlwaysOff,
//    onTrailAlwaysOn,
//    onTrailBugFix,
//    onWaveTrailDeath,
//
//    onAbsolutePosition,
//    onCopyHack,
//    onDefSongBypass,
//    onEditorExtension,
//    onEditorPreview,
//    onFreeScroll,
//    onHideGrid,
//    onHideTrigLine,
//    onHitboxBugFix,
//    onLevelEdit,
//    onNoCMark,
//    onNoDeathX,
//    onNoEditorTrail,
//    onObjBypass,
//    onPlaceOver,
//    onResetEditPercentage,
//    onSmoothEditTrail,
//    onSwitchToUpArrow,
//    onVerifyHack,
//    onZoomBypass,
//
//    onZeroPracticeComplete,
//    onAutoDeafen, deafenPercent, deafenKey,
//    onAutoKill, killPercent,
//    onAutoPractice,
//    onAutoDownloadSong,
//    onBallRotationFix,
//    onConfirmExit, 
//    onMusicSync,
//    onHiFPSRotation, 
//    onHitboxes,
//    hitboxOpacity, 
//    onPlayerHitbox,
//    onSolidsHitbox,
//    onHazardsHitbox,
//    onSpecialsHitbox,
//    onHitboxesOnDeath,
//    onInstantComplete,
//    onInstantTriggers,
//    onJumpHack,
//    onNoclip,
//    onNoclipUnstuck,
//    onPauseDurComp,
//    onPracticeFix,
//    onPracticeMusic,
//    onLastCheckpoint,
//    onRespawnTime,
//    respawnTime,
//    onShowLayout, 
//    onSmartStartPos,
//    onSPSwitcher, spsPrevKey, spsNextKey,
//    onSuicide,
//
//	onAllowLowVolume,
//	onAutoSafe,
//	onFastAlt,
//	onForceVis,
//	onFreeWinReSize,
//	onAlwaysHue,
//	onIgnoreSlider,
//	onImmortalIcons,
//	onInstantGameWork,
//	onKrmalMode,
//	onLockCursor,
//	onMenuGameplay,
//	onNoRotation,
//	onNoTransition,
//    onFastMenu,
//	onRetryBind, retryKeybind,
//	onSafeMode,
//	onRestartButton,
//	onTransparentBG,
//	onTransparentLists,
//	onTransparentTextLabels,
//    onZeroDelay,
//
//	onHideLabels,
//	labelsScale,
//	labelsOpacity,
//	onCheatIndicator,
//	onAttemptsLabel,
//	onBestRunLabel,
//	onClock,
//	onCPSLabel,
//	onFPSLabel,
//	onMessageLabel,
//	message,
//	onNoclipAccuracy,
//	onNoclipDeaths,
//	onSessionTime,
//	onMeta,
//
//	speedhack,
//	onSpeedhack,
//	onSpeedhackMusic,
//	onClassicMode,
//
//	onSameDualColor,
//    onSwapWaveTrailColors,
//	onRainbowIcon,
//	rgbSpeed,
//
//    fps,
//    onFPSBypass
//)