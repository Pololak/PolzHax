#pragma once
#include "pch.h"

namespace PlayerObject {
	inline bool(__thiscall* init)(gd::PlayerObject*, int, int, CCLayer*);
	bool __fastcall initH(gd::PlayerObject*, void*, int, int, CCLayer*);

	inline void(__thiscall* updatePlayerFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerFrameH(gd::PlayerObject*, void*, int);

	inline void(__thiscall* updatePlayerRollFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerRollFrameH(gd::PlayerObject*, void*, int);

	inline void(__thiscall* toggleFlyMode)(gd::PlayerObject*, bool);
	void __fastcall toggleFlyModeH(gd::PlayerObject*, void*, bool);

	inline void(__thiscall* toggleRollMode)(gd::PlayerObject*, bool);
	void __fastcall toggleRollModeH(gd::PlayerObject*, void*, bool);

	inline void(__thiscall* toggleBirdMode)(gd::PlayerObject*, bool);
	void __fastcall toggleBirdModeH(gd::PlayerObject*, void*, bool);

	inline void(__thiscall* toggleDartMode)(gd::PlayerObject*, bool);
	void __fastcall toggleDartModeH(gd::PlayerObject*, void*, bool);

	inline void(__thiscall* togglePlayerScale)(gd::PlayerObject*, bool);
	void __fastcall togglePlayerScaleH(gd::PlayerObject*, void*, bool);

	inline void(__thiscall* runBallRotation2)(gd::PlayerObject*);
	void __fastcall runBallRotation2H(gd::PlayerObject*);

	inline void(__thiscall* collidedWithObject)(gd::PlayerObject*, gd::GameObject*, CCRect);
	void __fastcall collidedWithObjectH(gd::PlayerObject*, void*, gd::GameObject*, CCRect);

	inline void(__thiscall* loadFromCheckpoint)(gd::PlayerObject*, gd::PlayerCheckpoint*);
	void __fastcall loadFromCheckpointH(gd::PlayerObject*, void*, gd::PlayerCheckpoint*);

	void setCubeIcon(int);
	void setShipIcon(int);
	void setRollIcon(int);
	void setBirdIcon(int);
	void setDartIcon(int);

	void mem_init();
}