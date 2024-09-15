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

	inline void(__thiscall* spawnPlayer2)(gd::PlayLayer* self);
	void __fastcall spawnPlayer2_H(gd::PlayLayer* self);

	inline bool(__thiscall* destroyPlayer)(gd::PlayLayer* self, gd::PlayerObject* player);
	void __fastcall destroyPlayer_H(gd::PlayLayer* self, void* edx, gd::PlayerObject* player);

	inline bool(__thiscall* pushButton)(gd::PlayLayer* self, int idk1, bool idk2);
	void __fastcall pushButton_H(gd::PlayLayer* self, void* edx, int idk1, bool idk2);

	inline void(__thiscall* releaseButton)(gd::PlayLayer*, int, bool);
	void __fastcall releaseButton_H(gd::PlayLayer* self, void* edx, int idk1, bool idk2);


	void mem_init();

	extern bool inPractice;
}

namespace PauseLayer {
	inline bool(__thiscall* customSetup)(gd::CCBlockLayer* self);
	bool __fastcall customSetup_H(gd::CCBlockLayer* self);

	inline bool(__thiscall* onEdit)(CCLayer* self);
	void __fastcall onEdit_H(CCLayer* self);

	inline bool(__thiscall* onRestart)(CCObject* sender);
	void __fastcall onRestart_H(CCObject* sender);

	inline bool(__thiscall* onQuit)(CCObject* btn);
	void __fastcall onQuit_H(CCObject* btn);

	class Callback {
	public:
		void PercentageToggler(CCObject*);
	};

	void mem_init();
}

namespace EndLevelLayer {
	inline void(__thiscall* customSetup)(gd::GJDropDownLayer*);
	void __fastcall customSetup_H(gd::GJDropDownLayer* self);

	void mem_init();
}

namespace PlayerObject {
	inline void(__thiscall* updatePlayerFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerFrame_H(gd::PlayerObject* self, void* edx, int frameID);

	inline void(__thiscall* updatePlayerRollFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerRollFrame_H(gd::PlayerObject* self, void* edx, int frameID);

	void mem_init();
}
