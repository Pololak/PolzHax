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
	bool onAlwaysNewBest;
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
	bool onModernParticles;
	bool onNoAnimations;
	bool onNoBackgroundEffect;
	bool onNoBackgroundFlash;
	bool onNoCompletionEffect;
	bool onNoCompletionPopup;
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
	bool onNoVehicleParticles;
	bool onNoWavePulse;
	bool onNoWaveTrail;
	bool onNoWaveTrailBehind;
	bool onPracticePulse;
	bool onShowTotalAttempts;
	bool onSolidPlayerGlow;
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
	bool onResetPercentage;
	bool onRotationHack;
	bool onSmoothEditorTrail;
	bool onVerifyHack;
	bool onZoomBypass;

	// Level
	bool onZeroPracticeComplete;
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
	bool onEverythingPulses;
	bool onFreezePlayer;
	bool onHighFPSRotationFix;
	bool onHitboxes; bool onSolidHitboxes = true; bool onHazardHitboxes = true; bool onSpecialHitboxes = true; bool onPlayerHitboxes = true; int hitboxesOpacity = 255; int solidR = 0, solidG = 0, solidB = 255; int hazardR = 255, hazardG = 0, hazardB = 0; int specialR = 0, specialG = 255, specialB = 0;
	bool onHitboxesOnDeath;
	bool onInstantComplete;
	bool onInvisibleDualFix;
	bool onJumpHack;
	bool onKrazyManMode;
	bool onNoclip, onNoclipAccuracyLimit, onNoclipDeathsLimit; float noclipAccuracyLimit = 95.f; int noclipDeathsLimit = 5; bool onNoclipTint; int noclipTintR = 255, noclipTintG = 0, noclipTintB = 0; bool onNoclipUnstuck;
	bool onPauseDuringCompletion;
	bool onPracticeFix, onStoreAccuracy, onStoreObjects;
	bool onPracticeMusic;
	bool onReplayLastCheckpoint;
	bool onRespawnTime; float respawnValue = 1000.f;
	bool onShipcopter;
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
	bool onNoTransition, onFastMenu;
	bool onPitchShifter; float pitchValue = 1.f;
	bool onQuickCheckpointMode;
	bool onRetryKeybind; int m_retryKeybind = 0x52;
	bool onSafeMode, isSafeMode;
	bool onShowRestartButton;
	bool onTransitionCustomizer; int selectedTransition;
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
	bool onCheatIndicator; int cheatIndicatorPos = 0, cheatIndicatorOrder = 0;
	bool onMessageLabel; std::string message; int messagePos = 0, messageOrder = 1;
	bool onFPSCounter, fpsPrefix = true, useImGuiFps = false; int fpsCounterPos = 0, fpsCounterOrder = 4;
	bool onCPSCounter, cpsPrefix = true, cpsMax = true, cpsTotal = true; int cpsCounterPos = 0, cpsCounterOrder = 5;
	bool onNoclipAccuracy, nocAccPrefix = true; int nocAccPos = 0, nocAccOrder = 9;
	bool onNoclipDeaths, nocDeathsPrefix = true; int nocDeathsPos = 0, nocDeathsOrder = 10;
	bool onClockLabel; int clockPos = 0, clockOrder = 7;
	bool onSessionTime; int sessionTimePos = 0, sessionTimeOrder = 8;
	bool onBestRunLabel, bestRunPrefix = true; int bestRunPos = 0, bestRunOrder = 2;
	bool onAttemptsLabel, attemptsPrefix = true; int attemptsPos = 0, attemptsOrder = 3;
	bool onJumpsLabel, jumpsPrefix = true; int jumpsPos = 0, jumpsOrder = 6;
	bool onMetaLabel, playerXPos = true, playerYPos = true, playerYVel, playerRot, playerGrav, playerSpd; int metaPos = 0;

	// Icons
	bool onIconEffects;
	bool onP1Color, onP1Color2;
	bool onP2Color, onP2Color2;
	int playerPrimaryColorR = 255, playerPrimaryColorG = 255, playerPrimaryColorB = 255;
	int player2PrimaryColorR = 255, player2PrimaryColorG = 255, player2PrimaryColorB = 255;
	int playerSecondaryColorR = 255, playerSecondaryColorG = 255, playerSecondaryColorB = 255;
	int player2SecondaryColorR = 255, player2SecondaryColorG = 255, player2SecondaryColorB = 255;
	bool onWaveTrailColor;
	int playerWaveTrailColorR = 255, playerWaveTrailColorG = 255, playerWaveTrailColorB = 255;

	bool onSameDualColor;
	bool onIconRandomizer; bool onRandomizeCube = true, onRandomizeShip = true, onRandomizeBall = true, onRandomizeUFO = true, onRandomizeDart = true, onRandomizeColor1 = true, onRandomizeColor2 = true, onRandomizeTrail = true;

	// Editor settings
	bool onSelectFilter;
	bool onShowObjectInfo;
	bool onPreviewMode;
	bool onShowGround;

	// Editor options
	bool onCopyString;
	bool onDurationLines;
	bool onExperimentalLayering;
	bool onHideUIOnPlaytest;
	bool onPreviewRotations;
	bool onShowClicks;
	bool onUnusedObjects;
	bool onNewColorSelectMenu;

	// Other stuff
	int m_p1click;
	int m_p2click;
	bool onShowPercentage;
	int colorFilter = 0;
	int colorFilterOrder = 0;
	int selectedDartIdx = 1;
	float m_customEditorGridSize = 30.f;
	bool onExtraLong;
	int m_openMenuKey;
	bool m_liveColorEnabled; // newcustomizeobjectlayer
	bool m_clearHitboxes;

	// GDPS Switcher
	std::string originalServerURL;
	bool onGDPSSwitcher, onEnableSwitcher; int m_serverIndex = 0; std::string m_customServerURL;

	// PolzBot
	bool onPlayMacro, onRecordMacro, onAutoSaveReplay;

	// Screenshot
	bool onHidePauseMenuOnShot, onHideStatusLabelsOnShot, onHideBackgroundOnShot, onHidePlayerOnShot, onCopyShotToClipboard;
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
	onAlwaysNewBest,
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
	onModernParticles,
	onNoAnimations,
	onNoBackgroundEffect,
	onNoBackgroundFlash,
	onNoCompletionEffect,
	onNoCompletionPopup,
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
	onNoVehicleParticles,
	onNoWavePulse,
	onNoWaveTrail,
	onNoWaveTrailBehind,
	onPracticePulse,
	onShowTotalAttempts,
	onSolidPlayerGlow,
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
	onResetPercentage,
	onRotationHack,
	onSmoothEditorTrail,
	onVerifyHack,
	onZoomBypass,

	// Level
	onZeroPracticeComplete,
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
	onEverythingPulses,
	onFreezePlayer,
	onHighFPSRotationFix,
	onHitboxes, onSolidHitboxes, onHazardHitboxes, onSpecialHitboxes, onPlayerHitboxes, hitboxesOpacity, solidR, solidG, solidB, hazardR, hazardG, hazardB, specialR, specialG, specialB,
	onHitboxesOnDeath,
	onInstantComplete,
	onInvisibleDualFix,
	onJumpHack,
	onNoclip, onNoclipAccuracyLimit, onNoclipDeathsLimit, noclipAccuracyLimit, noclipDeathsLimit, onNoclipTint, noclipTintR, noclipTintG, noclipTintB, onNoclipUnstuck,
	onPauseDuringCompletion,
	onPracticeFix, onStoreAccuracy, onStoreObjects,
	onPracticeMusic,
	onReplayLastCheckpoint,
	onRespawnTime, respawnValue,
	onShipcopter,
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
	onNoTransition, onFastMenu,
	onPitchShifter, pitchValue,
	onQuickCheckpointMode,
	onRetryKeybind, m_retryKeybind,
	onSafeMode,
	onShowRestartButton,
	onTransitionCustomizer, selectedTransition,
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
	onCheatIndicator, cheatIndicatorPos, cheatIndicatorOrder,
	onMessageLabel, message, messagePos, messageOrder,
	onFPSCounter, fpsPrefix, useImGuiFps, fpsCounterPos, fpsCounterOrder,
	onCPSCounter, cpsPrefix, cpsMax, cpsTotal, cpsCounterPos, cpsCounterOrder,
	onNoclipAccuracy, nocAccPrefix, nocAccPos, nocAccOrder,
	onNoclipDeaths, nocDeathsPrefix, nocDeathsPos, nocDeathsOrder,
	onClockLabel, clockPos, clockOrder,
	onSessionTime, sessionTimePos, sessionTimeOrder,
	onBestRunLabel, bestRunPrefix, bestRunPos, bestRunOrder,
	onAttemptsLabel, attemptsPrefix, attemptsPos, attemptsOrder,
	onJumpsLabel, jumpsPrefix, jumpsPos, jumpsOrder,
	onMetaLabel, playerXPos, playerYPos, playerYVel, playerRot, playerGrav, playerSpd, metaPos,

	// Icons
	onIconEffects,
	onP1Color, onP1Color2,
	onP2Color, onP2Color2,
	playerPrimaryColorR, playerPrimaryColorG, playerPrimaryColorB,
	player2PrimaryColorR, player2PrimaryColorG, player2PrimaryColorB,
	playerSecondaryColorR, playerSecondaryColorG, playerSecondaryColorB,
	player2SecondaryColorR, player2SecondaryColorG, player2SecondaryColorB,
	onWaveTrailColor,
	playerWaveTrailColorR, playerWaveTrailColorG, playerWaveTrailColorB,

	onSameDualColor,
	onIconRandomizer, onRandomizeCube, onRandomizeShip, onRandomizeBall, onRandomizeUFO, onRandomizeDart, onRandomizeColor1, onRandomizeColor2, onRandomizeTrail,

	// Editor settings
	onSelectFilter,
	onShowObjectInfo,
	onPreviewMode,
	onShowGround,

	// Editor options
	onCopyString,
	onDurationLines,
	onExperimentalLayering,
	onHideUIOnPlaytest,
	onShowClicks,
	onPreviewRotations,
	onUnusedObjects,
	onNewColorSelectMenu,

	// Other menu things
	m_p1click,
	m_p2click,
	onShowPercentage,
	selectedDartIdx,
	onExtraLong,
	m_openMenuKey,
	m_liveColorEnabled,

	onEnableSwitcher, m_serverIndex, m_customServerURL,

	onAutoSaveReplay,

	onHidePauseMenuOnShot, onHideStatusLabelsOnShot, onHideBackgroundOnShot, onHidePlayerOnShot, onCopyShotToClipboard
)