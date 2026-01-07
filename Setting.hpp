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

	float bgColor01 = .0f;
	float bgColor02 = .0f;
	float bgColor03 = .0f;
	float bgColor04 = .902f;

	float overlayColor01 = .235f;
	float overlayColor02 = .235f;
	float overlayColor03 = .235f;
	float overlayColor04 = 1.f;

	int cheatsCount = 0;
	int beforeRestartCheatsCount = 0;

	// Bypass
	bool onCharacterFilter;
	bool onIcons;
	bool onMainLevels;
	bool onSliderLimit;
	bool onTextLength;

	// Cosmetic
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

	// Editor settings
	bool onSelectFilter;
	bool onShowObjectInfo;
	bool onPreviewMode;

	// Editor options
	bool onCopyString;
	bool onDurationLines;
	bool onExperimentalLayering;
	bool onUnusedObjects;

	// Other menu things
	int m_p1click;
	int m_p2click;
	bool onShowPercentage;
};

SettingStruct& setting();

DEF_SCHEMA(
	SettingStruct,
	onAutoSave, onDeveloperMode,

	bgColor01, bgColor02, bgColor03, bgColor04,
	overlayColor01, overlayColor02, overlayColor03, overlayColor04,

	cheatsCount, beforeRestartCheatsCount,

	// Bypass
	onCharacterFilter,
	onIcons,
	onMainLevels,
	onSliderLimit,
	onTextLength,

	// Cosmetic
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
	onShowPercentage
)