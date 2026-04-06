#pragma once
#include "pch.h"
#include "schema.hpp"

struct SettingStruct {
	void load();
	void save();

	bool show;
	bool onAutoSave = true;
	bool onCocosExplorer;
	bool onDeveloperMode;
	bool onThreadPriority; int priority = 2;

	float bgColor01 = .0f;
	float bgColor02 = .0f;
	float bgColor03 = .0f;
	float bgColor04 = .902f;

	float overlayColor01 = .235f;
	float overlayColor02 = .235f;
	float overlayColor03 = .235f;
	float overlayColor04 = 1.f;

	float UISize = 1.f;

	int cheatsCount = 0;
	int beforeRestartCheatsCount = 0;

	// Bypass
	bool onCharacterFilter;
	bool onIcons;
	bool onMainLevels;
	bool onSliderLimit;
	bool onTextLength;

	// Cosmetic
	bool onAccuratePercentage; int decimalPlaces = 1;
	bool onCoinsShowUncollected;
	bool onCoinsInPractice;
	bool onForceDontEnter;
	bool onForceDontFade;
	bool onForceObjectsInvisible;
	bool onHideAttempts;
	bool onHidePauseButton;
	bool onHidePauseMenu;
	bool onHidePlayer;
	bool onHidePracticeButtons;
	bool onInstantMirror;
	bool onInversedTrail;
	bool onMaxParticles;
	bool onMiniCubeIcon;
	bool onNoAnimations;
	bool onNoBackgroundFlash;
	bool onNoDeathEffect;
	bool onNoEffectCircle;
	bool onNoEndShake;
	bool onNoForcePlayerGlow; // This thing still breaks gd lmao
	bool onNoGhostTrail;
	bool onNoObjectGlow;
	bool onNoGravityEffect;
	bool onNoMiniIcon;
	bool onNoMirror;
	bool onNoNewBestPopup;
	bool onNoOrbRing;
	bool onNoParticles;
	bool onNoPortalLightning;
	bool onNoPortalShine;
	bool onNoPulse;
	bool onNoRespawnFlash;
	bool onNoShadeEffect;
	bool onNoWavePulse;
	bool onNoWaveTrail;
	bool onNoWaveTrailBehind;
	bool onPracticePulse;
	bool onShowTotalAttempts;
	bool onSolidPlayerTrail;
	bool onSolidWaveTrail;
	bool onTrailAlwaysOff;
	bool onTrailAlwaysOn;
	bool onTrailBugFix;
	bool onWavePulseSize; float wavePulseSize = 2.3f;
	bool onWaveTrailBugFix;
	bool onWaveTrailOnDeath;

	// Creator
	bool onAbsolutePosition;
	bool onCopyHack;
	bool onDefaultSongBypass;
	bool onEditorExtension;
	bool onFreeScroll;
	bool onHideGrid;
	bool onHideTriggerLines;
	bool onHideUI;
	bool onHitboxBugFix;
	bool onLevelEdit;
	bool onNoCopyMark;
	bool onNoDeathX;
	bool onNoEditorTrail;
	bool onObjectBypass;
	bool onPlaceOver;
	bool onRotationHack;
	bool onSmoothEditorTrail;
	bool onVerifyHack;
	bool onZoomBypass;

	// Level
	bool onAutoDeafen; bool onPauseUndeafen; bool onPracticeDeafen; bool onTestmodeDeafen; float deafenPercent = 50; float undeafenPercent = 100; int m_autoDeafenKey;
	bool onAutoKill; float killPercentage = 50;
	bool onAutoPickupCoins;
	bool onAutoPracticeMode;
	bool onAutoSongDownload;
	bool onBallRotationBugFix;
	bool onCheckpointLagFix;
	bool onConfirmExit;
	bool onCorrectiveMusicSync;
	bool onEverythingHurts;
	bool onHighFPSRotationFix;
	bool onHitboxes; bool onSolidHitboxes = true; bool onHazardHitboxes = true; bool onSpecialHitboxes = true; bool onPlayerHitboxes = true; int hitboxesOpacity = 255; int solidR = 0, solidG = 0, solidB = 255; int hazardR = 255, hazardG = 0, hazardB = 0; int specialR = 0, specialG = 255, specialB = 0;
	bool onHitboxesOnDeath;
	bool onInstantComplete;
	bool onInvisibleDualFix;
	bool onJumpHack;
	bool onNoclip; bool onNoclipTint; int noclipTintR = 255, noclipTintG = 0, noclipTintB = 0; bool onNoclipUnstuck;
	bool onPauseDuringCompletion;
	bool onPracticeFix;
	bool onPracticeMusic;
	bool onReplayLastCheckpoint;
	bool onRespawnTime; float respawnValue = 1000.f;
	bool onShowLayout; int layoutBGR = 40, layoutBGG = 125, layoutBGB = 255; int layoutGR = 0, layoutGG = 102, layoutGB = 255;
	bool onSmartStartPos;
	bool onStartPosSwitcher; int m_previousStartPosKey = 0x25, m_nextStartPosKey = 0x27;
	bool onWaveSlide;

