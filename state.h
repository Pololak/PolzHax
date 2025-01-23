#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>
#include <gd.h>
#include <string>
#include <unordered_map>
#include "schema.hpp"

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
    bool b_blank8; //
    bool onNoDeathEffect;
    bool b_blank7; //
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
    bool b_blankIDK;
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
    bool b_blank6; //
    bool onShouldHue;
    bool onAlwaysHue;
    bool onSameDualColor;
    bool b_blank5; //
    bool onRespawnTime;
    bool b_blank4; //
    bool b_blank3; //
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
    bool b_blank9;
    bool b_blank10;
    bool b_blank11;
    bool b_blank12;
    bool b_blank13;
    bool b_blank14;
    bool b_blank15;
    bool b_blank16;
    bool b_blank17;
    bool b_blank18;
    bool b_blank19;
    bool b_blank20;
    bool b_blank21;
    bool b_blank22;
    bool b_blank23;
    bool b_blank24;
    bool b_blank25;
    bool b_blank26;
    bool b_blank27;
    bool b_blank28;
    bool b_blank29;
    bool b_blank30;
    bool b_blank31;
    bool b_blank32;
    bool b_blank33;
    bool b_blank34;
    bool b_blank35;
    bool b_blank36;
    bool b_blank37;
    bool b_blank38;
    bool b_blank39;
    bool b_blank40;

    float respawnTime = 1.f;
    float f_blank2;
    float f_blank3;
    float f_blank4;
    float f_blank5;
    float f_blank6;
    float f_blank7;
    float f_blank8;
    float f_blank9;
    float f_blank10;
    float f_blank11;
    float f_blank12;
    float f_blank13;
    float f_blank14;
    float f_blank15[3];
    float f_blank16[3];
    float f_blank17[3];
    float f_blank18[3];
    float LevelGColor[3] = { 1, 1, 1 };
    float LevelBGColor[3] = { 0.156, 0.490, 1 };
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
    float rgbSpeed = 1.f;
    
    int i_blank1;
    int i_blank2;
    int i_blank3;
    int i_blank4;
    int i_blank5;
    int i_blank6;
    int i_blank7;
    int i_blank8;
    int i_blank9;
    int i_blank10;
    int i_blank11;
    int i_blank12;
    int i_blank13;
    int i_blank14;
    int i_blank15;
    int i_blank16;
    int i_blank17;
    int i_blank18;
    int i_blank19;
    int i_blank20;
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

    // Icons thing

    bool onEnableP1;
    bool onEnableP2;

    void loadState();
    void saveState();
};

enum LabelPosition {
    TL,
    TR,
    BL,
    BR,
};

struct FuckThis {
    cocos2d::extension::CCControlColourPicker* onColorPicker = nullptr;
    bool onValueEnter = false;
};

SettingStruct& setting();
LayersStruct& layers();
FuckThis& fuckThis();

DEF_SCHEMA(
    SettingStruct,
    cheatsCount, beforeRestartCheatsCount, NoclipByte, CurrentNoclipByte,
    onAutoSave,
    UISize,
    priority,

    onCharFilter,
    onIcons,
    onMainLevels,
    onSliderLimit,
    onTextLength,

    onAlwaysNewBest,
    onSCoinUncoll,
    onCoinsPractice,
    onDontEnter,
    onDontFade,
    onFastAnimComp,
    onFastShowComp,
    onFObjectInvisible,
    onHideAttempts,
    onHidePauseButton,
    onHidePauseMenu,
    onHidePlayer,
    onHidePracticeBtn,
    onInstantMirror,
    onInversedTrail,
    onMaxParticles,
    onMiniCube,
    onNoAnimations,
    onNoBGFlash,
    onNoDeathEffect,
    onNoEffectCircle,
    onNoGhostTrail,
    onNoGlowObject,
    onNoGravityEffect,
    onNoMiniIcon,
    onNoMirror,
    onNoNewBest,
    onNoOrbRing,
    onNoParticles,
    onNoLightning,
    onNoPulse,
    onNoRespawnFlash,
    onNoShadeEffect,
    onNoWavePulse,
    wavePulseSize,
    onNoWaveTrail,
    onPracticePulse,
    onRespawnTime,
    respawnTime,
    onSolidPlayerTrail,
    onSolidWaveTrail,
    onTrailAlwaysOff,
    onTrailAlwaysOn,
    onTrailBugFix,
    onWaveTrailDeath,

    onAbsolutePosition,
    onCopyHack,
    onDefSongBypass,
    onEditorExtension,
    onEditorPreview,
    onFreeScroll,
    onHideGrid,
    onHideTrigLine,
    onLevelEdit,
    onNoCMark,
    onNoDeathX,
    onNoEditorTrail,
    onObjBypass,
    onPlaceOver,
    onResetEditPercentage,
    onSmoothEditTrail,
    onSwitchToUpArrow,
    onVerifyHack,
    onZoomBypass,

    onZeroPracticeComplete,
    onAutoDeafen,
    deafenPercent,
    deafenKey,
    onAutoKill,
    killPercent,
    onAutoPractice,
    onAutoDownloadSong,
    onBallRotationFix,
    onConfirmExit, 
    onMusicSync,
    onHiFPSRotation, 
    onHitboxes,
    hitboxOpacity, 
    onPlayerHitbox,
    onSolidsHitbox,
    onHazardsHitbox,
    onSpecialsHitbox,
    onHitboxesOnDeath,
    onInstantComplete,
    onInstantTriggers,
    onJumpHack,
    onNoclip,
    onNoclipUnstuck,
    onPauseDurComp,
    onPracticeFix,
    onPracticeMusic,
    onLastCheckpoint,
    onRespawnTime,
    respawnTime,
    onShowLayout, 
    onSmartStartPos,
    onSPSwitcher,
    onSuicide,

	onAllowLowVolume,
	onAutoSafe,
	onFastAlt,
	onForceVis,
	onFreeWinReSize,
	onAlwaysHue,
	onIgnoreSlider,
	onImmortalIcons,
	onInstantGameWork,
	onKrmalMode,
	onLockCursor,
	onMenuGameplay,
	onNoRotation,
	onNoTransition,
    onFastMenu,
	onRetryBind,
	onSafeMode,
	onRestartButton,
	onTransparentBG,
	onTransparentLists,
	onTransparentTextLabels,
    onZeroDelay,

	onHideLabels,
	labelsScale,
	labelsOpacity,
	onCheatIndicator,
	onAttemptsLabel,
	onBestRunLabel,
	onClock,
	onCPSLabel,
	onFPSLabel,
	onMessageLabel,
	message,
	onNoclipAccuracy,
	onNoclipDeaths,
	onSessionTime,
	onMeta,

	speedhack,
	onSpeedhack,
	onSpeedhackMusic,
	onClassicMode,

	onSameDualColor,
    onSwapWaveTrailColors,
	onRainbowIcon,
	rgbSpeed,

    fps,
    onFPSBypass,
    onVSync
)