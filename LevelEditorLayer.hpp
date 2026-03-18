#pragma once
#include "pch.h"

namespace LevelEditorLayer {
	inline bool(__thiscall* init)(gd::LevelEditorLayer*, gd::GJGameLevel*);
	bool __fastcall initH(gd::LevelEditorLayer*, void*, gd::GJGameLevel*);

	inline void(__thiscall* addSpecial)(gd::LevelEditorLayer*, gd::GameObject*);
	void __fastcall addSpecialH(gd::LevelEditorLayer*, void*, gd::GameObject*);

	inline void(__thiscall* removeSpecial)(gd::LevelEditorLayer*, gd::GameObject*);
	void __fastcall removeSpecialH(gd::LevelEditorLayer*, void*, gd::GameObject*);

	inline void(__thiscall* updateVisibility)(gd::LevelEditorLayer*, float);
	void __fastcall updateVisibilityH(gd::LevelEditorLayer*, void*, float);

	inline void(__thiscall* destructor)(gd::LevelEditorLayer*);
	void __fastcall destructorH(gd::LevelEditorLayer*);

	inline gd::GameObject*(__thiscall* addObjectFromString)(gd::LevelEditorLayer*, std::string);
	gd::GameObject* __fastcall addObjectFromStringH(gd::LevelEditorLayer*, void*, std::string);

	inline gd::GameObject*(__thiscall* createObject)(gd::LevelEditorLayer*, int, CCPoint);
	gd::GameObject* __fastcall createObjectH(gd::LevelEditorLayer*, void*, int, CCPoint);

	inline void(__thiscall* flipGravity)(gd::LevelEditorLayer*, gd::PlayerObject*, bool, bool);
	void __fastcall flipGravityH(gd::LevelEditorLayer*, void*, gd::PlayerObject*, bool, bool);

	inline void(__thiscall* onPlaytest)(gd::LevelEditorLayer*);
	void __fastcall onPlaytestH(gd::LevelEditorLayer*);

	inline void(__thiscall* onResumePlaytest)(gd::LevelEditorLayer*);
	void __fastcall onResumePlaytestH(gd::LevelEditorLayer*);

	inline void(__thiscall* update)(gd::LevelEditorLayer*, float);
	void __fastcall updateH(gd::LevelEditorLayer*, void*, float);

	inline void(__thiscall* draw)(gd::LevelEditorLayer*);
	void __fastcall drawH(gd::LevelEditorLayer*);

	void updatePreviewMode();
	void resetColors();
	bool getIsEditorPaused();
	void setIsEditorPaused(bool);
	bool isColorBlending(gd::GJCustomColorMode);
	void setLastPos(float);
	void moveTrigger(gd::GameObject*);
	void updateOrientedHitboxes(gd::LevelEditorLayer*);

	void updateShowHitboxes();

	gd::LevelEditorLayer* get();

	void mem_init();
}