#pragma once
#include "pch.h"

namespace PlayLayer {
	inline bool(__thiscall* init)(gd::PlayLayer*, gd::GJGameLevel*);
	bool __fastcall initH(gd::PlayLayer*, void*, gd::GJGameLevel*);

	inline void(__thiscall* update)(gd::PlayLayer*, float);
	void __fastcall updateH(gd::PlayLayer*, void*, float);

	inline void(__thiscall* resetLevel)(gd::PlayLayer*);
	void __fastcall resetLevelH(gd::PlayLayer*);

	inline void(__thiscall* addToSection)(gd::PlayLayer*, gd::GameObject*);
	void __fastcall addToSectionH(gd::PlayLayer*, void*, gd::GameObject*);

	inline void(__thiscall* togglePracticeMode)(gd::PlayLayer*, bool);
	void __fastcall togglePracticeModeH(gd::PlayLayer*, void*, bool);

	inline void(__thiscall* showNewBest)(gd::PlayLayer*);
	void __fastcall showNewBestH(gd::PlayLayer*);

	inline void(__thiscall* updateVisibility)(gd::PlayLayer*);
	void __fastcall updateVisibilityH(gd::PlayLayer*);

	inline void(__thiscall* updateAttempts)(gd::PlayLayer*);
	void __fastcall updateAttemptsH(gd::PlayLayer*);

	void mem_init();
}