	// Universal
	bool onFPSBypass; float fpsValue = 60.f;
	bool onTPSBypass; float tpsValue = 60.f;
	bool onAllowLowVolume;
	bool onAutoSafeMode;
	bool onDisableSongAlert;
	bool onFastAltTab;
	bool onForceVisibility;
	bool onFreeWindowResize;
	bool onHUEFix;
	bool onIncreaseMaxLevels;
	bool onLockCursor;
	bool onNoRotation;
	bool onNoTransition;
	bool onPitchShifter; float pitchValue = 1.f;
	bool onRetryKeybind; int m_retryKeybind = 0x52;
	bool onSafeMode, isSafeMode;
	bool onShowRestartButton;
	bool onTransparentBG;
	bool onTransparentLists;
	bool onZeroDelay;

	// Speedhack
	bool onSpeedhack; float speedhackValue = 1.f;
	bool onSpeedhackAudio;
	bool onClassicMode;

	// Status
	float labelsOpacity = 1.f;
	float labelsScale = 1.f;
	bool onHideLabels;
	bool onCheatIndicator; int cheatIndicatorPos = 0;
	bool onMessageLabel; std::string message; int messagePos = 0;
	bool onFPSCounter, fpsPrefix = true; int fpsCounterPos = 0;
	bool onCPSCounter, cpsPrefix = true, cpsMax = true, cpsTotal; int cpsCounterPos = 0;
	bool onNoclipAccuracy, nocAccPrefix = true; int nocAccPos = 0;
	bool onNoclipDeaths, nocDeathsPrefix = true; int nocDeathsPos = 0;
	bool onClockLabel; int clockPos = 0;
	bool onSessionTime; int sessionTimePos = 0;
	bool onBestRunLabel, bestRunPrefix = true; int bestRunPos = 0;
	bool onAttemptsLabel, attemptsPrefix = true; int attemptsPos = 0;
	bool onJumpsLabel, jumpsPrefix = true; int jumpsPos = 0;
	bool onMetaLabel; int metaPos = 0;

	// Icons
	bool onIconEffects; bool onIconColor1 = true, onIconColor2 = true;
	bool onSameDualColor;
	bool onIconRandomizer; bool onRandomizeCube = true, onRandomizeShip = true, onRandomizeBall = true, onRandomizeUFO = true, onRandomizeDart = true, onRandomizeColor1 = true, onRandomizeColor2 = true, onRandomizeTrail = true;

	// Editor settings
	bool onSelectFilter;
	bool onShowObjectInfo;
	bool onPreviewMode;

	// Editor options
	bool onCopyString;
	bool onDurationLines;
	bool onExperimentalLayering;
	bool onUnusedObjects;

	// Other stuff
	int m_p1click;
	int m_p2click;
	bool onShowPercentage;
	int colorFilter = 0;
	int colorFilterOrder = 0;
	int selectedDartIdx = 1;
	float m_customEditorGridSize = 30.f;

	// GDPS Switcher
	std::string originalServerURL;
	bool onGDPSSwitcher, onEnableSwitcher; int m_serverIndex = 0; std::string m_customServerURL;
};

SettingStruct& setting();

