#pragma once
#include "pch.h"

namespace PlayerObject {
	inline bool(__thiscall* init)(gd::PlayerObject*, int, int, CCLayer*);
	bool __fastcall initH(gd::PlayerObject*, void*, int, int, CCLayer*);

	inline void(__thiscall* updatePlayerFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerFrameH(gd::PlayerObject*, void*, int);

	inline void(__thiscall* updatePlayerRollFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerRollFrameH(gd::PlayerObject*, void*, int);

	inline void(__thiscall* toggleDartMode)(gd::PlayerObject*, bool);
	void __fastcall toggleDartModeH(gd::PlayerObject*, void*, bool);

	void mem_init();
}