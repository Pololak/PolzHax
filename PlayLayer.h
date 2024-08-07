#pragma once
#include "pch.h"
#include "CheckPoint.h"
#include <vector>
#include <Windows.h>

namespace PlayLayer {
	inline bool(__thiscall* init)(gd::PlayLayer* self, gd::GJGameLevel* level);
	bool __fastcall init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level);

	inline bool(__thiscall* update)(gd::PlayLayer* self, float dt);
	void __fastcall update_H(gd::PlayLayer* self, void*, float dt);

	inline bool(__thiscall* resume)(CCLayer* self);
	bool __fastcall resume_H(CCLayer* self);

	inline bool(__thiscall* onQuit)(gd::PlayLayer* self);
	void __fastcall onQuit_H(gd::PlayLayer* self, void* edx);

	inline bool(__thiscall* resetLevel)(gd::PlayLayer* self);
	void __fastcall resetLevel_H(gd::PlayLayer* self);

	inline void(__thiscall* togglePracticeMode)(gd::PlayLayer* self, bool practice);
	void __fastcall togglePracticeMode_H(gd::PlayLayer* self, int edx, bool practice);

	inline int(__thiscall* createCheckpoint)(gd::PlayLayer* self);
	int __fastcall createCheckpoint_H(gd::PlayLayer* self);

	inline int(__thiscall* removeLastCheckpoint)(gd::PlayLayer* self);
	int __fastcall removeLastCheckpoint_H(gd::PlayLayer* self);

	inline bool(__thiscall* levelComplete)(gd::PlayLayer* self);
	bool __fastcall levelComplete_H(gd::PlayLayer* self);

	inline bool(__thiscall* pushButton)(int idk1, bool idk2);
	void __fastcall pushButton_H(int idk1, bool idk2);

	inline bool(__thiscall* releaseButton)(int idk1, bool idk2);
	void __fastcall releaseButton_H(int idk1, bool idk2);

	inline void(__thiscall* spawnPlayer2)(gd::PlayLayer* self);
	void __fastcall spawnPlayer2_H(gd::PlayLayer* self);

	void mem_init();

	extern std::vector<CheckPoint> checkpoints;
	extern bool inPractice;
}

namespace HardStreak {
	inline void(__thiscall* updateStroke)(gd::HardStreak*, float);
	void __fastcall updateStroke_H(gd::HardStreak* self, float dt);

	void mem_init();
}