DEF_SCHEMA(
	SettingStruct,
	onAutoSave, onDeveloperMode,

	onThreadPriority, priority,

	bgColor01, bgColor02, bgColor03, bgColor04,
	overlayColor01, overlayColor02, overlayColor03, overlayColor04,
	UISize,

	cheatsCount, beforeRestartCheatsCount,

	// Bypass
	onCharacterFilter,
	onIcons,
	onMainLevels,
	onSliderLimit,
	onTextLength,

	// Cosmetic
	onAccuratePercentage, decimalPlaces,
	onCoinsShowUncollected,
	onCoinsInPractice,
	onForceDontEnter,
	onForceDontFade,
	onForceObjectsInvisible,
	onHideAttempts,
	onHidePauseButton,
	onHidePauseMenu,
	onHidePlayer,
	onHidePracticeButtons,
	onInstantMirror,
	onInversedTrail,
	onMaxParticles,
	onMiniCubeIcon,
	onNoAnimations,
	onNoBackgroundFlash,
	onNoDeathEffect,
	onNoEffectCircle,
	onNoEndShake,
	onNoForcePlayerGlow,
	onNoGhostTrail,
	onNoObjectGlow,
	onNoGravityEffect,
	onNoMiniIcon,
	onNoMirror,
	onNoNewBestPopup,
	onNoOrbRing,
	onNoParticles,
	onNoPortalLightning,
	onNoPortalShine,
	onNoPulse,
	onNoRespawnFlash,
	onNoShadeEffect,
	onNoWavePulse,
	onNoWaveTrail,
	onNoWaveTrailBehind,
	onPracticePulse,
	onShowTotalAttempts,
	onSolidPlayerTrail,
	onSolidWaveTrail,
	onTrailAlwaysOff,
	onTrailAlwaysOn,
	onTrailBugFix,
	onWavePulseSize, wavePulseSize,
	onWaveTrailBugFix,
	onWaveTrailOnDeath,

	// Creator
	onAbsolutePosition,
	onCopyHack,
	onDefaultSongBypass,
	onEditorExtension,
	onFreeScroll,
	onHideGrid,
	onHideTriggerLines,
	onHideUI,
	onHitboxBugFix,
	onLevelEdit,
	onNoCopyMark,
	onNoDeathX,
	onNoEditorTrail,
	onObjectBypass,
	onPlaceOver,
	onRotationHack,
	onSmoothEditorTrail,
	onVerifyHack,
	onZoomBypass,

	// Level
	onAutoDeafen, onPauseUndeafen, onPracticeDeafen, onTestmodeDeafen, deafenPercent, undeafenPercent, m_autoDeafenKey,
	onAutoKill, killPercentage,
	onAutoPickupCoins,
	onAutoPracticeMode,
	onAutoSongDownload,
	onBallRotationBugFix,
	onCheckpointLagFix,
	onConfirmExit,
	onCorrectiveMusicSync,
	onEverythingHurts,
	onHighFPSRotationFix,
	onHitboxes, onSolidHitboxes, onHazardHitboxes, onSpecialHitboxes, onPlayerHitboxes, hitboxesOpacity, solidR, solidG, solidB, hazardR, hazardG, hazardB, specialR, specialG, specialB,
	onHitboxesOnDeath,
	onInstantComplete,
	onInvisibleDualFix,
	onJumpHack,
	onNoclip, onNoclipTint, noclipTintR, noclipTintG, noclipTintB, onNoclipUnstuck,
	onPauseDuringCompletion,
	onPracticeFix,
	onPracticeMusic,
	onReplayLastCheckpoint,
	onRespawnTime, respawnValue,
	onShowLayout, layoutBGR, layoutBGG, layoutBGB, layoutGR, layoutGG, layoutGB,
	onSmartStartPos,
	onStartPosSwitcher, m_previousStartPosKey, m_nextStartPosKey,
	onWaveSlide,

	// Universal
	onFPSBypass, fpsValue,
	onTPSBypass, tpsValue,
	onAllowLowVolume,
	onAutoSafeMode,
	onDisableSongAlert,
	onFastAltTab,
	onForceVisibility,
	onFreeWindowResize,
	onHUEFix,
	onIncreaseMaxLevels,
	onLockCursor,
	onNoRotation,
	onNoTransition,
	onPitchShifter, pitchValue,
	onRetryKeybind, m_retryKeybind,
	onSafeMode,
	onShowRestartButton,
	onTransparentBG,
	onTransparentLists,
	onZeroDelay,

	// Speedhack
	onSpeedhack, speedhackValue,
	onSpeedhackAudio,
	onClassicMode,

	// Status
	labelsOpacity,
	labelsScale,
	onHideLabels,
	onCheatIndicator, cheatIndicatorPos,
	onMessageLabel, message, messagePos,
	onFPSCounter, fpsPrefix, fpsCounterPos,
	onCPSCounter, cpsPrefix, cpsMax, cpsTotal, cpsCounterPos,
	onNoclipAccuracy, nocAccPrefix, nocAccPos,
	onNoclipDeaths, nocDeathsPrefix, nocDeathsPos,
	onClockLabel, clockPos,
	onSessionTime, sessionTimePos,
	onBestRunLabel, bestRunPrefix, bestRunPos,
	onAttemptsLabel, attemptsPrefix, attemptsPos,
	onJumpsLabel, jumpsPrefix, jumpsPos,
	onMetaLabel, metaPos,

	// Icons
	onSameDualColor,
	onIconRandomizer, onRandomizeCube, onRandomizeShip, onRandomizeBall, onRandomizeUFO, onRandomizeDart, onRandomizeColor1, onRandomizeColor2, onRandomizeTrail,

	// Editor settings
	onSelectFilter,
	onShowObjectInfo,
	onPreviewMode,

	// Editor options
	onCopyString,
	onDurationLines,
	onExperimentalLayering,
	onUnusedObjects,

	// Other menu things
	m_p1click,
	m_p2click,
	onShowPercentage,
	selectedDartIdx,

	onEnableSwitcher, m_serverIndex, m_customServerURL
)